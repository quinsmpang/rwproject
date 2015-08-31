-- 数据
Data = {};
local this = Data;


-- 本地化文字表
Data.localize = {};

-- 使用语言
Data.language = 0;

-- 当前平台（指运营渠道）
Data.platid = 1;


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

