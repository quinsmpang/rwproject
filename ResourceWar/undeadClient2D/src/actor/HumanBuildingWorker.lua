-- ����������Ϣ
TownCenter = class("TownCenter",function()
    return cc.Node:create()
end)

-- ���캯��
function TownCenter:ctor()

end

-- ��ʼ��
function TownCenter:init()
	-- ����ͼƬ
	self._sprite = cc.Sprite:create( "building_human_Ground.png" )
    self._sprite:setPosition( cc.p(0, 0) )
    self:addChild( self._sprite )
    return true;
end

-- ����
function TownCenter.create()
    local object = TownCenter.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return TownCenter
