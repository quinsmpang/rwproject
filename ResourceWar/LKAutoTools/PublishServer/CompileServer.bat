rem 操作功能：将svn中的服务器代码以及lua脚本上传到内网服务器101进行编译

set SVN_HOST="https://192.168.16.102/svn/undead"
set LOCAL_PROJDIR="./undeadServer"
set REMOTE_IP="192.168.16.101"
set REMOTE_HOST="/192.168.16.101"

rem 1.先从svn中导出最新版本代码到本地XxServer目录
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/undeadServer/GameServer/AutoS %LOCAL_PROJDIR%/project/GameServer/AutoS
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/undeadServer/GameServer/include %LOCAL_PROJDIR%/project/GameServer/include
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/undeadServer/GameServer/src %LOCAL_PROJDIR%/project/GameServer/src

"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/undeadServer/exec/script %LOCAL_PROJDIR%/exec/script

"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/LKEngine/LKNetServer/include %LOCAL_PROJDIR%/project/LKNetServer/include
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/LKEngine/LKNetServer/src %LOCAL_PROJDIR%/project/LKNetServer/src

"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/LKEngine/LKUtils/include %LOCAL_PROJDIR%/project/LKUtils/include
"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/LKEngine/LKUtils/src %LOCAL_PROJDIR%/project/LKUtils/src

"C:\Program Files\TortoiseSVN\bin\svn.exe" --force export %SVN_HOST%/LKEngine/Lua/src %LOCAL_PROJDIR%/project/Lua/src

rem 2.执行ant脚本进行上传并编译
ant -buildfile	CompileServer.xml -verbose

pause