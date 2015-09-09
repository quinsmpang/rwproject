-- 建筑基类
local BaseBuilding = class("BaseBuilding",function()
    return cc.Node:create()
end)

-- 构造函数
function BaseBuilding:ctor()
	self._aclass = 0;			-- 建筑种族
	self._kind = 0;				-- 建筑种类
	self._beganHit = false;		-- 是否点击到
end
 
-- 初始化
function BaseBuilding:init()
	-- 建筑精灵
	self._buildingSprite = PixelSprite:create( "building_human_Ground.png" )
    self._buildingSprite:setPosition( cc.p(0, 0) )
    self:addChild( self._buildingSprite, 0 )
	
	-- 建筑等级框
	self._levelSprite = cc.Sprite:create( "building_human_levelback.png" )
    self._levelSprite:setPosition( cc.p(-60, -60) )
	self._levelSprite:setScale( 0.4 )
	self._levelSprite:setVisible( false )
    self:addChild( self._levelSprite, 10 )
	
	-- 建筑等级标签
	self._levelLabel = cc.Label:createWithSystemFont( "", "", 32 )
    self._levelLabel:setAnchorPoint( cc.p(0.5, 0.5) )
	self._levelLabel:setPosition( cc.p(36, 60) )
	self._levelSprite:addChild( self._levelLabel )
	
	
	-- 建筑点击事件
    local function onTouchBegan(touch, event)
        if self._buildingSprite:checkHit( touch:getLocation() ) then
			self._beganHit = true;
            return true
        end
		self:getParent():setLocalZOrder( 0 );
        return false
    end
	
	-- 在建筑上稍微移动一点，就认为没有点击上
	local function onTouchMoved( touch, event )
		if self._beganHit == true then
			local distance = cc.pGetDistance( touch:getStartLocation(), touch:getLocation() )
			if distance > 5 then -- 容差
				self._beganHit = false;
			end
		end
		return
	end 
	
	-- 触摸抬起的时候检测究竟是不是点击了这个建筑
	local function onTouchEnded( touch, event )
		if self._beganHit == false then
			return
		end
        if self._buildingSprite:checkHit( touch:getLocation() ) then
			-- 已经确定点击的是这个建筑
			self:getParent():setLocalZOrder( 1 );
			self:hit();
        end
	end
	-- 注册触摸事件
	local listener = cc.EventListenerTouchOneByOne:create()
	listener:setSwallowTouches(false)
	listener:registerScriptHandler( onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN );
	listener:registerScriptHandler( onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED );
	listener:registerScriptHandler( onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED );
	self:getEventDispatcher():addEventListenerWithSceneGraphPriority( listener, self );
end

-- 点击命中
function BaseBuilding:hit()
	if self._kind == 0 then
		BuildingCreateDlg.open( self:getTag() );
	else
		BuildingOpDlg.open( self, self:getTag() );
	end
end

-- 设置基础信息
function BaseBuilding:setBaseInfo( level )
	self._buildingSprite:setTexture( BuildingResList[self._kind]["res"][1] );
	if level < 1 then
		self._levelSprite:setVisible( false )
	else
		self._levelSprite:setVisible( true )
	end
	self._levelLabel:setString(level)
end

-- 设置时间信息
function BaseBuilding:setTimeInfo( state, time )
	-- 缺省状态
	if state == BUILDING_STATE_NORMAL then
		return;
	end
	-- 正在建造，升级，拆除中
	if state == BUILDING_STATE_CREATE_ING or state == BUILDING_STATE_UPGRADE_ING or state == BUILDING_STATE_DELETE_ING then
		-- 时间进度条
		local timeProgress = self:getChildByName("timeProgress");
		if timeProgress == nil then
			timeProgress = TimeProgressBar.create()
			timeProgress:setName("timeProgress");
			timeProgress:setPosition( cc.p(0, 0-self._buildingSprite:getContentSize().height/2) )
			self:addChild( timeProgress, 100 );
			timeProgress:setFinishTime( time, function() 
				-- 时间到，删除工人
				local worker = self:getChildByName("worker");
				if worker then
					worker:removeFromParent();
				end
			end );
			
			-- 工人
			local worker = FrameAnimUnit.create( "WorkerStand", function( sender, frame, numberOfFrames )
				
			end )
			worker:setName("worker");
			self:addChild( worker );
			worker:playLoop();
		end
	end
end

-- 设置属性信息（子类单独覆盖实现）
function BaseBuilding:setAbilityInfo( abilitys )

end


return BaseBuilding
