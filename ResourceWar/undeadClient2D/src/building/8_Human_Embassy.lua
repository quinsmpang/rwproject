-- 大使馆
local BaseBuilding = import("building/BaseBuilding")
local Human_Embassy = class("Human_Embassy", BaseBuilding)

-- 构造函数
function Human_Embassy:ctor()
	Human_Embassy.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Embassy;
end

-- 初始化
function Human_Embassy:init()
	Human_Embassy.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Embassy:setAbilityInfo( abilitys )

end

-- 创建
function Human_Embassy.create()
    local object = Human_Embassy.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Embassy