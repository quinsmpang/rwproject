#ifndef _EVENT_ANIMATE_H_
#define _EVENT_ANIMATE_H_
#include <vector>
#include "cocos2d.h"

USING_NS_CC;

// 带有事件回调的动画
class EventAnimate : public Animate
{
public:
	static EventAnimate* create( Animation *animation );
	virtual void update( float t ) override;

CC_CONSTRUCTOR_ACCESS:
	EventAnimate();
	virtual ~EventAnimate();
	void FrameEventCallFun( int frame, int numberOfFrames );

private:
	CC_DISALLOW_COPY_AND_ASSIGN( EventAnimate );
};


#endif
