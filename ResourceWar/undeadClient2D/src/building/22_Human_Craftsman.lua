-- ������
local BaseBuilding = import("building/BaseBuilding")
local Human_Craftsman = class("Human_Craftsman", BaseBuilding)

-- ���캯��
function Human_Craftsman:ctor()
	Human_Craftsman.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Craftsman;
end

-- ��ʼ��
function Human_Craftsman:init()
	Human_Craftsman.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Craftsman:setAbilityInfo( abilitys )

end

-- ����
function Human_Craftsman.create()
    local object = Human_Craftsman.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Craftsman