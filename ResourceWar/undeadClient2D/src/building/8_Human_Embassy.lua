-- ��ʹ��
local BaseBuilding = import("building/BaseBuilding")
local Human_Embassy = class("Human_Embassy", BaseBuilding)

-- ���캯��
function Human_Embassy:ctor()
	Human_Embassy.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Embassy;
end

-- ��ʼ��
function Human_Embassy:init()
	Human_Embassy.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Embassy:setAbilityInfo( abilitys )

end

-- ����
function Human_Embassy.create()
    local object = Human_Embassy.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Embassy