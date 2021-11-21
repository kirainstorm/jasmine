# obs-jasmine
````
由于WebView2有文件超过100M，所以不上传
下载 Microsoft.WebView2.FixedVersionRuntime.87.0.664.66.x86.cab 并解压到 obs-jasmine，
修改文件夹名称Microsoft.WebView2.FixedVersionRuntime.87.0.664.66.x86为WebView2


````


# Microsoft.Web.WebView2
````
NuGet 安装
Microsoft.Windows.ImplementationLibrary 1.0.191107.2
Microsoft.Web.WebView2 0.9.622.11

https://docs.microsoft.com/zh-cn/microsoft-edge/webview2/gettingstarted/win32


下载 WebView2 运行时安装程序
https://msedge.sf.dl.delivery.mp.microsoft.com/filestreamingservice/files/e7e43551-684d-46d5-9b0c-0e2538446664/MicrosoftEdgeWebView2RuntimeInstallerX86.exe


使用 WebView2 的应用分发
https://docs.microsoft.com/zh-cn/microsoft-edge/webview2/concepts/distribution


https://msedge.sf.dl.delivery.mp.microsoft.com/filestreamingservice/files/238de405-59a8-4f5a-a6e7-15bd7caaa0ed/Microsoft.WebView2.FixedVersionRuntime.87.0.664.66.x86.cab

下载 已修复的版本程序包。
使用 CreateCoreWebView2EnvironmentWithOptions 函数创建环境。 使用 browserExecutableFolder 参数指示包含的文件夹的路径 msedgewebview2.exe

