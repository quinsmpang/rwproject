MAP_TILEWIDTH = 256;
MAP_TILEHEIGHT = 128;

TMX_WIDTH = 120; -- һ��tmx��ͼ��С
TMX_HEIGHT = 120;-- һ��tmx��ͼ��С

-- �����ͼ
WorldMap = {}
WorldMap.m_nMaxWidth 		= 0; -- �����ͼ��С����������
WorldMap.m_nMaxHeight 		= 0; -- �����ͼ��С����������
WorldMap.m_nAreaWidth 		= 0; -- �����С��һ�����򼸸���ͼ���ӣ�
WorldMap.m_nAreaHeight 		= 0; -- �����С��һ�����򼸸���ͼ���ӣ�
WorldMap.m_nAreaXNum 		= 0; -- һ����ͼ�ж�������
WorldMap.m_nAreaYNum 		= 0; -- һ����ͼ�ж�������
WorldMap.m_nLastAreaIndex	= -1;-- ��һ�η�������
WorldMap.m_nMapUnitList		= {};-- ��ʵ���󻺴�����
WorldMap.m_nMarchRouteList	= {};-- �о�·�߻�������
WorldMap.m_nMyCityPosx		= 0; -- �������λ�ã����ӣ�
WorldMap.m_nMyCityPosy		= 0; -- �������λ�ã����ӣ�
WorldMap.m_nMyCityCameraX 	= 0; -- �������λ�ã���Ļ���꣩
WorldMap.m_nMyCityCameraY	= 0; -- �������λ�ã���Ļ���꣩
WorldMap.m_nMyCityUnitIndex = -1;-- ������ǵ���ʾ����
WorldMap.m_nLastCameraX 	= 0; -- �ϴ������λ�ã���Ļ���꣩
WorldMap.m_nLastCameraY		= 0; -- �ϴ������λ�ã���Ļ���꣩
WorldMap.m_nLastTouchGameCoorX 	= 0; -- �ϴε��λ�ã����ӣ�
WorldMap.m_nLastTouchGameCoorY	= 0; -- �ϴε��λ�ã����ӣ�
WorldMap.m_nLastTouchUnitIndex	= -1; -- �ϴε����λ

-- ��ͼ����
Area = {}
-- �������������ȡ������
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

-- ��ͼ�������
local WorldMapPrefab	= nil;	-- ��ͼ��
local MapTmxRoot		= nil;	-- tmx��ͼ���ڵ�
local MapUnitRoot		= nil;	-- ������ʾ����ĸ��ڵ�
local MapLineRoot		= nil;	-- �����߸��ڵ�
local MapClickEffect	= nil;	-- �����Ч
local MapCamera 		= nil;	-- ��ͼ�����

-- �����ť
local m_pClickFunSpy		= nil; -- ��鰴ť
local m_pClickFunAttack		= nil; -- ������ť
local m_pClickFunMoveCity	= nil; -- Ǩ�ǰ�ť
local m_pClickFunOccupy		= nil; -- ռ�찴ť
	
-- �����ͼ��Ϣ����-����˷�������
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
	-- �յ���Ϣ��ת���������ͼ����
	GameManager.ChangeScence( "worldmap" );
end
	
-- �����ͼ����
function WorldMap.Start( Prefab )	
	-- ��ʼ�����ڵ�
	WorldMapPrefab	= Prefab;

	-- ��ȡ���
	MapTmxRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapTmxRoot" );
	MapUnitRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapUnitRoot" );
	MapLineRoot		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapLineRoot" );
	MapClickEffect	= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapClickEffect" );
	MapCamera		= WorldMapPrefab:GetComponent("Transform"):FindChild( "MapCamera" );
	
	-- ��ȡ�����ͼ�Ŀؼ�
	local objs = MapClickEffect:FindChild( "WorldMapClickMod" ):GetComponent( "UIMod" ).relatedGameObject;
	m_pClickFunSpy		= objs[0]; -- ��鰴ť
	m_pClickFunAttack	= objs[1]; -- ������ť
	m_pClickFunMoveCity	= objs[2]; -- Ǩ�ǰ�ť
	m_pClickFunOccupy	= objs[3]; -- ռ�찴ť
	
	-- ���ݵ�ͼ��С��̬����N*N��Tmx��ͼ
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

	-- ���߽���
	local MapBorder	= MapLineRoot:FindChild( "MapBorder" ):GetComponent("LineRenderer");
	MapBorder:SetVertexCount(5)
	MapBorder:SetWidth(0.5,0.5);
	MapBorder:SetPosition( 0, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, 0, 0 ) );
	MapBorder:SetPosition( 1, Vector3.New( MAP_TILEWIDTH/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/2/100, 0 ) );
	MapBorder:SetPosition( 2, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/100, 0 ) );
	MapBorder:SetPosition( 3, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/100, -WorldMap.m_nMaxHeight*MAP_TILEHEIGHT/2/100, 0 ) );
	MapBorder:SetPosition( 4, Vector3.New( WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100,0, 0 ) );
	
	-- �����������ʼλ��Ϊ�ҵĳǳ�
	WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = WorldMap.ConvertGameToCamera( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy )
	MapCamera.transform.position = Vector3.New( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY, -100000 );
	
	-- ���ص�ͼ���ݸ�����һ��
	return WorldMap.m_nMaxWidth, WorldMap.m_nMaxHeight, MAP_TILEWIDTH, MAP_TILEHEIGHT
