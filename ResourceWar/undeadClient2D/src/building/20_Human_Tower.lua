-- ��¥
local BaseBuilding = import("building/BaseBuilding")
local Human_Tower = class("Human_Tower", BaseBuilding)

-- ���캯��
function Human_Tower:ctor()
	Human_Tower.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Tower;
end

-- ��ʼ��
function Human_Tower:init()
	Human_Tower.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Tower:setAbilityInfo( abilitys )

end

-- ����
function Human_Tower.create()
    local object = Human_Tower.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Tower