CreateCoreWebView2EnvironmentWithOptions(_T("Microsoft.WebView2.FixedVersionRuntime.87.0.664.66.x86"),

````


# vs2017 32位 编译 obs 25.0.0
````
OBS requires Windows 10 SDK version 10.0.18362.0 and above to compile.



编译release完成后，编译工程下的install工程，将会将文件安装到C:\Program Files (x86)\obs-studio


qt 5.12.4
cmake 3.18.5
obs 25.0.0

https://github.com/obsproject/obs-studio/wiki/Install-Instructions#windows-build-directions


Requirements for building OBS on windows

Development packages of FFmpeg, x264, cURL, and mbedTLS.
Pre-built windows dependencies for VS2017/VS2019 can be found here:
VS2017: https://obsproject.com/downloads/dependencies2017.zip
Qt5 (Grab the MSVC package for your version of Visual Studio)
We currently deploy with 5.10.1, which is no longer available from Qt. A download is provided here.
Windows version of cmake (3.16 or higher, latest preferred)
Windows version of git (git binaries must exist in path)
Visual Studio 2019 (recommended)
Windows 10 SDK (minimum 10.0.19041.0). Latest SDK



git clone --recursive https://github.com/obsproject/obs-studio.git -b 25.0.0
git branch -v
* (no branch) 327a6f59 win-capture: Fix potential crash due to unhandled exceptions

https://obsproject.com/downloads/dependencies2017.zip


#string(REPLACE "-" ";" UI_VERSION_SPLIT ${OBS_VERSION})
#list(GET UI_VERSION_SPLIT 0 UI_VERSION)
#string(REPLACE "." ";" UI_VERSION_SEMANTIC ${UI_VERSION})
#list(GET UI_VERSION_SEMANTIC 0 UI_VERSION_MAJOR)
#list(GET UI_VERSION_SEMANTIC 1 UI_VERSION_MINOR)
#list(GET UI_VERSION_SEMANTIC 2 UI_VERSION_PATCH)

disable python

1 VERSIONINFO
FILEVERSION 1.0.0.0

````

# vs2017 32位 编译 libevent
````
https://github.com/libevent/libevent/releases
https://github.com/libevent/libevent/archive/release-2.1.11-stable.zip

在开始菜单--->所有程序处打开VS 2017的开发人员命令提示符程序 x86 Native Tools Command Prompt for VS 2017


找到minheap-internal.h，加入头文件#include "stdint.h"


nmake /f Makefile.nmake clean
nmake /f Makefile.nmake

````

# windows编译nginx
````

C:\MinGW\msys\1.0\msys.bat


rm -fr nginx-rtmp-module-1.2.1/
tar -zxvf nginx-rtmp-module-1.2.1.tar.gz

rm -fr nginx-release-1.18.0/
tar -zxvf nginx-release-1.18.0.tar.gz

rm -fr openssl-1.0.2u
tar -zxvf openssl-1.0.2u.tar.gz

rm -fr pcre-8.44
tar -zxvf pcre-8.44.tar.gz

rm -fr zlib-1.2.11
tar -zxvf zlib-1.2.11.tar.gz

cd nginx-release-1.18.0/

这里会发现有一个异样的提示，auto/cc/msvc: line 117: [: : integer expression expected ，
只要修改文件 $nginx源码目录\auto\cc\msvc 即可，在 echo " + cl version: $NGX_MSVC_VER" 的前面加入一行 NGX_MSVC_VER=15.9.29 ，当然不修改也不会影响后续的编译。 

15.9.29 为vs2017


修改1：
编辑nginx\auto\lib\openssl\makefile.msvc文件：

找到“VC-WIN32”替换为“VC-WIN64A”
“if exist ms\do_ms.bat”替换为“if exist ms\do_win64a.bat”
“ms\do_ms”替换为“ms\do_win64a”


找到/nginx/src/http/ngx_http_header_filter_module.c文件，修改以下变量的声明：

static u_char ngx_http_server_string[] = "Server: nginx" CRLF;
static u_char ngx_http_server_full_string[] = "Server: " NGINX_VER CRLF;
static u_char ngx_http_server_build_string[] = "Server: " NGINX_VER_BUILD CRLF;      
修改为：
static u_char ngx_http_server_string[] = "Server: " CRLF;
static u_char ngx_http_server_full_string[] = "Server:  " CRLF;
static u_char ngx_http_server_build_string[] = "Server: " CRLF;


auto/configure --with-cc=cl --builddir=objs \
--with-debug --prefix= --conf-path=conf/nginx.conf \
--pid-path=logs/nginx.pid --http-log-path=logs/access.log \
--error-log-path=logs/error.log --sbin-path=nginx.exe \
--http-client-body-temp-path=temp/client_body_temp \
--http-proxy-temp-path=temp/proxy_temp \
--http-fastcgi-temp-path=temp/fastcgi_temp \
--http-scgi-temp-path=temp/scgi_temp \
--http-uwsgi-temp-path=temp/uwsgi_temp \
--with-cc-opt=-DFD_SETSIZE=1024 \
--with-pcre=./../pcre-8.44 \
--with-zlib=./../zlib-1.2.11 \
--with-select_module \
--with-http_v2_module \
--with-http_realip_module \
--with-http_addition_module \
--with-http_sub_module \
--with-http_dav_module \
--with-http_stub_status_module \
--with-http_flv_module \
--with-http_mp4_module \
--with-http_gunzip_module \
--with-http_gzip_static_module \
--with-http_auth_request_module \
--with-http_random_index_module \
--with-http_secure_link_module \
--with-http_slice_module \
--with-mail \
--with-stream \
--with-openssl=./../openssl-1.0.2u \
--with-openssl-opt=no-asm \
--with-http_ssl_module \
--with-mail_ssl_module \
--with-stream_ssl_module \
--add-module=./../nginx-rtmp-module-1.2.1/


如果选择支持nginx-rtmp-module，需要修改objs\Makefile文件：

将“-WX”删除，否则nmake时会报错“nginx error:c2220（警告被视为错误）。

适用于 VS 2017 的 x64 本机工具命令提示

nmake clean
nmake -f objs/Makefile


obs
rtmp://192.168.0.53:1935/live/
rjsdy

vlc
rtmp://192.168.0.53:1935/live/rjsdy
````
