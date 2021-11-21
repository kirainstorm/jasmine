# conf-jasmine
````
linphone-desktop 4.2.3
freeswitch 1.10.5
vs2017 ： 确保安装了： 。net桌面开发，使用C++的桌面开发，通用Windows平台开发
默认10.0.17763.0，并在右边选中“sdk 8.1 and urt”，不能仅仅安装sdk 8.1

linphone 实际发送的视频分辨率与网络，电脑摄像头支持的分辨率有关。动态调整



2020/12/24 视频通话时，无法预览本地图像。。。
2020/12/24 视频会议时，不同的布局导致发送不同的分辨率视频，画布变小时分辨率变小，画布变大时分辨率不变大（有可能与电脑摄像头支持的分辨率有关）








conference 100@video-mcu-stereo-480 dial user/1001 100
conference 100@video-mcu-stereo-480 dial user/1002 100

conference 100@video-mcu-stereo-720 dial user/1003 100
conference 100@video-mcu-stereo-720 dial user/1007 100


conference 100 hup all

conference 100 vid-layout overlaps

overlaps： 1080p的时候小图标分辨率 320 240




conference 100 list


conference 100 vid-layout 1x1
conference 100 vid-layout 2up_top+8


conference 100 vid-layer 23 vid-floor


conference 100 vid-layout presenter-overlap-small-top-righ

https://freeswitch.org/confluence/display/FREESWITCH/mod_conference
https://freeswitch.org/confluence/display/FREESWITCH/FreeSWITCH+1.6+Video#FreeSWITCH1.6Video-Examples




1.列出所有会议

    conference list

2.列出会议的所有成员

    conference <conference_name> list

    而conference_name就是会议的会议号+ip地址

3.列出会议可用的所有布局

    conference <conference_name> vid-layout list

4.改变画面布局

    conference <conference_name> vid_layout <layout_name>

    而layout_name就是 2x2、3x3等布局格式

5.更改角色在布局中的位置

    conference <conference_name> vid-layer <member_id> <position> 

6.静音与取消静音

    conference <conference_name> mute <member_id>

    conference <conference_name> unmute <member_id>



conference 100 vid-layout list

2x1-zoom
1x2
8x8
1up_top_left+5
1x1+2x1
5-grid-zoom
2x1
2x1-presenter-zoom
2up_middle+8
2up_top+8
1x1
presenter-dual-horizontal
3x1-zoom
presenter-overlap-large-bot-right
3up+4
presenter-overlap-small-top-right
presenter-dual-vertical
3up+9
5x5
4x4
4x2-zoom
overlaps
2x2
3x2-zoom
presenter-overlap-large-top-right
6x6
3x3
1up_top_left+7
7-grid-zoom
presenter-overlap-small-bot-right
2up_bottom+8
1up_top_left+9








````



# fs_cli
````
使用esl与freeswitch通讯，用的upd?



一下代码测试通过
import socket
import json
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1', 8021))
# send auth
sock.send(bytes('auth ClueCon\r\n\r\n','utf-8'))
# send command
sock.send(bytes('event json ALL\r\n\r\n','utf-8'))
#sock.send('event json ALL\r\n\r\n')
#sock.send('event plain ALL\r\n\r\n')
while True:
    print(sock.recv(10240))

````


# linphone-desktop (vs2017 win32)
````
https://www.linphone.org/technical-corner/liblinphone

https://www.linphone.org/releases/windows/sdk/linphone-sdk-4.4.0-win32.zip





https://github.com/BelledonneCommunications/linphone-desktop/releases


（一）设置代理

git config --global http.proxy http://127.0.0.1:10809
git config --global https.proxy http://127.0.0.1:10809

git config --global core.gitproxy  xxxx



git config --global --unset http.proxy
git config --global --unset https.proxy
git config --global --unset core.gitproxy


4.2.5没有编译过

#Clone repository:
git clone https://gitlab.linphone.org/BC/public/linphone-desktop.git --recursive  -b 4.2.3


#Update sub-modules
#git submodule update --init --recursive

#
git branch -v


vs2017 win32

cmake
https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-win64-x64.msi



python
https://www.python.org/ftp/python/3.8.5/python-3.8.5-amd64.exe

python -m pip install --upgrade pip

pip install pystache
pip install six
pip install wheel


Qt5.9及以上版本，设置对应环境变量
Qt5_DIR
C:\Qt\Qt5.12.10\5.12.10\msvc2017\lib\cmake
PATH
C:\Qt\Qt5.12.10\5.12.10\msvc2017\bin


安装Yasm

参考官方README.md。

下载yasm-1.3.0-win32.exe，将其改名为yasm.exe，拷贝到 

http://www.tortall.net/projects/yasm/releases/yasm-1.3.0-win32.exe

C:\Users\yinvision\AppData\Local\Programs\Python\Python38\Scripts
目录下（或是任何被包括在环境变量里的目录，目的是在cmd里可以直接使用yasm命令）





doxygen
http://doxygen.nl/files/doxygen-1.8.20-setup.exe
PATH
C:\Program Files\doxygen\bin


NASM
https://www.nasm.us/pub/nasm/releasebuilds/2.15/win32/nasm-2.15-installer-x86.exe
PATH
C:\Program Files (x86)\NASM



MinGW
https://osdn.net/frs/redir.php?m=xtom_hk&f=mingw%2F68260%2Fmingw-get-setup.exe
PATH
C:\MinGW\bin



安装windows sdk 8.1 给 libvpx 用



使用cmake生成build目录
在bulid目录下

cmake --build . --target install

cmake --build . --config Release --target install -j8



````



# freeswitch
````
windows 下载
https://files.freeswitch.org/windows/installer/x64/
将安装后的文件全部提取出来




winsows 10 编译 freeswitch
下载源码
http://files.freeswitch.org/releases/freeswitch/
http://files.freeswitch.org/releases/freeswitch/freeswitch-1.10.5.-release.zip
安装wix
https://wixtoolset.org/releases/
https://github.com/wixtoolset/wix3/releases 
安装vs2017
安装windows sdk 10.0.18362.0
安装git

freeswitch-1.10.5.-release.zip解压目录不能有中文

用vs2017打开Freeswitch.2017.sln，将所有的工程的windows sdk 8.1 改为 10.0.18362.0

编译FreeSwitchConsole

编译Setup.2017，生成安装包


````


C:/_sdk/linphone-desktop/linphone-sdk/external/libvpx/build/make/msvs_common.sh: line 44: declare: -n: invalid option declare: usage: declare [-afFirtx] [-p] [name[=value] ...]
generating filter 'Source Files' from 278 files

