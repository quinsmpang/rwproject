
--------------------------------
-- @module TcpManager
-- @extend TcpClientDelegate
-- @parent_module my

--------------------------------
-- 
-- @function [parent=#TcpManager] ConnectServer 
-- @param self
-- @param #char host
-- @param #unsigned short port
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onConnect 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onError 
-- @param self
-- @param #int error
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onClose 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#TcpManager] SendMessage 
-- @param self
-- @param #ByteBuffer buffer
        
--------------------------------
-- 
-- @function [parent=#TcpManager] OnDisconnected 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onMessage 
-- @param self
-- @param #char packdata
-- @param #int packlen
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onConnectFail 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#TcpManager] TcpManager 
-- @param self
        
return nil
