-- 法师营地
local BaseBuilding = import("building/BaseBuilding")
local Human_Magician = class("Human_Magician", BaseBuilding)

-- 构造函数
function Human_Magician:ctor()
	Human_Magician.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Magician;
end

-- 初始化
function Human_Magician:init()
	Human_Magician.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Magician:setAbilityInfo( abilitys )

end

-- 创建
function Human_Magician.create()
    local object = Human_Magician.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Magician