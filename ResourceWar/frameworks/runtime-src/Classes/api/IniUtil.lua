
--------------------------------
-- @module IniUtil
-- @parent_module my

--------------------------------
-- 
-- @function [parent=#IniUtil] getDouble 
-- @param self
-- @param #string section
-- @param #string key
-- @param #double defaultValue
-- @return double#double ret (return value: double)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] setDouble 
-- @param self
-- @param #string section
-- @param #string key
-- @param #double value
-- @param #int pre
        
--------------------------------
-- 
-- @function [parent=#IniUtil] getString 
-- @param self
-- @param #string section
-- @param #string key
-- @param #string defaultValue
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] deleteSection 
-- @param self
-- @param #string section
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] getInteger 
-- @param self
-- @param #string section
-- @param #string key
-- @param #int defaultValue
-- @return int#int ret (return value: int)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] loadFromFile 
-- @param self
-- @param #string filePath
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] isSectionExist 
-- @param self
-- @param #string section
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] getBoolean 
-- @param self
-- @param #string section
-- @param #string key
-- @param #bool defalutValue
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] setInteger 
-- @param self
-- @param #string section
-- @param #string key
-- @param #int value
        
--------------------------------
-- 
-- @function [parent=#IniUtil] flush 
-- @param self
-- @param #string filePath
        
--------------------------------
-- 
-- @function [parent=#IniUtil] getFilePath 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] setString 
-- @param self
-- @param #string section
-- @param #string key
-- @param #string value
        
--------------------------------
-- 
-- @function [parent=#IniUtil] setBoolean 
-- @param self
-- @param #string section
-- @param #string key
-- @param #bool value
        
--------------------------------
-- 
-- @function [parent=#IniUtil] createFromFile 
-- @param self
-- @param #string filePath
-- @return IniUtil#IniUtil ret (return value: IniUtil)
        
--------------------------------
-- 
-- @function [parent=#IniUtil] IniUtil 
-- @param self
        
return nil
