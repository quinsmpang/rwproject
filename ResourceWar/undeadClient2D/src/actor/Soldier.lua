-- 空地
local BaseActor = import("actor/BaseActor")
local Soldier = class("Soldier", BaseActor)

-- 构造函数
function Soldier:ctor()
	Soldier.super.ctor(self)
	
	-- 资源配置
	self._info = { 
	["attack"]={ ["plist"]="Human_Warrior.plist", ["file"]="Human_Warrior.pvr.ccz", ["fameName"]="Char_H_Warrior_Atk", ["frameCount"]=7, ["delay"]=0.1 }
	}
	-- 缓存动画使用
	self._actorType="Human_Warrior";
end

-- 初始化
function Soldier:init()
	Soldier.super.init(self)
	
	return true;
end

-- 创建
function Soldier.create()
    local object = Soldier.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Soldier