-- 例子
--[[AlertDlg = {}
local m_dlg = nil;

-- 初始化
function AlertDlg.init()
	m_dlg = cc.Node:create();
    m_dlg:setName("AlertDlg");
	UIManager.addTop( m_dlg );
end

-- 打开
function AlertDlg.open()
	if m_dlg then
		m_dlg:setVisible( true );
		return;
	end
	AlertDlg.init();
	
end

-- 关闭
function AlertDlg.close()
	if m_dlg then
		m_dlg:setVisible( false );
	end
end

-- 销毁
function AlertDlg.destroy()
	if m_dlg then
		m_dlg:removeFromParent();
	end
end--]]
