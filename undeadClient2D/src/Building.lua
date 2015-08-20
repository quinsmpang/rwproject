-- 建筑
Building = class("Building",function()
    return cc.Node:create()
end)

-- 构造函数
function Building:ctor()
end

-- 初始化
function Building:init( buildingid )
	
	-- 建筑图片
	self._sprite = cc.Sprite:create( "City_renlei_talou1.png" )
    self._sprite:setPosition( cc.p(0, 0) )
    self:addChild( self._sprite )
	
	-- 建筑点击事件
    local function onTouchBegan(touch, event)
        local locationInNode = self._sprite:convertToNodeSpace(touch:getLocation())
        local s = self._sprite:getContentSize()
        local rect = cc.rect(0, 0, s.width, s.height)
        if cc.rectContainsPoint(rect, locationInNode) then
            self._sprite:setColor(cc.c3b(255, 0, 0))
            return true
        end
        self._sprite:setColor(cc.c3b(255, 255, 255))
        return false
    end
	
	local function onTouchMoved( touch, event )
		return true
	end 
	local function onTouchEnded( touch, event )
	end
	local listener = cc.EventListenerTouchOneByOne:create()
	listener:setSwallowTouches(false)
	listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
	listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
	local eventDispatcher = self:getEventDispatcher()
	eventDispatcher:addEventListenerWithSceneGraphPriority( listener, self )
	
    return true;
end

-- 创建
function Building.create( buildingid )
    local object = Building.new();
    if object:init( buildingid ) == false then
        return nil;
    end
    return object;
end
return Building
