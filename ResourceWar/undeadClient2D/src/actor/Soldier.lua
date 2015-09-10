-- �յ�
local BaseActor = import("actor/BaseActor")
local Soldier = class("Soldier", BaseActor)

-- ���캯��
function Soldier:ctor()
	Soldier.super.ctor(self)
	
	-- ��Դ����
	self._info = { 
	["attack"]={ ["plist"]="Human_Warrior.plist", ["file"]="Human_Warrior.pvr.ccz", ["fameName"]="Char_H_Warrior_Atk", ["frameCount"]=7, ["delay"]=0.1 }
	}
	-- ���涯��ʹ��
	self._actorType="Human_Warrior";
end

-- ��ʼ��
function Soldier:init()
	Soldier.super.init(self)
	
	return true;
end

-- ����
function Soldier.create()
    local object = Soldier.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Soldier