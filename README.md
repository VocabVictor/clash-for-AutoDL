# 项目介绍

此项目是Fork [clash-for-linux](https://github.com/wanhebin/clash-for-linux)后针对AutoDL平台的一些简单适配

主要是为了解决我们在AutoDL平台服务器上下载GitHub等一些国外资源速度慢的问题。

> **注意：** 考虑到使用本仓库中的部分同学可能是这方面的新手，以下说明中添加了一些常见问题的解答和演示图片，请仔细阅读。如果图片看不清楚，可以安装[Imaugs](https://chromewebstore.google.com/detail/imagus/immpkjjlgappgfkkfieppnmlhakdmaab)，这是个老牌的Chrome插件，可以将图片放大查看。

> **注意：** 建议提issue的同学，可以给自己的GitHub账户绑定邮箱，这样子一旦收到反馈，会及时通知到你。

> **注意：** Ping使用的是 ICMP（Internet Control Message Protocol） 协议，是网络层协议，Clash 只会代理传输层的TCP和UDP流量，因此无论clash是否能够正常工作，ping google.com 都是不会有效果的。

> **注意：** 关于本项目的适配问题，对于RHEL/Debian系列Linux系统，x86_64/aarch64平台的一般云服务器和本地服务器应该都是适配的，比如阿里云，腾讯云，autodl，趋势云上，本地的3090，4090服务器上，作者都做过相关测试，可以正常运行。

# 功能特性

- [x] 支持自动下载Clash订阅地址，并自动配置Clash客户端。
- [x] 无需sudo权限
- [x] 支持RHEL/Debian系列Linux系统
- [x] 支持x86_64/aarch64平台
- [x] 支持自定义Clash Secret
- [x] 自带Clash Dashboard，可视化管理Clash。
- [x] 做了一些适配，适用于AutoDL平台。
- [x] 脚本进行了防呆措施，可自动处理常见错误。
- [x] 一次配置，永远起效，每打开一个新的shell，都会自动启动Clash。

<br>

# Todo List

- [ ] 封装为deg安装包，支持懒人配置
- [ ] 支持华为云-昇腾显卡平台的Euler系统
- [ ] 惰性下载，只下载对应CPU架构的clash 二进制文件

# 使用须知

- 使用过程中如遇到问题，请优先查已有的 [issues](https://github.com/VocabVictor/clash-for-AutoDL/issues?q=is%3Aissue+is%3Aclosed)。(你在网页上看不到issue或者issue很少，是因为部分issue我认为已经解决，被关闭了，请在issue中搜索关键字，或者在issue下留言。)
- 在进行issues提交前，请替换提交内容中是敏感信息（例如：订阅地址）。
- 此项目不提供任何订阅信息，请自行准备Clash订阅地址。
- 运行前请手动更改`.env`文件中的`CLASH_URL`变量值，否则无法正常运行。

> **注意**：当你在使用此项目时，遇到任何无法独自解决的问题请优先前往 [issues](https://github.com/VocabVictor/clash-for-AutoDL/issues?q=is%3Aissue+is%3Aclosed) 寻找解决方法。由于空闲时间有限，后续将不再对Issues中 "已经解答"、"已有解决方案" 的问题进行重复性的回答。

<br>

# 使用教程

## 下载项目

下载项目

```bash
git clone https://github.com/VocabVictor/clash-for-AutoDL.git
```

或者尝试kgithub(GitHub镜像站)下载

```bash
git clone https://kkgithub.com/VocabVictor/clash-for-AutoDL.git
```

![1.png](https://s2.loli.net/2024/06/20/8e4VzyTYZSGhPsC.png)

进入到项目目录，编辑`.env`文件，修改变量`CLASH_URL`的值。

```bash
cd clash-for-AutoDL
cp .env.example .env
vim .env
```

![2.png](https://s2.loli.net/2024/06/20/OXfREWDBgvjw4Kb.png)

![3.png](https://s2.loli.net/2024/06/20/S4t8ZlVjiOKuo7n.png)

> **注意：** `.env` 文件中的变量 `CLASH_SECRET` 为自定义 Clash Secret，值为空时，脚本将自动生成随机字符串。

<br>

## 启动程序

直接运行脚本文件`start.sh`

- 进入项目目录

```bash
cd clash-for-AutoDL
```

![4.png](https://s2.loli.net/2024/06/20/9yz4WwdoqrsCQt2.png)

由于现在AutoDL平台上的所有镜像都没有lsof，该工具是脚本中检测端口是否被占用的，所以需要安装一下。

```bash
apt-get update
apt-get install lsof
```

![5.png](https://s2.loli.net/2024/06/20/otEXxVMDOrez62Q.png)

- 运行启动脚本

```bash
source ./start.sh

配置文件已存在，无需下载。
配置文件格式正确，无需转换。

正在启动Clash服务...
服务启动成功！                                             [  OK  ]

Clash 控制面板访问地址: http://<your_ip>:6006/ui
Secret: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

已添加代理函数到 .bashrc。
请执行以下命令启动系统代理: proxy_on
若要临时关闭系统代理，请执行: proxy_off
若需要彻底删除，请调用: shutdown_system

[√] 系统代理已启用
正在测试网络连接...
网络连接测试成功。
```

![6.png](https://s2.loli.net/2024/06/20/txmaFbIAQpY2nES.png)


- 检查服务端口

```bash
lsof -i -P -n | grep LISTEN | grep -E ':6006|:789[0-9]'

tcp        0      0 127.0.0.1:6006          0.0.0.0:*               LISTEN     
tcp6       0      0 :::7890                 :::*                    LISTEN     
tcp6       0      0 :::7891                 :::*                    LISTEN     
tcp6       0      0 :::7892                 :::*                    LISTEN
```

![7.png](https://s2.loli.net/2024/06/20/WMVzH431c8gARPw.png)

以上步骤如果正常，说明服务clash程序启动成功，现在就可以体验高速下载github资源了。

<br>

## 重启程序

如果需要对Clash配置进行修改，请修改 `conf/config.yaml` 文件。然后运行 `restart.sh` 脚本进行重启。

> **注意：**
> 重启脚本 `restart.sh` 不会更新订阅信息。

<br>

## 停止程序

- 临时停止

```bash
proxy_off
```
![9.png](https://s2.loli.net/2024/06/20/FwoBc6COzYybl5Z.png)

- 彻底停止程序

```bash
shutdown_system
```

![10.png](https://s2.loli.net/2024/06/20/p6CmIkvycFzTHiE.png)

然后检查程序端口、进程以及环境变量`http_proxy|https_proxy`，若都没则说明服务正常关闭。

<br>

## Clash Dashboard (可选，不是梯子正常运行的必要选项)

- 安装并使用ngork

由于监管要求，AutoDL平台上禁止个人用户开放外网端口，所以需要使用ngrok进行内网穿透。

ngrog是一个外网映射工具，简单理解就是当你使用它之后，会给你生产一个域名，别人就可以通过这个域名来访问你的电脑了。

使用ngrok前，需要先安装ngrok。

```bash
 curl -s https://ngrok-agent.s3.amazonaws.com/ngrok.asc | sudo tee /etc/apt/trusted.gpg.d/ngrok.asc >/dev/null && echo "deb https://ngrok-agent.s3.amazonaws.com buster main" | sudo tee /etc/apt/sources.list.d/ngrok.list && sudo apt update && sudo apt install ngrok
```

![11.png](https://s2.loli.net/2024/06/20/uCi94eqBEJUafS3.png)

- 启动ngrok

安装成功后，我们先到ngork官网下载获取token，首先点击下面的链接注册登录，进入首页

[Ngork](https://dashboard.ngrok.com/login)

![12.png](https://s2.loli.net/2024/06/20/85kmWYwPQcjRZCB.png)

红框处即为你的token，复制一整条命令，然后在终端中运行。

- 映射端口

![13.png](https://s2.loli.net/2024/06/20/J8KftF1hm736WsB.png)

打开新的shell，运行下面的命令，映射6006端口

```bash
proxy_off
ngrok http 6006
```

![14.png](https://s2.loli.net/2024/06/20/FYJ4Bx37ovcemKt.png)

![15.png](https://s2.loli.net/2024/06/20/tGRdS2HnXKxPr7U.png)

- 登录管理界面

点击链接（例如图中是https://078d-58-144-141-213.ngrok-free.app.ngrok.io，要加上/ui），跳转到中间页面

![16.png](https://s2.loli.net/2024/06/20/oUykYI7zR8mxtri.png)

点击`Visit Site`, 跳转到管理界面

![17.png](https://s2.loli.net/2024/06/20/HzNquhIxLkPecTm.png)

- 最后的设置

在`API Base URL`一栏中输入ngork的映射地址（例如图中是https://078d-58-144-141-213.ngrok-free.app.ngrok.io） ，在`Secret(optional)`一栏中输入启动成功后输出的Secret。

Secret忘记了，也可以上conf/config.yaml文件中查看。

点击Add并选择刚刚输入的管理界面地址，之后便可在浏览器上进行一些配置。

最后，就得到了一个和Clash for Windows用法类似的Clash Dashboard管理界面。

![18.png](https://s2.loli.net/2024/06/20/pLRhr7WQiCZDBY3.png)

- 更多教程

此 Clash Dashboard 使用的是[yacd](https://github.com/haishanh/yacd)项目，详细使用方法请移步到yacd上查询。

<br>

# 常见问题

1. 部分Linux系统默认的 shell `/bin/sh` 被更改为 `dash`，运行脚本会出现报错（报错内容一般会有 `-en [ OK ]`）。建议使用 `bash xxx.sh` 运行脚本。

2. 部分用户在UI界面找不到代理节点，基本上是因为厂商提供的clash配置文件是经过base64编码的，且配置文件格式不符合clash配置标准。

   目前此项目已集成自动识别和转换clash配置文件的功能。如果依然无法使用，则需要通过自建或者第三方平台（不推荐，有泄露风险）对订阅地址转换。

3. 程序日志中出现`error: unsupported rule type RULE-SET`报错，解决方法查看官方[WIKI](https://github.com/Dreamacro/clash/wiki/FAQ#error-unsupported-rule-type-rule-set)

## Ubuntu 支持

我们现在提供了 Ubuntu 和 Debian 发行版的专用安装包，使安装更加简单。详情请参考 [Ubuntu 安装指南](README.ubuntu.md)。

```bash
# Ubuntu/Debian 安装方法
sudo dpkg -i clash-for-autodl-1.0.0.deb
sudo apt-get install -f  # 解决依赖问题
```
