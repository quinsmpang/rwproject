
cc.FileUtils:getInstance():addSearchPath("src")
cc.FileUtils:getInstance():addSearchPath("res")
cc.FileUtils:getInstance():addSearchPath("res/ui")

--CC_USE_FRAMEWORK = true

-- for module display
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
require "AskSend"
require "AskRecv"
require "EventProtocol"
require "GameManager"
require "Data"

ByteBuffer = my.ByteBuffer
TcpManager = my.TcpManager
GlobalSchedule = my.GlobalSchedule
IniUtil = my.IniUtil
TableUtil = my.TableUtil

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
    
    cc.Director:getInstance():getOpenGLView():setDesignResolutionSize(640, 960, 1)

    -- 游戏初始化成功
    GameManager.onInit();
end


local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
