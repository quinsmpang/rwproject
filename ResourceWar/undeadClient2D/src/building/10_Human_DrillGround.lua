-- У��
local BaseBuilding = import("building/BaseBuilding")
local Human_DrillGround = class("Human_DrillGround", BaseBuilding)

-- ���캯��
function Human_DrillGround:ctor()
	Human_DrillGround.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_DrillGround;
end

-- ��ʼ��
function Human_DrillGround:init()
	Human_DrillGround.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_DrillGround:setAbilityInfo( abilitys )

end

-- ����
function Human_DrillGround.create()
    local object = Human_DrillGround.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_DrillGround