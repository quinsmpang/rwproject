-- 玩家城池缓存数据
CityBuildingMax = 36

-- 唯一建筑
BUILDING_EMPTY				=	0	--	空地表
BUILDING_TownCenter			=	1	--	王城/圣殿
BUILDING_Wall				=	2	--	城墙/圣殿
BUILDING_Drunkery			=	3	--	酒馆/灵堂
BUILDING_Market				=	4	--  市场/市场
BUILDING_GamblingHouse		=	5	--  赌场/赌场	
BUILDING_Library			=	6	--	图书馆/神圣之地
BUILDING_WishingPool		=	7	--	许愿池/祭坛
BUILDING_Embassy			=	8	--	大使馆/幽堂
BUILDING_Smithy				=	9	--	铁匠铺/铁匠铺
BUILDING_DrillGround		=	10	--	校场/检阅场
BUILDING_StoreHouse			=	11	--	仓库/地窖

-- 资源
BUILDING_WoodFactory		=	12	--	木材厂/聚灵器
BUILDING_Farm				=	13	--	农田/魂器
BUILDING_IronFactory		=	14	--	铁矿厂/炼化室
BUILDING_Mithril			=	15	--	秘银穴/深坑
BUILDING_House				=	16	--	民居/地穴

-- 部队
BUILDING_Barracks			=	17	--	兵营
BUILDING_Stable				=	18	--	马厩
BUILDING_ShootingRange		=	19	--	靶场
BUILDING_Tower				=	20	--	塔楼
BUILDING_Magician			=	21	--	法师营地
BUILDING_Craftsman			=	22	--	工匠坊

-- 建筑当前状态
BUILDING_STATE_NORMAL		= 0 -- 缺省状态
BUILDING_STATE_CREATE_ING	= 1 -- 建造中
BUILDING_STATE_CREATE_ED	= 2 -- 已经建造完成，特殊使用，需点击方可真正的建造完毕
BUILDING_STATE_UPGRADE_ING	= 3 -- 升级中
BUILDING_STATE_UPGRADE_ED	= 4 -- 升级完成，特殊使用，需点击方可真正的升级完毕
BUILDING_STATE_DELETE_ING	= 5 -- 拆除中
BUILDING_STATE_DELETE_ED	= 6 -- 拆除完成，特殊使用，需点击方可真正的拆除成功


local CacheBuilding = class("CacheBuilding");
function CacheBuilding:ctor()
	self:Init();
end
function CacheBuilding:Init()
	
	-- 基本信息
	self.m_offset		=	0;
	self.m_kind			=	0;
	self.m_level		=	0;
	self.m_state		=	0;
	self.m_finishtime	=	0;
	self.m_abilitys		=	{ 0, 0, 0, 0 };
end
function CacheBuilding:Clear()
	self:Init();
end

--  城池缓存信息
local CacheCity = class("CacheCity");
function CacheCity:ctor()
	self:Init();
end
function CacheCity:Init()
	self.m_wood				=	0;	-- 木材
	self.m_food				=	0;	-- 粮食
	self.m_iron				=	0;	-- 铁
	self.m_mithril			=	0;	-- 秘银
	self.m_gold				=	0;	-- 金币
	self.m_towncenter_level	=	0;	-- 城镇中心等级
	self.m_enter_worldmap	=	0; 	-- 是否可进入世界地图，1可进入
	self.m_servertime		=	0;	-- 服务器时间戳
	self.m_clienttime		=	0;	-- 收到服务器时间时的客户端时间戳
	self.m_aclass			=	0;	-- 种族
	
	-- 建筑列表
	self.BuildingList 		=	{};
	for i=1,CityBuildingMax,1 do
		local object = CacheBuilding.new();
		table.insert( self.BuildingList, object );
	end
	
end

-- 属性变化
function CacheCity:Set( recvValue )
	self.m_wood				=	recvValue.m_wood;
	self.m_food				=	recvValue.m_food;
	self.m_iron				=	recvValue.m_iron;
	self.m_mithril			=	recvValue.m_mithril;
	self.m_gold				=	recvValue.m_gold;
	self.m_towncenter_level	=	recvValue.m_towncenter_level;
	self.m_enter_worldmap	=	recvValue.m_enter_worldmap;

	self:ChangeWood( recvValue.m_wood, 0 );
	self:ChangeFood( recvValue.m_food, 0 );
	self:ChangeIron( recvValue.m_iron, 0 );
	self:ChangeMithril( recvValue.m_mithril, 0 );
	self:ChangeGold( recvValue.m_gold, 0 );
