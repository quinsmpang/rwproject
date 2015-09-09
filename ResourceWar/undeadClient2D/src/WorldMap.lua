MAP_TILEWIDTH = 256;
MAP_TILEHEIGHT = 128;

TMX_WIDTH = 120; -- 一块tmx地图大小
TMX_HEIGHT = 120;-- 一块tmx地图大小

-- 世界地图
WorldMap = {}
WorldMap.m_nMaxWidth 		= 0; -- 世界地图大小（格子数）
WorldMap.m_nMaxHeight 		= 0; -- 世界地图大小（格子数）
WorldMap.m_nAreaWidth 		= 0; -- 区域大小（一个区域几个地图格子）
WorldMap.m_nAreaHeight 		= 0; -- 区域大小（一个区域几个地图格子）
WorldMap.m_nAreaXNum 		= 0; -- 一个地图有多少区域
WorldMap.m_nAreaYNum 		= 0; -- 一个地图有多少区域
WorldMap.m_nLastAreaIndex	= -1;-- 上一次发送区域
WorldMap.m_nMapUnitList		= {};-- 现实对象缓存链表
WorldMap.m_nMarchRouteList	= {};-- 行军路线缓存链表
WorldMap.m_nMyCityPosx		= 0; -- 玩家主城位置（格子）
WorldMap.m_nMyCityPosy		= 0; -- 玩家主城位置（格子）
WorldMap.m_nMyCityCameraX 	= 0; -- 玩家主城位置（屏幕坐标）
WorldMap.m_nMyCityCameraY	= 0; -- 玩家主城位置（屏幕坐标）
WorldMap.m_nMyCityUnitIndex = -1;-- 玩家主城的显示索引
WorldMap.m_nLastCameraX 	= 0; -- 上次摄像机位置（屏幕坐标）
WorldMap.m_nLastCameraY		= 0; -- 上次摄像机位置（屏幕坐标）
WorldMap.m_nLastTouchGameCoorX 	= 0; -- 上次点击位置（格子）
WorldMap.m_nLastTouchGameCoorY	= 0; -- 上次点击位置（格子）
WorldMap.m_nLastTouchUnitIndex	= -1; -- 上次点击单位

-- 地图区域
Area = {}
-- 根据世界坐标获取区域编号
function Area.GetIndex( posx, posy )
	local areax, areay;
	if posx >= WorldMap.m_nMaxWidth then
		posx = WorldMap.m_nMaxWidth - 1;
	end
	if posy >= WorldMap.m_nMaxHeight then
		posy = WorldMap.m_nMaxHeight - 1;
	end
	areax = math.modf((posx+1)/WorldMap.m_nAreaWidth);
	areay = math.modf((posy+1)/WorldMap.m_nAreaHeight);
	return math.modf( areay*(WorldMap.m_nAreaXNum) + areax );
end

-- 地图对象组件
local WorldMapPrefab	= nil;	-- 地图根
local MapTmxRoot		= nil;	-- tmx地图根节点
local MapUnitRoot		= nil;	-- 所有显示对象的根节点
local MapLineRoot		= nil;	-- 所有线根节点
local MapClickEffect	= nil;	-- 点击特效
local MapCamera 		= nil;	-- 地图摄像机

-- 点击按钮
local m_pClickFunSpy		= nil; -- 侦查按钮
local m_pClickFunAttack		= nil; -- 攻击按钮
local m_pClickFunMoveCity	= nil; -- 迁城按钮
local m_pClickFunOccupy		= nil; -- 占领按钮
	
-- 世界地图信息设置-服务端发送来的
function WorldMap.Init( recvValue )
	WorldMap.m_nMaxWidth		= 	recvValue.m_map_width;
	WorldMap.m_nMaxHeight		= 	recvValue.m_map_height;
	WorldMap.m_nAreaWidth		= 	recvValue.m_area_width;
	WorldMap.m_nAreaHeight		= 	recvValue.m_area_height;
	WorldMap.m_nAreaXNum		= 	recvValue.m_map_area_xnum;
	WorldMap.m_nAreaYNum		= 	recvValue.m_map_area_ynum;
	WorldMap.m_nMyCityPosx		= 	recvValue.m_my_city_posx;
	WorldMap.m_nMyCityPosy		= 	recvValue.m_my_city_posy;
	WorldMap.m_nMyCityUnitIndex = 	recvValue.m_my_city_unit_index;
	WorldMap.m_nLastAreaIndex 	= 	-1;
	WorldMap.m_nMapUnitList		= {};
	WorldMap.m_nMarchRouteList	= {};
	-- 收到信息后转换到世界地图场景
	GameManager.ChangeScence( "worldmap" );
