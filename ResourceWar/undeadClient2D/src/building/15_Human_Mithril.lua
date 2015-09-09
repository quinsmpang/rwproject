-- 秘银穴
local BaseBuilding = import("building/BaseBuilding")
local Human_Mithril = class("Human_Mithril", BaseBuilding)

-- 构造函数
function Human_Mithril:ctor()
	Human_Mithril.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Mithril;
end

-- 初始化
function Human_Mithril:init()
	Human_Mithril.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Mithril:setAbilityInfo( abilitys )

end

-- 创建
function Human_Mithril.create()
    local object = Human_Mithril.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Mithril