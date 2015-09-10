-- 角色基类
-- 角色两个方向，镜像成四方向 
-- 动作： 攻击 死亡 移动 待机 欢呼
ACTOR_WAY_RIGHTTOP		= 1 -- 右上(原始资源)
ACTOR_WAY_RIGHTBOTTOM	= 2 -- 右下(左下镜像)
ACTOR_WAY_LEFTTOP		= 3 -- 左上(右上镜像)
ACTOR_WAY_LEFTBOTTOM 	= 4 -- 左下(原始资源)

local BaseActor = class("BaseActor",function()
    return cc.Node:create()
end)

-- 构造函数
function BaseActor:ctor()
	self._allowTouch = false;	-- 是否允许点击
	self._beganHit = false;		-- 是否点击到
	
	self._actionName = "idle";			-- 当前动作
	self._way = ACTOR_WAY_RIGHTTOP;		-- 当前朝向
	
	-- 四个方向动画
	self._animate = { 
	["attack"]={ nil, nil, nil, nil },
	["dead"]={ nil, nil, nil, nil },
	["walk"]={ nil, nil, nil, nil },
	["idle"]={ nil, nil, nil, nil },
	["cheer"]={ nil, nil, nil, nil }, }
	
	-- 4方向动作
	self._action = { 
	["attack"]={ nil, nil, nil, nil },
	["dead"]={ nil, nil, nil, nil },
	["walk"]={ nil, nil, nil, nil },
	["idle"]={ nil, nil, nil, nil },
	["cheer"]={ nil, nil, nil, nil }, }
	
	-- 资源配置
	self._info = { 
	["attack"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["dead"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 }, 
	["walk"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["idle"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["cheer"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 }, }
	
	-- 缓存动画使用
	self._actorType="";
	
end
 
-- 初始化
function BaseActor:init()
	-- 角色精灵
	if self._allowTouch then
		self._actorSprite = PixelSprite:create( "UI_zhujiemian_liangshi.png" )
	else
		self._actorSprite = cc.Sprite:create( "UI_zhujiemian_liangshi.png" )
	end
    self:addChild( self._actorSprite, 0 )
	
	-- 方向后缀,文件命名规范
	local waySuffix={ "RT", nil, nil, "LB" }
	
	-- 缓存图集
	for k, v in pairs(self._info) do
		-- 缓存图集
		cc.SpriteFrameCache:getInstance():addSpriteFrames( v["plist"], v["file"] )

		-- 两方向原始资源
		for index, suffix in pairs(waySuffix) do
			if suffix ~= nil then
				-- 检查动画缓存集
				local animationCacheName = self._actorType.."_"..k.."_"..suffix;
				local animation = cc.AnimationCache:getInstance():getAnimation( animationCacheName )
				if animation == nil then
					-- 创建这个动作，这个方向的帧动画集
					local animFrames = {}
					for i=1,v["frameCount"], 1 do
						local frameName = string.format( v["fameName"].."_"..suffix.."_%02d.png", i )
						animFrames[i] = cc.SpriteFrameCache:getInstance():getSpriteFrame( frameName )
					end
					animation = cc.Animation:createWithSpriteFrames( animFrames )
					animation:setDelayPerUnit( v["delay"] )
					if k == "dead" then -- 死亡要停留在最后一帧
						animation:setRestoreOriginalFrame( false )
					else
						animation:setRestoreOriginalFrame( true )
					end
					-- 缓存帧动画集
					cc.AnimationCache:getInstance():addAnimation( animation, animationCacheName )
				end
				
				--  动画
				self._animate[k][index] = cc.Animate:create( animation )
			end
		end
	end
	return true;
end

-- 改变朝向
function BaseActor:changeWay( way )
	if self._way == way then
		return;
	end
	self._way = way;
	self:play( self._actionName );
end

--[[-- 播放一次
function BaseActor:playOnce( actionName, isRemove )
	self:playCount( actionName, 1, isRemove );
end

-- 播放次数
function BaseActor:playCount( actionName, count, isRemove )
	if self._action and self._action:isDone() == false then
		return;
	end
	if isRemove then
		self._action = cc.Sequence:create( cc.Repeat:create( self._animate, count ),
										   cc.CallFunc:create( function(sender) sender:removeFromParent() end) )
	else
		self._action = cc.Repeat:create( self._animate, count )
	end
	self._actorSprite:runAction( self._action );
end--]]

-- 循环播放
function BaseActor:play( actionName )
	self._actorSprite:stopAllActions();
	self._actionName = actionName;
	
	if self._way == ACTOR_WAY_RIGHTTOP then -- 右上(原始资源)
		self._action[actionName][ACTOR_WAY_RIGHTTOP] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:setFlippedX(false);

	elseif self._way == ACTOR_WAY_RIGHTBOTTOM then -- 右下(左下镜像)
		self._action[actionName][ACTOR_WAY_LEFTBOTTOM] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:setFlippedX(true);
		
	elseif self._way == ACTOR_WAY_LEFTTOP then-- 左上(右上镜像)
		self._action[actionName][ACTOR_WAY_RIGHTTOP] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:setFlippedX(true);
		
	elseif self._way == ACTOR_WAY_LEFTBOTTOM then-- 左下(原始资源)
		self._action[actionName][ACTOR_WAY_LEFTBOTTOM] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:setFlippedX(false);
		
	end
	
end

-- 停止播放
function BaseActor:stop()
	self._actorSprite:stopAllActions();
end

return BaseActor
