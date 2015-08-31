#ifndef __GLOBALSCHEDULE_H_ 
#define __GLOBALSCHEDULE_H_ 

#include "cocos2d.h" 

USING_NS_CC;

/**
* ȫ�ֶ�ʱ��
*/
class GlobalSchedule : public Ref 
{
public:
	// ��ʼȫ�ֶ�ʱ�� fInterval: ʱ���� ; fDelay: �ӳ����� 
	static void start( float fInterval = 0.0f, float fDelay = 0.0f );
	// ֹͣȫ�ֶ�ʱ�� 
	static void stop();
	// ȫ�ֶ�ʱ����ͣ 
	static void pause();
	// ȫ�ֶ�ʱ����ͣ�ָ� 
	static void resume();

	// ȫ�ֶ�ʱ�����߼�ʵ�� 
	void globalUpdate( float dt );

private:
	// ���캯��˽�л���ֻ��ͨ�� start ������ȫ�ֶ�ʱ�� 
	GlobalSchedule( float fInterval, float fDelay );
	~GlobalSchedule();

	// ��̬�������ֵ��� 
	static GlobalSchedule* m_pSchedule;
};

#endif
