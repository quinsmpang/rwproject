-- 酒馆
local BaseBuilding = import("building/BaseBuilding")
local Human_Drunkery = class("Human_Drunkery", BaseBuilding)

-- 构造函数
function Human_Drunkery:ctor()
	Human_Drunkery.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Drunkery;
end

-- 初始化
function Human_Drunkery:init()
	Human_Drunkery.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Drunkery:setAbilityInfo( abilitys )

end

-- 创建
function Human_Drunkery.create()
    local object = Human_Drunkery.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Drunkery