end

-- ���������=����Ϸ����(���θ�)
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

-- ��Ϸ����(���θ�)=�����������
function WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY )
	--print( "MainCity:"..gameCoorX.." "..gameCoorY )
	local px  = ( MAP_TILEWIDTH/2/100 ) * ( gameCoorX - gameCoorY );
    local py  = -( MAP_TILEHEIGHT/2/100 ) * ( gameCoorX + gameCoorY );
	return px + WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, py
end

-- �����ʾ����
function WorldMap.AddMapUnit( recvValue )
	--print(recvValue.m_unit_index.."|"..recvValue.m_type .."|".. recvValue.m_name)

	-- ���ݲ��뻺��
	table.insert( WorldMap.m_nMapUnitList, recvValue.m_unit_index, recvValue )
	-- ������ʾ����
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
	
	-- λ��
	local posx, posy = WorldMap.ConvertGameToCamera( recvValue.m_posx, recvValue.m_posy )
	unit.transform.localPosition = Vector3.New( posx, posy, zorder );
	
	-- ������Լ�����
	if WorldMap.m_nMyCityUnitIndex == recvValue.m_unit_index then
		-- λ��
		WorldMap.m_nMyCityPosx = recvValue.m_posx
		WorldMap.m_nMyCityPosy = recvValue.m_posy;
		WorldMap.m_nMyCityCameraX = posx; 
		WorldMap.m_nMyCityCameraY = posy;
		-- ����
		unit:GetComponent("Transform"):FindChild("Name"):GetComponent("Text").text = GetLocalizeText(1049);
	else
		unit:GetComponent("Transform"):FindChild("Name"):GetComponent("Text").text = recvValue.m_name;
	end
	
    

	-- �����Ŀ�ĵص㣬����
	if recvValue.m_tposx > 0 or recvValue.m_tposy > 0 or recvValue.m_type == 2 then
		WorldMap.MarchRouteAdd( recvValue.m_unit_index, recvValue.m_posx, recvValue.m_posy, -1, recvValue.m_tposx, recvValue.m_tposy )
		-- �����ƶ�
		local tposx, tposy = WorldMap.ConvertGameToCamera( recvValue.m_tposx, recvValue.m_tposy )
		unit:GetComponent("MapUnitMove").toPosition = Vector3.New( tposx, tposy, 0 );
		unit:GetComponent("MapUnitMove").speed = 0.01;
		unit:GetComponent("MapUnitMove").stat = 1;

        --���ö��� ���� ,todo ����Ӧ�ø��ݱ����������ʾ
        unit:GetComponent("MapUnit").AnimatorName="FightH_SOLDIER_1";
        --�ж��ƶ��ķ���
        if unit.transform.localPosition.x > tposx then
            unit.transform:FindChild("body").transform.rotation=Quaternion.New(0,180,0,1);
        end;
        unit:GetComponent("MapUnit"):Init();
        unit:GetComponent("MapUnit"):PlayAction(0);

	end

	-- ������˹��������Ԫ
	for tmpi = 1, recvValue.m_underfire_count, 1 do
		local from_unit_index = recvValue.m_underfire_frompos[tmpi].m_unit_index;
		local from_posx = recvValue.m_underfire_frompos[tmpi].m_posx;
		local from_posy = recvValue.m_underfire_frompos[tmpi].m_posy;
		WorldMap.MarchRouteAdd( from_unit_index, from_posx, from_posy, recvValue.m_unit_index, recvValue.m_posx, recvValue.m_posy );
	end
	
	-- ���뻺��
	WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"] = unit;
end

-- �Ƴ���ʾ����
function WorldMap.DelMapUnit( recvValue )
	-- �Ƴ���ʾ����
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index] ~= nil then
		GameObject.Destroy( WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"] );
	end
	-- ɾ������
	table.remove( WorldMap.m_nMapUnitList, recvValue.m_unit_index )
end

-- �ƶ���ʾ����
function WorldMap.MoveMapUnit( recvValue )
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index] == nil then
		return;
	end
	local unit = WorldMap.m_nMapUnitList[recvValue.m_unit_index]["unit"];
	local posx, posy = WorldMap.ConvertGameToCamera(recvValue.m_posx, recvValue.m_posy)
	unit.transform.localPosition = Vector3.New( posx, posy, 0 );
end

--  ÿ��֪ͨ3��λ�ñ仯��-�����������
function WorldMap.ViewChangeSec()
	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;

	-- ���������������
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY )
	local now_areaindex = Area.GetIndex( gameCoorX, gameCoorY );
	if now_areaindex < 0 or now_areaindex == WorldMap.m_nLastAreaIndex then
		return
	end
	WorldMap.m_nLastAreaIndex = now_areaindex;
	system_askinfo( ASKINFO_AREAENTER, "", now_areaindex );
