--client_structrecv_auto.lua

function struct_NetS_Login_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadInt();
	recvValue.m_username_length = buffer:ReadShort();
	recvValue.m_username = buffer:ReadStringWithLen( recvValue.m_username_length );
	recvValue.m_token_length = buffer:ReadShort();
	recvValue.m_token = buffer:ReadStringWithLen( recvValue.m_token_length );
	return recvValue;
end

function struct_ListInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_aclass = buffer:ReadSByte();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_offset = buffer:ReadSByte();
	recvValue.m_lockstat = buffer:ReadSByte();
	recvValue.m_delete_stoptime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_List_recv( buffer )
	local recvValue = {};
	recvValue.m_actor_num = buffer:ReadSByte();
	recvValue.m_listinfo = {};
	for tmpi=1,recvValue.m_actor_num,1 do
		local tmpValue={};
		tmpValue = struct_ListInfo_recv( buffer );
		table.insert( recvValue.m_listinfo, tmpValue );
	end
	return recvValue;
end

function struct_NetS_Create_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadSByte();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_EnterInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_data_int = buffer:ReadInt();
	recvValue.m_data_short = buffer:ReadShort();
	recvValue.m_data_char = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_Delete_recv( buffer )
	local recvValue = {};
	recvValue.m_result = buffer:ReadInt();
	recvValue.m_actorid = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_Heart_recv( buffer )
	local recvValue = {};
	recvValue.m_value={};
	for tmpi=1,2,1 do
		recvValue.m_value[tmpi] = buffer:ReadInt();
	end
	return recvValue;
end

function struct_NetS_Notify_recv( buffer )
	local recvValue = {};
	recvValue.m_msgid = buffer:ReadShort();
	recvValue.m_valuenum = buffer:ReadShort();
	recvValue.m_value={};
	for tmpi=1,recvValue.m_valuenum,1 do
		recvValue.m_value[tmpi] = buffer:ReadInt();
	end
	recvValue.m_msg_length = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msg_length );
	return recvValue;
end

function struct_NetS_Talk_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_msg_length = buffer:ReadShort();
	recvValue.m_msg = buffer:ReadStringWithLen( recvValue.m_msg_length );
	recvValue.m_shape = buffer:ReadSByte();
	recvValue.m_sendtime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_FightIn_recv( buffer )
	local recvValue = {};
	recvValue.m_srcpos = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_isenemy = buffer:ReadSByte();
	recvValue.m_posion = buffer:ReadSByte();
	recvValue.m_corps = buffer:ReadShort();
	recvValue.m_curnumber = buffer:ReadInt();
	recvValue.m_maxnumber = buffer:ReadInt();
	recvValue.m_ratenumber = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FightSet_recv( buffer )
	local recvValue = {};
	recvValue.m_turn = buffer:ReadShort();
	recvValue.m_fightflag = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FightMove_recv( buffer )
	local recvValue = {};
	recvValue.m_srcpos = buffer:ReadSByte();
	recvValue.m_movstep = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FightAction_recv( buffer )
	local recvValue = {};
	recvValue.m_srcpos = buffer:ReadSByte();
	recvValue.m_tgtpos = buffer:ReadSByte();
	recvValue.m_action = buffer:ReadSByte();
	recvValue.m_damage = buffer:ReadInt();
	recvValue.m_srcstat = buffer:ReadInt();
	recvValue.m_tgtstat = buffer:ReadInt();
	recvValue.m_nexttgt = buffer:ReadSByte();
	return recvValue;
end

function struct_ArmyCorrdinate_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_CityResInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_wood = buffer:ReadInt();
	recvValue.m_food = buffer:ReadInt();
	recvValue.m_iron = buffer:ReadInt();
	recvValue.m_mithril = buffer:ReadInt();
	recvValue.m_gold = buffer:ReadInt();
	recvValue.m_towncenter_level = buffer:ReadSByte();
	recvValue.m_enter_worldmap = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BuildingBaseInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_kind = buffer:ReadByte();
	recvValue.m_level = buffer:ReadSByte();
	recvValue.m_offset = buffer:ReadByte();
	return recvValue;
end

function struct_NetS_CityBuildingList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BuildingBaseInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_ActorInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_actorid = buffer:ReadInt();
	recvValue.m_name = buffer:ReadStringWithLen( 22 );
	recvValue.m_aclass = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadShort();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_experience = buffer:ReadLong();
	recvValue.m_experience_max = buffer:ReadLong();
	recvValue.m_leadership = buffer:ReadShort();
	recvValue.m_token = buffer:ReadInt();
	recvValue.m_viplevel = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ItemUse_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_usenum = buffer:ReadShort();
	recvValue.m_effres = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ItemPut_recv( buffer )
	local recvValue = {};
	recvValue.m_res_offset = buffer:ReadShort();
	recvValue.m_res_num = buffer:ReadShort();
	recvValue.m_target_offset = buffer:ReadShort();
	recvValue.m_target_num = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_ItemSettle_recv( buffer )
	local recvValue = {};
	recvValue.m_itemnum = buffer:ReadShort();
	recvValue.m_itemoffset={};
	for tmpi=1,recvValue.m_itemnum,1 do
		recvValue.m_itemoffset[tmpi] = buffer:ReadShort();
	end
	return recvValue;
