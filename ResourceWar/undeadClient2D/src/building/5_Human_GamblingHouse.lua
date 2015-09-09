-- 赌场
local BaseBuilding = import("building/BaseBuilding")
local Human_GamblingHouse = class("Human_GamblingHouse", BaseBuilding)

-- 构造函数
function Human_GamblingHouse:ctor()
	Human_GamblingHouse.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_GamblingHouse;
end

-- 初始化
function Human_GamblingHouse:init()
	Human_GamblingHouse.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_GamblingHouse:setAbilityInfo( abilitys )

end

-- 创建
function Human_GamblingHouse.create()
    local object = Human_GamblingHouse.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_GamblingHouse