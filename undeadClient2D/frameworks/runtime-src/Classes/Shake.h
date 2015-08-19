#ifndef _SHAKE_H_
#define _SHAKE_H_
#include "cocos2d.h"

USING_NS_CC;

class Shake : public ActionInterval
{
public:
	Shake();

	// ������Ч���ĳ�ʼ����������,����������ͬ
	// d �𶯳�����ʱ��
	// strength �𶯵ķ���
	static Shake* create( float d, float strength );
	
	// ������Ч���ĳ�ʼ������,��������ֵ��һ��
	static Shake* create( float d, float strength_x, float strength_y );
	bool initWithDuration( float d, float strength_x, float strength_y );

	//���¶�����д���������ĺ���(������д)
	virtual Shake* clone() const override;
	virtual Shake* reverse( void ) const override;
	virtual void startWithTarget( Node *target ) override;
	virtual void update( float time ) override;
	virtual void stop( void );

protected:
	// �����λ��
	float _initial_x, _initial_y;

	// �����ķ���
	float _strength_x, _strength_y;
};

#endif
