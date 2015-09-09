-- 校场
local BaseBuilding = import("building/BaseBuilding")
local Human_DrillGround = class("Human_DrillGround", BaseBuilding)

-- 构造函数
function Human_DrillGround:ctor()
	Human_DrillGround.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_DrillGround;
end

-- 初始化
function Human_DrillGround:init()
	Human_DrillGround.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_DrillGround:setAbilityInfo( abilitys )

end

-- 创建
function Human_DrillGround.create()
    local object = Human_DrillGround.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_DrillGround