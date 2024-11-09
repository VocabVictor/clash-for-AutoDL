#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <curl/curl.h>
#include "yaml-cpp/yaml.h"
#include <regex>

namespace fs = std::filesystem;

class ClashManager {
private:
    const std::string HOME = std::getenv("HOME");
    const std::string CLASH_HOME = HOME + "/.clash-for-autodl";
    const std::string CONF_DIR = CLASH_HOME + "/conf";
    const std::string LOG_DIR = CLASH_HOME + "/logs";
    const std::string BIN_DIR = CLASH_HOME + "/bin";
    
    std::string clash_url;
    std::string clash_secret;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    bool downloadFile(const std::string& url, const std::string& output_path) {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        FILE* fp = fopen(output_path.c_str(), "wb");
        if (!fp) {
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        
        fclose(fp);
        curl_easy_cleanup(curl);
        
        return (res == CURLE_OK);
    }

    bool checkProcess(const std::string& process_name) {
        std::string cmd = "pgrep -f " + process_name;
        return (system(cmd.c_str()) == 0);
    }

    void killProcess(const std::string& process_name) {
        std::string cmd = "pkill -f " + process_name;
        if (system(cmd.c_str()) == -1) {
            std::cerr << "Failed to execute command: " << cmd << std::endl;
        }
    }

    bool setupDirectories() {
        try {
            fs::create_directories(CONF_DIR);
            fs::create_directories(LOG_DIR);
            fs::create_directories(BIN_DIR);
            return true;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating directories: " << e.what() << std::endl;
            return false;
        }
    }

    bool loadConfig() {
        std::string env_path = CLASH_HOME + "/.env";
        std::ifstream env_file(env_path);
        if (!env_file.is_open()) {
            std::cerr << "Error: Cannot open .env file" << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(env_file, line)) {
            if (line.find("CLASH_URL=") == 0) {
                clash_url = line.substr(10);
                // Remove quotes if present
                if (clash_url.front() == '"') clash_url = clash_url.substr(1);
                if (clash_url.back() == '"') clash_url.pop_back();
            }
            if (line.find("CLASH_SECRET=") == 0) {
                clash_secret = line.substr(13);
                if (clash_secret.front() == '"') clash_secret = clash_secret.substr(1);
                if (clash_secret.back() == '"') clash_secret.pop_back();
            }
        }

        return !clash_url.empty();
    }

    // 检查字符串是否是 base64 编码
    bool isBase64(const std::string& str) {
        if (str.empty()) return false;
        
        // base64 字符串长度必须是 4 的倍数
        if (str.length() % 4 != 0) return false;
        
        // 检查是否只包含 base64 合法字符
        std::regex base64_regex("^[A-Za-z0-9+/]*={0,2}$");
        return std::regex_match(str, base64_regex);
    }

    // base64 解码
    std::string base64Decode(const std::string& encoded) {
        std::string cmd = "echo '" + encoded + "' | base64 -d";
        std::string result;
        char buffer[128];
        
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        
        pclose(pipe);
        return result;
    }

    // 检查并修复 YAML 配置文件
    bool fixConfigFile(const std::string& config_path) {
        std::ifstream file(config_path);
        if (!file.is_open()) return false;

        // 读取文件内容
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();

        // 检查是否是 base64 编码
        if (isBase64(content)) {
            try {
                // 解码 base64
                content = base64Decode(content);
                
                // 尝试解析 YAML
                YAML::Load(content);
                
                // 写回文件
                std::ofstream out_file(config_path);
                out_file << content;
                out_file.close();
                
                std::cout << "Config file was base64 encoded, decoded successfully." << std::endl;
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Failed to decode base64 or parse YAML: " << e.what() << std::endl;
                return false;
            }
        }

        // 尝试验证 YAML 格式
        try {
            YAML::Load(content);
            return true;
        } catch (const YAML::Exception& e) {
            std::cerr << "Invalid YAML format: " << e.what() << std::endl;
            return false;
        }
    }

public:
    bool init() {
        if (!setupDirectories() || !loadConfig()) {
            return false;
        }

        // Download configuration if needed
        std::string config_path = CONF_DIR + "/config.yaml";
        if (!fs::exists(config_path)) {
            if (!downloadFile(clash_url, config_path)) {
                std::cerr << "Failed to download configuration" << std::endl;
                return false;
            }
            
            // 检查并修复配置文件
            if (!fixConfigFile(config_path)) {
                std::cerr << "Failed to fix configuration file" << std::endl;
                return false;
            }
        }

        // Download clash binary if needed
        std::string arch = getArchitecture();
        std::string clash_bin = BIN_DIR + "/clash-linux-" + arch;
        
        if (!fs::exists(clash_bin)) {
            if (!downloadClashBinary(arch)) {
                std::cerr << "Failed to download Clash binary" << std::endl;
                return false;
            }
        }

        return true;
    }

    bool start() {
        if (!setupDirectories() || !loadConfig()) {
            return false;
        }

        // Download configuration if needed
        std::string config_path = CONF_DIR + "/config.yaml";
        if (!fs::exists(config_path)) {
            if (!downloadFile(clash_url, config_path)) {
                std::cerr << "Failed to download configuration" << std::endl;
                return false;
            }
            
            // 检查并修复配置文件
            if (!fixConfigFile(config_path)) {
                std::cerr << "Failed to fix configuration file" << std::endl;
                return false;
            }
        }

        // Kill existing process if running
        killProcess("clash-linux");

        // Start clash
        std::string arch = getArchitecture();
        std::string clash_bin = BIN_DIR + "/clash-linux-" + arch;
        
        if (!fs::exists(clash_bin)) {
            if (!downloadClashBinary(arch)) {
                std::cerr << "Failed to download Clash binary" << std::endl;
                return false;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            setsid();
            FILE* new_stdout = freopen((LOG_DIR + "/clash.log").c_str(), "a", stdout);
            FILE* new_stderr = freopen((LOG_DIR + "/clash.log").c_str(), "a", stderr);
            if (!new_stdout || !new_stderr) {
                std::cerr << "Failed to redirect output streams" << std::endl;
                exit(1);
            }
            execl(clash_bin.c_str(), clash_bin.c_str(), "-d", CONF_DIR.c_str(), NULL);
            exit(1);
        }

        sleep(1);
        return checkProcess("clash-linux");
    }

    bool stop() {
        killProcess("clash-linux");
        return !checkProcess("clash-linux");
    }

    bool restart() {
        stop();
        sleep(1);
        return start();
    }

private:
    std::string getArchitecture() {
        // 获取系统架构
        std::string arch;
        FILE* pipe = popen("uname -m", "r");
        if (pipe) {
            char buffer[128];
            if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                arch = buffer;
                arch = arch.substr(0, arch.find('\n'));
            }
            pclose(pipe);
        }

        if (arch == "x86_64") return "amd64";
        if (arch == "aarch64") return "arm64";
        if (arch == "armv7l") return "armv7";
        
        throw std::runtime_error("Unsupported architecture: " + arch);
    }

    bool downloadClashBinary(const std::string& arch) {
        const std::string version = "v1.18.7";
        std::string url = "https://kkgithub.com/MetaCubeX/mihomo/releases/download/" + 
                         version + "/mihomo-linux-" + arch + "-" + version + ".gz";
        
        std::string temp_gz = BIN_DIR + "/clash.gz";
        std::string output_bin = BIN_DIR + "/clash-linux-" + arch;

        if (!downloadFile(url, temp_gz)) {
            return false;
        }

        // Decompress
        std::string cmd = "gzip -dc " + temp_gz + " > " + output_bin;
        if (system(cmd.c_str()) != 0) {
            fs::remove(temp_gz);
            return false;
        }

        fs::remove(temp_gz);
        fs::permissions(output_bin, 
            fs::perms::owner_exec | fs::perms::owner_read | fs::perms::owner_write,
            fs::perm_options::add);
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [init|start|stop|restart]" << std::endl;
        return 1;
    }

    ClashManager manager;
    std::string command = argv[1];

    try {
        bool success;
        if (command == "init") {
            success = manager.init();
        } else if (command == "start") {
            success = manager.start();
        } else if (command == "stop") {
            success = manager.stop();
        } else if (command == "restart") {
            success = manager.restart();
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            return 1;
        }

        return success ? 0 : 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 