-- ���
local BaseBuilding = import("building/BaseBuilding")
local Human_House = class("Human_House", BaseBuilding)

-- ���캯��
function Human_House:ctor()
	Human_House.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_House;
end

-- ��ʼ��
function Human_House:init()
	Human_House.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_House:setAbilityInfo( abilitys )

end

-- ����
function Human_House.create()
    local object = Human_House.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_House