-- 消息框（无按钮）
NotifyDlg = class("NotifyDlg",function()
    return cc.Node:create()
end)

-- 构造函数
function NotifyDlg:ctor()

end

-- 初始化
function NotifyDlg:init()
	self:setName("NotifyDlg")
	
	self:setPosition(VisibleRect:center())
	self:setScaleY(0.0)
	
	-- 背景图片
    local imageView = ccui.ImageView:create()
    imageView:setScale9Enabled( true )
    imageView:loadTexture( "ui_jianbian1.png" )
    imageView:setContentSize( cc.size(480, 41) )
    self:addChild( imageView )
	
	-- 警告文字
	self._uiText = ccui.Text:create()
    self._uiText:setString( "" )
	self._uiText:setTextColor( cc.c4b( 255,255,255,255 ) )
    self._uiText:setFontSize( 24 )
	self._uiText:ignoreContentAdaptWithSize(false)
	self._uiText:setTextAreaSize( cc.size(480, 41) )
	self._uiText:setPosition( cc.p(imageView:getContentSize().width/2, imageView:getContentSize().height/2 ) )
	self._uiText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_CENTER )
	self._uiText:setTextVerticalAlignment( cc.TEXT_ALIGNMENT_CENTER )
    imageView:addChild( self._uiText )
	
	-- 显示动作
	local scaleTo = cc.ScaleTo:create( 0.1, 1.0, 1.0 );
	local delay = cc.DelayTime:create( 2.0 );
	local scaleToBack = cc.ScaleTo:create( 0.1, 1.0, 0.0 );
	local callFunc = cc.CallFunc:create(function() self:removeFromParent() end);
    local sequence = cc.Sequence:create( scaleTo, delay, scaleToBack, callFunc )
	self:runAction( sequence )
    return true;
end

-- 创建
function NotifyDlg.create()
    local object = NotifyDlg.new();
    if object:init() == false then
        return nil;
    end
    return object;
end

-- 显示
function Notify( szText )
	local dlg = NotifyDlg.create()
	dlg._uiText:setString( szText )
	UIManager.addTop( dlg );
end
