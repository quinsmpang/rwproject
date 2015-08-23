#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "TcpManager.h"
#include "LaunchScene.h"
using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 640, 960, ResolutionPolicy::NO_BORDER );

	// Æô¶¯³¡¾°
	LaunchScene *launchScene = LaunchScene::create();
	if ( Director::getInstance()->getRunningScene() )
		Director::getInstance()->replaceScene( launchScene );
	else
		Director::getInstance()->runWithScene( launchScene );

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
