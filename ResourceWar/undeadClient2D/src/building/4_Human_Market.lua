-- �г�
local BaseBuilding = import("building/BaseBuilding")
local Human_Market = class("Human_Market", BaseBuilding)

-- ���캯��
function Human_Market:ctor()
	Human_Market.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Market;
end

-- ��ʼ��
function Human_Market:init()
	Human_Market.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Market:setAbilityInfo( abilitys )

end

-- ����
function Human_Market.create()
    local object = Human_Market.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Market