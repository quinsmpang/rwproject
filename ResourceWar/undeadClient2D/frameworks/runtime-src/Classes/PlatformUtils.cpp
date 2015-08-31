#include <stdio.h>
#include "utils.h"
#include "PlatformUtils.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <Iphlpapi.h>
#endif// CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "cocos2d.h"
#include "jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>
#include <cstring>
#endif// CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

PlatformUtils *PlatformUtils::m_pInstance = nullptr;
PlatformUtils *PlatformUtils::GetInstance()
{
	if ( !m_pInstance )
	{
		m_pInstance = new PlatformUtils();
	}
	return m_pInstance;
}

PlatformUtils::PlatformUtils()
{
}

PlatformUtils::~PlatformUtils()
{
}

// ��ȡ�豸ΨһID
std::string PlatformUtils::GetDeviceID()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	char macBuf[32] = { 0 };
	char pBuf[18] = { 0 };
	int nSize = 18;
	UINT uSystemInfoLen = 0;
	UINT uErrorCode = 0;
	IP_ADAPTER_INFO iai;
	ULONG uSize = 0;
	DWORD dwResult = GetAdaptersInfo( &iai, &uSize );
	if ( nSize <= 0 )
		return "";

	if ( dwResult == ERROR_BUFFER_OVERFLOW )
	{
		IP_ADAPTER_INFO* piai = (IP_ADAPTER_INFO*)HeapAlloc( GetProcessHeap(), 0, uSize );
		if ( piai != NULL )
		{
			dwResult = GetAdaptersInfo( piai, &uSize );
			if ( ERROR_SUCCESS == dwResult )
			{
				IP_ADAPTER_INFO* piai2 = piai;
				while ( piai2 != NULL && (uSystemInfoLen + piai2->AddressLength) < (UINT)nSize )
				{
					CopyMemory( pBuf + uSystemInfoLen, piai2->Address, piai2->AddressLength );
					uSystemInfoLen += piai2->AddressLength;
					piai2 = piai2->Next;
				}
			}
			else
			{
				uErrorCode = 0xF0000000U + dwResult;
			}
			HeapFree( GetProcessHeap(), 0, piai );
		}
		else
		{
			return "";
		}
	}
	else
	{
		uErrorCode = 0xE0000000U + dwResult;
	}
	if ( uErrorCode != 0U )
	{
		return "";
	}
	char dev[64] = {0};
	for ( int tmpi = 0; tmpi < (int)uSystemInfoLen; tmpi++ )
	{
		sprintf_s( macBuf, "%2x", pBuf[tmpi] & 0xff );
		strcat( dev, macBuf );
	}
	return dev;

#endif// CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo t;
	std::string ret( "none" );
	if (cocos2d::JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "GetDeviceID", "()Ljava/lang/String;")) 
	{
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef( t.classID );
		ret = cocos2d::JniHelper::jstring2string( str );
		t.env->DeleteLocalRef( str );
	}
	return ret;
#endif// CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif// CC_TARGET_PLATFORM == CC_PLATFORM_IOS
}