-- ���
local BaseBuilding = import("building/BaseBuilding")
local Human_Stable = class("Human_Stable", BaseBuilding)

-- ���캯��
function Human_Stable:ctor()
	Human_Stable.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Stable;
end

-- ��ʼ��
function Human_Stable:init()
	Human_Stable.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Stable:setAbilityInfo( abilitys )

end

-- ����
function Human_Stable.create()
    local object = Human_Stable.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Stable