end

function struct_NetS_LostItem_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_itemnum = buffer:ReadShort();
	recvValue.m_targetid = buffer:ReadInt();
	recvValue.m_path = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_GetItem_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_kind = buffer:ReadInt();
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_color = buffer:ReadSByte();
	recvValue.m_num = buffer:ReadShort();
	recvValue.m_targetid = buffer:ReadInt();
	recvValue.m_path = buffer:ReadSByte();
	return recvValue;
end

function struct_ItemSmpInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadShort();
	recvValue.m_kind = buffer:ReadShort();
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_num = buffer:ReadShort();
	recvValue.m_colorlevel = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_ItemList_recv( buffer )
	local recvValue = {};
	recvValue.m_itemext = buffer:ReadSByte();
	recvValue.m_kindnum = buffer:ReadShort();
	recvValue.m_item = {};
	for tmpi=1,recvValue.m_kindnum,1 do
		local tmpValue={};
		tmpValue = struct_ItemSmpInfo_recv( buffer );
		table.insert( recvValue.m_item, tmpValue );
	end
	return recvValue;
end

function struct_ItemAttr_recv( buffer )
	local recvValue = {};
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_ability = buffer:ReadShort();
	recvValue.m_value = buffer:ReadInt();
	recvValue.m_addvalue = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_ItemInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_itemoffset = buffer:ReadShort();
	recvValue.m_itemkind = buffer:ReadShort();
	recvValue.m_type = buffer:ReadShort();
	recvValue.m_level = buffer:ReadShort();
	recvValue.m_color_level = buffer:ReadSByte();
	recvValue.m_price = buffer:ReadInt();
	recvValue.m_attr_num = buffer:ReadShort();
	recvValue.m_attr = {};
	for tmpi=1,recvValue.m_attr_num,1 do
		local tmpValue={};
		tmpValue = struct_ItemAttr_recv( buffer );
		table.insert( recvValue.m_attr, tmpValue );
	end
	return recvValue;
end

function struct_NetS_BuildTimeInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadByte();
	recvValue.m_time = buffer:ReadInt();
	recvValue.m_state = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BuildTimeInfoList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadByte();
	recvValue.m_timeinfo = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BuildTimeInfo_recv( buffer );
		table.insert( recvValue.m_timeinfo, tmpValue );
	end
	recvValue.m_servertime = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_CityResChangeInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_restype = buffer:ReadSByte();
	recvValue.m_resvalue = buffer:ReadInt();
	recvValue.m_path = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_BuildingAbilityInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_offset = buffer:ReadByte();
	recvValue.m_value={};
	for tmpi=1,4,1 do
		recvValue.m_value[tmpi] = buffer:ReadInt();
	end
	return recvValue;
end

function struct_NetS_AddMapUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_shape = buffer:ReadByte();
	recvValue.m_stat = buffer:ReadSByte();
	recvValue.m_lastway = buffer:ReadSByte();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_tposx = buffer:ReadShort();
	recvValue.m_tposy = buffer:ReadShort();
	recvValue.m_namelen = buffer:ReadSByte();
	recvValue.m_name = buffer:ReadStringWithLen( recvValue.m_namelen );
	recvValue.m_level = buffer:ReadByte();
	recvValue.m_underfire_count = buffer:ReadByte();
	recvValue.m_underfire_frompos = {};
	for tmpi=1,recvValue.m_underfire_count,1 do
		local tmpValue={};
		tmpValue = struct_ArmyCorrdinate_recv( buffer );
		table.insert( recvValue.m_underfire_frompos, tmpValue );
	end
	return recvValue;
end

function struct_NetS_DelMapUnit_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_WorldMapInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_map_width = buffer:ReadShort();
	recvValue.m_map_height = buffer:ReadShort();
	recvValue.m_area_width = buffer:ReadSByte();
	recvValue.m_area_height = buffer:ReadSByte();
	recvValue.m_map_area_xnum = buffer:ReadShort();
	recvValue.m_map_area_ynum = buffer:ReadShort();
	recvValue.m_my_city_posx = buffer:ReadShort();
	recvValue.m_my_city_posy = buffer:ReadShort();
	recvValue.m_my_city_unit_index = buffer:ReadInt();
	return recvValue;
