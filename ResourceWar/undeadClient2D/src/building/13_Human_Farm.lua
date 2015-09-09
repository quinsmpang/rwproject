-- ũ��
local BaseBuilding = import("building/BaseBuilding")
local Human_Farm = class("Human_Farm", BaseBuilding)

-- ���캯��
function Human_Farm:ctor()
	Human_Farm.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_Farm;
end

-- ��ʼ��
function Human_Farm:init()
	Human_Farm.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Farm:setAbilityInfo( abilitys )	
	if abilitys[1] > 0 and self._resImage == nil then 
		-- ��Դʰȡ��ʾ
		self._resImage = cc.Sprite:create( "UI_zhujiemian_liangshi.png" );
		self._resImage:setPosition( self:getParent():getPosition() )
		self._resImage:runAction( cc.RepeatForever:create(action_heartbeat()) );
		-- ��ӵ�������Դʰȡ��
		GameManager.gameScence._city:addResPickupObject( self._resImage, function()
			-- �Ƴ�
			self._resImage:removeFromParent();
			self._resImage = nil;
			-- ������ȡ
			system_askinfo( ASKINFO_BUILDING, "", 4, self:getTag() );
			-- ��������
			local particleSystem = cc.ParticleSystemQuad:create("res_pickup.plist")
			particleSystem:setTexture( cc.Director:getInstance():getTextureCache():addImage( "UI_zhujiemian_liangshi.png" ) )
			particleSystem:setPositionType( cc.POSITION_TYPE_GROUPED )
			particleSystem:setAutoRemoveOnFinish(true)
			self:addChild(particleSystem)
		end )
	end
end

-- ����
function Human_Farm.create()
    local object = Human_Farm.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Farm