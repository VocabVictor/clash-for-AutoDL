#!/bin/bash
set -e

# 获取项目根目录的绝对路径
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
cd "$PROJECT_ROOT"

# 设置包名和版本
PACKAGE_NAME="clash-for-autodl"
VERSION="1.0.0"
ARCHITECTURE="all"

# 创建主目录
mkdir -p ${PACKAGE_NAME}_${VERSION}
cd ${PACKAGE_NAME}_${VERSION}

# 创建 DEBIAN 控制目录
mkdir -p DEBIAN

# 创建安装目录结构
mkdir -p usr/share/${PACKAGE_NAME}
mkdir -p usr/local/bin 