#!/bin/bash
set -e

# 获取项目根目录的绝对路径
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

# 默认值
DEFAULT_BRANCH="deb"
DEFAULT_VERSION="v0.0.1"

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        --branch|-b)
            BRANCH="$2"
            shift 2
            ;;
        --initial-version|-v)
            INITIAL_VERSION="$2"
            shift 2
            ;;
        *)
            echo "未知参数: $1"
            exit 1
            ;;
    esac
done

# 使用默认值或命令行参数
BRANCH=${BRANCH:-$DEFAULT_BRANCH}
INITIAL_VERSION=${INITIAL_VERSION:-$DEFAULT_VERSION}

# 检查是否有未提交的更改
if [[ -n $(git status -s) ]]; then
    echo "检测到未提交的更改"
    git add .
    read -p "请输入提交信息: " commit_message
    git commit -m "$commit_message"
fi

# 获取最新的版本标签
latest_tag=$(git describe --tags --abbrev=0 2>/dev/null || echo "$INITIAL_VERSION")

# 增加版本号
if [[ $latest_tag =~ v([0-9]+)\.([0-9]+)\.([0-9]+) ]]; then
    major="${BASH_REMATCH[1]}"
    minor="${BASH_REMATCH[2]}"
    patch="${BASH_REMATCH[3]}"
    
    # 增加补丁版本号
    patch=$((patch + 1))
    new_tag="v$major.$minor.$patch"
else
    new_tag="$INITIAL_VERSION"
fi

# 创建新标签
echo "创建新标签: $new_tag"
git tag $new_tag

# 推送更改和标签
echo "推送更改到远程仓库..."
git push origin "$BRANCH"
git push origin "$new_tag"

echo "完成！新版本 $new_tag 已发布到分支 $BRANCH"



