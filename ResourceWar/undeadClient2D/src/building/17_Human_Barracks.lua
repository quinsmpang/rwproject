-- 兵营
local BaseBuilding = import("building/BaseBuilding")
local Human_Barracks = class("Human_Barracks", BaseBuilding)

-- 构造函数
function Human_Barracks:ctor()
	Human_Barracks.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Barracks;
end

-- 初始化
function Human_Barracks:init()
	Human_Barracks.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Barracks:setAbilityInfo( abilitys )

end

-- 创建
function Human_Barracks.create()
    local object = Human_Barracks.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Barracks