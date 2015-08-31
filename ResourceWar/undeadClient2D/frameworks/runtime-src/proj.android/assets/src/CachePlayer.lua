--  角色缓存信息
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	
	-- 玩家基本信息
	self.m_viplevel 		=	0;  -- VIP等级
	self.m_token			=	0;	-- 钻石
	
	-- 玩家属性信息
	self.m_actorid			=	0;	-- 角色编号
	self.m_name				=	"";	-- 角色名称
	self.m_aclass			=	0;	-- 角色种类
	self.m_shape			=	0;	-- 形象
	self.m_level			=	0;	-- 级别
	self.m_experience		=	0;	-- 经验
	self.m_leadership		=	0;	-- 领导力

    --屏蔽列表
    self.m_maskactors      =   {};--屏蔽的用户列表
end

-- 属性变化
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

-- 钻石改变
-- EventProtocol.addEventListener( "ChangeToken", function( recvValue ) end )
function Player:ChangeToken( nValue, nPath )
	local oldValue = self.m_token;
	self.m_token = nValue;
	EventProtocol.dispatchEvent( "ChangeToken", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 经验改变
-- EventProtocol.addEventListener( "ChangeExp", function( recvValue ) end )
function Player:ChangeExp( nValue, nMaxValue, nPath )
	local oldValue = self.m_experience;
	self.m_experience = nValue;
	EventProtocol.dispatchEvent( "ChangeExp", { value=nValue, maxvalue=nMaxValue, change=nValue-oldValue, path=nPath } );
end

-- 设置用户的屏蔽列表
function Player:SetMaskActors(recvValue)
    self.m_maskactors={};
    for tmpi=1,recvValue.m_count,1 do
        self.m_maskactors[#self.m_maskactors+1] = recvValue.m_chat_mask_list[tmpi];
    end;
end
-- 增加屏蔽用户
function Player:AddMaskActor(actorID)
    self.m_maskactors[#self.m_maskactors+1] = actorID;
end
-- 删除屏蔽用户
function Player:DelMaskActor(actorID)
    for tmpi=1,#self.m_maskactors,1 do
        if self.m_maskactors[tmpi] == actorID then
            self.m_maskactors[tmpi] = nil;
            break;
        end;
    end;
end
--判断一个用户是否是被屏蔽的用户
function Player:IsMaskActor(actorID)
    for tmpi=1,#self.m_maskactors,1 do
        if self.m_maskactors[tmpi]==actorID then
            return true;
        end;
    end;
    return false;
end

-- 全局
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
