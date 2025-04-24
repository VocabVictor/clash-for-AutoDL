#!/bin/bash

# 定义颜色变量
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 创建必要的目录
INSTALL_DIR="/usr/share/clash-for-autodl"
CONFIG_DIR="/etc/clash-for-autodl"
LOG_DIR="/var/log/clash-for-autodl"

# 确保目录存在
mkdir -p "$CONFIG_DIR" "$LOG_DIR"

# 复制示例配置文件
if [ ! -f "$CONFIG_DIR/.env" ]; then
    cp "$INSTALL_DIR/.env.example" "$CONFIG_DIR/.env"
    echo -e "${GREEN}[√] 已创建配置文件: $CONFIG_DIR/.env${NC}"
    echo -e "${RED}[!] 请编辑 $CONFIG_DIR/.env 文件，配置您的Clash订阅链接${NC}"
fi

# 复制模板配置
if [ ! -f "$CONFIG_DIR/template.yaml" ]; then
    cp "$INSTALL_DIR/conf/template.yaml" "$CONFIG_DIR/template.yaml"
    echo -e "${GREEN}[√] 已创建模板配置: $CONFIG_DIR/template.yaml${NC}"
fi

# 创建软链接到系统路径
ln -sf "$INSTALL_DIR/start.sh" /usr/local/bin/clash-start
ln -sf "$INSTALL_DIR/restart.sh" /usr/local/bin/clash-restart
ln -sf "$INSTALL_DIR/shutdown.sh" /usr/local/bin/clash-shutdown

# 创建系统服务
cat <<EOF > /etc/systemd/system/clash-for-autodl.service
[Unit]
Description=Clash for AutoDL Service
After=network.target

[Service]
Type=simple
User=root
Environment="CLASH_CONFIG_DIR=$CONFIG_DIR"
Environment="CLASH_LOG_DIR=$LOG_DIR"
ExecStart=$INSTALL_DIR/start.sh --service
ExecStop=$INSTALL_DIR/shutdown.sh --service
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF

# 重新加载systemd
systemctl daemon-reload

echo -e "${GREEN}[√] 安装完成！${NC}"
echo -e "使用方法:"
echo -e "  clash-start     - 启动Clash服务"
echo -e "  clash-restart   - 重启Clash服务"
echo -e "  clash-shutdown  - 关闭Clash服务"
echo -e "  systemctl start clash-for-autodl   - 作为系统服务启动"
echo -e "  systemctl enable clash-for-autodl  - 设置开机自启"
echo -e ""
echo -e "${RED}[!] 重要: 首次使用前，请编辑 $CONFIG_DIR/.env 文件，配置您的Clash订阅链接${NC}"

exit 0 