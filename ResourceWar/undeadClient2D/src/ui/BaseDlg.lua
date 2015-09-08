-- 对话框基类
local BaseDlg = class("BaseDlg",function()
    return cc.Node:create()
end)

-- 构造函数
function BaseDlg:ctor()
	--屏蔽层
	local function onTouchBegan( touch, event )
		return true
	end
	self._listener = cc.EventListenerTouchOneByOne:create()
	self._listener:registerScriptHandler( onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN )
	self._listener:setSwallowTouches( true )
	self:getEventDispatcher():addEventListenerWithSceneGraphPriority( self._listener, self )
end

-- 打开
function BaseDlg:_open()
	self:setVisible( true );
	self._listener:setSwallowTouches( true )
end

-- 关闭
function BaseDlg:_close()
	self:setVisible( false );
	self._listener:setSwallowTouches( false )
end

return BaseDlg