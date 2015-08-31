--[[
 服务器列表
 --]]
ServerListLayer = class("ServerListLayer",function()
    return cc.Node:create()
end)

-- 构造函数
function ServerListLayer:ctor()
end

-- 初始化
function ServerListLayer:init()
	self:setName("ServerListLayer")
	
	-- 创建按钮和上次登录的服务器名称
	self:createEnterGameNode()	   
    return true;
end

-- 创建列表视图节点
function ServerListLayer:createEnterGameNode()
	local node = self:getChildByName( "EnterGameNode" );
	if node then
		node:setVisible( true )
		self:setSelectServer()
		return;
	end
    node = cc.Node:create();
    node:setName("EnterGameNode")
    self:addChild(node)
	
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "lsatserver" then
                -- 点击获取服务器列表
				GameManager.GetServerList()
				
            elseif name == "entergame" then
                -- 点击进入游戏
				Data.curServerinfo = self._curServerInfo;
				GameManager.tcpManager:ConnectServer( self._curServerInfo["h"], self._curServerInfo["p"] );
				
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- 上次登录的服务器
	self._lastServer = ccui.Button:create()
	self._lastServer:setName("lsatserver")
	self._lastServer:setTitleText("")
	self._lastServer:setTitleFontSize(24);
	self._lastServer:setScale9Enabled(true)
	self._lastServer:loadTextures("ui_jianbian1.png", "ui_jianbian1.png", "")
	self._lastServer:setPressedActionEnabled(true)
	self._lastServer:setContentSize(cc.size(320, 42))
	self._lastServer:setPosition( cc.p( VisibleRect:center().x, VisibleRect:bottom().y+200 ) )
	self._lastServer:addTouchEventListener(buttonEvent)
	node:addChild(self._lastServer)
	self:setSelectServer();
	
	-- 进入游戏按钮
	self._enterGame = ccui.Button:create()
	self._enterGame:setName("entergame")
	self._enterGame:setTitleText("进入游戏")
	self._enterGame:setTitleFontSize(32);
	self._enterGame:setScale9Enabled(true)
	self._enterGame:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	self._enterGame:setContentSize(cc.size(220, 60))
	self._enterGame:setPosition( cc.p( VisibleRect:center().x, VisibleRect:bottom().y+100 ) )
	self._enterGame:addTouchEventListener(buttonEvent)
	node:addChild(self._enterGame)
	
end

-- 创建列表视图节点
function ServerListLayer:createListViewNode()
	local node = self:getChildByName( "ListViewNode" );
	if node then
		node:setVisible( true )
		self:setServerListInfo()
		return;
	end
    node = cc.Node:create();
    node:setName("ListViewNode")
    self:addChild(node)
    
    -- 列表事件
    local function serverListViewEvent(sender, eventType)
        if eventType == ccui.ListViewEventType.ONSELECTEDITEM_END then
            local selectedIndex = sender:getCurSelectedIndex()+1;
			cc.UserDefault:getInstance():setIntegerForKey("LAST_SERVERID", Data.serverinfo["list"][selectedIndex]["id"])
			self:getChildByName( "ListViewNode" ):setVisible( false )
			self:createEnterGameNode()
        end
    end
    
    -- 列表视图
    self._listView = ccui.ListView:create()
    self._listView:setDirection( ccui.ScrollViewDir.vertical )
    self._listView:setBounceEnabled( true )
    self._listView:setBackGroundImage( "ui_back3.png" )
    self._listView:setBackGroundImageScale9Enabled( true )
    self._listView:setContentSize( cc.size(240, 48*5) )
    self._listView:setPosition( cc.p( VisibleRect:center().x-240/2, VisibleRect:bottom().y+300 ) )
    self._listView:addEventListener( serverListViewEvent )
    node:addChild(self._listView)
	-- 添加服务器
    self:setServerListInfo()
end

-- 设置服务器列表
function ServerListLayer:setServerListInfo()
	self._listView:removeAllChildren();
    for key, value in ipairs(Data.serverinfo["list"]) do	
		
        local widget = ccui.Widget:create()
        widget:setContentSize(cc.size(220, 48))
        
        local button = ccui.Button:create()
        button:setTitleText("（"..value["id"].."区）"..value["n"])
        button:setTitleFontSize(22);
        button:setScale9Enabled(true)
        button:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
        button:setContentSize(cc.size(220, 48))
        button:setAnchorPoint(cc.p(0, 0))
		button:setPosition( cc.p( 10, 0 ) )
        widget:addChild(button)
        
        self._listView:pushBackCustomItem(widget)
    end
end

-- 设置选择的服务器
function ServerListLayer:setSelectServer()
    local last_serverid = cc.UserDefault:getInstance():getIntegerForKey("LAST_SERVERID")
	self._curServerInfo = GetServerInfo( last_serverid );
	if self._curServerInfo == nil then
		-- 没有登录过的信息，那么使用缺省推荐的
		self._curServerInfo = GetServerInfo( Data.serverinfo["default"] );
		-- 没有缺省推荐的，那么就打开列表吧
		if self._curServerInfo == nil then
			self:createListViewNode()
			return;
		end
	end
	self._lastServer:setTitleText("（"..self._curServerInfo["id"].."区）"..self._curServerInfo["n"])
	cc.UserDefault:getInstance():setIntegerForKey( "LAST_SERVERID", self._curServerInfo["id"] );
end

-- 创建
function ServerListLayer.create()
    local object = ServerListLayer.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return ServerListLayer
