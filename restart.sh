#!/bin/bash

# 检查是否作为服务运行
SERVICE_MODE=false
if [[ "$1" == "--service" ]]; then
    SERVICE_MODE=true
fi

# 获取脚本工作目录绝对路径
export Server_Dir=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)

# 执行关闭脚本
if [[ "$SERVICE_MODE" == "true" ]]; then
    $Server_Dir/shutdown.sh --service
else
    $Server_Dir/shutdown.sh
fi

# 执行启动脚本
if [[ "$SERVICE_MODE" == "true" ]]; then
    $Server_Dir/start.sh --service
else
    $Server_Dir/start.sh
fi

exit 0