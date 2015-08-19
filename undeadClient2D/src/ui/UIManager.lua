--[[
 UI������
 --]]
UIManager = {}
UIManager._root = nil;
UIManager._bottomLayer = nil; -- �ײ�
UIManager._centerLayer = nil; -- �м��
UIManager._topLayer = nil;-- ����

-- ��ʼ��
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
