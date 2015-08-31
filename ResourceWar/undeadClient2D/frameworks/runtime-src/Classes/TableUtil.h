#ifndef _TABLEUTIL_H_
#define _TABLEUTIL_H_
#include <string>
#include "cocos2d.h"
USING_NS_CC;

class TableUtil : public Ref
{
public:
	TableUtil();
	~TableUtil();
	static TableUtil* createFromFile( const std::string& fileName );
	int GetRecordsNum(){ return m_nRecordsNum; }
	int GetFieldsNum(){ return m_nFieldsNum; };
	std::string GetValue( int recordLine, int columNum );
private:
	bool loadFromFile( const std::string& fileName );
	std::vector<std::string> split( std::string str, const std::string &separator );

private:
	// 行数
	int m_nRecordsNum;
	// 列数
	int m_nFieldsNum;
	// 数据
	std::vector<std::string> m_DataBuf;
};

#endif
