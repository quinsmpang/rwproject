#include "utils.h"
#include "LaunchScene.h"
#include "CCLuaEngine.h"
#include "lua_module_register.h"
#include "lua_oneself_auto.hpp"
#include "lua_oneself_manual.hpp"

LaunchScene::LaunchScene()
{
}

LaunchScene::~LaunchScene()
{
}

LaunchScene * LaunchScene::create()
{
	LaunchScene * pScene = new (std::nothrow) LaunchScene();
	if ( pScene && pScene->init() )
	{
		pScene->autorelease();
		return pScene;
	}
	CC_SAFE_DELETE( pScene );
	return nullptr;
}

bool LaunchScene::init()
{
	Scene::init();
	return true;
}

void LaunchScene::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();

	FileUtils::getInstance()->addSearchPath( "res" );
	auto mainPage = Sprite::create( "launch/main_page.jpg" );
	mainPage->setPosition( Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2 );
	addChild( mainPage );

	//  启动逻辑
	this->start();
}

void LaunchScene::onExit()
{
	Scene::onExit();
}

void LaunchScene::start()
{
#ifdef WIN32
	std::string appFile = "app.ini";
#else
	std::string appFile = FileUtils::getInstance()->getWritablePath() + "/app.ini";
	if ( !FileUtils::getInstance()->isFileExist( appFile ) )
	{
		u_copy_file( "app.ini", (char*)appFile.c_str() );
	}
#endif // !WIN32
	LOGE( "jiazhi:%s",FileUtils::getInstance()->getWritablePath().c_str() );
	//m_pAppini = IniUtil::createFromFile( appFile );
	// 当前资源版本号
	//int resource_version = m_pAppini->getInteger( "Section", "RESOURCE_VERSION", 0 );
	// 检查更新地址
	//std::string update_url = m_pAppini->getString( "Section", "UPDATE_URL", "" );



	// register lua module
	auto engine = LuaEngine::getInstance();
	ScriptEngineManager::getInstance()->setScriptEngine( engine );
	lua_State* L = engine->getLuaStack()->getLuaState();
	lua_module_register( L );
	register_all_oneself( L );
	register_all_oneself_manual( L );
	// If you want to use Quick-Cocos2d-X, please uncomment below code
	//register_all_quick_manual(L);

	LuaStack* stack = engine->getLuaStack();
	stack->setXXTEAKeyAndSign( "2dxLua", strlen( "2dxLua" ), "XXTEA", strlen( "XXTEA" ) );

	//register custom function
	//LuaStack* stack = engine->getLuaStack();
	//register_custom_function(stack->getLuaState());

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
	// NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
	RuntimeEngine::getInstance()->start();
	cocos2d::log( "iShow!" );
#else
	if ( engine->executeScriptFile( "src/main.lua" ) )
	{
		return;
	}
#endif
}
