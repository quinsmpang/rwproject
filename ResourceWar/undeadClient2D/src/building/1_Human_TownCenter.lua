-- ����
local BaseBuilding = import("building/BaseBuilding")
local Human_TownCenter = class("Human_TownCenter", BaseBuilding)

-- ���캯��
function Human_TownCenter:ctor()
	Human_TownCenter.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_TownCenter;
end

-- ��ʼ��
function Human_TownCenter:init()
	Human_TownCenter.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_TownCenter:setAbilityInfo( abilitys )

end

-- ����
function Human_TownCenter.create()
    local object = Human_TownCenter.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_TownCenter
