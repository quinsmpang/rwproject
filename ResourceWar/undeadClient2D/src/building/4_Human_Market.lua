-- 市场
local BaseBuilding = import("building/BaseBuilding")
local Human_Market = class("Human_Market", BaseBuilding)

-- 构造函数
function Human_Market:ctor()
	Human_Market.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Market;
end

-- 初始化
function Human_Market:init()
	Human_Market.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Market:setAbilityInfo( abilitys )

end

-- 创建
function Human_Market.create()
    local object = Human_Market.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Market