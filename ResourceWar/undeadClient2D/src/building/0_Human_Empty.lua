-- 空地
local BaseBuilding = import("building/BaseBuilding")
local Human_Empty = class("Human_Empty", BaseBuilding)

-- 构造函数
function Human_Empty:ctor()
	Human_Empty.super.ctor(self)
	
end

-- 初始化
function Human_Empty:init()
	Human_Empty.super.init(self)
	self._aclass = 0;
	self._kind = 0;
end

-- 命中（子类单独覆盖实现）
function Human_Empty:hit()
	BuildingCreateDlg.open( self:getTag() );
end

-- 设置属性信息（子类单独覆盖实现）
function Human_Empty:setAbilityInfo( abilitys )

end

-- 创建
function Human_Empty.create()
    local object = Human_Empty.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Empty