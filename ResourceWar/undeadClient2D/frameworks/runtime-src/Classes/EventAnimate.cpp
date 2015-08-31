#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"
#include "EventAnimate.h"
#define ScriptHandler_FrameAnimEventCallFun	9000000  // 帧事件回调

EventAnimate* EventAnimate::create( Animation *animation )
{
	EventAnimate *animate = new (std::nothrow) EventAnimate();
	animate->initWithAnimation( animation );
	animate->autorelease();

	return animate;
}

EventAnimate::EventAnimate()
{

}

EventAnimate::~EventAnimate()
{
	ScriptHandlerMgr::HandlerType handlerType = (ScriptHandlerMgr::HandlerType)( ScriptHandler_FrameAnimEventCallFun );
	ScriptHandlerMgr::getInstance()->removeObjectHandler( (void*)this, handlerType );
}

void EventAnimate::update( float t )
{
	// if t==1, ignore. Animation should finish with t==1
	if ( t < 1.0f ) {
		t *= _animation->getLoops();

		// new loop?  If so, reset frame counter
		unsigned int loopNumber = (unsigned int)t;
		if ( loopNumber > _executedLoops ) {
			_nextFrame = 0;
			_executedLoops++;
		}

		// new t for animations
		t = fmodf( t, 1.0f );
	}

	auto& frames = _animation->getFrames();
	auto numberOfFrames = frames.size();
	SpriteFrame *frameToDisplay = nullptr;

	for ( int i = _nextFrame; i < numberOfFrames; i++ ) {
		float splitTime = _splitTimes->at( i );

		if ( splitTime <= t ) {
			AnimationFrame* frame = frames.at( i );
			frameToDisplay = frame->getSpriteFrame();
			static_cast<Sprite*>(_target)->setSpriteFrame( frameToDisplay );

			FrameEventCallFun( _nextFrame, numberOfFrames );

			_nextFrame = i + 1;
		}
		// Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
		else {
			break;
		}
	}
}

void EventAnimate::FrameEventCallFun( int frame, int numberOfFrames )
{
	int handler = ScriptHandlerMgr::getInstance()->getObjectHandler( (void*)this, (ScriptHandlerMgr::HandlerType)(ScriptHandler_FrameAnimEventCallFun) );
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
		pStack->pushObject( this, "EventAnimate" );
		pStack->pushInt( frame );
		pStack->pushInt( numberOfFrames );
		nRet = pStack->executeFunctionByHandler( handler, 3 );
		pStack->clean();
	}
}