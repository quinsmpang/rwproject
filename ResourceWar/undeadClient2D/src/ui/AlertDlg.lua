-- 警示框（单按钮）
local BaseDlg = import("ui/BaseDlg")
AlertDlg = class("AlertDlg", BaseDlg)

-- 构造函数
function AlertDlg:ctor()
	AlertDlg.super.ctor(self)
	self._callBackFun = nil;
end

-- 初始化
function AlertDlg:init()
	UIManager.addTop( self );
	self:setName("AlertDlg")
	self:setPosition(VisibleRect:center())

	--模糊背景层
	local layout = ccui.Layout:create()
	layout:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
	layout:setBackGroundColor(cc.c3b(142,143,137))
	layout:setContentSize( SCREEN_SIZE )
	layout:setAnchorPoint(cc.p(0.5, 0.5))
	layout:setBackGroundColorOpacity(100)
	layout:setLocalZOrder(-1)
	self:addChild(layout)

	-- 背景图片
    local imageView = ccui.ImageView:create()
    imageView:setScale9Enabled( true )
    imageView:loadTexture( "ui_back1.png" )
    imageView:setContentSize( cc.size(360, 240) )
    self:addChild( imageView )
	
	-- 警告文字
	self._uiText = ccui.Text:create()
    self._uiText:setString( "" )
	self._uiText:setTextColor( cc.c4b( 0,0,0,255 ) )
    self._uiText:setFontSize( 24 )
	self._uiText:ignoreContentAdaptWithSize(false)
	self._uiText:setTextAreaSize( cc.size(320, 120) )
	self._uiText:setPosition( cc.p(imageView:getContentSize().width/2, imageView:getContentSize().height/2+32 ) )
	self._uiText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_CENTER )
	self._uiText:setTextVerticalAlignment( cc.TEXT_ALIGNMENT_CENTER )
    imageView:addChild( self._uiText )
	
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "button" then
				AlertDlg.close()
				if self._callBackFun then
					self._callBackFun()
				end
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- button按钮
	self._uiButton = ccui.Button:create()
	self._uiButton:setName("button")
	self._uiButton:setTitleText("")
	self._uiButton:setTitleFontSize(32);
	self._uiButton:setScale9Enabled(true)
	self._uiButton:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	self._uiButton:setContentSize(cc.size(220, 60))
	self._uiButton:setPosition( cc.p(imageView:getContentSize().width/2, imageView:getContentSize().height/2-64 ) )
	self._uiButton:addTouchEventListener(buttonEvent)
	imageView:addChild(self._uiButton)
	return true;
end

-- 创建
function AlertDlg.create()
    local object = AlertDlg.new();
    if object:init() == false then
        return nil;
    end
    return object;
end

-- 唯一实例
function AlertDlg.getInstance()
	local dlg = UIManager.getTopDlg( "AlertDlg" )
	if dlg == nil then
		dlg = AlertDlg.create()
	end
	return dlg;
end

-- 打开
function AlertDlg.open()
	AlertDlg.getInstance():_open()
end

-- 关闭
function AlertDlg.close()
	AlertDlg.getInstance():_close()
end

function Alert( text, button_name, callback )
	MsgBoxDlg.close()
	AlertDlg.open()
	AlertDlg.getInstance()._uiText:setString( text )
	AlertDlg.getInstance()._uiButton:setTitleText( button_name )
	AlertDlg.getInstance()._callBackFun = callback
end
