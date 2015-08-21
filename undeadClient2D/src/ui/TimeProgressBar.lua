-- 消息框（无按钮）
TimeProgressBar = class("TimeProgressBar",function()
    return cc.Node:create()
end)

-- 构造函数
function TimeProgressBar:ctor()
	self._totalSec = 0;
	self._sec = 0;
end

-- 初始化
function TimeProgressBar:init()
	
	 -- 监听对象事件
    local function onNodeEvent(event)
        if event == "enter" then
           
        elseif event == "exit" then
		
        end
    end
    self:registerScriptHandler(onNodeEvent)
	
	-- 每秒调用
	schedule( self, function(dt)  
		self._sec = self._sec - 1;
		if self._sec >= 0 then
			return;
		end
		self._uiText:setString( GetTimeFomat( self._sec ) );
		self._uiProgress:setPercent( (self._totalSec-self._sec)/self._totalSec*100 )
	end, 1 );
	
	-- 进度条
    self._uiProgress = ccui.LoadingBar:create()
    self._uiProgress:loadTexture("UI_zaobing_lantiao.png")
    self._uiProgress:setPercent(0)
	self:addChild( self._uiProgress )
	
	-- 进度显示
	self._uiText = ccui.Text:create()
    self._uiText:setString( "" )
	self._uiText:setTextColor( cc.c4b( 0,255,0,255 ) )
    self._uiText:setFontSize( 16 )
	self._uiText:setPosition( cc.p(0, 0) )
	self._uiText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_CENTER )
	self._uiText:setTextVerticalAlignment( cc.TEXT_ALIGNMENT_CENTER )
    self:addChild( self._uiText )
    return true;
end

-- 设置倒计时
function TimeProgressBar:setCountdown( totalsec, sec )
	self._totalSec = totalsec;
	self._sec = sec;
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
