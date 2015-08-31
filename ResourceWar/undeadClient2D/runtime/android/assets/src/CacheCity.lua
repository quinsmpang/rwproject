-- ��ҳǳػ�������
CityBuildingMax = 256

-- Ψһ����
BUILDING_TownCenter			=	1	--	����/ʥ��
BUILDING_Wall				=	2	--	��ǽ/ʥ��
BUILDING_Drunkery			=	3	--	�ƹ�/����
BUILDING_Market				=	4	--  �г�/�г�
BUILDING_GamblingHouse		=	5	--  �ĳ�/�ĳ�	
BUILDING_Library			=	6	--	ͼ���/��ʥ֮��
BUILDING_WishingPool		=	7	--	��Ը��/��̳
BUILDING_Embassy			=	8	--	��ʹ��/����
BUILDING_Smithy				=	9	--	������/������
BUILDING_DrillGround		=	10	--	У��/���ĳ�
BUILDING_StoreHouse			=	11	--	�ֿ�/�ؽ�
BUILDING_House				=	16	--	���/��Ѩ
-- ��Դ
BUILDING_WoodFactory		=	12	--	ľ�ĳ�/������
BUILDING_Farm				=	13	--	ũ��/����
BUILDING_IronFactory		=	14	--	����/������
BUILDING_Mithril			=	15	--	����Ѩ/���
-- ����
BUILDING_Barracks			=	17	--	��Ӫ
BUILDING_Stable				=	18	--	���
BUILDING_ShootingRange		=	19	--	�г�
BUILDING_Tower				=	20	--	��¥
BUILDING_Magician			=	21	--	��ʦӪ��
BUILDING_Craftsman			=	22	--	������

-- ������ǰ״̬
BUILDING_STATE_NORMAL		= 0 -- ȱʡ״̬
BUILDING_STATE_CREATE_ING	= 1 -- ������
BUILDING_STATE_CREATE_ED	= 2 -- �Ѿ�������ɣ�����ʹ�ã��������������Ľ������
BUILDING_STATE_UPGRADE_ING	= 3 -- ������
BUILDING_STATE_UPGRADE_ED	= 4 -- ������ɣ�����ʹ�ã����������������������
BUILDING_STATE_DELETE_ING	= 5 -- �����
BUILDING_STATE_DELETE_ED	= 6 -- �����ɣ�����ʹ�ã��������������Ĳ���ɹ�


local Building = class("Building");
function Building:ctor()
	self:Init();
end
function Building:Init()
	
	-- ������Ϣ
	self.m_offset		=	0;
	self.m_kind			=	0;
	self.m_level		=	0;
	self.m_state		=	0;
	self.m_finishtime	=	0;
	self.m_abilitys		=	{ 0, 0, 0, 0 };
end
function Building:Clear()
	self:Init();
end

--  �ǳػ�����Ϣ
local City = class("City");
function City:ctor()
	self:Init();
end
function City:Init()
	self.m_wood				=	0;	-- ľ��
	self.m_food				=	0;	-- ��ʳ
	self.m_iron				=	0;	-- ��
	self.m_mithril			=	0;	-- ����
	self.m_gold				=	0;	-- ���
	self.m_towncenter_level	=	0;	-- �������ĵȼ�
	self.m_enter_worldmap	=	0; 	-- �Ƿ�ɽ��������ͼ��1�ɽ���
	self.m_servertime		=	0;	-- ������ʱ���
	self.m_clienttime		=	0;	-- �յ�������ʱ��ʱ�Ŀͻ���ʱ���
	self.m_aclass			=	0;	-- ����
	
	-- �����б�
	self.BuildingList 		=	{};
	for i=1,CityBuildingMax,1 do
		local object = Building.new();
		table.insert( self.BuildingList, object );
	end
	
end

-- ���Ա仯
function City:Set( recvValue )
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

-- ľ�ĸı�
-- EventProtocol.addEventListener( "ChangeWood", function( recvValue ) end )
function City:ChangeWood( nValue, nPath )
	local oldValue = self.m_wood;
	self.m_wood = nValue;
	EventProtocol.dispatchEvent( "ChangeWood", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ��ʳ
-- EventProtocol.addEventListener( "ChangeFood", function( recvValue ) end )
function City:ChangeFood( nValue, nPath )
	local oldValue = self.m_food;
	self.m_food = nValue;
	EventProtocol.dispatchEvent( "ChangeFood", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ��
-- EventProtocol.addEventListener( "ChangeIron", function( recvValue ) end )
function City:ChangeIron( nValue, nPath )
	local oldValue = self.m_iron;
	self.m_iron = nValue;
	EventProtocol.dispatchEvent( "ChangeIron", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ����
-- EventProtocol.addEventListener( "ChangeMithril", function( recvValue ) end )
function City:ChangeMithril( nValue, nPath )
	local oldValue = self.m_mithril;
	self.m_mithril = nValue;
	EventProtocol.dispatchEvent( "ChangeMithril", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ��Ҹı�
-- EventProtocol.addEventListener( "ChangeGold", function( recvValue ) end )
function City:ChangeGold( nValue, nPath )
	local oldValue = self.m_gold;
	self.m_gold = nValue;
	EventProtocol.dispatchEvent( "ChangeGold", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ��ӽ���
function City:AddBuilding( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1; -- lua����1��ʼ
	self.BuildingList[offset].m_offset 	= info.m_offset;
	self.BuildingList[offset].m_kind 	= info.m_kind;
	self.BuildingList[offset].m_level 	= info.m_level;

	BuildingAdd( info.m_offset, info.m_kind, info.m_level );
end

-- ɾ������
function City:DelBuilding( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset]:Clear();
	
	
	BuildingDelete( info.m_offset );
end

-- ���ý���ʱ����Ϣ
function City:SetBuildingTimeInfo( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset].m_state 		= info.m_state;
	self.BuildingList[offset].m_finishtime 	= info.m_time;
	
	local level = self.BuildingList[offset].m_level;
	local id = self.BuildingList[offset].m_kind;
	BuildingSetTime( info.m_offset, info.m_state, info.m_time,  Data.building_upgrade[id][level].sec );
end

-- ���ý����������
function  City:SetBuildingAbility( info )
	if info.m_offset < 0 or info.m_offset >= CityBuildingMax then
		return;
	end
	local offset = info.m_offset + 1;
	self.BuildingList[offset].m_abilitys = info.m_value;

	BuildingRefreshAbility( info.m_offset );
end

-- ��������������ʱ���
function City:SetServerTime( servertime )
	self.m_servertime = servertime;
	self.m_clienttime = os.time();
end

-- ������ʱ�������Ϸ����ʱ����Է�����ʱ���Ϊ׼
-- ��ǰʱ��-�ͻ����յ�������ʱ�����ʱ��Ϊ=����ʱ��
-- ����ʱ��+������ʱ��=��ǰʱ��
function GetServerTime()
	return GetCity().m_servertime + (os.time()-GetCity().m_clienttime);
end

-- ȫ��
G_City = nil;
function GetCity()
	if G_City == nil then
		G_City = City.new();
	end
	return G_City;
end