end
	
-- 世界地图启动
function WorldMap.Start( Prefab )	
	-- 初始化根节点
	WorldMapPrefab	= Prefab;

	-- 获取组件
	MapTmxRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapTmxRoot" );
	MapUnitRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapUnitRoot" );
	MapLineRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapLineRoot" );
	MapClickEffect	= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapClickEffect" );
	MapCamera		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapCamera" );
	
	-- 获取点击地图的控件
	local objs = MapClickEffect:FindChild( "WorldMapClickMod" ):GetComponent( "UIMod" ).relatedGameObject;
	m_pClickFunSpy		= objs[0]; -- 侦查按钮
	m_pClickFunAttack	= objs[1]; -- 攻击按钮
	m_pClickFunMoveCity	= objs[2]; -- 迁城按钮
	m_pClickFunOccupy	= objs[3]; -- 占领按钮
	
	-- 根据地图大小动态加载N*N个Tmx地图
	local rownum = math.floor( WorldMap.m_nMaxWidth/TMX_WIDTH );
	local colnum = math.floor( WorldMap.m_nMaxHeight/TMX_HEIGHT );
	for j=1, colnum, 1 do
		for i=1, rownum, 1 do
			local obj = LoadPrefab( "TmxMap" );
			local tmx = GameObject.Instantiate( obj );
			tmx.transform:SetParent( MapTmxRoot );
			tmx.transform.localScale = Vector3.one;
			tmx.transform.localPosition = Vector3.New( (WorldMap.m_nMaxWidth*MAP_TILEWIDTH)/2/100 + (i-2)*TMX_WIDTH*MAP_TILEWIDTH/2/100 - (j-1)*TMX_WIDTH*MAP_TILEWIDTH/2/100,
												   -(i-1)*TMX_HEIGHT*MAP_TILEHEIGHT/2/100 - (j-1)*TMX_HEIGHT*MAP_TILEHEIGHT/2/100, 0 );
		end
	end

	-- 画边界线
	local MapBorder	= MapLineRoot:FindChild( "MapBorder" ):GetComponent("LineRenderer");
	MapBorder:SetVertexCount(5)
	MapBorder:SetWidth(0.5,0.5);
	MapBorder:SetPosition( 0, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, 0, 0 ) );
	MapBorder:SetPosition( 1, Vector3.New( MAP_TILEWIDTH/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/2/100, 0 ) );
	MapBorder:SetPosition( 2, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/100, 0 ) );
	MapBorder:SetPosition( 3, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/2/100, 0 ) );
	MapBorder:SetPosition( 4, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100,0, 0 ) );
	
	-- 设置摄像机初始位置为我的城池
	WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = WorldMap.ConvertGameToCamera( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy )
	MapCamera.transform.position = Vector3.New( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY, -100000 );
	
	-- 返回地图数据给程序一份
	return WorldMap.m_nMaxWidth, WorldMap.m_nMaxHeight, MAP_TILEWIDTH, MAP_TILEHEIGHT
end

-- 摄像机坐标=》游戏坐标(菱形格)
function WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY )
	local screen_x = math.modf(cameraPosX * 100) - WorldMap.m_nMaxWidth/2 * MAP_TILEWIDTH;
	local screen_y = math.modf(cameraPosY * 100) + WorldMap.m_nMaxHeight/2 * MAP_TILEHEIGHT;
	local logic_ptx = math.modf(screen_x - math.modf(MAP_TILEWIDTH*screen_y/MAP_TILEHEIGHT));
	local logic_pty = -math.modf(screen_x + math.modf(MAP_TILEWIDTH*screen_y/MAP_TILEHEIGHT));
	if logic_ptx < 0 then
		logic_ptx = logic_ptx - MAP_TILEWIDTH;
	end
	if logic_pty < 0 then
		logic_pty = logic_pty - MAP_TILEWIDTH;
	end
	local gameCoorX = math.modf(logic_ptx/MAP_TILEWIDTH) + WorldMap.m_nMaxWidth/2;
	local gameCoorY = math.modf(logic_pty/MAP_TILEWIDTH) + WorldMap.m_nMaxHeight/2;
	--print( screen_x..","..screen_y.." | "..gameCoorX.." "..gameCoorY )
	return gameCoorX, gameCoorY
