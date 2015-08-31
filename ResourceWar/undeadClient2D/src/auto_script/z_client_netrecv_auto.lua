--client_netrecv_auto.lua

function netrecv_login_C( buffer )
	local recvValue = struct_NetS_Login_recv( buffer );
	proc_login_C( recvValue );
end

function netrecv_list_C( buffer )
	local recvValue = struct_NetS_List_recv( buffer );
	proc_list_C( recvValue );
end

function netrecv_create_C( buffer )
	local recvValue = struct_NetS_Create_recv( buffer );
	proc_create_C( recvValue );
end

function netrecv_enterinfo_C( buffer )
	local recvValue = struct_NetS_EnterInfo_recv( buffer );
	proc_enterinfo_C( recvValue );
end

function netrecv_delete_C( buffer )
	local recvValue = struct_NetS_Delete_recv( buffer );
	proc_delete_C( recvValue );
end

function netrecv_heart_C( buffer )
	local recvValue = struct_NetS_Heart_recv( buffer );
	proc_heart_C( recvValue );
end

function netrecv_notify_C( buffer )
	local recvValue = struct_NetS_Notify_recv( buffer );
	proc_notify_C( recvValue );
end

function netrecv_talk_C( buffer )
	local recvValue = struct_NetS_Talk_recv( buffer );
	proc_talk_C( recvValue );
end

function netrecv_fightin_C( buffer )
	local recvValue = struct_NetS_FightIn_recv( buffer );
	proc_fightin_C( recvValue );
end

function netrecv_fightset_C( buffer )
	local recvValue = struct_NetS_FightSet_recv( buffer );
	proc_fightset_C( recvValue );
end

function netrecv_fightmove_C( buffer )
	local recvValue = struct_NetS_FightMove_recv( buffer );
	proc_fightmove_C( recvValue );
end

function netrecv_fightaction_C( buffer )
	local recvValue = struct_NetS_FightAction_recv( buffer );
	proc_fightaction_C( recvValue );
end

function netrecv_actorinfo_C( buffer )
	local recvValue = struct_NetS_ActorInfo_recv( buffer );
	proc_actorinfo_C( recvValue );
end

function netrecv_cityresinfo_C( buffer )
	local recvValue = struct_NetS_CityResInfo_recv( buffer );
	proc_cityresinfo_C( recvValue );
end

function netrecv_citybuildinglist_C( buffer )
	local recvValue = struct_NetS_CityBuildingList_recv( buffer );
	proc_citybuildinglist_C( recvValue );
end

function netrecv_addmapunit_C( buffer )
	local recvValue = struct_NetS_AddMapUnit_recv( buffer );
	proc_addmapunit_C( recvValue );
end

function netrecv_delmapunit_C( buffer )
	local recvValue = struct_NetS_DelMapUnit_recv( buffer );
	proc_delmapunit_C( recvValue );
end

function netrecv_itemuse_C( buffer )
	local recvValue = struct_NetS_ItemUse_recv( buffer );
	proc_itemuse_C( recvValue );
end

function netrecv_itemput_C( buffer )
	local recvValue = struct_NetS_ItemPut_recv( buffer );
	proc_itemput_C( recvValue );
end

function netrecv_itemsettle_C( buffer )
	local recvValue = struct_NetS_ItemSettle_recv( buffer );
	proc_itemsettle_C( recvValue );
end

function netrecv_lostitem_C( buffer )
	local recvValue = struct_NetS_LostItem_recv( buffer );
	proc_lostitem_C( recvValue );
end

function netrecv_getitem_C( buffer )
	local recvValue = struct_NetS_GetItem_recv( buffer );
	proc_getitem_C( recvValue );
end

function netrecv_itemlist_C( buffer )
	local recvValue = struct_NetS_ItemList_recv( buffer );
	proc_itemlist_C( recvValue );
end

function netrecv_iteminfo_C( buffer )
	local recvValue = struct_NetS_ItemInfo_recv( buffer );
	proc_iteminfo_C( recvValue );
end

function netrecv_buildtimeinfo_C( buffer )
	local recvValue = struct_NetS_BuildTimeInfo_recv( buffer );
	proc_buildtimeinfo_C( recvValue );
end

function netrecv_buildtimeinfolist_C( buffer )
	local recvValue = struct_NetS_BuildTimeInfoList_recv( buffer );
	proc_buildtimeinfolist_C( recvValue );
end

function netrecv_cityreschangeinfo_C( buffer )
	local recvValue = struct_NetS_CityResChangeInfo_recv( buffer );
	proc_cityreschangeinfo_C( recvValue );
end

function netrecv_buildingbaseinfo_C( buffer )
	local recvValue = struct_NetS_BuildingBaseInfo_recv( buffer );
	proc_buildingbaseinfo_C( recvValue );
end

function netrecv_buildingabilityinfo_C( buffer )
	local recvValue = struct_NetS_BuildingAbilityInfo_recv( buffer );
	proc_buildingabilityinfo_C( recvValue );
end

function netrecv_worldmapinfo_C( buffer )
	local recvValue = struct_NetS_WorldMapInfo_recv( buffer );
	proc_worldmapinfo_C( recvValue );
end

function netrecv_mapunitcorrdinate_C( buffer )
	local recvValue = struct_NetS_MapUnitCorrdinate_recv( buffer );
	proc_mapunitcorrdinate_C( recvValue );
