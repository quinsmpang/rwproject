-- 主游戏场景
local GameScene = class("GameScene",function()
    return cc.Scene:create()
end)

-- 构造
function GameScene:ctor()
    
end

-- 初始化
function GameScene:init()

	-- 监听事件
    local function onNodeEvent(event)
        if event == "enter" then
			
        elseif event == "exit" then
			print( "游戏场景释放前LUA使用内存："..(collectgarbage("count")/1024).."M" )
            collectgarbage("collect")
			print( "游戏场景释放后LUA使用内存："..(collectgarbage("count")/1024).."M" )
        elseif event == "enterTransitionFinish" then
            self:onEnterTransitionDidFinish()
			-- 场景创建完毕，发送进入游戏
			netsend_entergame_C( GetPlayer().m_actorid );
			print( "角色进入游戏："..GetPlayer().m_actorid )
			print( "游戏场景启动后LUA使用内存："..(collectgarbage("count")/1024).."M" )
			print(cc.Director:getInstance():getTextureCache():getCachedTextureInfo())
        end
    end
    self:registerScriptHandler(onNodeEvent)
    return true;
end

-- 场景进入完毕后调用
function GameScene:onEnterTransitionDidFinish()
	
	-- 主界面创建后先隐藏，动画之后显示
	MainLayer.open( false )
	
	-- 主城地图
	self._city = City.create()
	self:addChild( self._city );
end

function GameScene.create()
    local scene = GameScene.new()
    if scene:init() == false then
        return nil;
    end
    return scene
end

return GameScene