end

-- 游戏坐标(菱形格)=》摄像机坐标
function WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY )
	--print( "MainCity:"..gameCoorX.." "..gameCoorY )
	local px  = ( MAP_TILEWIDTH/2/100 ) * ( gameCoorX - gameCoorY );
    local py  = -( MAP_TILEHEIGHT/2/100 ) * ( gameCoorX + gameCoorY );
	return px + WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, py
end

-- 添加显示对象
function WorldMap.AddMapUnit( recvValue )
	--print(recvValue.m_unit_index.."|"..recvValue.m_type .."|".. recvValue.m_name)

	-- 数据插入缓存
	table.insert( WorldMap.m_nMapUnitList, recvValue.m_unit_index, recvValue )
	-- 创建显示对象
	local zorder = 0;
	local obj
	if recvValue.m_type == 1 then
		obj = LoadPrefab( "MapCity" );
	elseif recvValue.m_type == 2 then
		obj = LoadPrefab( "MapArmy" );
		zorder = -1;
	end
	local unit = GameObject.Instantiate( obj );
	unit.transform:SetParent( MapUnitRoot );
	unit.transform.localScale = Vector3.one;
	unit.transform:GetComponent("ShareData"):AddValue( "unit_index", recvValue.m_unit_index );
	
	-- 位置
	local posx, posy = WorldMap.ConvertGameToCamera( recvValue.m_posx, recvValue.m_posy )
	unit.transform.localPosition = Vector3.New( posx, posy, zorder );
	
	-- 是玩家自己主城
	if WorldMap.m_nMyCityUnitIndex == recvValue.m_unit_index then
		-- 位置
		WorldMap.m_nMyCityPosx = recvValue.m_posx
		WorldMap.m_nMyCityPosy = recvValue.m_posy;
		WorldMap.m_nMyCityCameraX = posx; 
		WorldMap.m_nMyCityCameraY = posy;
		-- 名字
		unit:GetComponent("Transform"):FindChild("Name"):GetComponent("Text").text = GetLocalizeText(1049);
	else
		unit:GetComponent("Transform"):FindChild("Name"):GetComponent("Text").text = recvValue.m_name;
	end
	
    

	-- 如果有目的地点，画线
	if recvValue.m_tposx > 0 or recvValue.m_tposy > 0 or recvValue.m_type == 2 then
		WorldMap.MarchRouteAdd( recvValue.m_unit_index, recvValue.m_posx, recvValue.m_posy, -1, recvValue.m_tposx, recvValue.m_tposy )
		-- 设置移动
		local tposx, tposy = WorldMap.ConvertGameToCamera( recvValue.m_tposx, recvValue.m_tposy )
		unit:GetComponent("MapUnitMove").toPosition = Vector3.New( tposx, tposy, 0 );
		unit:GetComponent("MapUnitMove").speed = 0.01;
		unit:GetComponent("MapUnitMove").stat = 1;

        --设置动作 内容 ,todo 后面应该根据兵种类别来显示
        unit:GetComponent("MapUnit").AnimatorName="FightH_SOLDIER_1";
        --判断移动的方向
        if unit.transform.localPosition.x > tposx then
            unit.transform:FindChild("body").transform.rotation=Quaternion.New(0,180,0,1);
        end;
        unit:GetComponent("MapUnit"):Init();
        unit:GetComponent("MapUnit"):PlayAction(0);

	end

	-- 如果有人攻击这个单元
	for tmpi = 1, recvValue.m_underfire_count, 1 do
		local from_unit_index = recvValue.m_underfire_frompos[tmpi].m_unit_index;
		local from_posx = recvValue.m_underfire_frompos[tmpi].m_posx;
		local from_posy = recvValue.m_underfire_frompos[tmpi].m_posy;
		WorldMap.MarchRouteAdd( from_unit_index, from_posx, from_posy, recvValue.m_unit_index, recvValue.m_posx, recvValue.m_posy );
	end
	
	-- 插入缓存
	WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"] = unit;
end

