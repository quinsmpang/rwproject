-- �յ�
local BaseBuilding = import("building/BaseBuilding")
local Human_Empty = class("Human_Empty", BaseBuilding)

-- ���캯��
function Human_Empty:ctor()
	Human_Empty.super.ctor(self)
	
end

-- ��ʼ��
function Human_Empty:init()
	Human_Empty.super.init(self)
	self._aclass = 0;
	self._kind = 0;
end

-- ���У����൥������ʵ�֣�
function Human_Empty:hit()
	BuildingCreateDlg.open( self:getTag() );
end

-- ����������Ϣ�����൥������ʵ�֣�
function Human_Empty:setAbilityInfo( abilitys )

end

-- ����
function Human_Empty.create()
    local object = Human_Empty.new();
    if object:init() == false then
        return nil;
    end
    return object;
end
return Human_Empty