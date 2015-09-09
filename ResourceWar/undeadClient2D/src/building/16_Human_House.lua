-- 民居
local BaseBuilding = import("building/BaseBuilding")
local Human_House = class("Human_House", BaseBuilding)

-- 构造函数
function Human_House:ctor()
	Human_House.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_House;
end

-- 初始化
function Human_House:init()
	Human_House.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_House:setAbilityInfo( abilitys )

end

-- 创建
function Human_House.create()
    local object = Human_House.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_House