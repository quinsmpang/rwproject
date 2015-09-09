-- 图书馆
local BaseBuilding = import("building/BaseBuilding")
local Human_Library = class("Human_Library", BaseBuilding)

-- 构造函数
function Human_Library:ctor()
	Human_Library.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Library;
end

-- 初始化
function Human_Library:init()
	Human_Library.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Library:setAbilityInfo( abilitys )

end

-- 创建
function Human_Library.create()
    local object = Human_Library.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Library