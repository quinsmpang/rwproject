-- ��ǽ
local BaseBuilding = import("building/BaseBuilding")
local Human_Wall = class("Human_Wall", BaseBuilding)

-- ���캯��
function Human_Wall:ctor()
	Human_Wall.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Wall;
end

-- ��ʼ��
function Human_Wall:init()
	Human_Wall.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Wall:setAbilityInfo( abilitys )

end

-- ����
function Human_Wall.create()
    local object = Human_Wall.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Wall
