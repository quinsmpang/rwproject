-- 王城
local BaseBuilding = import("building/BaseBuilding")
local Human_TownCenter = class("Human_TownCenter", BaseBuilding)

-- 构造函数
function Human_TownCenter:ctor()
	Human_TownCenter.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_TownCenter;
end

-- 初始化
function Human_TownCenter:init()
	Human_TownCenter.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_TownCenter:setAbilityInfo( abilitys )

end

-- 创建
function Human_TownCenter.create()
    local object = Human_TownCenter.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_TownCenter
