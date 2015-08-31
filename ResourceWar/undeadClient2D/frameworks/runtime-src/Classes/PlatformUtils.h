#ifndef _PLATFORMUTILS_H_
#define _PLATFORMUTILS_H_
#include <string>

// 平台工具
class PlatformUtils
{
public:
	static PlatformUtils* GetInstance();
public:
	std::string GetDeviceID();

private:
	PlatformUtils();
	~PlatformUtils();
	static PlatformUtils *m_pInstance;

private:

};

#endif
