-- �ƹ�
local BaseBuilding = import("building/BaseBuilding")
local Human_Drunkery = class("Human_Drunkery", BaseBuilding)

-- ���캯��
function Human_Drunkery:ctor()
	Human_Drunkery.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Drunkery;
end

-- ��ʼ��
function Human_Drunkery:init()
	Human_Drunkery.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Drunkery:setAbilityInfo( abilitys )

end

-- ����
function Human_Drunkery.create()
    local object = Human_Drunkery.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Drunkery