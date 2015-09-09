-- ����
local BaseBuilding = import("building/BaseBuilding")
local Human_IronFactory = class("Human_IronFactory", BaseBuilding)

-- ���캯��
function Human_IronFactory:ctor()
	Human_IronFactory.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_IronFactory;
end

-- ��ʼ��
function Human_IronFactory:init()
	Human_IronFactory.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_IronFactory:setAbilityInfo( abilitys )

end

-- ����
function Human_IronFactory.create()
    local object = Human_IronFactory.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_IronFactory