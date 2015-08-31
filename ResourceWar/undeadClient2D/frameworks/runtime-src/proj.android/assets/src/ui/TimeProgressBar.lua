-- 消息框（无按钮）
TimeProgressBar = class("TimeProgressBar",function()
    return cc.Node:create()
end)

-- 构造函数
function TimeProgressBar:ctor()
	self._schedulerEntry = nil;
	self._callbackFun = nil;
	self._finishSec = 0;
	self._needSec = 0
end

-- 初始化
function TimeProgressBar:init()
	
	-- 时间逻辑
	local function timer()
		-- 还差多少秒 = 完成时间-当前时间
		local diffSec = self._finishSec - GetServerTime();
		if diffSec < 0 then
			if self._callbackFun then
				self._callbackFun();
			end
			self:removeFromParent();
			return;
		end
		self._uiText:setString( GetTimeFomat( diffSec ) );
		self._uiProgress:setPercent( (self._needSec-diffSec)/self._needSec*100 )
	end

	 -- 监听对象事件
    local function onNodeEvent(event)
        if event == "enter" then
           self._schedulerEntry = scheduler:scheduleScriptFunc(timer, 1, false)
        elseif event == "exit" then
			if self._schedulerEntry ~= nil then
                scheduler:unscheduleScriptEntry(self._schedulerEntry)
            end
        end
    end
    self:registerScriptHandler(onNodeEvent)
		
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

-- 设置完成时间
function TimeProgressBar:setFinishTime( finish_sec, callback )
	self._finishSec = finish_sec;
	self._needSec = finish_sec - GetServerTime();
	self._callbackFun = callback;
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
