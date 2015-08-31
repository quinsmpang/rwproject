-- 通用短消息接收
NOTIFY_NORMAL		=	0	-- 缺省文字提示，服务端发文字
NOTIFY_TEXTTABLE	= 	1	-- 服务端发文字表信息
NOTIFY_ITEMNUM		=	2	-- 整理背包更新道具数量
NOTIFY_WORLDMAP		=	3	-- 世界地图操作
NOTIFY_MAIL			=	4	-- 邮件操作
NOTIFY_CHAT			= 	5	-- 聊天操作
NOTIFY_ACTOR        =   6   -- 对玩家的操作

-- 处理接收到的消息
function RecvActorNotify( recvValue )
	local msgid = recvValue.m_msgid;
	local value = recvValue.m_value;
	local msg = recvValue.m_msg;

	if msgid == NOTIFY_NORMAL then
		Notify( msg );
	elseif msgid == NOTIFY_TEXTTABLE then
		Notify( GetLocalizeText( tonumber(msg) ) );
	elseif msgid == NOTIFY_ITEMNUM then
	elseif msgid == NOTIFY_WORLDMAP then
	
	-- 邮件
	elseif msgid == NOTIFY_MAIL then
		if value[1] == 1 then
			-- 未读邮件数量
			
		elseif value[1] == 10 then
			-- 未读战报数量
			
		end
	
	-- 对玩家的操作
	elseif msgid == NOTIFY_ACTOR then
		
	end
end