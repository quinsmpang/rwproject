#ifndef _SHAKE_H_
#define _SHAKE_H_
#include "cocos2d.h"

USING_NS_CC;

class Shake : public ActionInterval
{
public:
	Shake();

	// 产生震动效果的初始化函数参数,两个方向相同
	// d 震动持续的时间
	// strength 震动的幅度
	static Shake* create( float d, float strength );
	
	// 产生震动效果的初始化函数,两个方向值不一样
	static Shake* create( float d, float strength_x, float strength_y );
	bool initWithDuration( float d, float strength_x, float strength_y );

	//以下都是重写父类抽象类的函数(必须重写)
	virtual Shake* clone() const override;
	virtual Shake* reverse( void ) const override;
	virtual void startWithTarget( Node *target ) override;
	virtual void update( float time ) override;
	virtual void stop( void );

protected:
	// 精灵的位置
	float _initial_x, _initial_y;

	// 抖动的幅度
	float _strength_x, _strength_y;
};

#endif
