-- 工具

-- 获取时间格式
function GetTimeFomat( totalsecond )
	if totalsecond <= 0 then
		return ""
	end
	local day = math.floor( totalsecond / 86400 );
	local hour = math.floor(math.floor(totalsecond/3600)%24);
	local minitue = math.floor(math.floor(totalsecond/60)%60);
	local second = math.floor(totalsecond%60);
	
	local szTime = "";
	if day > 0 then
		szTime = szTime .. day .. "d";
		if hour > 0 then
			szTime = szTime .. " " .. hour .. "h";
		end
		return szTime;
	end	

	if hour > 0 then
		szTime = szTime .. hour .. "h";
		if minitue > 0 then
			szTime = szTime .. " " .. minitue .. "m";
		end
		return szTime;
	end

	if minitue > 0 then
		szTime = szTime .. minitue .. "m";
		if second > 0 then
			szTime = szTime .. " " .. second .. "s";
		end
		return szTime;
	end	

	if second > 0 then
		szTime = szTime .. second .. "s";
	end
	return szTime;
end

-- 16进制颜色转换
--[[function my.c3b( colorHex )
	local _r = 
	local _g = 
	local _b = 
	return cc.c3b( _r,_g,_b )
end

-- 16进制颜色转换
function my.c4b( colorHex )
	local _r = 
	local _g = 
	local _b = 
	local _r = 
	return cc.c4b( _r,_g,_b,_a )
end--]]
