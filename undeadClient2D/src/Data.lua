-- 数据
Data = {};
local this = Data;


-- 本地化文字表
Data.localize = {};

-- 使用语言
Data.language = 0;

-- 当前平台（指运营渠道）
Data.platid = 1;

-- 服务器列表
Data.serverinfo = nil;
-- 当前服务器信息
Data.curServerinfo = nil;

-- 建筑信息本地表，人类
Data.building_human={};

-- 建筑信息本地表，死灵
Data.building_undead={};

-- 建筑信息表，升级建造信息
Data.building_upgrade={};

-- 配置数据初始化 第一级
function Data.OnFristInit()
	
	-- 玩家选择的语言
	Data.language = cc.UserDefault:getInstance():getIntegerForKey("LANGUAGE")
	if Data.language == 0 then
		Data.language = 1
		cc.UserDefault:getInstance():setIntegerForKey( "LANGUAGE", Data.language );
	end
	
	-- 本地化文字表初始化
	local _tableUtil = TableUtil:createFromFile( "localize.txt" );
	if _tableUtil then
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local id = _tableUtil:GetValue( records, 0 );
			local text = "";
			text = _tableUtil:GetValue( records, Data.language )
			table.insert(Data.localize, id, text);
		end
	end
end	

-- 配置数据初始化 第二级
function Data.OnSecondInit()
	
	-- 建筑升级信息表初始化
	_tableUtil = TableUtil:createFromFile( "building_upgrade.txt" );
	if _tableUtil then
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			if Data.building_upgrade[kind] == nil then
				table.insert( Data.building_upgrade, kind, {} );
			end
		end
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			local level = tonumber( _tableUtil:GetValue( records, 1 ) );
			table.insert( Data.building_upgrade[kind], level, {
				limit_kind1		= tonumber( _tableUtil:GetValue( records, 3 ) ),
				limit_lv1		= tonumber( _tableUtil:GetValue( records, 4 ) ),
				limit_kind2		= tonumber( _tableUtil:GetValue( records, 5 ) ),
				limit_lv2		= tonumber( _tableUtil:GetValue( records, 6 ) ),
				restype1		= tonumber( _tableUtil:GetValue( records, 7 ) ),
				resvalue1		= tonumber( _tableUtil:GetValue( records, 8 ) ),
				restype2		= tonumber( _tableUtil:GetValue( records, 9 ) ),
				resvalue2		= tonumber( _tableUtil:GetValue( records, 10 ) ),
				restype3		= tonumber( _tableUtil:GetValue( records, 11 ) ),
				resvalue3		= tonumber( _tableUtil:GetValue( records, 12 ) ),
				restype4		= tonumber( _tableUtil:GetValue( records, 13 ) ),
				resvalue4		= tonumber( _tableUtil:GetValue( records, 14 ) ),
				worker			= tonumber( _tableUtil:GetValue( records, 15 ) ),
				sec				= tonumber( _tableUtil:GetValue( records, 16 ) ),
				value1			= tonumber( _tableUtil:GetValue( records, 17 ) ),
				value2			= tonumber( _tableUtil:GetValue( records, 18 ) ),
				value3			= tonumber( _tableUtil:GetValue( records, 19 ) ),
				value4			= tonumber( _tableUtil:GetValue( records, 20 ) ),
				} );
		end
	end
	
	-- 建筑信息本地资源表，人类初始化
	_tableUtil = TableUtil:createFromFile( "building_human.txt" );
	if _tableUtil then
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			if Data.building_human[kind] == nil then
				table.insert( Data.building_human, kind, {} );
			end
		end
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			local level = tonumber( _tableUtil:GetValue( records, 1 ) );
			table.insert( Data.building_human[kind], level, {
				nameid 			= tonumber( _tableUtil:GetValue( records, 3 ) ),
				class 			= tonumber( _tableUtil:GetValue( records, 4 ) ),
				prefab 			= _tableUtil:GetValue( records, 5 ),
				sprite 			= _tableUtil:GetValue( records, 6 ),
				desc 			= tonumber( _tableUtil:GetValue( records, 7 ) ),
				} );
		end
	end

	-- 建筑信息本地资源表，亡灵初始化
	_tableUtil = TableUtil:createFromFile( "building_undead.txt" );
	if _tableUtil then
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			if Data.building_undead[kind] == nil then
				table.insert( Data.building_undead, kind, {} );
			end
		end
		for records = 0, _tableUtil:GetRecordsNum()-1, 1 do
			local kind = tonumber( _tableUtil:GetValue( records, 0 ) );
			local level = tonumber( _tableUtil:GetValue( records, 1 ) );
			table.insert( Data.building_undead[kind], level, {
				nameid 			= tonumber( _tableUtil:GetValue( records, 3 ) ),
				class 			= tonumber( _tableUtil:GetValue( records, 4 ) ),
				prefab 			= _tableUtil:GetValue( records, 5 ),
				sprite 			= _tableUtil:GetValue( records, 6 ),
				desc 			= tonumber( _tableUtil:GetValue( records, 7 ) ),
				} );
		end
	end
end

-- 配置数据析构
function Data.OnDestroy()
	Data.localize={};
end

-- 获取本地化文字
function GetLocalizeText( id )
	return Data.localize[id];
end

-- 获取服务器信息
function GetServerInfo( serverid )
	for key, value in ipairs(Data.serverinfo["list"]) do
		if tonumber(serverid) == tonumber(value["id"]) then
			return value
		end
	end
	return nil;
end