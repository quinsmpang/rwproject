
cc.FileUtils:getInstance():addSearchPath("src")
cc.FileUtils:getInstance():addSearchPath("res")
cc.FileUtils:getInstance():addSearchPath("res/fonts")
cc.FileUtils:getInstance():addSearchPath("res/data")
cc.FileUtils:getInstance():addSearchPath("res/ui")
cc.FileUtils:getInstance():addSearchPath("res/city")
cc.FileUtils:getInstance():addSearchPath("res/building")
cc.FileUtils:getInstance():addSearchPath("res/anim")
cc.FileUtils:getInstance():addSearchPath("res/particle")
cc.FileUtils:getInstance():addSearchPath("res/actor")
--CC_USE_FRAMEWORK = true

-- for module display deviceUniqueIdentifier
CC_DESIGN_RESOLUTION = {
    width = 960,
    height = 640,
    autoscale = "FIXED_HEIGHT",
    callback = function(framesize)
        local ratio = framesize.width / framesize.height
        if ratio <= 1.34 then
            -- iPad 768*1024(1536*2048) is 4:3 screen
            return {autoscale = "FIXED_WIDTH"}
        end
    end
}

-- cclog
cclog = function(...)
    print(string.format(...))
end

-- 包含文件
require "cocos.init"
require "auto_script/z_client_struct_auto"
require "auto_script/z_client_process_auto"
require "auto_script/z_client_structrecv_auto"
require "auto_script/z_client_structsend_auto"
require "auto_script/z_client_netrecv_auto"
require "auto_script/z_client_netsend_auto"
require "ui/VisibleRect"
require "ui/UIManager"
require "ui/MainLayer"
require "ui/ServerListLayer"
require "ui/TimeProgressBar"
require "ui/BaseDlg"
require "ui/AlertDlg"
require "ui/MsgBoxDlg"
require "ui/NotifyDlg"
require "ui/BuildingCreateDlg"
require "ui/BuildingOpDlg"
require "ui/ChatDlg"
require "Utils"
require "AskSend"
require "AskRecv"
require "ActionFunction"
require "FrameAnimUnit"
require "EventProtocol"
require "GameManager"
require "GameScene"
require "Data"
require "City"
require "Building"
require "CacheCity"
require "CachePlayer"

-- 自定义绑定的类
ByteBuffer = my.ByteBuffer
TcpManager = my.TcpManager
GlobalSchedule = my.GlobalSchedule
IniUtil = my.IniUtil
TableUtil = my.TableUtil
ScrollViewEx = my.ScrollViewEx
PlatformUtils = my.PlatformUtils
EventAnimate = my.EventAnimate
Shake = my.Shake
PixelSprite = my.PixelSprite

-- 自定义回调事件
ScriptHandler_FrameAnimEventCallFun         = 9000000  -- 帧事件回调
ScriptHandler_ScrollViewExTouchEnd 			= 9000010

-- 计时器
scheduler = cc.Director:getInstance():getScheduler()
-- 屏幕分辨率
FRAME_SIZE = cc.Director:getInstance():getOpenGLView():getFrameSize()
-- 屏幕大小
SCREEN_SIZE = cc.Director:getInstance():getWinSize()
-- 设计分辨率可视区域大小
VISIBLE_SIZE = cc.Director:getInstance():getVisibleSize()
-- 设计分辨率可视区域起点
ORIGIN = cc.Director:getInstance():getVisibleOrigin()
-- 设计原始分辨率
DESIGN_SIZE={}
DESIGN_SIZE.width = 640
DESIGN_SIZE.height = 960
-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    return msg
end

local function main()
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    -- initialize director
    local director = cc.Director:getInstance()

    --turn on display FPS
    director:setDisplayStats(true)

    --set FPS. the default value is 1.0/60 if you don't call this
    director:setAnimationInterval(1.0 / 60)		
    cc.Director:getInstance():getOpenGLView():setDesignResolutionSize(DESIGN_SIZE.width, DESIGN_SIZE.height, cc.ResolutionPolicy.NO_BORDER)

    -- 游戏初始化成功
    GameManager.onInit();
end


local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end


--[[void releaseCaches() 
{ 
AnimationCache::destroyInstance(); 
SpriteFrameCache::getInstance()->removeUnusedSpriteFrames(); 
TextureCache::getInstance()->removeUnuserdTextures(); 
} 
 
值得注意的是清理的顺序，我们推荐先清理动画缓存，然后清理精灵帧缓存，最后是纹理缓存。按照引用层级由高到低，以保证释放引用有效。--]]