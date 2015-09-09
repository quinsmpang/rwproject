-- 许愿池
local BaseBuilding = import("building/BaseBuilding")
local Human_WishingPool = class("Human_WishingPool", BaseBuilding)

-- 构造函数
function Human_WishingPool:ctor()
	Human_WishingPool.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_WishingPool;
end

-- 初始化
function Human_WishingPool:init()
	Human_WishingPool.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_WishingPool:setAbilityInfo( abilitys )

end

-- 创建
function Human_WishingPool.create()
    local object = Human_WishingPool.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_WishingPool