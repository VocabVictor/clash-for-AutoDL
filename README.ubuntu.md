# Clash for AutoDL - Ubuntu 版本

本项目是 Clash for AutoDL 的 Ubuntu 支持版本，提供了标准 Debian 包安装方式，使得在各种 Ubuntu 和 Debian 发行版上安装更加简单。

## 安装方法

### 方法1：直接安装 Debian 包

1. 下载最新的 Debian 包
   ```bash
   wget https://github.com/用户名/clash-for-AutoDL/releases/download/v1.0.0/clash-for-autodl-1.0.0.deb
   ```

2. 安装软件包
   ```bash
   sudo dpkg -i clash-for-autodl-1.0.0.deb
   ```

3. 如果有依赖问题，运行：
   ```bash
   sudo apt-get install -f
   ```

### 方法2：从源码构建

1. 克隆仓库
   ```bash
   git clone https://github.com/用户名/clash-for-AutoDL.git
   cd clash-for-AutoDL
   git checkout ubuntu-support
   ```

2. 运行构建脚本
   ```bash
   ./build-deb.sh
   ```

3. 安装生成的包
   ```bash
   sudo dpkg -i clash-for-autodl-1.0.0.deb
   ```

## 使用方法

安装后，系统会自动配置以下命令：

- `clash-start` - 启动 Clash 服务
- `clash-restart` - 重启 Clash 服务
- `clash-shutdown` - 关闭 Clash 服务

您也可以使用系统服务方式管理：

```bash
# 启动服务
sudo systemctl start clash-for-autodl

# 设置开机自启
sudo systemctl enable clash-for-autodl

# 查看服务状态
sudo systemctl status clash-for-autodl

# 停止服务
sudo systemctl stop clash-for-autodl
```

## 配置文件

安装后，配置文件位于：
- `/etc/clash-for-autodl/.env` - 主要配置文件，设置订阅链接
- `/etc/clash-for-autodl/template.yaml` - Clash 模板配置

日志文件位于：
- `/var/log/clash-for-autodl/clash.log`

## 代理设置

安装后，您可以在任何 shell 中使用以下命令来控制系统代理：

```bash
# 开启系统代理
proxy_on

# 关闭系统代理
proxy_off
```

## 特性

- 通过 apt 包管理器安装和卸载
- 标准的系统服务集成
- 配置文件集中管理
- 自动添加代理命令到系统环境

## 卸载

```bash
sudo apt remove clash-for-autodl
```

如果想完全删除配置文件：

```bash
sudo apt purge clash-for-autodl
``` 