end

-- 木材改变
-- EventProtocol.addEventListener( "ChangeWood", function( recvValue ) end )
function CacheCity:ChangeWood( nValue, nPath )
	local oldValue = self.m_wood;
	self.m_wood = nValue;
	EventProtocol.dispatchEvent( "ChangeWood", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 粮食
-- EventProtocol.addEventListener( "ChangeFood", function( recvValue ) end )
function CacheCity:ChangeFood( nValue, nPath )
	local oldValue = self.m_food;
	self.m_food = nValue;
	EventProtocol.dispatchEvent( "ChangeFood", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 铁
-- EventProtocol.addEventListener( "ChangeIron", function( recvValue ) end )
function CacheCity:ChangeIron( nValue, nPath )
	local oldValue = self.m_iron;
	self.m_iron = nValue;
	EventProtocol.dispatchEvent( "ChangeIron", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 秘银
-- EventProtocol.addEventListener( "ChangeMithril", function( recvValue ) end )
function CacheCity:ChangeMithril( nValue, nPath )
	local oldValue = self.m_mithril;
	self.m_mithril = nValue;
	EventProtocol.dispatchEvent( "ChangeMithril", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 金币改变
-- EventProtocol.addEventListener( "ChangeGold", function( recvValue ) end )
function CacheCity:ChangeGold( nValue, nPath )
	local oldValue = self.m_gold;
	self.m_gold = nValue;
	EventProtocol.dispatchEvent( "ChangeGold", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 添加建筑
function CacheCity:AddBuilding( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1; -- lua索引1开始
	self.BuildingList[offset].m_offset 	= info.m_offset;
	self.BuildingList[offset].m_kind 	= info.m_kind;
	self.BuildingList[offset].m_level 	= info.m_level;

	-- 设置显示
	if GameManager.gameScence then
		local building = GameManager.gameScence._city._buildingLayer:getChildByTag( info.m_offset )
		if building then
			building:setBaseInfo( info.m_kind, info.m_level )
		end
	end
end

-- 删除建筑
function CacheCity:DelBuilding( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset]:Clear();
	
	-- 设置显示
	if GameManager.gameScence then
		local building = GameManager.gameScence._city._buildingLayer:getChildByTag( info.m_offset )
		if building then
			building:setBaseInfo( 0, 0 )
		end
	end
end

-- 设置建筑时间信息
function CacheCity:SetBuildingTimeInfo( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset].m_state 		= info.m_state;
	self.BuildingList[offset].m_finishtime 	= info.m_time;
	
	local level = self.BuildingList[offset].m_level;
	local id = self.BuildingList[offset].m_kind;

	-- 设置显示
	if GameManager.gameScence then
		local building = GameManager.gameScence._city._buildingLayer:getChildByTag( info.m_offset )
		if building then
			building:setTimeInfo( info.m_state, info.m_time )
		end
	end
end

-- 设置建筑相关数据
function  CacheCity:SetBuildingAbility( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset].m_abilitys = info.m_value;

	-- 设置显示
	if GameManager.gameScence then
		local building = GameManager.gameScence._city._buildingLayer:getChildByTag( info.m_offset )
		if building then
			building:setAbilityInfo( info.m_value )
		end
	end
end

-- 服务器发过来的时间戳
function CacheCity:SetServerTime( servertime )
	self.m_servertime = servertime;
	self.m_clienttime = os.time();
end

-- 服务器时间戳，游戏所有时间均以服务器时间戳为准
-- 当前时间-客户端收到服务器时间戳的时间为=流逝时间
-- 流逝时间+服务器时间=当前时间
function GetServerTime()
	return GetCacheCity().m_servertime + (os.time()-GetCacheCity().m_clienttime);
end

-- 全局
G_CacheCity = nil;
function GetCacheCity()
	if G_CacheCity == nil then
		G_CacheCity = CacheCity.new();
	end
	return G_CacheCity;
end
