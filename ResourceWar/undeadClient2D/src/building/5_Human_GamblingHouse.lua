-- �ĳ�
local BaseBuilding = import("building/BaseBuilding")
local Human_GamblingHouse = class("Human_GamblingHouse", BaseBuilding)

-- ���캯��
function Human_GamblingHouse:ctor()
	Human_GamblingHouse.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_GamblingHouse;
end

-- ��ʼ��
function Human_GamblingHouse:init()
	Human_GamblingHouse.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_GamblingHouse:setAbilityInfo( abilitys )

end

-- ����
function Human_GamblingHouse.create()
    local object = Human_GamblingHouse.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_GamblingHouse