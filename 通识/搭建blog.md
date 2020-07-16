# 个人博客搭建过程记录

windows下基于框架hexo在云服务器搭建博客并绑定域名。



# 搭建过程



## 1. 本地计算机安装hexo环境

1.  安装Node.js
2.  安装Git
3.  安装Hexo
4.  本地端文件位置D:\HexoBlog\myblogs

## 2. 服务器端准备工作

1.  购买服务器和域名，实名认证与备案
2.  连接实例
    1.  腾讯云自带 webshell方式，VNC方式
    2.  windows Putty连接，可以基于口令或秘钥
    3.  linux ssh连接
3.  配置安全组，放行80端口访问nginx服务(可省)

## 3. 服务器端准备工作

1.  安装nginx
    1.  放开80端口外部访问权限
2.  配置nginx服务器路由
    1.  主要用于域名转发
3.  安装Node.js
4.  安装Git

## 4. 服务器端部署hexo

1.  创建git用户

2.  给git配置ssh免密公钥登录

    1.  配置服务端ftp服务
    2.  本地端使用FileZilla传递公钥给服务器
    3.  主机中(windows)私钥放在 /user/.ssh 中
    4.  链接成功后，在本地计算机应该可以使用 $ssh -v git@ xxx.xxx.xxx.xxxxxx.xxx.xxx.xxx(服务器公网ip)连接服务器，且不需要密码

    ![image-20200629182358697](.\搭建blog.assets\image-20200629182358697.png)

3.  配置git仓库

    1.  在~目录下(/home/git)新建git仓库，在该仓库新建post-receive钩子文件。
    2.  git仓库为/home/git/hexo.git

4.  重启服务器

    

## 5.hexo配置

1.  客户端配置hexo设置

    1.  修改_config.yml
    2.  安装插件hexo-deployer-git  git自动部署
    3.  安装插件hexo-server hexo本地简单服务器

2.  本地使用hexo生成发布博客   ip:80可以进入个人博客

    ```
    $ hexo clean
    $ hexo generate
    $ hexo deploy
    ```

3.  域名绑定





## 常见问题

*   502报错: 常见于没有启动nginx，导致80端口没有监听并进行返回，尝试进入/usr/local/nginx/sbin 目录并启动nginx进行重试。

    ```
    #cd /usr/local/nginx/sbin
    #./nginx
    ```

    



服务器端hexo部署目录 /home/wwm/hexo





参考文献:

https://mp.weixin.qq.com/s/JTTUYJTvtdT6X2fvLUBFZg