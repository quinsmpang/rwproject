-- 木材厂
local BaseBuilding = import("building/BaseBuilding")
local Human_WoodFactory = class("Human_WoodFactory", BaseBuilding)

-- 构造函数
function Human_WoodFactory:ctor()
	Human_WoodFactory.super.ctor(self)
	self._aclass = 0;
	self._kind = BUILDING_WoodFactory;
end

-- 初始化
function Human_WoodFactory:init()
	Human_WoodFactory.super.init(self)
end

-- 设置属性信息（子类单独覆盖实现）
function Human_WoodFactory:setAbilityInfo( abilitys )
	if abilitys[1] > 0 and self._resImage == nil then 
		-- 资源拾取标示
		self._resImage = cc.Sprite:create( "UI_zhujiemian_mutou2.png" );
		self._resImage:setPosition( self:getParent():getPosition() )
		self._resImage:runAction( cc.RepeatForever:create(action_heartbeat()) );
		-- 添加到城市资源拾取层
		GameManager.gameScence._city:addResPickupObject( self._resImage, function()
			-- 移除
			self._resImage:removeFromParent();
			self._resImage = nil;
			-- 发送收取
			system_askinfo( ASKINFO_BUILDING, "", 4, self:getTag() );
			-- 播放粒子
			local particleSystem = cc.ParticleSystemQuad:create("res_pickup.plist")
			particleSystem:setTexture( cc.Director:getInstance():getTextureCache():addImage( "UI_zhujiemian_mutou2.png" ) )
			particleSystem:setPositionType( cc.POSITION_TYPE_GROUPED )
			particleSystem:setAutoRemoveOnFinish(true)
			self:addChild(particleSystem)
		end )
	end
end

-- 创建
function Human_WoodFactory.create()
    local object = Human_WoodFactory.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_WoodFactory