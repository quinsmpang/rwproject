-- �г�
local BaseBuilding = import("building/BaseBuilding")
local Human_ShootingRange = class("Human_ShootingRange", BaseBuilding)

-- ���캯��
function Human_ShootingRange:ctor()
	Human_ShootingRange.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_ShootingRange;
end

-- ��ʼ��
function Human_ShootingRange:init()
	Human_ShootingRange.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_ShootingRange:setAbilityInfo( abilitys )

end

-- ����
function Human_ShootingRange.create()
    local object = Human_ShootingRange.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_ShootingRange