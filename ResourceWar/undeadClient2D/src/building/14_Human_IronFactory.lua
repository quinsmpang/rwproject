-- 铁矿厂
local BaseBuilding = import("building/BaseBuilding")
local Human_IronFactory = class("Human_IronFactory", BaseBuilding)

-- 构造函数
function Human_IronFactory:ctor()
	Human_IronFactory.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_IronFactory;
end

-- 初始化
function Human_IronFactory:init()
	Human_IronFactory.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_IronFactory:setAbilityInfo( abilitys )

end

-- 创建
function Human_IronFactory.create()
    local object = Human_IronFactory.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_IronFactory