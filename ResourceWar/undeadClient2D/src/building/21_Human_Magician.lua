-- ��ʦӪ��
local BaseBuilding = import("building/BaseBuilding")
local Human_Magician = class("Human_Magician", BaseBuilding)

-- ���캯��
function Human_Magician:ctor()
	Human_Magician.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Magician;
end

-- ��ʼ��
function Human_Magician:init()
	Human_Magician.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Magician:setAbilityInfo( abilitys )

end

-- ����
function Human_Magician.create()
    local object = Human_Magician.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Magician