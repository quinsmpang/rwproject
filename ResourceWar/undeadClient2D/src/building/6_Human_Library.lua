-- ͼ���
local BaseBuilding = import("building/BaseBuilding")
local Human_Library = class("Human_Library", BaseBuilding)

-- ���캯��
function Human_Library:ctor()
	Human_Library.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Library;
end

-- ��ʼ��
function Human_Library:init()
	Human_Library.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Library:setAbilityInfo( abilitys )

end

-- ����
function Human_Library.create()
    local object = Human_Library.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Library