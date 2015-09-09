-- 塔楼
local BaseBuilding = import("building/BaseBuilding")
local Human_Tower = class("Human_Tower", BaseBuilding)

-- 构造函数
function Human_Tower:ctor()
	Human_Tower.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Tower;
end

-- 初始化
function Human_Tower:init()
	Human_Tower.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Tower:setAbilityInfo( abilitys )

end

-- 创建
function Human_Tower.create()
    local object = Human_Tower.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Tower