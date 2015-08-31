-- ��Ϸ���̹���
GameManager = {};
local this = GameManager;

-- ��ǰ��Ϸ״̬
GameManager.status = 0;

-- ��ǰ����
GameManager.currentScence = "";

-- ���������
GameManager.tcpManager = nil;

-- ��Ϸ����
function GameManager.onInit()
	
	-- �������ݳ�ʼ��
	Data.OnInit();
	
	-- ����ȫ�ֶ�ʱ��
	GlobalSchedule:start( 1/30, 0.0);
	
	-- ��ʼ�����������
	GameManager.tcpManager = TcpManager:new();
	
	-- ��ȡ�������б�
	GameManager.GetServerList()
	
    -- ������½����
    --[[local scene = require("SceneMain")
    local sceneMain = scene.create()
    
    if cc.Director:getInstance():getRunningScene() then
        cc.Director:getInstance():replaceScene(sceneMain)
    else
        cc.Director:getInstance():runWithScene(sceneMain)
    end--]]
end

-- ��ȡ�������б�
function GameManager.GetServerList()
	local http = cc.XMLHttpRequest:new()
    http.responseType = cc.XMLHTTPREQUEST_RESPONSE_JSON
    http:open("GET", "http://114.215.190.229/Fruit/server/serverlist.php?c=2&v1=0&v2=11")
	local function onReadyStateChange()
		if http.readyState == 4 and (http.status >= 200 and http.status < 207) then
			local json = require("json")
			local info = json.decode( http.response, 1 )
			local cmd = info["cmd"];
			if cmd == "2" then
				-- ��ȡ�ɹ���������Ϸ������
				GameManager.tcpManager:ConnectServer( info["h"], info["p"] );
			end
		else
			print("http.readyState is:", http.readyState, "http.status is: ",http.status)
		end
	end
	http:registerScriptHandler(onReadyStateChange)
	http:send()
end

-- ���ӷ������ɹ�
function GameManager.onConnectServer()
    if GameManager.status == 0 then
		-- �ѽ�������
		GameManager.status = 1;
		-- �������Ӻ󣬷��Ͱ汾��Ϣ
		local buffer = ByteBuffer:new();
		buffer:WriteInt(100);
		buffer:WriteInt(200);
		buffer:WriteInt(300);
		buffer:WriteInt(400);
		buffer:WriteInt(2);	-- ƽ̨
		buffer:WriteInt(1); -- ʹ������
		buffer:WriteInt(700);
		buffer:WriteInt(800);
		SendMessage(buffer);
    end
end

-- ���ӷ�����ʧ�� 
function GameManager.onConnectFail()
	GameManager.status = 0;
end

-- �����жϣ����߱��ߵ�
function GameManager.onDisconnect()
	GameManager.status = 0;
end

-- ��Ϸ����
function GameManager.onStart()
	
	-- ������½����
    --[[local scene = require("SceneMain")
    local sceneMain = scene.create()
    
    if cc.Director:getInstance():getRunningScene() then
        cc.Director:getInstance():replaceScene(sceneMain)
    else
        cc.Director:getInstance():runWithScene(sceneMain)
    end--]]
end

-- ��Ϸ֡�߼�
function GameManager.onLogic()

end

-- ��Ϸ�˳�
function GameManager.onExit()

end

-- ��Ϸ��Ϣ����
function onMessageProc( cmd, buf, size )
	-- ����buff����Ϊ�˸�C#�汾ͳһ
	local buffer = ByteBuffer:new( buf, size );
	
	-- ���ط����������ӳɹ�,����ʧ��,���ߵ���Ϣ
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
	-- ���ӿ�ʼ���ܷ������������İ汾��Կ�ȵ���Ϣ
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
			-- �ѽ��ռ�����Կ����Ϣ
			GameManager.status = 2;
			-- ����Ϸ�߼���ʼ
			GameManager.onStart();
		end
		return;
	end
	
	-- ��ͨ��Ϸ���ݰ�����
	if in_proc_command_C( cmd, buffer ) == 0 then
	end
end

-- ������Ϣ
function SendMessage(buffer)
	GameManager.tcpManager:SendMessage(buffer);
end