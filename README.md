<a href="https://github.com/breakerthb/TinyHttpd">
<img style="position: absolute; top: 0; left: 0; border: 0;" src="https://camo.githubusercontent.com/82b228a3648bf44fc1163ef44c62fcc60081495e/68747470733a2f2f73332e616d617a6f6e6177732e636f6d2f6769746875622f726962626f6e732f666f726b6d655f6c6566745f7265645f6161303030302e706e67" alt="Fork me on GitHub" data-canonical-src="https://s3.amazonaws.com/github/ribbons/forkme_left_red_aa0000.png">
</a>

# TinyHttpd Reuse

从TinyHttpd项目中提炼出的简单框架

项目管理工具：qtcreator

# 静态库

## JSonCPP

Json处理开源库

具体用法请参考 : <https://github.com/breakerthb/JsonCPPDemo/tree/linux>

## SocketLib

提供创建Socket服务程序基础方法。主要有两个类：

- SocketServ类

实现Socket服务器的基本功能，包括创建Socket连接和数据传递。
创建时，需要传入一个SocketOperate子类对象，实现服务器的具体功能。

- SocketOperate类

处理服务器Response的抽象类，代码复用时只需要通过继承这个类就能实现不同功能的服务器

# SocketServer

Socket实现的服务器程序，实现简单的通信功能。

引用：SocketLib工程

- SocketChat

SocketOperate类的一个子类，实现了把Client端发来的数据原样返回的功能。

- 效果

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/socketserver.png)

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/socketclient1.png)

# HttpServer

提供一个简单的Http服务器程序，实现简单的Get和Post协议。可以通过浏览器访问。

引用：JsonCPP和SocketLib项目

注意：使用QT Creator编译后，要html文件夹拷贝到执行目录

- 使用

启动服务器

    $ ./HttpServer

浏览器访问

- Get

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/get.png)

- Post

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/post.png)

# HttpClient

通过命令行完成Get和Post的HTTP访问

引用：

直接运行即可

# SocketClient

Socket客户端程序，建立和SocketServer的数据连接，进行简单的数据传递。

# RunProcessServer

提供一个可以调用第三方程序的SocketServer

引用：SocketLib

调用Run工程实现功能。

- 效果

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/runprocessserver.png)

![](https://raw.githubusercontent.com/breakerthb/Tinyhttpd/reuse/pic/socketclient2.png)

# Run

一个简单程序，打印所有命令行参数和stdin中的一行数据。