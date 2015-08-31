-- 主城
City = class("City",function()
    return cc.Node:create()
end)

-- 构造函数
function City:ctor()
	self._resPickupTagID = 1; -- 资源拾取对象唯一ID
	self._resPickupCallBack = {} -- 资源拾取对象回调
	self._resSchedulerEntry = nil -- 资源定时获取
end

-- 初始化
function City:init()
	self:setName("City")

	-- 监听对象事件
    local function onNodeEvent(event)
        if event == "enter" then
			self._resSchedulerEntry = cc.Director:getInstance():getScheduler():scheduleScriptFunc(function() 
				system_askinfo( ASKINFO_BUILDING, "", 7 );
			end, 2, false)
        elseif event == "exit" then
			if self._resSchedulerEntry then
				cc.Director:getInstance():getScheduler():unscheduleScriptEntry( self._resSchedulerEntry )
			end
        end
    end
    self:registerScriptHandler(onNodeEvent)
	
	 -- 创建滑动层对象
	local function scrollViewDidZoom()
		--local scale = self._scrollViewProxy:getZoomScale()
		--if scale == 0.9 or scale == 1.1 then
			--self._scrollViewProxy:setZoomScaleInDuration( 1.0, 0.2 );
		--end
    end
    self._scrollViewProxy = ScrollViewEx:create()
    self._scrollViewProxy:setViewSize(SCREEN_SIZE)
    self._scrollViewProxy:setPosition(cc.p(0,0))
    self._scrollViewProxy:setDirection(cc.SCROLLVIEW_DIRECTION_BOTH)
    self._scrollViewProxy:ignoreAnchorPointForPosition(true)
    self._scrollViewProxy:setClippingToBounds(true)
    self._scrollViewProxy:setBounceable(false)
	self._scrollViewProxy:setDelegate()
	self._scrollViewProxy:registerScriptHandler(scrollViewDidZoom,cc.SCROLLVIEW_SCRIPT_ZOOM)
    self:addChild(self._scrollViewProxy);
	
	-- 创建地图
	self._cityTmxMap = ccexp.TMXTiledMap:create("human_map.tmx")
		
	-- 设置滑动层
	self._scrollViewProxy:setContainer( self._cityTmxMap )
    self._scrollViewProxy:updateInset()
	
	-- 移动放大地图
	self._scrollViewProxy:setMinScale(0.1)
	self._scrollViewProxy:setMaxScale(1.0)
	self._scrollViewProxy:setZoomScale( 0.5 );
	self._scrollViewProxy:setContentOffsetInDuration(cc.p(-1200, -800), 1);
	self._scrollViewProxy:setZoomScaleInDuration( 1.0, 1 );
	-- 1秒后重设缩放范围
	performWithDelay( self, function(dt)  
		self._scrollViewProxy:setMinScale(0.9)
		self._scrollViewProxy:setMaxScale(1.1)
		-- 主界面显示
		MainLayer.open( true )
	end, 1 );
	
	
	-- 创建建筑层
	self:createBuilding()
	
	-- 创建天空层
	self:createSky();
	
	-- 创建资源拾取层
	self:createResPickup()
    return true;
end

-- 创建建筑层
function City:createBuilding()
	-- 建筑节点
	self._buildingLayer = cc.Node:create();
	self._cityTmxMap:addChild( self._buildingLayer );
	-- 默认建筑列表
	local group = self._cityTmxMap:getObjectGroup("building_layer")
    local objects = group:getObjects()
	for i = 1, #objects, 1 do
		local dict = objects[i]
        if dict == nil then
            break
        end
        
        local x = dict["x"]
        local y = dict["y"]
        local width = dict["width"]
        local height = dict["height"]
		local buildingindex = dict["buildingindex"]

		-- 创建默认建筑
		local building = Building.create()
		building:setTag( buildingindex );
		building:setPosition( cc.p (x+width/2, y+height+height/2) )
		self._buildingLayer:addChild( building )
	end
end

