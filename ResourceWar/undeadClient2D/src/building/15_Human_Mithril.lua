-- ����Ѩ
local BaseBuilding = import("building/BaseBuilding")
local Human_Mithril = class("Human_Mithril", BaseBuilding)

-- ���캯��
function Human_Mithril:ctor()
	Human_Mithril.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Mithril;
end

-- ��ʼ��
function Human_Mithril:init()
	Human_Mithril.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Mithril:setAbilityInfo( abilitys )

end

-- ����
function Human_Mithril.create()
    local object = Human_Mithril.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Mithril