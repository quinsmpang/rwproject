-- �ֿ�
local BaseBuilding = import("building/BaseBuilding")
local Human_StoreHouse = class("Human_StoreHouse", BaseBuilding)

-- ���캯��
function Human_StoreHouse:ctor()
	Human_StoreHouse.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_StoreHouse;
end

-- ��ʼ��
function Human_StoreHouse:init()
	Human_StoreHouse.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_StoreHouse:setAbilityInfo( abilitys )

end

-- ����
function Human_StoreHouse.create()
    local object = Human_StoreHouse.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_StoreHouse