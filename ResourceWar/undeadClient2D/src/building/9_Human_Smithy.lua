-- ������
local BaseBuilding = import("building/BaseBuilding")
local Human_Smithy = class("Human_Smithy", BaseBuilding)

-- ���캯��
function Human_Smithy:ctor()
	Human_Smithy.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Smithy;
end

-- ��ʼ��
function Human_Smithy:init()
	Human_Smithy.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Smithy:setAbilityInfo( abilitys )

end

-- ����
function Human_Smithy.create()
    local object = Human_Smithy.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Smithy