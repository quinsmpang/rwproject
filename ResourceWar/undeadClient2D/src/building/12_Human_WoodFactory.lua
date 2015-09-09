-- ľ�ĳ�
local BaseBuilding = import("building/BaseBuilding")
local Human_WoodFactory = class("Human_WoodFactory", BaseBuilding)

-- ���캯��
function Human_WoodFactory:ctor()
	Human_WoodFactory.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_WoodFactory;
end

-- ��ʼ��
function Human_WoodFactory:init()
	Human_WoodFactory.super.init(self)
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_WoodFactory:setAbilityInfo( abilitys )
	if abilitys[1] > 0 and self._resImage == nil then 
		-- ��Դʰȡ��ʾ
		self._resImage = cc.Sprite:create( "UI_zhujiemian_mutou2.png" );
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
			particleSystem:setTexture( cc.Director:getInstance():getTextureCache():addImage( "UI_zhujiemian_mutou2.png" ) )
			particleSystem:setPositionType( cc.POSITION_TYPE_GROUPED )
			particleSystem:setAutoRemoveOnFinish(true)
			self:addChild(particleSystem)
		end )
	end
end

-- ����
function Human_WoodFactory.create()
    local object = Human_WoodFactory.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_WoodFactory