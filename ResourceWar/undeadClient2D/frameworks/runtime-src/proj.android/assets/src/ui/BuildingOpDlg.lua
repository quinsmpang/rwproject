-- 建筑升级和其他操作
BuildingOpDlg = {}
local this = BuildingOpDlg;
local m_dlg = nil;

local m_hit = false;

-- 建筑类型
local m_openBuildingindex = -1;

-- 初始化
function BuildingOpDlg.init()
	m_dlg = cc.Node:create();
    m_dlg:setName("BuildingOpDlg");
	
	-- 自定义初始化
	-- 半圆背景
    local semiCircleImageNode = ccui.ImageView:create()
    semiCircleImageNode:loadTexture( "UI_zhujiemian_jianzhutandi.png" )
    m_dlg:addChild( semiCircleImageNode )
	
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "button1" then
				system_askinfo( ASKINFO_BUILDING, "", 1, m_openBuildingindex );
				this.close()
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- 按钮1
	local button = ccui.Button:create()
	button:setName("button1")
	button:setTitleText("升级")
    button:setTitleFontSize(22);
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_zhujiemian_jianzhutankuang.png", "", "")
	button:setPosition( cc.p( 80, 296 ) )
	button:addTouchEventListener(buttonEvent)
	semiCircleImageNode:addChild(button)
	
	-- 按钮2
	button = ccui.Button:create()
	button:setName("button2")
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_zhujiemian_jianzhutankuang.png", "", "")
	button:setPosition( cc.p( 114, 220 ) )
	button:addTouchEventListener(buttonEvent)
	semiCircleImageNode:addChild(button)
	
	-- 按钮3
	button = ccui.Button:create()
	button:setName("button3")
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_zhujiemian_jianzhutankuang.png", "", "")
	button:setPosition( cc.p( 116, 136 ) )
	button:addTouchEventListener(buttonEvent)
	semiCircleImageNode:addChild(button)
	
	-- 按钮4
	button = ccui.Button:create()
	button:setName("button4")
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_zhujiemian_jianzhutankuang.png", "", "")
	button:setPosition( cc.p( 80, 60 ) )
	button:addTouchEventListener(buttonEvent)
	semiCircleImageNode:addChild(button)
	
	
	-- 点击事件,用作点击框外关闭
    local function onTouchBegan(touch, event)
		m_hit = true;
        return true
    end
	local function onTouchMoved( touch, event )
		if m_hit == true then
			local distance = cc.pGetDistance( touch:getStartLocation(), touch:getLocation() )
			if distance > 5 then -- 容差
				m_hit = false
			end
		end
		return
	end 
	local function onTouchEnded( touch, event )
		if m_hit == false then
			return
		end
		this.close()
	end
	local listener = cc.EventListenerTouchOneByOne:create()
	listener:setSwallowTouches(false)
	listener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
	listener:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
	listener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
	local eventDispatcher = m_dlg:getEventDispatcher()
	eventDispatcher:addEventListenerWithSceneGraphPriority( listener, m_dlg )
end

-- 打开
function BuildingOpDlg.open( parent, index )
	BuildingOpDlg.close()
	if m_dlg == nil then
		this.init();
	end
	m_openBuildingindex = index;
	parent:addChild( m_dlg )
	m_dlg:setPosition( cc.p( 100, 0 ) )
	m_dlg:setVisible( true );
end

-- 关闭
function BuildingOpDlg.close()
	if m_dlg then
		m_dlg:removeFromParent();
		m_dlg = nil;
	end
	m_openBuildingindex = -1;
end


