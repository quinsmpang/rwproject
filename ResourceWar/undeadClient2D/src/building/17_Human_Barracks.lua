-- ��Ӫ
local BaseBuilding = import("building/BaseBuilding")
local Human_Barracks = class("Human_Barracks", BaseBuilding)

-- ���캯��
function Human_Barracks:ctor()
	Human_Barracks.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Barracks;
end

-- ��ʼ��
function Human_Barracks:init()
	Human_Barracks.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Barracks:setAbilityInfo( abilitys )

end

-- ����
function Human_Barracks.create()
    local object = Human_Barracks.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Barracks