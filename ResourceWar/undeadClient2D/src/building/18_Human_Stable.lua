-- 马厩
local BaseBuilding = import("building/BaseBuilding")
local Human_Stable = class("Human_Stable", BaseBuilding)

-- 构造函数
function Human_Stable:ctor()
	Human_Stable.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Stable;
end

-- 初始化
function Human_Stable:init()
	Human_Stable.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Stable:setAbilityInfo( abilitys )

end

-- 创建
function Human_Stable.create()
    local object = Human_Stable.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Stable