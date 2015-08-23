#include "TableUtil.h"

TableUtil::TableUtil()
{
	m_nRecordsNum = 0;
	m_nFieldsNum = 0;
}

TableUtil::~TableUtil()
{
	m_DataBuf.clear();
}

TableUtil* TableUtil::createFromFile( const std::string& fileName )
{
	TableUtil* pTableUtil = new TableUtil();
	if ( !pTableUtil->loadFromFile( fileName ) )
	{
		delete pTableUtil;
		return NULL;
	}
	return pTableUtil;
}
bool TableUtil::loadFromFile( const std::string& fileName )
{
	std::string stringBuf = FileUtils::getInstance()->getStringFromFile( fileName );
	if ( stringBuf == "" )
		return false;

	// 拆分得到每行的内容
	std::vector<std::string> lineArray = split( stringBuf, "\r\n" );

	if ( lineArray.size() < 1 )
		return false;

	// 分解第一行标题行
	std::vector<std::string> strArray = split( lineArray[0], "\t" );

	int recordsNum = 0;
	int fieldsNum = strArray.size();

	// 遍历余下行数
	for ( int i = 1; i < (int)lineArray.size(); ++i )
	{
		if ( lineArray[i].size() == 0 )
			break;

		strArray = split( lineArray[i], "\t" );

		// 是不是有内容
		if ( strArray.size() == 0 )
			break;

		if ( strArray[0].size() == 0 )
			break;

		if ( strArray[0][0] == '\0' )
			break;

		// 是不是注释行
		if ( strArray[0][0] == '#' )
			continue;

		// 填充数据区
		for ( int n = 0; n < fieldsNum; ++n )
		{
			m_DataBuf.push_back( strArray[n] );
		}

		++recordsNum;
	}

	// 记录相关信息
	m_nRecordsNum = recordsNum;
	m_nFieldsNum = fieldsNum;
	return true;
}

std::vector<std::string> TableUtil::split( std::string str, const std::string &separator )
{
	std::vector<std::string> result;
	int cutAt;
	while ( (cutAt = str.find_first_of( separator )) != str.npos )
	{
		if ( cutAt > 0 )
		{
			result.push_back( str.substr( 0, cutAt ) );
		}
		str = str.substr( cutAt + 1 );
	}
	if ( str.length() > 0 )
	{
		result.push_back( str );
	}
	return result;
}

std::string TableUtil::GetValue( int recordLine, int columNum )
{
	int position = recordLine * m_nFieldsNum + columNum;

	if ( position < 0 || position > (int)m_DataBuf.size() )
	{
		return "";
	}

	return m_DataBuf[position];
}