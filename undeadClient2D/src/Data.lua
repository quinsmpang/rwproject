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

-- 配置数据初始化
function Data.OnInit()
	
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