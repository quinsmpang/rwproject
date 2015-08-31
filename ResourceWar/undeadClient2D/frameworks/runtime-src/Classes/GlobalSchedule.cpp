#include "ClientMessage.h"
#include "GlobalSchedule.h" 

#define SCHEDULE Director::getInstance()->getScheduler()

GlobalSchedule* GlobalSchedule::m_pSchedule = NULL;

GlobalSchedule::GlobalSchedule( float fInterval, float fDelay ) 
{
	CCAssert( !m_pSchedule, "�Ѷ��壬�����ظ�����" );

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

	CCAssert( m_pSchedule, "δ����" );
	CC_SAFE_DELETE( m_pSchedule );
}

void GlobalSchedule::pause() 
{
	cocos2d::log( "GlobalSchedule().pause()" );

	CCAssert( m_pSchedule, "δ����" );
	SCHEDULE->pauseTarget( m_pSchedule );
}

void GlobalSchedule::resume() 
{
	cocos2d::log( "GlobalSchedule().resume()" );

	CCAssert( m_pSchedule, " δ����" );
	SCHEDULE->resumeTarget( m_pSchedule );
}

void GlobalSchedule::globalUpdate( float dt )
{
	// ����дȫ�ֶ�ʱ�����߼�������� 
	//cocos2d::log( "global update" );

	ClientMessage::GetInstance()->Dispatch();
}