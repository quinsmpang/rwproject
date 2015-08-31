-- 建筑基础信息
TownCenter = class("TownCenter",function()
    return cc.Node:create()
end)

-- 构造函数
function TownCenter:ctor()

end

-- 初始化
function TownCenter:init()
	-- 建筑图片
	self._sprite = cc.Sprite:create( "building_human_Ground.png" )
    self._sprite:setPosition( cc.p(0, 0) )
    self:addChild( self._sprite )
    return true;
end

-- 创建
function TownCenter.create()
    local object = TownCenter.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return TownCenter
