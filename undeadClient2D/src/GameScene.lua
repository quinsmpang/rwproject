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
            if self._schedulerEntry then
                scheduler:unscheduleScriptEntry(self._schedulerEntry)
            end
            collectgarbage("collect")
            
        elseif event == "enterTransitionFinish" then
            self:onEnterTransitionDidFinish()
        end
    end
    self:registerScriptHandler(onNodeEvent)
    return true;
end

-- 场景进入完毕后调用
function GameScene:onEnterTransitionDidFinish()
	-- UI管理器(转换场景后要重新初始化)
	UIManager.init()
	
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
