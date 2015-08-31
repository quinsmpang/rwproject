-- 建筑建造
BuildingCreateDlg = {}
local this = BuildingCreateDlg;
local m_dlg = nil;

-- 展示的建筑图片
local m_uiBuildShowImage = nil;

-- 建筑列表容器控件
local m_uiBuildingListView = nil;

-- 建筑列表缓存
local m_buildingCache = {};

-- 当前选择的建筑信息
local m_selectBuilding = nil;

-- 打开的建筑地块索引
local m_openBuildingindex = -1;

-- 建造按钮
local m_uiCreateBtn = nil;

-- 建造按钮
local m_uiCreateBtnEx = nil;

-- 初始化
function BuildingCreateDlg.init()
	m_dlg = cc.Node:create();
    m_dlg:setName("BuildingCreateDlg");
	UIManager.addCenter( m_dlg );
	
	-- 自定义初始化
	m_dlg:setPosition(VisibleRect:center())
	
	-- 背景图片
    local backImageNode = ccui.ImageView:create()
    backImageNode:setScale9Enabled( true )
    backImageNode:loadTexture( "ui/ui_back1.png" )
    backImageNode:setContentSize( SCREEN_SIZE )
    m_dlg:addChild( backImageNode )
	
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "close" then
				this.close()
			elseif name == "create" then -- 建造
				if m_selectBuilding == nil then
					return;
				end 
				system_askinfo( ASKINFO_BUILDING, "", 0, m_openBuildingindex, m_selectBuilding["kind"] );
				this.close()
			elseif name == "createex" then -- 立即建造
				this.close()
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- 关闭按钮
	local button = ccui.Button:create()
	button:setName("close")
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_jianzao_fanhui.png", "UI_jianzao_fanhui.png", "")
	button:setAnchorPoint(cc.p(0, 1))
	button:setPosition( VisibleRect:leftTop() )
	button:addTouchEventListener(buttonEvent)
	backImageNode:addChild(button)
	
	-- 建造按钮
	m_uiCreateBtn = ccui.Button:create()
	m_uiCreateBtn:setName("create")
	m_uiCreateBtn:setTitleText("建造")
	m_uiCreateBtn:setTitleFontSize(32);
	m_uiCreateBtn:setScale9Enabled(true)
	m_uiCreateBtn:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	m_uiCreateBtn:setContentSize(cc.size(220, 60))
	m_uiCreateBtn:setPosition( cc.p( VisibleRect:center().x + 160, VisibleRect:rightBottom().y+100) )
	m_uiCreateBtn:addTouchEventListener(buttonEvent)
	backImageNode:addChild(m_uiCreateBtn)
	
	-- 立即建造按钮
	m_uiCreateBtnEx = ccui.Button:create()
	m_uiCreateBtnEx:setName("createex")
	m_uiCreateBtnEx:setTitleText("立即建造")
	m_uiCreateBtnEx:setTitleFontSize(32);
	m_uiCreateBtnEx:setScale9Enabled(true)
	m_uiCreateBtnEx:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	m_uiCreateBtnEx:setContentSize(cc.size(220, 60))
	m_uiCreateBtnEx:setPosition( cc.p( VisibleRect:center().x - 160, VisibleRect:rightBottom().y+100) )
	m_uiCreateBtnEx:addTouchEventListener(buttonEvent)
	backImageNode:addChild(m_uiCreateBtnEx)
	
	-- 建筑图片展示
	m_uiBuildShowImage = ccui.ImageView:create()
	m_uiBuildShowImage:loadTexture("")
	m_uiBuildShowImage:setPosition(cc.p(VisibleRect:center().x-80, VisibleRect:center().y+80))
	backImageNode:addChild(m_uiBuildShowImage)
		
	 -- 列表事件
    local function serverListViewEvent(sender, eventType)
        if eventType == ccui.ListViewEventType.ONSELECTEDITEM_END then
            local selectedIndex = sender:getCurSelectedIndex()+1;
			m_selectBuilding = m_buildingCache[selectedIndex];
			m_uiBuildShowImage:loadTexture(m_selectBuilding["image"])
        end
    end
    
    -- 建筑列表视图
    m_uiBuildingListView = ccui.ListView:create()
    m_uiBuildingListView:setDirection( ccui.ScrollViewDir.vertical )
    m_uiBuildingListView:setBounceEnabled( true )
    m_uiBuildingListView:setContentSize( cc.size(128, 128*5) )
    m_uiBuildingListView:setPosition( VisibleRect:right().x-128, (VisibleRect:leftTop().y-128*5) )
    m_uiBuildingListView:addEventListener( serverListViewEvent )
    backImageNode:addChild(m_uiBuildingListView)
end

-- 设置服务器列表
function BuildingCreateDlg.setBuildingList( grid )
	m_buildingCache = {}
	m_uiBuildShowImage:loadTexture(BuildingResList[0]["res"][1]);
	m_uiBuildingListView:removeAllChildren();
    for i= 1, #BuildingResList, 1 do
		if BuildingResList[i]["res"][1] ~= "" then
			-- 插入缓存
			table.insert( m_buildingCache, {kind=i,image=BuildingResList[i]["res"][1]} )
			-- 创建布局
			local layout = ccui.Layout:create()
			layout:setTag( i );
			layout:setContentSize(cc.size(128, 128))
			-- 建筑以按钮形式添加
			local buildButton = ccui.Button:create()
			buildButton:loadTextures(BuildingResList[i]["res"][1], "", "")
			buildButton:setAnchorPoint(cc.p(0.5, 0.5))
			buildButton:setScale(0.5)
			buildButton:setPosition( cc.p( layout:getContentSize().width/2, layout:getContentSize().height/2 ) )
			layout:addChild(buildButton)
			m_uiBuildingListView:pushBackCustomItem(layout)
		end
    end
end

-- 打开
function BuildingCreateDlg.open( index )
	m_dlg = UIManager.getCenterDlg( "BuildingCreateDlg" )
	if m_dlg == nil then
		this.init();
	end
	m_openBuildingindex = index;
	this.setBuildingList( 1 )
	m_dlg:setVisible( true );
end

-- 关闭
function BuildingCreateDlg.close()
	m_dlg = UIManager.getCenterDlg( "BuildingCreateDlg" )
	if m_dlg then
		m_dlg:setVisible( false );
	end
	m_selectBuilding = nil;
	m_openBuildingindex = -1;
end

-- 销毁
function BuildingCreateDlg.destroy()
	m_dlg = UIManager.getCenterDlg( "BuildingCreateDlg" )
	if m_dlg then
		m_dlg:removeFromParent();
		m_dlg = nil;
	end
end

