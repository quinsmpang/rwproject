--client_structsend_auto.lua

function struct_NetC_Login_send( buf, sendValue )
	buf:WriteShort( sendValue.m_username_length );
	buf:WriteStringWithLength( sendValue.m_username );
	buf:WriteShort( sendValue.m_password_length );
	buf:WriteStringWithLength( sendValue.m_password );
	buf:WriteShort( sendValue.m_deviceid_length );
	buf:WriteStringWithLength( sendValue.m_deviceid );
end

function struct_NetC_Create_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_aclass );
	buf:WriteShort( sendValue.m_name_length );
	buf:WriteStringWithLength( sendValue.m_name );
end

function struct_NetC_Heart_send( buf, sendValue )
	for tmpi=1,2,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
end

function struct_NetC_Gmcmd_send( buf, sendValue )
	buf:WriteShort( sendValue.m_cmd );
	for tmpi=1,4,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
	buf:WriteShort( sendValue.m_msg_length );
	buf:WriteStringWithLength( sendValue.m_msg );
end

function struct_NetC_Talk_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_type );
	buf:WriteShort( sendValue.m_msg_length );
	buf:WriteStringWithLength( sendValue.m_msg );
end

function struct_NetC_AskInfo_send( buf, sendValue )
	buf:WriteShort( sendValue.m_msgid );
	buf:WriteShort( sendValue.m_valuenum );
	for tmpi=1,sendValue.m_valuenum,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
	buf:WriteShort( sendValue.m_msg_length );
	buf:WriteStringWithLength( sendValue.m_msg );
end

function struct_NetC_MailOp_send( buf, sendValue )
	buf:WriteLong( sendValue.m_mailid );
	buf:WriteSByte( sendValue.m_op );
end

function struct_NetC_CityBattleTroopList_send( buf, sendValue )
	buf:WriteShort( sendValue.m_corps );
	buf:WriteSByte( sendValue.m_level );
	buf:WriteInt( sendValue.m_count );
end

function struct_NetC_CityBattleInfo_send( buf, sendValue )
	buf:WriteInt( sendValue.m_to_unit_index );
	buf:WriteShort( sendValue.m_to_posx );
	buf:WriteShort( sendValue.m_to_posy );
	buf:WriteShort( sendValue.m_troop_count );
	for tmpi=1,sendValue.m_troop_count do
		struct_NetC_CityBattleTroopList_send( buf, sendValue.m_troop_list[tmpi] );
	end
	buf:WriteShort( sendValue.m_hero_count );
	for tmpi=1,sendValue.m_hero_count,1 do
		buf:WriteShort( sendValue.m_hero_list[tmpi] );
	end
end

