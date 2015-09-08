-- 聊天框
local BaseDlg = import("ui/BaseDlg")
ChatDlg = class("ChatDlg", BaseDlg)

-- 构造函数
function ChatDlg:ctor()
	ChatDlg.super.ctor(self)
end

-- 初始化
function ChatDlg:init()
	UIManager.addCenter( self );
	self:setName("ChatDlg")
	self:setPosition(VisibleRect:center())
	
	-- 按钮事件
	local function buttonEvent(sender,eventType)
        local name = sender:getName();
        if eventType == ccui.TouchEventType.began then
        elseif eventType == ccui.TouchEventType.moved then
        elseif eventType == ccui.TouchEventType.ended then
            if name == "close" then
				ChatDlg.close()
			
			elseif name == "sendmsg" then -- 发送聊天信息
				local sendValue = { };
				sendValue.m_type = 0;
				sendValue.m_msg = self._edit:getText();
				sendValue.m_msg_length = string.len( sendValue.m_msg );
				netsend_talk_C(sendValue);
				self._edit:setText("")
			
			elseif name == "xx" then
            end
        elseif eventType == ccui.TouchEventType.canceled then
        end
    end    
	
	-- 背景图片
    local backImageNode = ccui.ImageView:create()
    backImageNode:setScale9Enabled( true )
    backImageNode:loadTexture( "UI_zhujiemian_tixingkuang.png" )
    backImageNode:setContentSize( SCREEN_SIZE )
    self:addChild( backImageNode )
	
	-- 关闭按钮
	local button = ccui.Button:create()
	button:setName("close")
	button:setPressedActionEnabled(true)
	button:loadTextures("UI_jianzao_fanhui.png", "UI_jianzao_fanhui.png", "")
	button:setAnchorPoint(cc.p(0, 1))
	button:setPosition( VisibleRect:leftTop() )
	button:addTouchEventListener(buttonEvent)
	backImageNode:addChild(button)
	
	-- 发送按钮
	self._sendMsg = ccui.Button:create()
	self._sendMsg:setName("sendmsg")
	self._sendMsg:setTitleText("发送")
	self._sendMsg:setTitleFontSize(24);
	self._sendMsg:setScale9Enabled(true)
	self._sendMsg:loadTextures("ui_button_normal_1.png", "ui_button_click_1.png", "")
	self._sendMsg:setContentSize(cc.size(80, 50))
	self._sendMsg:setPosition( cc.p(VisibleRect:leftBottom().x+494, VisibleRect:bottom().y+40) )
	self._sendMsg:addTouchEventListener(buttonEvent)
	backImageNode:addChild(self._sendMsg)
    
	-- 滚动事件方法回调
	local function scrollViewEvent(sender, eventType)
		-- 滚动到底部
		if eventType == ccui.ScrollviewEventType.scrollToBottom then
			
		-- 滚动到顶部
		elseif eventType == ccui.ScrollviewEventType.scrollToTop then

		elseif eventType == ccui.ScrollviewEventType.bounceTop then	
		end
	end
	
	-- ListView点击事件回调
	local function listViewEvent(sender, eventType)
		-- 事件类型为点击结束
		if eventType == ccui.ListViewEventType.ONSELECTEDITEM_END then
			
		end
	end
	
    -- 列表视图
    self._listView = ccui.ListView:create()
    self._listView:setDirection( ccui.ScrollViewDir.vertical )
    self._listView:setBounceEnabled( true )
    self._listView:setContentSize( cc.size(600, SCREEN_SIZE.height-300) )
    self._listView:setPosition( cc.p( VisibleRect:leftBottom().x, VisibleRect:leftBottom().y+100 ) )
	self._listView:setItemsMargin(20)
	self._listView:addScrollViewEventListener( scrollViewEvent )
	self._listView:addEventListener( listViewEvent )
    backImageNode:addChild( self._listView )
	
	-- 编辑框事件
	local function editBoxTextEventHandle( eventName, sender )
		if eventName == "began" then
		elseif eventName == "ended" then	
		elseif eventName == "return" then
		elseif eventName == "changed" then
		end
	end
	-- 编辑框
	self._edit = cc.EditBox:create(cc.size(350, 40), cc.Scale9Sprite:create("UI_liaotian_labadi.png"))
	self._edit:setFontSize(20)
	self._edit:setMaxLength(256)
	self._edit:setAnchorPoint(cc.p(0, 0))
    self._edit:setPosition(cc.p(VisibleRect:leftBottom().x+100, VisibleRect:bottom().y+20))
    self._edit:setPlaceHolder("点击此处输入内容")
    self._edit:setInputMode(cc.EDITBOX_INPUT_MODE_ANY)
    self._edit:registerScriptEditBoxHandler(editBoxTextEventHandle)
    backImageNode:addChild(self._edit)
	
	 -- 开始监听聊天信息
    EventProtocol.addEventListener("proc_talk_C", function(recvValue)
        if recvValue.m_msg == "" then
			return;
		end
		-- 最多允许128条
		local items_count = table.getn(self._listView:getItems())
		if items_count > 128 then
			local item = self._listView:getItem( 0 )
			local index = self._listView:getIndex( item )
			self._listView:removeItem( index )
		end
		-- 添加
		self:addMsg( recvValue.m_actorid, 100, 6, "", recvValue.m_name, recvValue.m_msg, recvValue.m_sendtime )
    end )
    return true;