end

function netrecv_marchroute_C( buffer )
	local recvValue = struct_NetS_MarchRoute_recv( buffer );
	proc_marchroute_C( recvValue );
end

function netrecv_outsidemapinfo_C( buffer )
	local recvValue = struct_NetS_OutsideMapInfo_recv( buffer );
	proc_outsidemapinfo_C( recvValue );
end

function netrecv_mailsimpleinfo_C( buffer )
	local recvValue = struct_NetS_MailSimpleInfo_recv( buffer );
	proc_mailsimpleinfo_C( recvValue );
end

function netrecv_maillist_C( buffer )
	local recvValue = struct_NetS_MailList_recv( buffer );
	proc_maillist_C( recvValue );
end

function netrecv_mailfullinfo_C( buffer )
	local recvValue = struct_NetS_MailFullInfo_recv( buffer );
	proc_mailfullinfo_C( recvValue );
end

function netrecv_fightmsgsimpleinfo_C( buffer )
	local recvValue = struct_NetS_FightMsgSimpleInfo_recv( buffer );
	proc_fightmsgsimpleinfo_C( recvValue );
end

function netrecv_fightmsglist_C( buffer )
	local recvValue = struct_NetS_FightMsgList_recv( buffer );
	proc_fightmsglist_C( recvValue );
end

function netrecv_fightmsginfo_C( buffer )
	local recvValue = struct_NetS_FightMsgInfo_recv( buffer );
	proc_fightmsginfo_C( recvValue );
end

function netrecv_buildingabilitylist_C( buffer )
	local recvValue = struct_NetS_BuildingAbilityList_recv( buffer );
	proc_buildingabilitylist_C( recvValue );
end

function netrecv_chatmasklist_C( buffer )
	local recvValue = struct_NetS_ChatMaskList_recv( buffer );
	proc_chatmasklist_C( recvValue );
end

Proc_Command = {
	[CMDS_LOGIN]=netrecv_login_C;
	[CMDS_LIST]=netrecv_list_C;
	[CMDS_CREATE]=netrecv_create_C;
	[CMDS_ENTERINFO]=netrecv_enterinfo_C;
	[CMDS_DELETE]=netrecv_delete_C;
	[CMDS_HEART]=netrecv_heart_C;
	[CMDS_NOTIFY]=netrecv_notify_C;
	[CMDS_TALK]=netrecv_talk_C;
	[CMDS_FIGHTIN]=netrecv_fightin_C;
	[CMDS_FIGHTSET]=netrecv_fightset_C;
	[CMDS_FIGHTMOVE]=netrecv_fightmove_C;
	[CMDS_FIGHTACTION]=netrecv_fightaction_C;
	[CMDS_ACTORINFO]=netrecv_actorinfo_C;
	[CMDS_CITYRESINFO]=netrecv_cityresinfo_C;
	[CMDS_CITYBUILDINGLIST]=netrecv_citybuildinglist_C;
	[CMDS_ADDMAPUNIT]=netrecv_addmapunit_C;
	[CMDS_DELMAPUNIT]=netrecv_delmapunit_C;
	[CMDS_ITEMUSE]=netrecv_itemuse_C;
	[CMDS_ITEMPUT]=netrecv_itemput_C;
	[CMDS_ITEMSETTLE]=netrecv_itemsettle_C;
	[CMDS_LOSTITEM]=netrecv_lostitem_C;
	[CMDS_GETITEM]=netrecv_getitem_C;
	[CMDS_ITEMLIST]=netrecv_itemlist_C;
	[CMDS_ITEMINFO]=netrecv_iteminfo_C;
	[CMDS_BUILDTIMEINFO]=netrecv_buildtimeinfo_C;
	[CMDS_BUILDTIMEINFOLIST]=netrecv_buildtimeinfolist_C;
	[CMDS_CITYRESCHANGEINFO]=netrecv_cityreschangeinfo_C;
	[CMDS_BUILDINGBASEINFO]=netrecv_buildingbaseinfo_C;
	[CMDS_BUILDINGABILITYINFO]=netrecv_buildingabilityinfo_C;
	[CMDS_WORLDMAPINFO]=netrecv_worldmapinfo_C;
	[CMDS_MAPUNITCORRDINATE]=netrecv_mapunitcorrdinate_C;
	[CMDS_MARCHROUTE]=netrecv_marchroute_C;
	[CMDS_OUTSIDEMAPINFO]=netrecv_outsidemapinfo_C;
	[CMDS_MAILSIMPLEINFO]=netrecv_mailsimpleinfo_C;
	[CMDS_MAILLIST]=netrecv_maillist_C;
	[CMDS_MAILFULLINFO]=netrecv_mailfullinfo_C;
	[CMDS_FIGHTMSGSIMPLEINFO]=netrecv_fightmsgsimpleinfo_C;
	[CMDS_FIGHTMSGLIST]=netrecv_fightmsglist_C;
	[CMDS_FIGHTMSGINFO]=netrecv_fightmsginfo_C;
	[CMDS_BUILDINGABILITYLIST]=netrecv_buildingabilitylist_C;
	[CMDS_CHATMASKLIST]=netrecv_chatmasklist_C;
}

function in_proc_command_C( cmd, buffer )
	if Proc_Command[cmd] ~= nil then
		Proc_Command[cmd]( buffer );
		return 1;
	end
	return 0;
end
