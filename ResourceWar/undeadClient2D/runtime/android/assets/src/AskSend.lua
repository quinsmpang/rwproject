-- ͨ�ö���Ϣ����
ASKINFO_NORMAL = 0;
ASKINFO_BUILDING = 1;	-- ��������
ASKINFO_WORLDMAP = 2;	-- �����ͼ����
ASKINFO_AREAENTER = 3;	-- �������
ASKINFO_CHATCATCH = 4;    --��ȡ���컺��
ASKINFO_CHAT = 5; --�������
ASKINFO_ACTOR =6;--����ҵĲ���

function system_askinfo( msgid, msg, ... )
	-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
	local arg={...}
	local sendValue = {};
	sendValue.m_msgid = msgid;
	sendValue.m_msg = msg;
	sendValue.m_msg_length = string.len( sendValue.m_msg );
	sendValue.m_valuenum = #arg;
	sendValue.m_value={};
	for i,v in ipairs(arg) do
		sendValue.m_value[i] = v;
   	end	
	netsend_askinfo_C( sendValue );
end
