--client_process_auto.lua

-- m_result=0,m_username_length=0,m_username="[m_username_length]",m_token_length=0,m_token="[m_token_length]",
function proc_login_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_login_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_login_C", recvValue );
end

-- m_actor_num=0,m_listinfo={m_actorid=0,m_name="[22]",m_aclass=0,m_level=0,m_offset=0,m_lockstat=0,m_delete_stoptime=0,[m_actor_num]},
function proc_list_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_list_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_list_C", recvValue );
	if recvValue.m_actor_num <= 0 then
		-- 创建角色
		local sendValue = {};
		sendValue.m_aclass = 0;
		sendValue.m_name = "name"
		sendValue.m_name_length = string.len( sendValue.m_name );
		netsend_create_C( sendValue );
	else
		GetPlayer().m_actorid = recvValue.m_listinfo[1].m_actorid;
		-- 开始加载资源
		GameManager.onLoadRes()
	end
end

-- m_result=0,m_actorid=0,
function proc_create_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_create_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_create_C", recvValue );
	if recvValue.m_result == 0 then
		GetPlayer().m_actorid = recvValue.m_actorid
		-- 创建成功，开始加载资源
		GameManager.onLoadRes()
	else
		print( "创建角色失败,角色重名" )
	end
end

-- m_actorid=0,m_data_int=0,m_data_short=0,m_data_char=0,
function proc_enterinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_enterinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_enterinfo_C", recvValue );
end

-- m_result=0,m_actorid=0,
function proc_delete_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_delete_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_delete_C", recvValue );
end

-- m_value={[2]},
function proc_heart_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_heart_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_heart_C", recvValue );
end

-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
function proc_notify_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_notify_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_notify_C", recvValue );
end

-- m_type=0,m_actorid=0,m_name="[22]",m_msg_length=0,m_msg="[m_msg_length]",m_shape=0,m_sendtime=0,
function proc_talk_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_talk_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_talk_C", recvValue );
end

-- m_srcpos=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_isenemy=0,m_posion=0,m_corps=0,m_curnumber=0,m_maxnumber=0,m_ratenumber=0,
function proc_fightin_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightin_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightin_C", recvValue );
end

-- m_turn=0,m_fightflag=0,
function proc_fightset_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightset_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightset_C", recvValue );
end

-- m_srcpos=0,m_movstep=0,
function proc_fightmove_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightmove_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightmove_C", recvValue );
end

-- m_srcpos=0,m_tgtpos=0,m_action=0,m_damage=0,m_srcstat=0,m_tgtstat=0,m_nexttgt=0,
function proc_fightaction_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightaction_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightaction_C", recvValue );
end

-- m_actorid=0,m_name="[22]",m_aclass=0,m_shape=0,m_level=0,m_experience=0,m_experience_max=0,m_leadership=0,m_token=0,m_viplevel=0,
function proc_actorinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_actorinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_actorinfo_C", recvValue );
end

-- m_wood=0,m_food=0,m_iron=0,m_mithril=0,m_gold=0,m_towncenter_level=0,m_enter_worldmap=0,
function proc_cityresinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_cityresinfo_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_cityresinfo_C", recvValue );
	GetCacheCity():Set( recvValue )
end

-- m_count=0,m_list={m_kind=0,m_level=0,m_offset=0,[m_count]},
function proc_citybuildinglist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_citybuildinglist_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_citybuildinglist_C", recvValue );
	for tmpi=1, recvValue.m_count, 1 do
		GetCacheCity():AddBuilding( recvValue.m_list[tmpi] );
	end
end

-- m_unit_index=0,m_type=0,m_shape=0,m_stat=0,m_lastway=0,m_posx=0,m_posy=0,m_tposx=0,m_tposy=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_underfire_count=0,m_underfire_frompos={m_unit_index=0,m_posx=0,m_posy=0,[m_underfire_count]},
function proc_addmapunit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_addmapunit_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_addmapunit_C", recvValue );
end

-- m_unit_index=0,
function proc_delmapunit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_delmapunit_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_delmapunit_C", recvValue );
end

-- m_itemoffset=0,m_usenum=0,m_effres=0,
function proc_itemuse_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemuse_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemuse_C", recvValue );
end

-- m_res_offset=0,m_res_num=0,m_target_offset=0,m_target_num=0,
function proc_itemput_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemput_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemput_C", recvValue );
end

-- m_itemnum=0,m_itemoffset={}[m_itemnum],
function proc_itemsettle_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemsettle_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemsettle_C", recvValue );
end

-- m_itemoffset=0,m_itemnum=0,m_targetid=0,m_path=0,
function proc_lostitem_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_lostitem_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_lostitem_C", recvValue );
end

-- m_itemoffset=0,m_kind=0,m_type=0,m_color=0,m_num=0,m_targetid=0,m_path=0,
function proc_getitem_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_getitem_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_getitem_C", recvValue );
end

-- m_itemext=0,m_kindnum=0,m_item={m_offset=0,m_kind=0,m_type=0,m_num=0,m_colorlevel=0,[m_kindnum]},
function proc_itemlist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemlist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemlist_C", recvValue );
end

-- m_itemoffset=0,m_itemkind=0,m_type=0,m_level=0,m_color_level=0,m_price=0,m_attr_num=0,m_attr={m_type=0,m_ability=0,m_value=0,m_addvalue=0,[m_attr_num]},
function proc_iteminfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_iteminfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_iteminfo_C", recvValue );
end

-- m_offset=0,m_time=0,m_state=0,
function proc_buildtimeinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_buildtimeinfo_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_buildtimeinfo_C", recvValue );
	GetCacheCity():SetBuildingTimeInfo( recvValue )