-- 创建天空层
function City:createSky()
	-- 天空节点
	self._skyLayer = cc.Node:create();
	self._cityTmxMap:addChild( self._skyLayer );
	
	-- 龙背影
	self._dragonShadowNode = cc.Node:create();
	self._skyLayer:addChild( self._dragonShadowNode );
	self._dragonShadowFrame = FrameAnimUnit.create( "DragonShadow", nil )
	self._dragonShadowNode:addChild( self._dragonShadowFrame );
	self._dragonShadowFrame:playLoop();
	self._dragonShadowNode:setPosition( cc.p(self._cityTmxMap:getContentSize().width/2, self._cityTmxMap:getContentSize().height/2) )
	-- 龙的逻辑
	local function dragonShadowLogic()
		-- 当前点
		local curPos = cc.p( self._dragonShadowNode:getPosition() );
		-- 移动多少个点
		local moveByPos = cc.p( math.random(-500,500), math.random(-500,500) )
		-- 移动之后的目标点
		local moveToPos = cc.pAdd( curPos, moveByPos )
		-- 计算朝向
		local angle = ppAngle( moveToPos, curPos )
		-- 时间=路程/速度
		local t = cc.pGetDistance( curPos,moveToPos )/50;
		self._dragonShadowNode:runAction(cc.Sequence:create(cc.RotateTo:create( 0.5, angle ),
															cc.MoveTo:create( t, moveToPos ), 
															cc.CallFunc:create(dragonShadowLogic) ) )	
	end
	dragonShadowLogic()
	
	-- 雨
	self._rainEmitter = cc.ParticleRain:create()
	self:addChild( self._rainEmitter )
	self._rainEmitter:setTexture(cc.Director:getInstance():getTextureCache():addImage("rain.png"))
    self._rainEmitter:setLife(6)
	
	-- 云
	
end

-- 创建资源拾取层，因为要有滑动方式拾取
function City:createResPickup()
	-- 拾取层节点
	self._resPickupLayer = cc.Node:create();
	self._cityTmxMap:addChild( self._resPickupLayer );
	self._pickupHit = false;
	local function onTouchBegan(touch, event)
		local  pChildrenArray = self._resPickupLayer:getChildren()
		local len = table.getn(pChildrenArray)
		for i = 1, len, 1 do
			local pObject = pChildrenArray[i]
			local locationInNode = pObject:convertToNodeSpace(touch:getLocation())
			local s = pObject:getContentSize()
			local rect = cc.rect(0, 0, s.width, s.height)
			if cc.rectContainsPoint(rect, locationInNode) then
				-- 如果有一个命中了， 那么移动时候同时检查其它所有对象
				self._pickupHit = true
				-- 回调
				local tagid = pObject:getTag();
				if self._resPickupCallBack[tagid] then
					self._resPickupCallBack[tagid]()
					self._resPickupCallBack[tagid] = nil;
				end
				return true
			end
		end
		return false
	end
	local function onTouchMoved( touch, event )
		if self._pickupHit == true then
			local  pChildrenArray = self._resPickupLayer:getChildren()
			local len = table.getn(pChildrenArray)
			for i = 1, len, 1 do
				local pObject = pChildrenArray[i]
				local locationInNode = pObject:convertToNodeSpace(touch:getLocation())
				local s = pObject:getContentSize()
				local rect = cc.rect(0, 0, s.width, s.height)
				if cc.rectContainsPoint(rect, locationInNode) then
					-- 回调
					local tagid = pObject:getTag();
					if self._resPickupCallBack[tagid] then
						self._resPickupCallBack[tagid]()
						self._resPickupCallBack[tagid] = nil;
					end
					return;
				end
			end
		end
	end 
			
	local listener = cc.EventListenerTouchOneByOne:create()
	listener:setSwallowTouches(true)
	listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
	listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
	self:getEventDispatcher():addEventListenerWithSceneGraphPriority( listener, self._resPickupLayer )
end

-- 添加资源拾取对象
function City:addResPickupObject( node, callback )
	self._resPickupLayer:addChild( node, 0, self._resPickupTagID );
	table.insert( self._resPickupCallBack, self._resPickupTagID, callback );
	self._resPickupTagID = self._resPickupTagID + 1;
end

-- 创建
function City.create()
    local object = City.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return City
