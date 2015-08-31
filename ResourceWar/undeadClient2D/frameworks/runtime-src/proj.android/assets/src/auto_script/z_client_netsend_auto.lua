--client_netsend_auto.lua

-- m_username_length=0,m_username="[m_username_length]",m_password_length=0,m_password="[m_password_length]",m_deviceid_length=0,m_deviceid="[m_deviceid_length]",
function netsend_login_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_LOGIN );
	struct_NetC_Login_send( buf, sendValue );
	SendMessage(buf);
end

-- m_aclass=0,m_name_length=0,m_name="[m_name_length]",
function netsend_create_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_CREATE );
	struct_NetC_Create_send( buf, sendValue );
	SendMessage(buf);
end

-- number
function netsend_list_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_LIST );
	buf:WriteSByte( sendValue );
	SendMessage(buf);
end

-- number
function netsend_entergame_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_ENTERGAME );
	buf:WriteInt( sendValue );
	SendMessage(buf);
end

-- number
function netsend_delete_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_DELETE );
	buf:WriteInt( sendValue );
	SendMessage(buf);
end

-- m_value={[2]},
function netsend_heart_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_HEART );
	struct_NetC_Heart_send( buf, sendValue );
	SendMessage(buf);
end

-- m_cmd=0,m_value={[4]},m_msg_length=0,m_msg="[m_msg_length]",
function netsend_gmcmd_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_GMCMD );
	struct_NetC_Gmcmd_send( buf, sendValue );
	SendMessage(buf);
end

-- m_type=0,m_msg_length=0,m_msg="[m_msg_length]",
function netsend_talk_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_TALK );
	struct_NetC_Talk_send( buf, sendValue );
	SendMessage(buf);
end

-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
function netsend_askinfo_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_ASKINFO );
	struct_NetC_AskInfo_send( buf, sendValue );
	SendMessage(buf);
end

-- m_mailid=0,m_op=0,
function netsend_mailop_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_MAILOP );
	struct_NetC_MailOp_send( buf, sendValue );
	SendMessage(buf);
end

-- m_to_unit_index=0,m_to_posx=0,m_to_posy=0,m_troop_count=0,m_troop_list={m_corps=0,m_level=0,m_count=0,[m_troop_count]},m_hero_count=0,m_hero_list={}[m_hero_count],
function netsend_citybattle_C( sendValue )
	local buf = ByteBuffer:new();
	buf:WriteShort( CMDC_CITYBATTLE );
	struct_NetC_CityBattleInfo_send( buf, sendValue );
	SendMessage(buf);
end

