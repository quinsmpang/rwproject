-- 靶场
local BaseBuilding = import("building/BaseBuilding")
local Human_ShootingRange = class("Human_ShootingRange", BaseBuilding)

-- 构造函数
function Human_ShootingRange:ctor()
	Human_ShootingRange.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_ShootingRange;
end

-- 初始化
function Human_ShootingRange:init()
	Human_ShootingRange.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_ShootingRange:setAbilityInfo( abilitys )

end

-- 创建
function Human_ShootingRange.create()
    local object = Human_ShootingRange.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_ShootingRange