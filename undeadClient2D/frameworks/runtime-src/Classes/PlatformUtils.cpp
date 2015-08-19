#include <stdio.h>
#include "utils.h"
#include "PlatformUtils.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <Iphlpapi.h>
#endif// CC_TARGET_PLATFORM == CC_PLATFORM_WIN32


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

// 获取设备唯一ID
std::string PlatformUtils::GetDeviceID()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	#include <Iphlpapi.h>
	char macBuf[64] = { 0 };
	char pBuf[128] = { 0 };
	int nSize = 128;
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
	for ( int tmpi = 0; tmpi < (int)uSystemInfoLen; tmpi++ )
	{
		sprintf( macBuf, "0.2x", pBuf[tmpi] );
	}
	return macBuf;

#endif// CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif// CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif// CC_TARGET_PLATFORM == CC_PLATFORM_IOS
}
