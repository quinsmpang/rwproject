-- 城墙
local BaseBuilding = import("building/BaseBuilding")
local Human_Wall = class("Human_Wall", BaseBuilding)

-- 构造函数
function Human_Wall:ctor()
	Human_Wall.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Wall;
end

-- 初始化
function Human_Wall:init()
	Human_Wall.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Wall:setAbilityInfo( abilitys )

end

-- 创建
function Human_Wall.create()
    local object = Human_Wall.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Wall
