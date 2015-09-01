#include "platform/CCDevice.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "ScrollViewEx.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define ScriptHandler_ScrollViewExTouchEnd 9000010
#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f
#define MOVE_INCH            7.0f/160.0f
#define BOUNCE_BACK_FACTOR   0.35f

static float convertDistanceFromPointToInch( float pointDis )
{
	auto glview = Director::getInstance()->getOpenGLView();
	float factor = (glview->getScaleX() + glview->getScaleY()) / 2;
	return pointDis * factor / Device::getDPI();
}

ScrollViewEx::ScrollViewEx()
:_enable(true)
{

}

ScrollViewEx::~ScrollViewEx()
{
	ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType)(ScriptHandler_ScrollViewExTouchEnd);
	ScriptHandlerMgr::getInstance()->removeObjectHandler( (void*)this, handlerType );
}

ScrollViewEx* ScrollViewEx::create(Size size, Node* container/* = nullptr*/)
{
	ScrollViewEx* pRet = new (std::nothrow) ScrollViewEx( );
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

ScrollViewEx* ScrollViewEx::create()
{
	ScrollViewEx* pRet = new (std::nothrow) ScrollViewEx( );
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool ScrollViewEx::isTouchEnabled() const
{
	return _enable;
}

void ScrollViewEx::setTouchEnabled(bool enabled)
{
	_enable = enabled;
	if ( _enable )
	{
		if ( _touchListener == nullptr )
		{
			_touchListener = EventListenerTouchOneByOne::create();
			_touchListener->onTouchBegan = CC_CALLBACK_2( ScrollViewEx::onTouchBegan, this );
			_touchListener->onTouchMoved = CC_CALLBACK_2( ScrollViewEx::onTouchMoved, this );
			_touchListener->onTouchEnded = CC_CALLBACK_2( ScrollViewEx::onTouchEnded, this );
			_touchListener->onTouchCancelled = CC_CALLBACK_2( ScrollViewEx::onTouchCancelled, this );

			_eventDispatcher->addEventListenerWithSceneGraphPriority( _touchListener, this );
		}
	}
	else
	{
		_dragging = false;
		_touchMoved = false;
		_touches.clear();
	}
}


bool ScrollViewEx::onTouchBegan(Touch* touch, Event* event)
{
	if ( !_enable )
	{
		return false;
	}
    if (!this->isVisible() || !this->hasVisibleParents())
    {
        return false;
    }
    
    Rect frame = getViewRect();

    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (_touches.size() > 2 ||
        _touchMoved          ||
        !frame.containsPoint(touch->getLocation()))
    {
        return false;
    }

    if (std::find(_touches.begin(), _touches.end(), touch) == _touches.end())
    {
        _touches.push_back(touch);
    }

    if (_touches.size() == 1)
    { // scrolling
        _touchPoint     = this->convertTouchToNodeSpace(touch);
        _touchMoved     = false;
        _dragging     = true; //dragging started
        _scrollDistance = Vec2(0.0f, 0.0f);
        _touchLength    = 0.0f;
    }
    else if (_touches.size() == 2)
    {
        _touchPoint = (this->convertTouchToNodeSpace(_touches[0]).getMidpoint(
                        this->convertTouchToNodeSpace(_touches[1])));
        
        _touchLength = _container->convertTouchToNodeSpace(_touches[0]).getDistance(
                       _container->convertTouchToNodeSpace(_touches[1]));
        
        _dragging  = false;
    } 
    return true;
}

void ScrollViewEx::onTouchMoved(Touch* touch, Event* event)
{
	if ( !_enable )
	{
		return;
	}
	
    if (!this->isVisible())
    {
        return;
    }

    if (std::find(_touches.begin(), _touches.end(), touch) != _touches.end())
    {
        if (_touches.size() == 1 && _dragging)
        { // scrolling
            Vec2 moveDistance, newPoint;
            Rect  frame;
            float newX, newY;
            
            frame = getViewRect();

            newPoint     = this->convertTouchToNodeSpace(_touches[0]);
            moveDistance = newPoint - _touchPoint;
            
            float dis = 0.0f;
            if (_direction == Direction::VERTICAL)
            {
                dis = moveDistance.y;
                float pos = _container->getPosition().y;
                if (!(minContainerOffset().y <= pos && pos <= maxContainerOffset().y)) {
                    moveDistance.y *= BOUNCE_BACK_FACTOR;
                }
            }
            else if (_direction == Direction::HORIZONTAL)
            {
                dis = moveDistance.x;
                float pos = _container->getPosition().x;
                if (!(minContainerOffset().x <= pos && pos <= maxContainerOffset().x)) {
                    moveDistance.x *= BOUNCE_BACK_FACTOR;
                }
            }
            else
            {
                dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);
                
                float pos = _container->getPosition().y;
                if (!(minContainerOffset().y <= pos && pos <= maxContainerOffset().y)) {
                    moveDistance.y *= BOUNCE_BACK_FACTOR;
                }
                
                pos = _container->getPosition().x;
                if (!(minContainerOffset().x <= pos && pos <= maxContainerOffset().x)) {
                    moveDistance.x *= BOUNCE_BACK_FACTOR;
                }
            }

            if (!_touchMoved && fabs(convertDistanceFromPointToInch(dis)) < MOVE_INCH )
            {
                //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
                return;
            }
            
            if (!_touchMoved)
            {
                moveDistance = Vec2::ZERO;
            }
            
            _touchPoint = newPoint;
            _touchMoved = true;
            
            if (_dragging)
            {
                switch (_direction)
                {
                    case Direction::VERTICAL:
                        moveDistance = Vec2(0.0f, moveDistance.y);
                        break;
                    case Direction::HORIZONTAL:
                        moveDistance = Vec2(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }

                newX     = _container->getPosition().x + moveDistance.x;
                newY     = _container->getPosition().y + moveDistance.y;

                _scrollDistance = moveDistance;
                this->setContentOffset(Vec2(newX, newY));
            }
        }
        else if (_touches.size() == 2 && !_dragging)
        {
            const float len = _container->convertTouchToNodeSpace(_touches[0]).getDistance(
                                            _container->convertTouchToNodeSpace(_touches[1]));
            this->setZoomScale(this->getZoomScale()*len/_touchLength);
        }
    }
}

void ScrollViewEx::onTouchEnded(Touch* touch, Event* event)
{
	if ( !_enable )
	{
		return;
	}
    if (!this->isVisible())
    {
        return;
    }
    
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    
    if (touchIter != _touches.end())
    {
        if (_touches.size() == 1 && _touchMoved)
        {
            this->schedule(CC_SCHEDULE_SELECTOR(ScrollViewEx::deaccelerateScrolling));
        }
        _touches.erase(touchIter);
    } 

	if ( !_dragging )
	{ // 用于缩放的反弹
		TouchEndCallFun();
	}

    if (_touches.size() == 0)
    {
        _dragging = false;    
        _touchMoved = false;
    }
}

void ScrollViewEx::onTouchCancelled(Touch* touch, Event* event)
{
    if (!this->isVisible())
    {
        return;
    }
    
    auto touchIter = std::find(_touches.begin(), _touches.end(), touch);
    _touches.erase(touchIter);
    
    if (_touches.size() == 0)
    {
        _dragging = false;    
        _touchMoved = false;
    }
}

void ScrollViewEx::deaccelerateScrolling( float dt )
{
	if ( _dragging )
	{
		this->unschedule( CC_SCHEDULE_SELECTOR( ScrollViewEx::deaccelerateScrolling ) );
		return;
	}

	float newX, newY;
	Vec2 maxInset, minInset;

	_container->setPosition( _container->getPosition() + _scrollDistance );

	if ( _bounceable )
	{
		maxInset = _maxInset;
		minInset = _minInset;
	}
	else
	{
		maxInset = this->maxContainerOffset();
		minInset = this->minContainerOffset();
	}

	newX = _container->getPosition().x;
	newY = _container->getPosition().y;

	_scrollDistance = _scrollDistance * SCROLL_DEACCEL_RATE;
	this->setContentOffset( Vec2( newX, newY ) );

	// 此处重写
	if ( fabsf( _scrollDistance.x ) <= SCROLL_DEACCEL_DIST &&
		 fabsf( _scrollDistance.y ) <= SCROLL_DEACCEL_DIST )
	{
		this->unschedule( CC_SCHEDULE_SELECTOR( ScrollViewEx::deaccelerateScrolling ) );
		this->relocateContainer( true );
		return;
	}

	if ( (newY >= maxInset.y || newY <= minInset.y) && (newX >= maxInset.x || newX <= minInset.x) )
	{
		this->unschedule( CC_SCHEDULE_SELECTOR( ScrollViewEx::deaccelerateScrolling ) );
		this->relocateContainer( true );
	}
	
}

void ScrollViewEx::TouchEndCallFun()
{
	int handler = ScriptHandlerMgr::getInstance()->getObjectHandler( (void*)this, (ScriptHandlerMgr::HandlerType)(ScriptHandler_ScrollViewExTouchEnd) );
	if ( 0 != handler )
	{
		if ( NULL == ScriptEngineManager::getInstance()->getScriptEngine() ) {
			return;
		}

		LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
		if ( NULL == pStack ) {
			return;
		}

		lua_State *tolua_s = pStack->getLuaState();
		if ( NULL == tolua_s ) {
			return;
		}

		int nRet = 0;
		nRet = pStack->executeFunctionByHandler( handler, 0 );
		pStack->clean();
	}
}