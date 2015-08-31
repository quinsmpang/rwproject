local totle_minute;

-- 系统启动时执行
function IN_Script_Init()
end

-- 系统每分钟执行一次
function IN_Script_Timer()
end

-- 系统GM指令
function IN_Script_Command( v1, v2, v3, v4, msg, PlayerIdx )
	local temp = os.date("*t", os.time());
	if v1 == 1 then
		
	elseif v1 == 2 then

	end
	return 0;
end

