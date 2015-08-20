-- 游戏流程管理
GameManager = {};
local this = GameManager;

-- 当前游戏状态
GameManager.status = 0;

-- 当前场景
GameManager.currentScence = "";

-- 网络管理器
GameManager.tcpManager = nil;

-- 游戏启动
function GameManager.onInit()
	
	-- UI管理器(转换场景后要重新初始化)
	UIManager.init()
	
	-- 配置数据初始化
	Data.OnFristInit();
	
	-- 启动全局定时器
	GlobalSchedule:start( 1/30, 0.0);
	
	-- 初始化网络管理器
	GameManager.tcpManager = TcpManager:new();
	
	-- 获取服务器列表
	GameManager.GetServerList()
end

-- 获取服务器列表
function GameManager.GetServerList()
	local deviceID = PlatformUtils:GetInstance():GetDeviceID();
	local http = cc.XMLHttpRequest:new()
    http.responseType = cc.XMLHTTPREQUEST_RESPONSE_JSON
    http:open("GET", "http://114.215.190.229/Fruit/server/serverlist_cocos.php?c=2&v1="..deviceID.."&v2=11")
	local function onReadyStateChange()
		if http.readyState == 4 and (http.status >= 200 and http.status < 207) then
			local json = require("json")
			Data.serverinfo = json.decode( http.response, 1 )
			-- 创建服务器列表
			local serverList = cc.Director:getInstance():getRunningScene():getChildByName( "ServerListLayer" );
			if serverList then
				serverList:createListViewNode();
			else
				serverList = ServerListLayer.create();
				cc.Director:getInstance():getRunningScene():addChild( serverList )
			end
		else
			print("http.readyState is:", http.readyState, "http.status is: ",http.status)
		end
	end
	http:registerScriptHandler(onReadyStateChange)
	http:send()
end

-- 连接服务器成功
function GameManager.onConnectServer()
    if GameManager.status == 0 then
		-- 已建立连接
		GameManager.status = 1;
		-- 建立连接后，发送版本信息
		local buffer = ByteBuffer:new();
		buffer:WriteInt(100);
		buffer:WriteInt(200);
		buffer:WriteInt(300);
		buffer:WriteInt(400);
		buffer:WriteInt(Data.platid);	-- 平台
		buffer:WriteInt(Data.language); -- 使用语言
		buffer:WriteInt(700);
		buffer:WriteInt(800);
		SendMessage(buffer);
    end
end

-- 连接服务器失败 
function GameManager.onConnectFail()
	GameManager.status = 0;
	Alert( "连接服务器失败", "重试", function()
		GameManager.tcpManager:ConnectServer( Data.curServerinfo["h"], Data.curServerinfo["p"] )
	end )
end

-- 连接中断，或者被踢掉
function GameManager.onDisconnect()
	GameManager.status = 0;
	Alert( "连接中断", "重试", function() 
		GameManager.tcpManager:ConnectServer( Data.curServerinfo["h"], Data.curServerinfo["p"] )
	end )
end

-- 游戏登陆验证
function GameManager.onLogin()
	-- 登陆
	local sendValue = {};
	sendValue.m_username = ""
	sendValue.m_username_length = string.len( sendValue.m_username );
	sendValue.m_password = ""
	sendValue.m_password_length = string.len( sendValue.m_password );
	sendValue.m_deviceid = PlatformUtils:GetInstance():GetDeviceID();
	sendValue.m_deviceid_length = string.len( sendValue.m_deviceid );
	netsend_login_C( sendValue )
	
	-- 监听登陆返回事件
	EventProtocol.addEventListener( "proc_login_C", function( recvValue ) 
	end )
	
	-- 监听返回列表事件
	EventProtocol.addEventListener( "proc_list_C", function( recvValue ) 
		if recvValue.m_actor_num <= 0 then
		
			-- 创建角色
			local sendValue = {};
			sendValue.m_aclass = 0;
			sendValue.m_name = "我是人类"
			sendValue.m_name_length = string.len( sendValue.m_name );
			netsend_create_C( sendValue );
			print( "创建角色" )
			
		else
			
			-- 进入游戏
			netsend_entergame_C( recvValue.m_listinfo[1].m_actorid );
			print( "进入游戏："..recvValue.m_listinfo[1].m_actorid )
		end
		
	end )
	
	-- 监听创建返回事件
	EventProtocol.addEventListener( "proc_create_C", function( recvValue ) 
		
			if recvValue.m_result == 0 then
			
				-- 创建成功，进入游戏
				netsend_entergame_C( recvValue.m_actorid );
				print( "创建成功进入游戏："..recvValue.m_actorid )
				GameManager.onStart()
			else
				print( "创建角色失败,角色重名" )
			end
			
	end )
end

-- 游戏启动
function GameManager.onStart()
	
	-- 创建游戏主场景
    local scene = require("GameScene")
    local gameScene = scene.create()
    if cc.Director:getInstance():getRunningScene() then
        cc.Director:getInstance():replaceScene(gameScene)
    else
        cc.Director:getInstance():runWithScene(gameScene)
    end
	
	
end

-- 游戏帧逻辑
function GameManager.onLogic()

end

-- 游戏退出
function GameManager.onExit()

end

-- 游戏消息处理
function onMessageProc( cmd, buf, size )
	-- 创建buff流，为了跟C#版本统一
	local buffer = ByteBuffer:new( buf, size );
	
	-- 本地发过来的连接成功,连接失败,断线的消息
	if cmd == -101 then 
		GameManager.onConnectServer()
		return;
	elseif cmd == -102 then
		GameManager.onConnectFail()
		return;
	elseif cmd == -103 then 
		GameManager.onDisconnect()	
		return;
	end
	-- 连接开始接受服务器发过来的版本秘钥等的信息
	if GameManager.status == 1 then
		if cmd == 2015 then
			local err_code = buffer:ReadInt();
			local m_recv_key = buffer:ReadInt();
			local m_send_key = buffer:ReadInt();
			local m_code_len = buffer:ReadInt();
			local m_value1 = buffer:ReadInt();
			local m_value2 = buffer:ReadInt();
			local m_value3 = buffer:ReadInt();
			local m_value4 = buffer:ReadInt();
			-- 已接收加密秘钥等信息
			GameManager.status = 2;
			-- 启动登陆验证
			GameManager.onLogin();
		end
		return;
	end
	
	-- 普通游戏数据包处理
	if in_proc_command_C( cmd, buffer ) == 0 then
	end
end

-- 发送消息
function SendMessage(buffer)
	GameManager.tcpManager:SendMessage(buffer);
end