-- 预加载
local LoadTextureList= { 
"human_map.jpg",
"human_worker.png",
"dragon_shadow.pvr.ccz",
}

-- 预加载
local LoadScene = class("LoadScene",function()
    return cc.Scene:create()
end)

-- 构造
function LoadScene:ctor()
    self._callbackfun = nil
	self._textureOffset = 0
end

-- 初始化
function LoadScene:init( callbackfun )
	self._callbackfun = callbackfun
	-- 监听事件
    local function onNodeEvent(event)
        if event == "enter" then
        
        elseif event == "exit" then
            collectgarbage("collect")
            
        elseif event == "enterTransitionFinish" then
            self:onEnterTransitionDidFinish()
        end
    end
    self:registerScriptHandler(onNodeEvent)
	
	-- 进度条
    self._uiProgress = ccui.LoadingBar:create()
    self._uiProgress:loadTexture("UI_zaobing_lantiao.png")
    self._uiProgress:setPercent(0)
	self._uiProgress:setPosition( VisibleRect:center() )
	self:addChild( self._uiProgress )
	
	-- loading
	self._uiLoadingLabel = cc.Label:createWithBMFont("Font1.fnt", "LOADING")
    self:addChild(self._uiLoadingLabel)
    self._uiLoadingLabel:setPosition( cc.p(VisibleRect:center().x, VisibleRect:center().y+60) )
	
	-- 执行动作
	for i=0, 6, 1 do
		local sequence = cc.Sequence:create( cc.DelayTime:create((i+1)*0.1), cc.CallFunc:create(function()
			self._uiLoadingLabel:getLetter(i):runAction(cc.RepeatForever:create( cc.JumpBy:create(0.5, cc.p(0, 0), 20, 1) ))
		end) )
		self._uiLoadingLabel:getLetter(i):runAction(sequence)
	end
	
	
    return true;
end

-- 场景进入完毕后调用
function LoadScene:onEnterTransitionDidFinish()
	
	-- 配置数据初始化 
	Data.OnSecondInit()
		
	-- 纹理已加载
	local function textureLoaded( texture )
        self._textureOffset = self._textureOffset + 1;
		-- 设置进度条进度
		self._uiProgress:setPercent( self._textureOffset/#LoadTextureList*100 )
		-- 加载完毕，回调
		if self._textureOffset >= #LoadTextureList then
			if self._callbackfun then
				self._callbackfun();
			end
		end
    end
	
	-- 加载列表里的纹理
	for i=1, #LoadTextureList, 1 do
		cc.Director:getInstance():getTextureCache():addImageAsync( LoadTextureList[i], textureLoaded )
	end
end

function LoadScene.create( callbackfun )
    local scene = LoadScene.new()
    if scene:init( callbackfun ) == false then
        return nil;
    end
    return scene
end

return LoadScene