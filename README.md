# Clash for AutoDL

此项目是 Fork [clash-for-linux](https://github.com/wanhebin/clash-for-linux) 后针对 AutoDL 平台的改进版本，主要用于解决在 AutoDL 平台服务器上下载 GitHub 等国外资源速度慢的问题。

## 功能特性

- [x] 支持自动下载和配置 Clash
- [x] 支持 deb 包安装，一键配置
- [x] 支持 systemd 服务自动启动
- [x] 支持 base64 编码的订阅地址
- [x] 支持 x86_64/aarch64 平台
- [x] 支持 RHEL/Debian 系列 Linux 系统
- [x] 用户级安装，无需 root 权限运行
- [x] 多用户隔离，每个用户独立配置

## 安装说明

### 方法一：直接安装 deb 包

1. 下载对应架构的 deb 包：
```bash
# 从 Releases 页面下载对应架构的 deb 包
# amd64: clash-for-autodl_1.0.0_amd64.deb
# arm64: clash-for-autodl_1.0.0_arm64.deb
```

2. 安装包：
```bash
sudo dpkg -i clash-for-autodl_1.0.0_*.deb
```

### 方法二：从软件源安装

1. 添加软件源：
```bash
# 添加软件源
echo "deb [trusted=yes] https://vocabvictor.github.io/clash-for-autodl/dists/stable/main/binary-$(dpkg --print-architecture) ./" | sudo tee /etc/apt/sources.list.d/clash-for-autodl.list
```

2. 安装软件：
```bash
sudo apt update
sudo apt install clash-for-autodl
```

## 配置和使用

1. 编辑配置文件：
```bash
nano ~/.clash-for-autodl/.env
```

2. 添加你的 Clash 订阅地址：
```
CLASH_URL="你的订阅地址"
```

3. 管理服务：
```bash
# 启动服务
clash_manager start

# 停止服务
clash_manager stop

# 重启服务
clash_manager restart

# 检查服务状态
systemctl --user status clash-for-autodl
```

## 目录结构

```
~/.clash-for-autodl/
├── bin/          # 二进制文件目录
├── conf/         # 配置文件目录
├── logs/         # 日志文件目录
└── .env          # 环境配置文件
```

## 注意事项

1. Ping 使用的是 ICMP 协议（网络层），而 Clash 只代理 TCP 和 UDP 流量（传输层），因此 ping 命令不会通过代理。

2. 如果遇到订阅地址无法使用，可能是因为：
   - 订阅地址是 base64 编码的（已自动处理）
   - 配置文件格式不符合 Clash 标准（已自动处理）
   - 订阅地址已过期或无效

3. 服务启动失败时，可以查看日志：
```bash
# 查看服务日志
journalctl --user -u clash-for-autodl

# 查看 Clash 日志
cat ~/.clash-for-autodl/logs/clash.log
```

## 卸载

```bash
sudo apt remove clash-for-autodl
```

## 问题反馈

如果遇到问题，请：
1. 先查看 [已关闭的 issues](https://github.com/VocabVictor/clash-for-autodl/issues?q=is%3Aissue+is%3Aclosed)
2. 检查日志文件
3. 提交新的 issue（请隐藏敏感信息）

## 开发相关

如果你想参与开发，可以：
1. Fork 本仓库
2. 安装依赖：`sudo apt install build-essential cmake libcurl4-openssl-dev libyaml-cpp-dev`
3. 构建项目：`./dev/build/build_all.sh`
