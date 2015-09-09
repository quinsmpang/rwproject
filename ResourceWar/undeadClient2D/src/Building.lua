-- 建筑
Building = class("Building",function()
    return cc.Node:create()
end)

-- 资源列表
BuildingResList = {
[0]={ filename="0_Human_Empty", res={"building_human_Ground.png"} },  --	空地
[1]={ filename="1_Human_TownCenter", res={"building_human_TownCenter.png"} },  --	王城/圣殿
[2]={ filename="2_Human_Wall", res={""} },  --	城墙/城墙
[3]={ filename="3_Human_Drunkery", res={"building_human_Drunkery.png"} },  --  酒馆/灵堂
[4]={ filename="4_Human_Market", res={"building_human_Market.png"} },  --  市场/市场
[5]={ filename="5_Human_GamblingHouse", res={"building_human_GamblingHouse.png"} },  --  赌场/赌场
[6]={ filename="6_Human_Library", res={"building_human_Library.png"} },  --  图书馆/神圣之地
[7]={ filename="7_Human_WishingPool", res={"building_human_WishingPool.png"} },  --  许愿池/祭坛
[8]={ filename="8_Human_Embassy", res={"building_human_Embassy.png"} },  --  大使馆/幽堂
[9]={ filename="9_Human_Smithy", res={"building_human_Smithy.png"} },  --  铁匠铺/铁匠铺
[10]={ filename="10_Human_DrillGround", res={"building_human_DrillGround.png"} }, --  校场/检阅场	
[11]={ filename="11_Human_StoreHouse", res={"building_human_StoreHouse.png"} }, --  仓库/地窖
[12]={ filename="12_Human_WoodFactory", res={"building_human_WoodFactory_canbe.png","building_human_WoodFactory_over.png"} }, --  木材厂/聚灵器
[13]={ filename="13_Human_Farm", res={"building_human_Farm_ing.png","building_human_Farm_canbe.png","building_human_Farm_over.png"} }, --  农田/魂器
[14]={ filename="14_Human_IronFactory", res={"building_human_IronFactory.png"} }, --  铁矿厂/炼化室
[15]={ filename="15_Human_Mithril", res={"building_human_Mithril.png"} }, --  秘银穴/深坑
[16]={ filename="16_Human_House", res={"building_human_House.png"} }, --  民居/地穴
[17]={ filename="17_Human_Barracks", res={"building_human_Barracks.png"} }, --  兵营
[18]={ filename="18_Human_Stable", res={"building_human_Stable.png"} }, --  马厩
[19]={ filename="19_Human_ShootingRange", res={"building_human_ShootingRange.png"} }, --  靶场
[20]={ filename="20_Human_Tower", res={"building_human_Tower.png"} }, --  塔楼
[21]={ filename="21_Human_Magician", res={"building_human_Magician.png"} }, --  法师营地
[22]={ filename="22_Human_Craftsman", res={"building_human_Craftsman.png"} } --  工匠坊
}

-- 构造函数
function Building:ctor()
	self._object = nil
end
 
-- 初始化
function Building:init()
	-- 初始创建空地表
	local object = require( "building/"..BuildingResList[BUILDING_EMPTY]["filename"] )
	self._object = object.create()
	self:addChild( self._object );
	return true;
end

-- 设置基础信息
function Building:setBuildingIndex( buildingindex )
	self:setTag( buildingindex );
	self._object:setTag( buildingindex )
end

-- 设置基础信息
function Building:setBaseInfo( kind, level )
	if self._object._kind ~= kind then
		local object = require( "building/"..BuildingResList[kind]["filename"] )
		if object then
			self._object:removeFromParent();
			self._object = object.create()
			self._object:setTag( self:getTag() )
			self:addChild( self._object );
		end
	end
	self._object:setBaseInfo( level )
end

-- 设置时间信息
function Building:setTimeInfo( state, time )
	self._object:setTimeInfo( state, time )
end

-- 设置属性信息
function Building:setAbilityInfo( abilitys )
	self._object:setAbilityInfo( abilitys )
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
