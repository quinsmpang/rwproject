-- 消息框（无按钮）
TimeProgressBar = class("TimeProgressBar",function()
    return cc.Node:create()
end)

-- 构造函数
function TimeProgressBar:ctor()
	
end

-- 初始化
function TimeProgressBar:init()
	
	-- 进度条
    self._uiProgress = ccui.LoadingBar:create()
    self._uiProgress:loadTexture("UI_zaobing_lantiao.png")
	self._uiProgress:setScale9Enabled(true)
	self._uiProgress:setContentSize(cc.size(140, 20))
    self._uiProgress:setPercent(100)
	self:addChild( self._uiProgress )
	
	-- 进度显示
	self._uiText = ccui.Text:create()
    self._uiText:setString( "23h:45h:19s" )
	self._uiText:setTextColor( cc.c4b( 0,255,0,255 ) )
    self._uiText:setFontSize( 16 )
	self._uiText:setPosition( cc.p(0, 0) )
	self._uiText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_CENTER )
	self._uiText:setTextVerticalAlignment( cc.TEXT_ALIGNMENT_CENTER )
    self:addChild( self._uiText )
    return true;
end

-- 设置进度显示字符
function TimeProgressBar:setProgressString( str )
	--self._label:setString( "1111111111111111111111111111111111111" )
end


-- 创建
function TimeProgressBar.create()
    local object = TimeProgressBar.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return TimeProgressBar
