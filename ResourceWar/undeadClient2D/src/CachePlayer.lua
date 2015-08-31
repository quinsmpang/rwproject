--  ��ɫ������Ϣ
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	
	-- ��һ�����Ϣ
	self.m_viplevel 		=	0;  -- VIP�ȼ�
	self.m_token			=	0;	-- ��ʯ
	
	-- ���������Ϣ
	self.m_actorid			=	0;	-- ��ɫ���
	self.m_name				=	"";	-- ��ɫ����
	self.m_aclass			=	0;	-- ��ɫ����
	self.m_shape			=	0;	-- ����
	self.m_level			=	0;	-- ����
	self.m_experience		=	0;	-- ����
	self.m_leadership		=	0;	-- �쵼��

    --�����б�
    self.m_maskactors      =   {};--���ε��û��б�
end

-- ���Ա仯
function Player:Set( recvValue )
	self.m_viplevel			=	recvValue.m_viplevel;
	self.m_actorid 			= 	recvValue.m_actorid;
	self.m_name				=	recvValue.m_name;
	self.m_aclass			=	tonumber(recvValue.m_aclass);
	self.m_shape			=	recvValue.m_shape;
	self.m_level			=	recvValue.m_level;
	self.m_leadership		=	recvValue.m_leadership;
	
	self:ChangeToken( recvValue.m_token, 0 )
	self:ChangeExp( recvValue.m_experience, recvValue.m_experience_max, 0 )
	
end

-- ��ʯ�ı�
-- EventProtocol.addEventListener( "ChangeToken", function( recvValue ) end )
function Player:ChangeToken( nValue, nPath )
	local oldValue = self.m_token;
	self.m_token = nValue;
	EventProtocol.dispatchEvent( "ChangeToken", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- ����ı�
-- EventProtocol.addEventListener( "ChangeExp", function( recvValue ) end )
function Player:ChangeExp( nValue, nMaxValue, nPath )
	local oldValue = self.m_experience;
	self.m_experience = nValue;
	EventProtocol.dispatchEvent( "ChangeExp", { value=nValue, maxvalue=nMaxValue, change=nValue-oldValue, path=nPath } );
end

-- �����û��������б�
function Player:SetMaskActors(recvValue)
    self.m_maskactors={};
    for tmpi=1,recvValue.m_count,1 do
        self.m_maskactors[#self.m_maskactors+1] = recvValue.m_chat_mask_list[tmpi];
    end;
end
-- ���������û�
function Player:AddMaskActor(actorID)
    self.m_maskactors[#self.m_maskactors+1] = actorID;
end
-- ɾ�������û�
function Player:DelMaskActor(actorID)
    for tmpi=1,#self.m_maskactors,1 do
        if self.m_maskactors[tmpi] == actorID then
            self.m_maskactors[tmpi] = nil;
            break;
        end;
    end;
end
--�ж�һ���û��Ƿ��Ǳ����ε��û�
function Player:IsMaskActor(actorID)
    for tmpi=1,#self.m_maskactors,1 do
        if self.m_maskactors[tmpi]==actorID then
            return true;
        end;
    end;
    return false;
end

-- ȫ��
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
