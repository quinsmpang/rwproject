-- 提示框（双按钮）
local BaseDlg = import("ui/BaseDlg")
MsgBoxDlg = class("MsgBoxDlg", BaseDlg)

-- 构造函数
function MsgBoxDlg:ctor()
	MsgBoxDlg.super.ctor( self );
	self._uiButton = { nil, nil }
	self._callBackFun = { nil, nil };
end

-- 初始化
function MsgBoxDlg:init()
	UIManager.addTop( self );
	self:setName("MsgBoxDlg")
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
    imageView:setContentSize( cc.size(400, 240) )
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
            if name == "button1" then
				MsgBoxDlg.close()
				if self._callBackFun[1] then
					self._callBackFun[1]()
				end
			elseif name == "button2" then
				MsgBoxDlg.close()
				if self._callBackFun[2] then
					self._callBackFun[2]()
				end
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- button按钮
	local texRes = { "UI_jianzao_anniu1.png", "UI_jianzao_anniu2.png" }
	local pos = { -100, 100 }
	for i=1, 2, 1 do
		self._uiButton[i] = ccui.Button:create()
		self._uiButton[i]:setName("button"..i)
		self._uiButton[i]:setTitleText("")
		self._uiButton[i]:setTitleFontSize(32);
		self._uiButton[i]:setScale9Enabled(true)
		self._uiButton[i]:loadTextures(texRes[i], "", "")
		self._uiButton[i]:setContentSize(cc.size(180, 60))
		self._uiButton[i]:setPosition( cc.p(imageView:getContentSize().width/2+pos[i], imageView:getContentSize().height/2-64 ) )
		self._uiButton[i]:addTouchEventListener(buttonEvent)
		imageView:addChild(self._uiButton[i])
	end
	return true;
end

-- 创建
function MsgBoxDlg.create()
    local object = MsgBoxDlg.new();
    if object:init() == false then
        return nil;
    end
    return object;
end

-- 唯一实例
function MsgBoxDlg.getInstance()
	local dlg = UIManager.getTopDlg( "MsgBoxDlg" )
	if dlg == nil then
		dlg = MsgBoxDlg.create()
	end
	return dlg;
end

-- 打开
function MsgBoxDlg.open()
	MsgBoxDlg.getInstance():_open()
end

-- 关闭
function MsgBoxDlg.close()
	MsgBoxDlg.getInstance():_close()
end

function MsgBox( text, btnname1, callback1, btnname2, callback2 )
	AlertDlg.close()
	MsgBoxDlg.open()
	MsgBoxDlg.getInstance()._uiText:setString( text )
	MsgBoxDlg.getInstance()._uiButton[1]:setTitleText( btnname1 )
	MsgBoxDlg.getInstance()._uiButton[2]:setTitleText( btnname2 )
	MsgBoxDlg.getInstance()._callBackFun[1] = callback1
	MsgBoxDlg.getInstance()._callBackFun[2] = callback2
end
