-- 警示框（单按钮）
AlertDlg = {}
local m_dlg = nil;
local m_uiText = nil;
local m_uiButton = nil;
local m_maskListener = nil;
local m_callBackFun = nil;

-- 初始化
function AlertDlg.init()
	m_dlg = cc.Node:create();
    m_dlg:setName("AlertDlg");
	UIManager.addTop( m_dlg );
	
	-- 自定义初始化
	m_dlg:setPosition(VisibleRect:center())
	
	--模糊背景层
	local layout = ccui.Layout:create()
	layout:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
	layout:setBackGroundColor(cc.c3b(142,143,137))
	layout:setContentSize( SCREEN_SIZE )
	layout:setAnchorPoint(cc.p(0.5, 0.5))
	layout:setBackGroundColorOpacity(100)
	layout:setLocalZOrder(-1)
	m_dlg:addChild(layout)

	-- 背景图片
    local imageView = ccui.ImageView:create()
    imageView:setScale9Enabled( true )
    imageView:loadTexture( "ui/ui_back1.png" )
    imageView:setContentSize( cc.size(360, 240) )
    m_dlg:addChild( imageView )
	
	-- 警告文字
	m_uiText = ccui.Text:create()
    m_uiText:setString( "" )
	m_uiText:setTextColor( cc.c4b( 0,0,0,255 ) )
    m_uiText:setFontSize( 24 )
	m_uiText:ignoreContentAdaptWithSize(false)
	m_uiText:setTextAreaSize( cc.size(320, 120) )
	m_uiText:setPosition( cc.p(imageView:getContentSize().width/2, imageView:getContentSize().height/2+32 ) )
	m_uiText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_CENTER )
	m_uiText:setTextVerticalAlignment( cc.TEXT_ALIGNMENT_CENTER )
    imageView:addChild( m_uiText )
	
	--屏蔽层
	local function onTouchBegan( touch, event )
		return true
	end
	local function onTouchMoved( touch, event )
		return true
	end 
	local function onTouchEnded( touch, event )
	end
	m_maskListener = cc.EventListenerTouchOneByOne:create()
	m_maskListener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
	m_maskListener:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
	m_maskListener:setSwallowTouches(true)
	local eventDispatcher = m_dlg:getEventDispatcher()
	eventDispatcher:addEventListenerWithSceneGraphPriority(m_maskListener, m_dlg)

	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "button" then
				AlertDlg.close()
				if m_callBackFun then
					m_callBackFun()
				end
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- button按钮
	m_uiButton = ccui.Button:create()
	m_uiButton:setName("button")
	m_uiButton:setTitleText("")
	m_uiButton:setTitleFontSize(32);
	m_uiButton:setScale9Enabled(true)
	m_uiButton:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	m_uiButton:setContentSize(cc.size(220, 60))
	m_uiButton:setPosition( cc.p(imageView:getContentSize().width/2, imageView:getContentSize().height/2-64 ) )
	m_uiButton:addTouchEventListener(buttonEvent)
	imageView:addChild(m_uiButton)
end

-- 打开
function AlertDlg.open()
	if m_dlg then
		m_dlg:setVisible( true );
		m_maskListener:setSwallowTouches(true)
		return;
	end
	AlertDlg.init();
	
end

-- 关闭
function AlertDlg.close()
	if m_dlg then
		m_dlg:setVisible( false );
		m_maskListener:setSwallowTouches(false)
	end
end

-- 销毁
function AlertDlg.destroy()
	if m_dlg then
		m_dlg:removeFromParent();
	end
end

function Alert( text, button_name, callback )
	AlertDlg.open()
	m_uiText:setString( text )
	m_uiButton:setTitleText(button_name)
	m_callBackFun = callback
end
