-- 消息框（无按钮）
NotifyDlg = class("NotifyDlg",function()
    return cc.Node:create()
end)

-- 构造函数
function NotifyDlg:ctor()

end

-- 初始化
function NotifyDlg:init()
	self:setName("NotifyDlg") 
    return true;
end




-- 创建
function NotifyDlg.create()
    local object = NotifyDlg.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return NotifyDlg
