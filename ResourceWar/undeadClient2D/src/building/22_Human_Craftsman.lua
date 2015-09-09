-- 工匠坊
local BaseBuilding = import("building/BaseBuilding")
local Human_Craftsman = class("Human_Craftsman", BaseBuilding)

-- 构造函数
function Human_Craftsman:ctor()
	Human_Craftsman.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Craftsman;
end

-- 初始化
function Human_Craftsman:init()
	Human_Craftsman.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Craftsman:setAbilityInfo( abilitys )

end

-- 创建
function Human_Craftsman.create()
    local object = Human_Craftsman.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Craftsman