-- 铁匠铺
local BaseBuilding = import("building/BaseBuilding")
local Human_Smithy = class("Human_Smithy", BaseBuilding)

-- 构造函数
function Human_Smithy:ctor()
	Human_Smithy.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Smithy;
end

-- 初始化
function Human_Smithy:init()
	Human_Smithy.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Smithy:setAbilityInfo( abilitys )

end

-- 创建
function Human_Smithy.create()
    local object = Human_Smithy.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Smithy