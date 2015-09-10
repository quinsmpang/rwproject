-- ��ɫ����
-- ��ɫ�������򣬾�����ķ��� 
-- ������ ���� ���� �ƶ� ���� ����
ACTOR_WAY_RIGHTTOP		= 1 -- ����(ԭʼ��Դ)
ACTOR_WAY_RIGHTBOTTOM	= 2 -- ����(���¾���)
ACTOR_WAY_LEFTTOP		= 3 -- ����(���Ͼ���)
ACTOR_WAY_LEFTBOTTOM 	= 4 -- ����(ԭʼ��Դ)

local BaseActor = class("BaseActor",function()
    return cc.Node:create()
end)

-- ���캯��
function BaseActor:ctor()
	self._allowTouch = false;	-- �Ƿ�������
	self._beganHit = false;		-- �Ƿ�����
	
	self._actionName = "idle";			-- ��ǰ����
	self._way = ACTOR_WAY_RIGHTTOP;		-- ��ǰ����
	
	-- �ĸ����򶯻�
	self._animate = { 
	["attack"]={ nil, nil, nil, nil },
	["dead"]={ nil, nil, nil, nil },
	["walk"]={ nil, nil, nil, nil },
	["idle"]={ nil, nil, nil, nil },
	["cheer"]={ nil, nil, nil, nil }, }
	
	-- 4������
	self._action = { 
	["attack"]={ nil, nil, nil, nil },
	["dead"]={ nil, nil, nil, nil },
	["walk"]={ nil, nil, nil, nil },
	["idle"]={ nil, nil, nil, nil },
	["cheer"]={ nil, nil, nil, nil }, }
	
	-- ��Դ����
	self._info = { 
	["attack"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["dead"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 }, 
	["walk"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["idle"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 },
	["cheer"]={ ["plist"]="", ["file"]="", ["fameName"]="", ["frameCount"]=0, ["delay"]=0.0 }, }
	
	-- ���涯��ʹ��
	self._actorType="";
	
end
 
-- ��ʼ��
function BaseActor:init()
	-- ��ɫ����
	if self._allowTouch then
		self._actorSprite = PixelSprite:create( "UI_zhujiemian_liangshi.png" )
	else
		self._actorSprite = cc.Sprite:create( "UI_zhujiemian_liangshi.png" )
	end
    self:addChild( self._actorSprite, 0 )
	
	-- �����׺,�ļ������淶
	local waySuffix={ "RT", nil, nil, "LB" }
	
	-- ����ͼ��
	for k, v in pairs(self._info) do
		-- ����ͼ��
		cc.SpriteFrameCache:getInstance():addSpriteFrames( v["plist"], v["file"] )

		-- ������ԭʼ��Դ
		for index, suffix in pairs(waySuffix) do
			if suffix ~= nil then
				-- ��鶯�����漯
				local animationCacheName = self._actorType.."_"..k.."_"..suffix;
				local animation = cc.AnimationCache:getInstance():getAnimation( animationCacheName )
				if animation == nil then
					-- ���������������������֡������
					local animFrames = {}
					for i=1,v["frameCount"], 1 do
						local frameName = string.format( v["fameName"].."_"..suffix.."_%02d.png", i )
						animFrames[i] = cc.SpriteFrameCache:getInstance():getSpriteFrame( frameName )
					end
					animation = cc.Animation:createWithSpriteFrames( animFrames )
					animation:setDelayPerUnit( v["delay"] )
					if k == "dead" then -- ����Ҫͣ�������һ֡
						animation:setRestoreOriginalFrame( false )
					else
						animation:setRestoreOriginalFrame( true )
					end
					-- ����֡������
					cc.AnimationCache:getInstance():addAnimation( animation, animationCacheName )
				end
				
				--  ����
				self._animate[k][index] = cc.Animate:create( animation )
			end
		end
	end
	return true;
end

-- �ı䳯��
function BaseActor:changeWay( way )
	if self._way == way then
		return;
	end
	self._way = way;
	self:play( self._actionName );
end

--[[-- ����һ��
function BaseActor:playOnce( actionName, isRemove )
	self:playCount( actionName, 1, isRemove );
end

-- ���Ŵ���
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

-- ѭ������
function BaseActor:play( actionName )
	self._actorSprite:stopAllActions();
	self._actionName = actionName;
	
	if self._way == ACTOR_WAY_RIGHTTOP then -- ����(ԭʼ��Դ)
		self._action[actionName][ACTOR_WAY_RIGHTTOP] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:setFlippedX(false);

	elseif self._way == ACTOR_WAY_RIGHTBOTTOM then -- ����(���¾���)
		self._action[actionName][ACTOR_WAY_LEFTBOTTOM] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:setFlippedX(true);
		
	elseif self._way == ACTOR_WAY_LEFTTOP then-- ����(���Ͼ���)
		self._action[actionName][ACTOR_WAY_RIGHTTOP] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_RIGHTTOP] );
		self._actorSprite:setFlippedX(true);
		
	elseif self._way == ACTOR_WAY_LEFTBOTTOM then-- ����(ԭʼ��Դ)
		self._action[actionName][ACTOR_WAY_LEFTBOTTOM] = cc.RepeatForever:create( self._animate[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:runAction( self._action[actionName][ACTOR_WAY_LEFTBOTTOM] );
		self._actorSprite:setFlippedX(false);
		
	end
	
end

-- ֹͣ����
function BaseActor:stop()
	self._actorSprite:stopAllActions();
end

return BaseActor