end

-- m_count=0,m_timeinfo={m_offset=0,m_time=0,m_state=0,[m_count]},m_servertime=0,
function proc_buildtimeinfolist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_buildtimeinfolist_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_buildtimeinfolist_C", recvValue );
	GetCacheCity():SetServerTime( recvValue.m_servertime )
	for tmpi=1, recvValue.m_count, 1 do
		GetCacheCity():SetBuildingTimeInfo( recvValue.m_timeinfo[tmpi] )
	end
end

-- m_restype=0,m_resvalue=0,m_path=0,
function proc_cityreschangeinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_cityreschangeinfo_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_cityreschangeinfo_C", recvValue );
	if recvValue.m_restype == 1 then
		GetCacheCity():ChangeWood( recvValue.m_resvalue, recvValue.m_path );
	elseif recvValue.m_restype == 2 then
		GetCacheCity():ChangeFood( recvValue.m_resvalue, recvValue.m_path );
	elseif recvValue.m_restype == 3 then
		GetCacheCity():ChangeIron( recvValue.m_resvalue, recvValue.m_path );
	elseif recvValue.m_restype == 4 then
		GetCacheCity():ChangeMithril( recvValue.m_resvalue, recvValue.m_path );
	elseif recvValue.m_restype == 5 then
		GetCacheCity():ChangeGold( recvValue.m_resvalue, recvValue.m_path );
	end
end

-- m_kind=0,m_level=0,m_offset=0,
function proc_buildingbaseinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_buildingbaseinfo_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_buildingbaseinfo_C", recvValue );
	-- 更新建筑基本信息
	if recvValue.m_kind > 0 then
		GetCacheCity():AddBuilding( recvValue )
		
	-- 删除这个建筑
	else
		GetCacheCity():DelBuilding( recvValue )
	end
end

-- m_offset=0,m_value={[4]},
function proc_buildingabilityinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_buildingabilityinfo_C", function( recvValue ) end )
	 -- EventProtocol.dispatchEvent( "proc_buildingabilityinfo_C", recvValue );
	GetCacheCity():SetBuildingAbility( recvValue );
end

-- m_map_width=0,m_map_height=0,m_area_width=0,m_area_height=0,m_map_area_xnum=0,m_map_area_ynum=0,m_my_city_posx=0,m_my_city_posy=0,m_my_city_unit_index=0,
function proc_worldmapinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_worldmapinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_worldmapinfo_C", recvValue );
end

-- m_unit_index=0,m_posx=0,m_posy=0,
function proc_mapunitcorrdinate_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_mapunitcorrdinate_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_mapunitcorrdinate_C", recvValue );
end

-- m_from_unit_index=0,m_from_posx=0,m_from_posy=0,m_to_unit_index=0,m_to_posx=0,m_to_posy=0,
function proc_marchroute_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_marchroute_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_marchroute_C", recvValue );
end

-- m_map_width=0,m_map_height=0,m_my_city_posx=0,m_my_city_posy=0,m_match_city_posx=0,m_match_city_posy=0,
function proc_outsidemapinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_outsidemapinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_outsidemapinfo_C", recvValue );
end

-- m_mailid=0,m_type=0,m_from_name_length=0,m_from_name="[m_from_name_length]",m_title_length=0,m_title="[m_title_length]",m_simple_content_length=0,m_simple_content="[m_simple_content_length]",m_has_attach=0,m_recvtime=0,m_deltime=0,m_read=0,m_attach_isget=0,
function proc_mailsimpleinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_mailsimpleinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_mailsimpleinfo_C", recvValue );
end

-- m_count=0,m_list={m_mailid=0,m_type=0,m_from_name_length=0,m_from_name="[m_from_name_length]",m_title_length=0,m_title="[m_title_length]",m_simple_content_length=0,m_simple_content="[m_simple_content_length]",m_has_attach=0,m_recvtime=0,m_deltime=0,m_read=0,m_attach_isget=0,[m_count]},
function proc_maillist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_maillist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_maillist_C", recvValue );
end

-- m_mailid=0,m_content_length=0,m_content="[m_content_length]",m_attach_length=0,m_attach="[m_attach_length]",m_read=0,
function proc_mailfullinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_mailfullinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_mailfullinfo_C", recvValue );
end

-- m_msgid=0,m_read=0,m_recvtime=0,m_status=0,m_target_type=0,m_title_length=0,m_title="[m_title_length]",
function proc_fightmsgsimpleinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightmsgsimpleinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightmsgsimpleinfo_C", recvValue );
end

-- m_count=0,m_list={m_msgid=0,m_read=0,m_recvtime=0,m_status=0,m_target_type=0,m_title_length=0,m_title="[m_title_length]",[m_count]},
function proc_fightmsglist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightmsglist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightmsglist_C", recvValue );
end

-- m_msgid=0,m_read=0,m_recvtime=0,m_posx=0,m_posy=0,m_status=0,m_target_type=0,m_target_id=0,m_target_posx=0,m_target_posy=0,m_wood=0,m_iron=0,m_food=0,m_recordid=0,m_title_length=0,m_title="[m_title_length]",
function proc_fightmsginfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightmsginfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fightmsginfo_C", recvValue );
end

-- m_count=0,m_list={m_offset=0,m_value={[4]},[m_count]},
function proc_buildingabilitylist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_buildingabilitylist_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_buildingabilitylist_C", recvValue );
	for tmpi = 1, recvValue.m_count, 1 do
		GetCacheCity():SetBuildingAbility( recvValue.m_list[tmpi] );
	end
end

-- m_count=0,m_chat_mask_list={}[m_count],
function proc_chatmasklist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_chatmasklist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_chatmasklist_C", recvValue );
end

