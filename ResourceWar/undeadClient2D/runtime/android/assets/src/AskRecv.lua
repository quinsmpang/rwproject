-- ͨ�ö���Ϣ����
NOTIFY_NORMAL		=	0	-- ȱʡ������ʾ������˷�����
NOTIFY_TEXTTABLE	= 	1	-- ����˷����ֱ���Ϣ
NOTIFY_ITEMNUM		=	2	-- ���������µ�������
NOTIFY_WORLDMAP		=	3	-- �����ͼ����
NOTIFY_MAIL			=	4	-- �ʼ�����
NOTIFY_CHAT			= 	5	-- �������
NOTIFY_ACTOR        =   6   -- ����ҵĲ���

-- ������յ�����Ϣ
function RecvActorNotify( recvValue )
	local msgid = recvValue.m_msgid;
	local value = recvValue.m_value;
	local msg = recvValue.m_msg;

	if msgid == NOTIFY_NORMAL then
		Notify( msg );
	elseif msgid == NOTIFY_TEXTTABLE then
		Notify( GetLocalizeText( tonumber(msg) ) );
	elseif msgid == NOTIFY_ITEMNUM then
	elseif msgid == NOTIFY_WORLDMAP then
	
	-- �ʼ�
	elseif msgid == NOTIFY_MAIL then
		if value[1] == 1 then
			-- δ���ʼ�����
			
		elseif value[1] == 10 then
			-- δ��ս������
			
		end
	
	-- ����ҵĲ���
	elseif msgid == NOTIFY_ACTOR then
		
	end
end