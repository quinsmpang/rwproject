-- 动作的封装
-- 可复用的动作

-- 心跳
function action_heartbeat()
	local action_scale1 = cc.ScaleTo:create(0.2,1.2);
	local action_scale2 = cc.ScaleTo:create(0.1,0.9);
	local action_delay  = cc.DelayTime:create(0.75);
	local action_scale3 = cc.ScaleTo:create(0.2,1.2);
	local action_scale4 = cc.ScaleTo:create(0.1,0.9);
	local seq = cc.Sequence:create(action_scale1,action_scale2,action_delay,action_scale3,action_scale4);
	return seq
end

-- 资源跳跳
function action_resjump()
	local jumpBy = cc.JumpBy:create( 0.5, cc.p(0,0), 10, 1 );
	local jumpByBack = jumpBy:reverse();
	local seq = cc.Sequence:create( jumpBy, jumpByBack );
	return jumpBy
end