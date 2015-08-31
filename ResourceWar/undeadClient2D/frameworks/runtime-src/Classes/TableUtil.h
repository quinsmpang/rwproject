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
	// ����
	int m_nRecordsNum;
	// ����
	int m_nFieldsNum;
	// ����
	std::vector<std::string> m_DataBuf;
};

#endif