-- 移除显示对象
function WorldMap.DelMapUnit( recvValue )
	-- 移除显示对象
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index] ~= nil then
		GameObject.Destroy( WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"] );
	end
	-- 删除缓存
	table.remove( WorldMap.m_nMapUnitList, recvValue.m_unit_index )
end

-- 移动显示对象
function WorldMap.MoveMapUnit( recvValue )
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index] == nil then
		return;
	end
	local unit = WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"];
	local posx, posy = WorldMap.ConvertGameToCamera(recvValue.m_posx, recvValue.m_posy)
	unit.transform.localPosition = Vector3.New( posx, posy, 0 );
end

--  每秒通知3次位置变化了-负责区域计算
function WorldMap.ViewChangeSec()
	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;

	-- 坐标计算区域索引
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY )
	local now_areaindex = Area.GetIndex( gameCoorX, gameCoorY );
	if now_areaindex < 0 or now_areaindex == WorldMap.m_nLastAreaIndex then
		return
	end
	WorldMap.m_nLastAreaIndex = now_areaindex;
	system_askinfo( ASKINFO_AREAENTER, "", now_areaindex );
end

--  每秒通知30次位置变化了-负责距离和方位
function WorldMap.ViewChangeFrame()
	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;
	
	-- 位置没变化就不计算了
	if cameraPosX == WorldMap.m_nLastCameraX and cameraPosY == WorldMap.m_nLastCameraY then
		return;
	end
	WorldMap.m_nLastCameraX = cameraPosX;
	WorldMap.m_nLastCameraY	= cameraPosY;
	
	-- 箭头位置
	local arrowPos = MainDlgOnDirectionArrowPosition();
	-- 计算自己主城与当前视口中心的方位和距离
	local cityPos = cc.p( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
	local cameraPos = cc.p( cameraPosX, cameraPosY );
	-- 距离
	local distance = cc.pGetDistance( cameraPos, cityPos );
	-- 角度
	local o = cameraPos.x + arrowPos.x - cityPos.x;
	local a = cameraPos.y + arrowPos.y - cityPos.y;
	local angle = math.deg( math.atan( o/a ) );
	if a < 0 then
		if o < 0 then
			angle = 180 + math.abs(angle);
		else
			angle = 180 - math.abs(angle);
		end 
	end
	
	--local ScreenCityPos = Camera.main:WorldToScreenPoint( cityPos );Screen.currentResolution.width;
	--local ScreenCameraPos = Camera.main:WorldToScreenPoint( cameraPos );
	--local r = (Screen.height-60)/2;
	--local k = (ScreenCameraPos.y-ScreenCityPos.y)/(ScreenCameraPos.x-ScreenCityPos.x);
	--[[local m1 = math.sqrt( (r*r) / (k*k + 1) );
    local m2 = math.sqrt(*pow(l/3*2,2)/(pow(k,2)+1));
	local v = circle_target_position( ScreenCameraPos, k, r, cc.p(0,0) );--]]
	-- 主城坐标
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY );
	MainDlgOnDirectionArrow( distance, 180-angle, cc.p(gameCoorX,gameCoorY) );
end

-- 点击地图
function WorldMap.OnClick( obj, touchpos )
	if obj == nil and MapClickEffect.gameObject.activeSelf == true then
		MapClickEffect.gameObject:SetActive( false );
		return;
	end
	
	-- 显示点击效果
	MapClickEffect.gameObject:SetActive( true );
	
	-- 计算位置
	local touchpos = Camera.main:ScreenToWorldPoint( touchpos );
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( touchpos.x, touchpos.y );
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	WorldMap.m_nLastTouchGameCoorX = gameCoorX;
	WorldMap.m_nLastTouchGameCoorY = gameCoorY;
	
	-- 如果点击的是空地
	if obj == nil then
		MapClickEffect:FindChild("Back").gameObject:SetActive( true );
		MapClickEffect:GetComponent("Transform").localPosition = Vector3.New( cameraPosX, cameraPosY-MAP_TILEHEIGHT/2/100, -30 );
	else
		MapClickEffect:FindChild("Back").gameObject:SetActive( false );
		MapClickEffect:GetComponent("Transform").localPosition = Vector3.New( obj:GetComponent("Transform").position.x, obj:GetComponent("Transform").position.y, -30 );
	end

	-- 如果是空地或者不是城池
	if obj == nil then
		m_pClickFunSpy:SetActive( false );
		m_pClickFunAttack:SetActive( false );
		m_pClickFunMoveCity:SetActive( true );
		m_pClickFunOccupy:SetActive( true );
		WorldMap.m_nLastTouchUnitIndex = -1;
	else
		m_pClickFunSpy:SetActive( true );
		m_pClickFunAttack:SetActive( true );
		m_pClickFunMoveCity:SetActive( false );
		m_pClickFunOccupy:SetActive( false );
		WorldMap.m_nLastTouchUnitIndex = obj:GetComponent("ShareData"):GetValue( "unit_index" );
	end
