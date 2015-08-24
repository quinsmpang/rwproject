-- 建筑基础信息
Building = class("Building",function()
    return cc.Node:create()
end)

-- 资源列表
BuildingResList = {
[0]={ res={"building_human_Ground.png"} },  --	空地
[1]={ res={"building_human_TownCenter.png"} },  --	王城/圣殿
[2]={ res={""} },  --	城墙/城墙
[3]={ res={"building_human_Drunkery.png"} },  --  酒馆/灵堂
[4]={ res={"building_human_Market.png"} },  --  市场/市场
[5]={ res={"building_human_GamblingHouse.png"} },  --  赌场/赌场
[6]={ res={"building_human_Library.png"} },  --  图书馆/神圣之地
[7]={ res={"building_human_WishingPool.png"} },  --  许愿池/祭坛
[8]={ res={"building_human_Embassy.png"} },  --  大使馆/幽堂
[9]={ res={"building_human_Smithy.png"} },  --  铁匠铺/铁匠铺
[10]={ res={"building_human_DrillGround.png"} }, --  校场/检阅场	
[11]={ res={"building_human_StoreHouse.png"} }, --  仓库/地窖
[12]={ res={"building_human_WoodFactory_canbe.png","building_human_WoodFactory_over.png"} }, --  木材厂/聚灵器
[13]={ res={"building_human_Farm_ing.png","building_human_Farm_canbe.png","building_human_Farm_over.png"} }, --  农田/魂器
[14]={ res={"building_human_IronFactory.png"} }, --  铁矿厂/炼化室
[15]={ res={"building_human_Mithril.png"} }, --  秘银穴/深坑
[16]={ res={"building_human_House.png"} }, --  民居/地穴
[17]={ res={"building_human_Barracks.png"} }, --  兵营
[18]={ res={"building_human_Stable.png"} }, --  马厩
[19]={ res={"building_human_ShootingRange.png"} }, --  靶场
[20]={ res={"building_human_Tower.png"} }, --  塔楼
[21]={ res={"building_human_Magician.png"} }, --  法师营地
[22]={ res={"building_human_Craftsman.png"} } --  工匠坊--]]
}

-- 构造函数
function Building:ctor()
	self._kind = 0;
	self._timeProgress = nil;
	self._privateNode = nil;
end
 
-- 初始化
function Building:init()
	
	-- 建筑图片
	self._sprite = cc.Sprite:create( "building_human_Ground.png" )
    self._sprite:setPosition( cc.p(0, 0) )
    self:addChild( self._sprite )
	
	-- 等级背景
	self._levelSprite = cc.Sprite:create( "building_human_levelback.png" )
    self._levelSprite:setPosition( cc.p(-60, -60) )
	self._levelSprite:setScale( 0.4 )
	self._levelSprite:setVisible( false )
    self:addChild( self._levelSprite )
	-- 等级
	self._levelLabel = cc.Label:createWithSystemFont("", "Marker Felt", 32)
    self._levelLabel:setAnchorPoint(cc.p(0.5, 0.5))
	self._levelLabel:setPosition(cc.p(36, 60))
	self._levelSprite:addChild(self._levelLabel)
	
	-- 建筑点击事件
    local function onTouchBegan(touch, event)
        local locationInNode = self._sprite:convertToNodeSpace(touch:getLocation())
        local s = self._sprite:getContentSize()
        local rect = cc.rect(0, 0, s.width, s.height)
        if cc.rectContainsPoint(rect, locationInNode) then
			self._beganHit = true;
            return true
        end
        --self._sprite:setColor(cc.c3b(255, 255, 255))
		self:setLocalZOrder(0)
        return false
    end
	local function onTouchMoved( touch, event )
		if self._beganHit == true then
			local distance = cc.pGetDistance( touch:getStartLocation(), touch:getLocation() )
			if distance > 5 then -- 容差
				self._beganHit = false
			end
		end
		return
	end 
	local function onTouchEnded( touch, event )
		if self._beganHit == false then
			return
		end
		local locationInNode = self._sprite:convertToNodeSpace(touch:getLocation())
        local s = self._sprite:getContentSize()
        local rect = cc.rect(0, 0, s.width, s.height)
        if cc.rectContainsPoint(rect, locationInNode) then
			-- 已经确定点击的是这个建筑
			if self._kind == 0 then
				BuildingCreateDlg.open( self:getTag() );
			else
				--self._sprite:setColor(cc.c3b(255, 0, 0))
				self:setLocalZOrder(1)
				BuildingOpDlg.open( self, self:getTag() )
			end
        end
	end
	local listener = cc.EventListenerTouchOneByOne:create()
	listener:setSwallowTouches(false)
	listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
	listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
	listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
	local eventDispatcher = self:getEventDispatcher()
	eventDispatcher:addEventListenerWithSceneGraphPriority( listener, self )
	
    return true;
end

-- 设置基础信息
function Building:setBaseInfo( kind, level )
	self._kind = kind;
	self._sprite:setTexture( BuildingResList[kind]["res"][1] );
	if level <= 1 then
		self._levelSprite:setVisible( false )
	else
		self._levelSprite:setVisible( true )
	end
	self._levelLabel:setString(level)
end

-- 设置时间信息
function Building:setTimeInfo( state, time )
	if state == BUILDING_STATE_NORMAL then
		if self._timeProgress ~= nil then
			--self._timeProgress:removeFromParent();
			self._timeProgress = nil;
		end
		return;
	end
	-- 创建时间进度条
	if state == BUILDING_STATE_CREATE_ING or state == BUILDING_STATE_UPGRADE_ING or state == BUILDING_STATE_DELETE_ING then
		if self._timeProgress == nil then
			self._timeProgress = TimeProgressBar.create()
			self._timeProgress:setPosition( cc.p(0, 0-self._sprite:getContentSize().height/2) )
			self:addChild( self._timeProgress, 1 );
			self._timeProgress:setFinishTime( time, function() 
				
			end );
		end
	end
end

-- 设置属性信息
function Building:setAbilityInfo( abilitys )
	
end


-- 创建
function Building.create()
    local object = Building.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Building
