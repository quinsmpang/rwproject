ScriptHandler_FrameAnimEventCallFun        = 9000000  -- 帧事件回调

-- 帧动画配置列表
local FrameAnimInfoList = { 
["WorkerSquat"] = { fileName="human_worker.png", plistName="human_worker.plist", fameName="human_worker_squat",  frameCount=6, delay=0.1 }, -- 工人蹲下建造
["WorkerStand"] = { fileName="human_worker.png", plistName="human_worker.plist", fameName="human_worker_stand",  frameCount=6, delay=0.1 }, -- 工人蹲下建造
["DragonShadow"] = { fileName="dragon_shadow.pvr.ccz", plistName="dragon_shadow.plist", fameName="dragon_shadow",  frameCount=6, delay=0.3 }, -- 龙背影
}


-- 帧动画单元，封装了帧动画
FrameAnimUnit = class("FrameAnimUnit",function()
    return cc.Sprite:create()
end)

-- 构造函数
function FrameAnimUnit:ctor()
	self._animation = nil;
	self._animate = nil;
	self._action = nil;
end
 
-- 初始化
function FrameAnimUnit:init( name, eventListener )	
	-- 获取配置信息
	local info = FrameAnimInfoList[name]
	
	-- 缓存图集
    cc.SpriteFrameCache:getInstance():addSpriteFrames( info["plistName"], info["fileName"] )
	
	-- 检查动画缓存
	self._animation = cc.AnimationCache:getInstance():getAnimation( name )
	if self._animation == nil then
		-- 创建帧动画
		local animFrames = {}
		for i=1,info["frameCount"], 1 do
			local frameName = string.format( info["fameName"].."_%02d.png", i )
			animFrames[i] = cc.SpriteFrameCache:getInstance():getSpriteFrame( frameName )
		end
		self._animation = cc.Animation:createWithSpriteFrames( animFrames )
		self._animation:setDelayPerUnit( info["delay"] )
		self._animation:setRestoreOriginalFrame( true )
		
		-- 缓存帧动画
		cc.AnimationCache:getInstance():addAnimation( self._animation, name )
	end
	
	-- 创建动作,如果有事件监听,就用自定义带有事件监听功能的帧动画组件
	if eventListener then
		self._animate = EventAnimate:create( self._animation )
		self._animate:registerScriptHandlerSelf( eventListener, ScriptHandler_FrameAnimEventCallFun );
	else
		self._animate = cc.Animate:create( self._animation )
	end
    return true
end

-- 播放一次
function FrameAnimUnit:playOnce( isRemove )
	self:playCount( 1, isRemove );
end

-- 播放次数
function FrameAnimUnit:playCount( count, isRemove )
	if self._action and self._action:isDone() == false then
		return;
	end
	if isRemove then
		self._action = cc.Sequence:create( cc.Repeat:create( self._animate, count ),
										   cc.CallFunc:create( function(sender) sender:removeFromParent() end) )
	else
		self._action = cc.Repeat:create( self._animate, count )
	end
	self:runAction( self._action );
end

-- 循环播放
function FrameAnimUnit:playLoop()
	if self._action and self._action:isDone() == false then
		return;
	end
	self._action = cc.RepeatForever:create( self._animate )
	self:runAction( self._action );
end

-- 停止播放
function FrameAnimUnit:stop()
	self:stopAllActions();
end

-- 创建
function FrameAnimUnit.create( name, eventListener )
    local object = FrameAnimUnit.new();
    if object:init( name, eventListener ) == false then
        return nil;
    end
    return object;
end
return FrameAnimUnit
