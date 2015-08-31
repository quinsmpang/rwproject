#include "ClientMessage.h"
#include "GlobalSchedule.h" 

#define SCHEDULE Director::getInstance()->getScheduler()

GlobalSchedule* GlobalSchedule::m_pSchedule = NULL;

GlobalSchedule::GlobalSchedule( float fInterval, float fDelay ) 
{
	CCAssert( !m_pSchedule, "已定义，不能重复定义" );

	SCHEDULE->schedule( CC_SCHEDULE_SELECTOR( GlobalSchedule::globalUpdate ), this, fInterval, CC_REPEAT_FOREVER, fDelay, false );

	m_pSchedule = this;
}

GlobalSchedule::~GlobalSchedule() 
{
	cocos2d::log( "GlobalSchedule().~()" );

	SCHEDULE->unschedule( CC_SCHEDULE_SELECTOR( GlobalSchedule::globalUpdate ), this );
}

void GlobalSchedule::start( float fInterval, float fDelay ) 
{
	new GlobalSchedule( fInterval, fDelay );
}

void GlobalSchedule::stop() 
{
	cocos2d::log( "GlobalSchedule().stop()" );

	CCAssert( m_pSchedule, "未定义" );
	CC_SAFE_DELETE( m_pSchedule );
}

void GlobalSchedule::pause() 
{
	cocos2d::log( "GlobalSchedule().pause()" );

	CCAssert( m_pSchedule, "未定义" );
	SCHEDULE->pauseTarget( m_pSchedule );
}

void GlobalSchedule::resume() 
{
	cocos2d::log( "GlobalSchedule().resume()" );

	CCAssert( m_pSchedule, " 未定义" );
	SCHEDULE->resumeTarget( m_pSchedule );
}

void GlobalSchedule::globalUpdate( float dt )
{
	// 这里写全局定时器的逻辑处理代码 
	//cocos2d::log( "global update" );

	ClientMessage::GetInstance()->Dispatch();
}