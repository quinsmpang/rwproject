
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
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onConnect 
-- @param self
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onError 
-- @param self
-- @param #int error
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onClose 
-- @param self
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] SendMessage 
-- @param self
-- @param #ByteBuffer buffer
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] OnDisconnected 
-- @param self
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onMessage 
-- @param self
-- @param #char packdata
-- @param #int packlen
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] onConnectFail 
-- @param self
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
--------------------------------
-- 
-- @function [parent=#TcpManager] TcpManager 
-- @param self
-- @return TcpManager#TcpManager self (return value: TcpManager)
        
return nil
