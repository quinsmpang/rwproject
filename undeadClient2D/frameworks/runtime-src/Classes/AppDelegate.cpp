#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
//#include "lua_module_register.h"
//#include "lua_oneself_auto.hpp"
//#include "lua_oneself_manual.hpp"
#include "TcpManager.h"
#include "LaunchScene.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "ide-support/CodeIDESupport.h"
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
#include "runtime/Runtime.h"
#endif


using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif
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
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);
	Director::getInstance()->getOpenGLView()->setDesignResolutionSize( 640, 960, ResolutionPolicy::NO_BORDER );
	// 启动场景
	LaunchScene *launchScene = LaunchScene::create();
	if ( Director::getInstance()->getRunningScene() )
		Director::getInstance()->replaceScene( launchScene );
	else
		Director::getInstance()->runWithScene( launchScene );

	// 1.检查getWritablePath目录的配置文件，确定包里的assets文件是否拷贝出来了
	// 2.根据getWritablePath目录的配置文件，里面的版本号信息和更新服务器的版本号信息比较，确定是否进行更新
	// 3.需要更新-》启动更新流程
	//   不需更新-》进入游戏

//    // register lua module
//    auto engine = LuaEngine::getInstance();
//    ScriptEngineManager::getInstance()->setScriptEngine(engine);
//    lua_State* L = engine->getLuaStack()->getLuaState();
//    lua_module_register( L );
//	register_all_oneself( L );
//	register_all_oneself_manual( L );
//    // If you want to use Quick-Cocos2d-X, please uncomment below code
//    //register_all_quick_manual(L);
//
//    LuaStack* stack = engine->getLuaStack();
//    stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));
//    
//    //register custom function
//    //LuaStack* stack = engine->getLuaStack();
//    //register_custom_function(stack->getLuaState());
//    
//#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
//    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
//    RuntimeEngine::getInstance()->start();
//    cocos2d::log("iShow!");
//#else
//    if (engine->executeScriptFile("src/main.lua"))
//    {
//        return false;
//    }
//#endif
    
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