end

function struct_NetS_MapUnitCorrdinate_recv( buffer )
	local recvValue = {};
	recvValue.m_unit_index = buffer:ReadInt();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MarchRoute_recv( buffer )
	local recvValue = {};
	recvValue.m_from_unit_index = buffer:ReadInt();
	recvValue.m_from_posx = buffer:ReadShort();
	recvValue.m_from_posy = buffer:ReadShort();
	recvValue.m_to_unit_index = buffer:ReadInt();
	recvValue.m_to_posx = buffer:ReadShort();
	recvValue.m_to_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_OutsideMapInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_map_width = buffer:ReadShort();
	recvValue.m_map_height = buffer:ReadShort();
	recvValue.m_my_city_posx = buffer:ReadShort();
	recvValue.m_my_city_posy = buffer:ReadShort();
	recvValue.m_match_city_posx = buffer:ReadShort();
	recvValue.m_match_city_posy = buffer:ReadShort();
	return recvValue;
end

function struct_NetS_MailSimpleInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_mailid = buffer:ReadLong();
	recvValue.m_type = buffer:ReadSByte();
	recvValue.m_from_name_length = buffer:ReadShort();
	recvValue.m_from_name = buffer:ReadStringWithLen( recvValue.m_from_name_length );
	recvValue.m_title_length = buffer:ReadShort();
	recvValue.m_title = buffer:ReadStringWithLen( recvValue.m_title_length );
	recvValue.m_simple_content_length = buffer:ReadShort();
	recvValue.m_simple_content = buffer:ReadStringWithLen( recvValue.m_simple_content_length );
	recvValue.m_has_attach = buffer:ReadSByte();
	recvValue.m_recvtime = buffer:ReadInt();
	recvValue.m_deltime = buffer:ReadInt();
	recvValue.m_read = buffer:ReadSByte();
	recvValue.m_attach_isget = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_MailList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_MailSimpleInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_MailFullInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_mailid = buffer:ReadLong();
	recvValue.m_content_length = buffer:ReadShort();
	recvValue.m_content = buffer:ReadStringWithLen( recvValue.m_content_length );
	recvValue.m_attach_length = buffer:ReadShort();
	recvValue.m_attach = buffer:ReadStringWithLen( recvValue.m_attach_length );
	recvValue.m_read = buffer:ReadSByte();
	return recvValue;
end

function struct_NetS_FightMsgSimpleInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_msgid = buffer:ReadLong();
	recvValue.m_read = buffer:ReadSByte();
	recvValue.m_recvtime = buffer:ReadInt();
	recvValue.m_status = buffer:ReadSByte();
	recvValue.m_target_type = buffer:ReadSByte();
	recvValue.m_title_length = buffer:ReadShort();
	recvValue.m_title = buffer:ReadStringWithLen( recvValue.m_title_length );
	return recvValue;
end

function struct_NetS_FightMsgList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_FightMsgSimpleInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_FightMsgInfo_recv( buffer )
	local recvValue = {};
	recvValue.m_msgid = buffer:ReadLong();
	recvValue.m_read = buffer:ReadSByte();
	recvValue.m_recvtime = buffer:ReadInt();
	recvValue.m_posx = buffer:ReadShort();
	recvValue.m_posy = buffer:ReadShort();
	recvValue.m_status = buffer:ReadSByte();
	recvValue.m_target_type = buffer:ReadSByte();
	recvValue.m_target_id = buffer:ReadInt();
	recvValue.m_target_posx = buffer:ReadShort();
	recvValue.m_target_posy = buffer:ReadShort();
	recvValue.m_wood = buffer:ReadInt();
	recvValue.m_iron = buffer:ReadInt();
	recvValue.m_food = buffer:ReadInt();
	recvValue.m_recordid = buffer:ReadLong();
	recvValue.m_title_length = buffer:ReadShort();
	recvValue.m_title = buffer:ReadStringWithLen( recvValue.m_title_length );
	return recvValue;
end

function struct_NetS_BuildingAbilityList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_list = {};
	for tmpi=1,recvValue.m_count,1 do
		local tmpValue={};
		tmpValue = struct_NetS_BuildingAbilityInfo_recv( buffer );
		table.insert( recvValue.m_list, tmpValue );
	end
	return recvValue;
end

function struct_NetS_ChatMaskList_recv( buffer )
	local recvValue = {};
	recvValue.m_count = buffer:ReadShort();
	recvValue.m_chat_mask_list={};
	for tmpi=1,recvValue.m_count,1 do
		recvValue.m_chat_mask_list[tmpi] = buffer:ReadInt();
	end
	return recvValue;
end

