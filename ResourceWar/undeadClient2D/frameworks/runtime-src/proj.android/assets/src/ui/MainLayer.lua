-- 主UI
MainLayer = class("MainLayer",function()
    return cc.Node:create()
end)

-- 构造函数
function MainLayer:ctor()
  
end

-- 初始化
function MainLayer:init()
	UIManager.addBottom( self );
	self:setName("MainLayer")

	-- 创建最上层
	self:createTopNode();
	
	-- 创建最下层
	self:createBottomNode();

    return true;
end

-- 创建最上层
function MainLayer:createTopNode()
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "xx" then
			elseif name == "xxx" then
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- 底图
	local imageView = ccui.ImageView:create()
    imageView:loadTexture( "UI_zhujiemian_shuxinglandi.png" )
	imageView:setScale9Enabled(true)
	imageView:setContentSize( cc.size( SCREEN_SIZE.width, 91 ) )
	imageView:setAnchorPoint(cc.p(0.5, 1))
	imageView:setPosition( VisibleRect:top() )
	self:addChild( imageView )	

	-- 木头
	self._resWoodSprite = cc.Sprite:create( "UI_zhujiemian_mutou2.png" )
	self._resWoodSprite:setPosition( cc.p( 150, 30 ) )
	self._resWoodSprite:setScale(0.4)
	imageView:addChild( self._resWoodSprite )
	
	self._resWoodText = ccui.Text:create()
    self._resWoodText:setString( "0" )
    self._resWoodText:setFontSize( 20 )
	self._resWoodText:setAnchorPoint(ccp(0,0.5));
	self._resWoodText:setPosition( cc.p( 165, 30 ) )
	self._resWoodText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
    imageView:addChild( self._resWoodText )
	EventProtocol.addEventListener( "ChangeWood", function( recvValue )
		self._resWoodText:setString( recvValue.value )
		self._resWoodSprite:runAction( action_resjump() );
	end )
	
	-- 粮食
	self._resFoodSprite = cc.Sprite:create( "UI_zhujiemian_liangshi.png" )
	self._resFoodSprite:setPosition( cc.p( 250, 30 ) )
	self._resFoodSprite:setScale(0.4)
	imageView:addChild( self._resFoodSprite )
	
	self._resFoodText = ccui.Text:create()
    self._resFoodText:setString( "0" )
    self._resFoodText:setFontSize( 20 )
	self._resFoodText:setAnchorPoint(ccp(0,0.5));
	self._resFoodText:setPosition( cc.p( 265, 30 ) )
	self._resFoodText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
    imageView:addChild( self._resFoodText )
	EventProtocol.addEventListener( "ChangeFood", function( recvValue ) 
		self._resFoodText:setString( recvValue.value )
		self._resFoodSprite:runAction( action_resjump() );
	end )
	
	-- 铁
	self._resIronSprite = cc.Sprite:create( "UI_zhujiemian_tiekuang2.png" )
	self._resIronSprite:setPosition( cc.p( 350, 30 ) )
	self._resIronSprite:setScale(0.4)
	imageView:addChild( self._resIronSprite )
	
	self._resIronText = ccui.Text:create()
    self._resIronText:setString( "0" )
    self._resIronText:setFontSize( 20 )
	self._resIronText:setAnchorPoint(ccp(0,0.5));
	self._resIronText:setPosition( cc.p( 365, 30 ) )
	self._resIronText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
    imageView:addChild( self._resIronText )
	EventProtocol.addEventListener( "ChangeIron", function( recvValue ) 
		self._resIronText:setString( recvValue.value )
		self._resIronSprite:runAction( action_resjump() );
	end )
	
	-- 秘银
	self._resMithrilSprite = cc.Sprite:create( "UI_zhujiemian_miyin2.png" )
	self._resMithrilSprite:setPosition( cc.p( 450, 30 ) )
	self._resMithrilSprite:setScale(0.4)
	imageView:addChild( self._resMithrilSprite )
	
	self._resMithrilText = ccui.Text:create()
    self._resMithrilText:setString( "0" )
    self._resMithrilText:setFontSize( 20 )
	self._resMithrilText:setAnchorPoint(ccp(0,0.5));
	self._resMithrilText:setPosition( cc.p( 465, 30 ) )
	self._resMithrilText:setTextHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
    imageView:addChild( self._resMithrilText )
	EventProtocol.addEventListener( "ChangeMithril", function( recvValue ) 
		self._resMithrilText:setString( recvValue.value )
		self._resMithrilSprite:runAction( action_resjump() );
	end )
end
	
-- 创建最下层
function MainLayer:createBottomNode()
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "worldmap" then
			elseif name == "hero" then
			elseif name == "quest" then
			elseif name == "item" then
			elseif name == "mail" then
			elseif name == "club" then
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    

	-- 下层按钮栏
    local buttonFieldView = ccui.ImageView:create()
    buttonFieldView:loadTexture( "UI_zhujiemian_caozuodi.png" )
	buttonFieldView:setAnchorPoint(cc.p(0.5, 0))
	buttonFieldView:setPosition( VisibleRect:bottom() )
	buttonFieldView:setScale(VISIBLE_SIZE.width/DESIGN_SIZE.width)
    self:addChild( buttonFieldView )
	
	-- 世界地图 按钮
	local button = ccui.Button:create()
	button:setName("worldmap")
	button:setTitleText("世界地图")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_ditutubiao.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 65, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 64, 24 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
	
	-- 英雄
	button = ccui.Button:create()
	button:setName("hero")
	button:setTitleText("英雄")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_yingxiong.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 173, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 48, 0 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
	
	-- 任务
	button = ccui.Button:create()
	button:setName("quest")
	button:setTitleText("任务")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_renwu.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 271, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 48, 0 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
	
	-- 背包
	button = ccui.Button:create()
	button:setName("item")
	button:setTitleText("背包")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_baoguo.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 369, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 48, 0 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
	
	-- 邮件
	button = ccui.Button:create()
	button:setName("mail")
	button:setTitleText("邮件")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_youjian.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 467, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 48, 0 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
	
	-- 联盟
	button = ccui.Button:create()
	button:setName("club")
	button:setTitleText("联盟")
	button:setTitleFontSize(24);
	button:loadTextures("UI_zhujiemian_lianmengbiao.png", "", "")
	button:setPressedActionEnabled(true)
	button:setPosition( cc.p( 576, 63 ) )
	buttonFieldView:addChild( button )
	button:getTitleRenderer():setPosition( cc.p( 64, 24 ) )
	button:getTitleRenderer():enableOutline(cc.c4b(0,0,0,255),1);
	button:setTitleColor(cc.c3b(255,252,217))
	button:addTouchEventListener(buttonEvent)
end

function MainLayer.open( visible )
	MainLayer.getInstance():setVisible( visible );
end

function MainLayer.close()
	MainLayer.getInstance():setVisible( false );
end

function MainLayer.getInstance()
	local dlg = UIManager.getBottomDlg( "MainLayer" )
	if dlg == nil then
		dlg = MainLayer.create()
	end
	return dlg;
end

-- 创建
function MainLayer.create()
    local object = MainLayer.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
