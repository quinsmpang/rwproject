--[[
 UI管理器
 --]]
UIManager = {}
UIManager._root = nil;
UIManager._bottomLayer = nil; -- 底层
UIManager._centerLayer = nil; -- 中间层
UIManager._topLayer = nil;-- 顶层

-- 初始化
function UIManager.init()
	local runningScene = cc.Director:getInstance():getRunningScene();
	UIManager._root = runningScene:getChildByName("UIManager");
	if UIManager._root then
		return;
	end
	
	UIManager._root = cc.Node:create()
	UIManager._root:setName("UIManager")
	runningScene:addChild( UIManager._root, 10240 )
	
    UIManager._bottomLayer = cc.Node:create()
    UIManager._root:addChild( UIManager._bottomLayer, 1 )
	
    UIManager._centerLayer = cc.Node:create()
    UIManager._root:addChild( UIManager._centerLayer, 2 )
	
    UIManager._topLayer = cc.Node:create()
    UIManager._root:addChild( UIManager._topLayer, 3 )
    return true;
end

-- 
function UIManager.addBottom( node )
	UIManager._bottomLayer:addChild( node )
end
-- 
function UIManager.getBottomDlg( name )
	UIManager._bottomLayer.getChildByName(name);
end

-- 
function UIManager.addCenter( node )
	UIManager._centerLayer:addChild( node )
end
-- 
function UIManager.getCenterDlg( name )
	UIManager._centerLayer.getChildByName(name);
end

-- 
function UIManager.addTop( node )
	UIManager._topLayer:addChild( node )
end
-- 
function UIManager.getTopDlg( name )
	UIManager._topLayer.getChildByName(name);
end