end

--  ÿ��֪ͨ30��λ�ñ仯��-�������ͷ�λ
function WorldMap.ViewChangeFrame()
	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;
	
	-- λ��û�仯�Ͳ�������
	if cameraPosX == WorldMap.m_nLastCameraX and cameraPosY == WorldMap.m_nLastCameraY then
		return;
	end
	WorldMap.m_nLastCameraX = cameraPosX;
	WorldMap.m_nLastCameraY	= cameraPosY;
	
	-- ��ͷλ��
	local arrowPos = MainDlgOnDirectionArrowPosition();
	-- �����Լ������뵱ǰ�ӿ����ĵķ�λ�;���
	local cityPos = cc.p( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
	local cameraPos = cc.p( cameraPosX, cameraPosY );
	-- ����
	local distance = cc.pGetDistance( cameraPos, cityPos );
	-- �Ƕ�
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
	-- ��������
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY );
	MainDlgOnDirectionArrow( distance, 180-angle, cc.p(gameCoorX,gameCoorY) );
end

-- �����ͼ
function WorldMap.OnClick( obj, touchpos )
	if obj == nil and MapClickEffect.gameObject.activeSelf == true then
		MapClickEffect.gameObject:SetActive( false );
		return;
	end
	
	-- ��ʾ���Ч��
	MapClickEffect.gameObject:SetActive( true );
	
	-- ����λ��
	local touchpos = Camera.main:ScreenToWorldPoint( touchpos );
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( touchpos.x, touchpos.y );
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	WorldMap.m_nLastTouchGameCoorX = gameCoorX;
	WorldMap.m_nLastTouchGameCoorY = gameCoorY;
	
	-- ���������ǿյ�
	if obj == nil then
		MapClickEffect:FindChild("Back").gameObject:SetActive( true );
		MapClickEffect:GetComponent("Transform").localPosition = Vector3.New( cameraPosX, cameraPosY-MAP_TILEHEIGHT/2/100, -30 );
	else
		MapClickEffect:FindChild("Back").gameObject:SetActive( false );
		MapClickEffect:GetComponent("Transform").localPosition = Vector3.New( obj:GetComponent("Transform").position.x, obj:GetComponent("Transform").position.y, -30 );
	end

	-- ����ǿյػ��߲��ǳǳ�
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

-- ���
function WorldMap.ClickFunSpy()
	system_askinfo( ASKINFO_WORLDMAP, "", 2 );
	MapClickEffect.gameObject:SetActive( false );
end

-- ����
function WorldMap.ClickFunAttack()
    MapClickEffect.gameObject:SetActive( false );
    TroopDlgOpen();
	
end

-- Ǩ��
function WorldMap.ClickFunMoveCity()
	system_askinfo( ASKINFO_WORLDMAP, "", 4, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY );
	MapClickEffect.gameObject:SetActive( false );
end

-- ռ��
function WorldMap.ClickFunOccupy()
	--system_askinfo( ASKINFO_WORLDMAP, "", 5, -1, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY );
	
    TroopDlgOpen();
    MapClickEffect.gameObject:SetActive( false );
end

-- ���سǳ�
function WorldMap.ReturnCity()
	WorldMap.m_nLastAreaIndex 	= 	-1;
	system_askinfo( ASKINFO_AREAENTER, "", WorldMap.m_nLastAreaIndex );
	GameManager.ChangeScence( "city" );
end

-- ����о�·�ߵ�����
function WorldMap.MarchRouteAdd( from_unit_index, from_posx, from_posy, to_unit_index, to_posx, to_posy )
	-- ���֮ǰ�ȼ�黺��
	if  WorldMap.m_nMarchRouteList[from_unit_index] ~= nil then
		return;
	end

	-- ����
	local fposx, fposy = WorldMap.ConvertGameToCamera(from_posx, from_posy)
	local tposx, tposy = WorldMap.ConvertGameToCamera(to_posx, to_posy)
	local obj = WorldMap.MarchRouteDraw( Vector3.New( fposx, fposy, 0 ), Vector3.New( tposx, tposy, 0 ), 1, MapLineRoot )
	-- ���뻺��
	table.insert( WorldMap.m_nMarchRouteList, from_unit_index, 
	{m_from_posx=from_posx, m_from_posy=from_posy, m_to_unit_index=to_unit_index, m_to_posx=to_posx, m_to_posy=to_posy, m_obj = obj} );
end

-- ɾ���о�·��
function WorldMap.MarchRouteDel( from_unit_index )
	-- ɾ��֮ǰ�ȼ�黺��
	if  WorldMap.m_nMarchRouteList[from_unit_index] == nil then
		return;
	end
	-- ɾ����
	GameObject.Destroy( WorldMap.m_nMarchRouteList[from_unit_index]["m_obj"] );
	-- ɾ������
	table.remove( WorldMap.m_nMarchRouteList, from_unit_index );
end

-- ���о�·��
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
