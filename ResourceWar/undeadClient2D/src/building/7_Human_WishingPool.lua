-- ��Ը��
local BaseBuilding = import("building/BaseBuilding")
local Human_WishingPool = class("Human_WishingPool", BaseBuilding)

-- ���캯��
function Human_WishingPool:ctor()
	Human_WishingPool.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_WishingPool;
end

-- ��ʼ��
function Human_WishingPool:init()
	Human_WishingPool.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_WishingPool:setAbilityInfo( abilitys )

end

-- ����
function Human_WishingPool.create()
    local object = Human_WishingPool.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_WishingPool