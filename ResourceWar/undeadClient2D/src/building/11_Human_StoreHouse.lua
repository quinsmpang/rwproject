-- 仓库
local BaseBuilding = import("building/BaseBuilding")
local Human_StoreHouse = class("Human_StoreHouse", BaseBuilding)

-- 构造函数
function Human_StoreHouse:ctor()
	Human_StoreHouse.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_StoreHouse;
end

-- 初始化
function Human_StoreHouse:init()
	Human_StoreHouse.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_StoreHouse:setAbilityInfo( abilitys )

end

-- 创建
function Human_StoreHouse.create()
    local object = Human_StoreHouse.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_StoreHouse