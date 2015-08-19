
--------------------------------
-- @module ByteBuffer
-- @parent_module my

--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteLong 
-- @param self
-- @param #long v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadLong 
-- @param self
-- @return long#long ret (return value: long)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteInt 
-- @param self
-- @param #int v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteUInt 
-- @param self
-- @param #unsigned int v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteString 
-- @param self
-- @param #string v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteShort 
-- @param self
-- @param #short v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteSByte 
-- @param self
-- @param #unsigned char v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadStringWithLen 
-- @param self
-- @param #unsigned short len
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadByte 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteStringWithLength 
-- @param self
-- @param #string v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadSByte 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteUShort 
-- @param self
-- @param #unsigned short v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadFloat 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteDouble 
-- @param self
-- @param #double v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteFloat 
-- @param self
-- @param #float v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadShort 
-- @param self
-- @return short#short ret (return value: short)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadInt 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] WriteByte 
-- @param self
-- @param #char v
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadDouble 
-- @param self
-- @return double#double ret (return value: double)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadUShort 
-- @param self
-- @return unsigned short#unsigned short ret (return value: unsigned short)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadUInt 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#ByteBuffer] ReadString 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @overload self, void, int         
-- @overload self         
-- @function [parent=#ByteBuffer] ByteBuffer
-- @param self
-- @param #void readbuf
-- @param #int readsize
-- @return ByteBuffer#ByteBuffer self (return value: ByteBuffer)

return nil