end

-- 侦查
function WorldMap.ClickFunSpy()
	system_askinfo( ASKINFO_WORLDMAP, "", 2 );
	MapClickEffect.gameObject:SetActive( false );
end

-- 攻击
function WorldMap.ClickFunAttack()
    MapClickEffect.gameObject:SetActive( false );
    TroopDlgOpen();
	
end

-- 迁城
function WorldMap.ClickFunMoveCity()
	system_askinfo( ASKINFO_WORLDMAP, "", 4, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY );
	MapClickEffect.gameObject:SetActive( false );
end

-- 占领
function WorldMap.ClickFunOccupy()
	--system_askinfo( ASKINFO_WORLDMAP, "", 5, -1, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY );
	
    TroopDlgOpen();
    MapClickEffect.gameObject:SetActive( false );
end

-- 返回城池
function WorldMap.ReturnCity()
	WorldMap.m_nLastAreaIndex 	= 	-1;
	system_askinfo( ASKINFO_AREAENTER, "", WorldMap.m_nLastAreaIndex );
	GameManager.ChangeScence( "city" );
end

-- 添加行军路线到缓存
function WorldMap.MarchRouteAdd( from_unit_index, from_posx, from_posy, to_unit_index, to_posx, to_posy )
	-- 添加之前先检查缓存
	if  WorldMap.m_nMarchRouteList[from_unit_index] ~= nil then
		return;
	end

	-- 画线
	local fposx, fposy = WorldMap.ConvertGameToCamera(from_posx, from_posy)
	local tposx, tposy = WorldMap.ConvertGameToCamera(to_posx, to_posy)
	local obj = WorldMap.MarchRouteDraw( Vector3.New( fposx, fposy, 0 ), Vector3.New( tposx, tposy, 0 ), 1, MapLineRoot )
	-- 插入缓存
	table.insert( WorldMap.m_nMarchRouteList, from_unit_index, 
	{m_from_posx=from_posx, m_from_posy=from_posy, m_to_unit_index=to_unit_index, m_to_posx=to_posx, m_to_posy=to_posy, m_obj = obj} );
end

-- 删除行军路线
function WorldMap.MarchRouteDel( from_unit_index )
	-- 删除之前先检查缓存
	if  WorldMap.m_nMarchRouteList[from_unit_index] == nil then
		return;
	end
	-- 删除线
	GameObject.Destroy( WorldMap.m_nMarchRouteList[from_unit_index]["m_obj"] );
	-- 删除缓存
	table.remove( WorldMap.m_nMarchRouteList, from_unit_index );
end

-- 画行军路线
local m_SpriteWidth = 0.17;
local m_SpriteHeight = 0.455;
function WorldMap.MarchRouteDraw( from, to, type, parent )
	local obj = GameObject.Instantiate( LoadPrefab( "MarchRoute" ) );
	local plane = obj:GetComponent( "GizmoPlane" );

	local length = ( from - to ):Magnitude();
	plane.vertices[0] = Vector3.New( -m_SpriteWidth / 2, 0, 0 );
	plane.vertices[1] = Vector3.New( -m_SpriteWidth / 2, length, 0 );
	plane.vertices[2] = Vector3.New( m_SpriteWidth / 2, 0, 0 );
	plane.vertices[3] = Vector3.New( m_SpriteWidth / 2, length, 0 );

	plane.uv[1] = Vector2.New( 0, length / m_SpriteHeight );
	plane.uv[3] = Vector2.New( 1, length / m_SpriteHeight );

	obj.transform:SetParent( parent );
	obj.transform.localScale = Vector3.one;
	obj.transform.localPosition = from; 

	local offset = ( to - from ).x >= 0 and -1 or 1;
	obj.transform.localEulerAngles = Vector3.New( 0, 0, Vector3.Angle( Vector3( 0, 1, 0 ), to - from ) * offset );

	if type == 1 then
		plane:SetColor( Color.green );
	else
		plane:SetColor( Color.red );
	end

	return obj;
end
