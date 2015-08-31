#ifndef _LAUNCHSCENE_H_
#define _LAUNCHSCENE_H_
#include "cocos2d.h"
USING_NS_CC;
#include "IniUtil.h"

// Æô¶¯³¡¾°
class LaunchScene : public Scene
{
public:
	static LaunchScene *create();
	void start();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
protected:
	LaunchScene();
	~LaunchScene();
	bool init();

private:
	IniUtil *m_pAppini;
};


#endif
