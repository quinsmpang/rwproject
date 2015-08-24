-- 主城
City = class("City",function()
    return cc.Node:create()
end)

-- 构造函数
function City:ctor()
end

-- 初始化
function City:init()
	self:setName("City")

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
	-- 创建建筑层
	self._buildingLayer = cc.Node:create();
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
	self._cityTmxMap:addChild( self._buildingLayer );

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
	end, 1 );
    return true;
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