end

-- 添加消息节点
function ChatDlg:addMsg( actorid, shape, vip, clubname, actorname, msg, time )
	local height = 60;
	local fontSize = 20
	local layout = ccui.Layout:create()
	--layout:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
	--layout:setBackGroundColor(cc.c3b(142,143,137))
	
	-- 内容
	local msgLabel = cc.Label:createWithSystemFont("", "", fontSize)
	msgLabel:setAnchorPoint(cc.p( 0, 1 ))
	msgLabel:setLineBreakWithoutSpace(true)
	msgLabel:setWidth( 320 )
	msgLabel:setString( msg )
	msgLabel:setHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
	msgLabel:setVerticalAlignment( cc.VERTICAL_TEXT_ALIGNMENT_TOP )
	msgLabel:setColor( cc.c4b(0, 0, 0, 255))
	layout:addChild( msgLabel, 10 )
	height = height + msgLabel:getContentSize().height
	
	-- 根据文字内容确定布局大小
	layout:setContentSize( cc.size( msgLabel:getContentSize().width+100, height ) )
	
	-- 设置内容位置
	msgLabel:setPosition( cc.p( 100, layout:getContentSize().height-fontSize*2 ) )
	
	-- 内容底框
	local msgBackButton = ccui.Button:create()
	msgBackButton:loadTextures("UI_talk_gonggao.png", "UI_talk_gonggao.png", "")
	msgBackButton:setScale9Enabled(true)
	msgBackButton:setAnchorPoint(cc.p( 0, 1 ))
	msgBackButton:setPosition( cc.p( 96, layout:getContentSize().height ) )
	msgBackButton:setContentSize( cc.size( layout:getContentSize().width-90, layout:getContentSize().height) )
	layout:addChild( msgBackButton, 1 )
	
	-- 头像
	local shapeButton = ccui.Button:create()
	shapeButton:loadTextures("UI_liaotian_teshu.png", "", "")
	shapeButton:setPressedActionEnabled(false)
	shapeButton:setAnchorPoint( cc.p(0, 1) )
	shapeButton:setPosition( cc.p( 20, layout:getContentSize().height ) )
	layout:addChild( shapeButton, 10 )
	
	
	-- 名称包含（vip+联盟+名称）
	local nameLabel = cc.Label:createWithSystemFont("", "", 18)
	nameLabel:setString( actorname )
	nameLabel:setAnchorPoint(cc.p( 0, 1 ))
	nameLabel:setPosition( cc.p( 100, layout:getContentSize().height-6 ) )
	nameLabel:setHorizontalAlignment( cc.TEXT_ALIGNMENT_LEFT )
	nameLabel:setVerticalAlignment( cc.VERTICAL_TEXT_ALIGNMENT_TOP )
	nameLabel:setColor( cc.c4b(0, 0, 0, 255))
	layout:addChild( nameLabel, 10 )
	
	-- 时间
	local timeLabel = cc.Label:createWithSystemFont("", "", 14)
	timeLabel:setString( os.date("%H:%M:%S",time) )
	timeLabel:setAnchorPoint(cc.p( 1, 1 ))
	timeLabel:setPosition( cc.p( layout:getContentSize().width, layout:getContentSize().height-8 ) )
	timeLabel:setHorizontalAlignment( cc.TEXT_ALIGNMENT_RIGHT )
	timeLabel:setVerticalAlignment( cc.VERTICAL_TEXT_ALIGNMENT_TOP )
	timeLabel:setColor( cc.c4b(0, 0, 0, 255))
	layout:addChild( timeLabel, 10 )
	
	-- 添加到列表
	self._listView:pushBackCustomItem( layout )
	
	-- 必须延迟调用自动移动
	performWithDelay( self, function(dt)  
		self._listView:scrollToBottom( 0.1, false )
	end, 1/30 );
end

-- 创建
function ChatDlg.create()
    local object = ChatDlg.new();
    if object:init() == false then
        return nil;
    end
    return object;
end

-- 唯一实例
function ChatDlg.getInstance()
	local dlg = UIManager.getCenterDlg( "ChatDlg" )
	if dlg == nil then
		dlg = ChatDlg.create()
	end
	return dlg;
end

-- 打开
function ChatDlg.open()
	ChatDlg.getInstance():_open()
end

-- 关闭
function ChatDlg.close()
	ChatDlg.getInstance():_close()
end
