-- 提示框（双按钮）
MsgBoxDlg = {}
local m_dlg = nil;

-- 初始化
function MsgBoxDlg.init()
	m_dlg = cc.Node:create();
    m_dlg:setName("MsgBoxDlg");
	UIManager.addTop( m_dlg );
end

-- 打开
function MsgBoxDlg.open()
	if m_dlg then
		m_dlg:setVisible( true );
		return;
	end
	MsgBoxDlg.init();
	
end

-- 关闭
function MsgBoxDlg.close()
	if m_dlg then
		m_dlg:setVisible( false );
	end
end

-- 销毁
function MsgBoxDlg.destroy()
	if m_dlg then
		m_dlg:removeFromParent();
		m_dlg = nil;
	end
end
