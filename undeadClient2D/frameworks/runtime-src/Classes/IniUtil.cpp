#include "IniUtil.h"
#include <fstream>
#include <sstream>

IniUtil::IniUtil()
{
}
IniUtil::~IniUtil()
{
	if ( !this->m_propsMap.empty() ) 
	{
		this->m_propsMap.clear();
	}
}
IniUtil* IniUtil::createFromFile( const std::string& filePath )
{
	IniUtil* pIniUtil = new IniUtil;
	if ( !pIniUtil->loadFromFile( filePath ) )
	{
		delete pIniUtil;
		return NULL;
	}
	return pIniUtil;
}
bool IniUtil::loadFromFile( const std::string& filePath )
{
	std::ifstream fin( filePath.c_str() );
	if ( !fin.is_open() ) 
	{
		return false;
	}

	std::string lineBuffer;
	while ( !fin.eof() )
	{
		std::getline( fin, lineBuffer );
		this->trimString( lineBuffer, std::string( " " ), true );
		this->parseLine( lineBuffer );
	}
	fin.close();
	this->m_filePath = filePath;
	return true;
}
IniUtil* IniUtil::createFromStream( const std::stringstream& sstream )
{
	IniUtil* pIniUtil = new IniUtil;
	if ( !pIniUtil->loadFromStream( sstream ) )
	{
		delete pIniUtil;
		return NULL;
	}
	return pIniUtil;
}
bool IniUtil::loadFromStream( const std::stringstream& sstream )
{
	std::string lineBuffer;
	std::istringstream isstream( sstream.str() );
	while ( !isstream.eof() )
	{
		std::getline( isstream, lineBuffer );
		this->trimString( lineBuffer, std::string( " " ), true );
		this->parseLine( lineBuffer );
	}
	return true;
}
void IniUtil::flush( const std::string& filePath )
{
	std::ofstream fout( filePath.c_str() );
	if ( !fout.is_open() ) {
		return;
	}
	std::string buffer;
	SKVtype::iterator skv_iter;
	KVtype::iterator  kv_iter;
	for ( skv_iter = this->m_propsMap.begin(); skv_iter != this->m_propsMap.end(); ++skv_iter )
	{
		fout << std::endl;
		fout << std::string( "[" ) << skv_iter->first << std::string( "]" ) << std::endl;
		for ( kv_iter = skv_iter->second.begin(); kv_iter != skv_iter->second.end(); ++kv_iter )
		{
			fout << kv_iter->first << std::string( "=" ) << kv_iter->second << std::endl;
		}
	}
	fout.close();
}
bool IniUtil::isSectionExist( const std::string& section ) const
{
	return (this->m_propsMap.find( section ) != this->m_propsMap.end());
}
bool IniUtil::deleteSection( const std::string& section )
{
	if ( !section.empty() ) {
		return false;
	}
	SKVtype::iterator skv_iter = this->m_propsMap.find( section );
	if ( skv_iter != this->m_propsMap.end() )
	{
		this->m_propsMap.erase( skv_iter );
		return true;
	}
	return false;
}
void IniUtil::getSectionNames( std::vector<std::string>& sectionVector )
{
	if ( !sectionVector.empty() ) {
		sectionVector.clear();
	}
	SKVtype::iterator skv_iter = this->m_propsMap.begin();
	for ( ; skv_iter != this->m_propsMap.end(); ++skv_iter )
	{
		sectionVector.push_back( skv_iter->first );
	}
}
void IniUtil::setString( const std::string& section, const std::string& key, const std::string& value )
{
	if ( !section.empty() && !key.empty() )
	{
		this->m_propsMap[section][key] = value;
	}
}
std::string IniUtil::getString( const std::string& section, const std::string& key, const std::string& defaultValue )
{
	if ( !section.empty() && !key.empty() )
	{
		SKVtype::iterator skv_iter = this->m_propsMap.find( section );
		if ( skv_iter != this->m_propsMap.end() )
		{
			KVtype::iterator kv_iter = skv_iter->second.find( key );
			if ( kv_iter != skv_iter->second.end() )
			{
				return kv_iter->second;
			}
		}
	}
	return defaultValue;
}
void IniUtil::setInteger( const std::string& section, const std::string& key, int value )
{
	std::stringstream sstream;
	sstream << value;
	this->setString( section, key, sstream.str() );
}
int IniUtil::getInteger( const std::string& section, const std::string& key, int defaultValue )
{
	std::string tmp = this->getString( section, key, std::string( "" ) );
	std::stringstream sstream;
	sstream << tmp;
	sstream >> defaultValue;
	return defaultValue;
}
void IniUtil::setDouble( const std::string& section, const std::string& key, double value, int pre )
{
	std::stringstream sstream;
	if ( pre ) {
		sstream.precision( pre );
	}
	sstream << value;
	this->setString( section, key, sstream.str() );
}
double IniUtil::getDouble( const std::string& section, const std::string& key, double defaultValue )
{
	std::string tmp = this->getString( section, key, std::string( "" ) );
	std::stringstream sstream;
	if ( !tmp.empty() )
	{
		sstream << tmp;
		sstream >> defaultValue;
	}
	return defaultValue;
}
void IniUtil::setBoolean( const std::string& section, const std::string& key, bool value )
{
	this->setInteger( section, key, value ? 1 : 0 );
}
bool IniUtil::getBoolean( const std::string& section, const std::string& key, bool defaultValue )
{
	std::string tmp = this->getString( section, key, std::string( "" ) );
	if ( !tmp.empty() ) {
		return this->stringToBoolean( tmp, defaultValue );
	}
	return defaultValue;
}
void IniUtil::trimString( std::string& buffer, const std::string& trim, bool isAll )
{
	if ( buffer.empty() ) {
		return;
	}
	while ( buffer.find( trim ) == 0 )
	{
		buffer.erase( 0, trim.length() );
		if ( !isAll ) {
			break;
		}
	}
	while ( !buffer.empty() && (buffer.rfind( trim ) == (buffer.length() - trim.length())) )
	{
		buffer.erase( buffer.length() - trim.length(), trim.length() );
		if ( !isAll ) {
			break;
		}
	}
}
void IniUtil::parseLine( const std::string& buffer )
{
	if ( buffer.empty() ) {
		return;
	}
	switch ( buffer[0] )
	{
	case '#':
	case '%':
		return;
	case '[':
	{
				std::string section = this->parseSection( buffer, std::string( "[" ), std::string( "]" ) );
				this->trimString( section, std::string( " " ), true );
				if ( !section.empty() ) {
					this->m_currentSection = section;
				}
	}
		return;
	default:
	{
				if ( buffer.find( std::string( "=" ) ) != std::string::npos && !this->m_currentSection.empty() )
				{
					std::string key = this->parseSection( buffer, std::string( "" ), std::string( "=" ) );
					this->trimString( key, std::string( " " ), true );
					std::string value = this->parseSection( buffer, std::string( "=" ), std::string( "" ) );
					this->trimString( value, std::string( " " ), true );
					if ( !key.empty() ) {
						this->m_propsMap[this->m_currentSection][key] = value;
					}
				}
	}
		return;
	}
}
std::string IniUtil::parseSection( const std::string& buffer, const std::string& leftTag, const std::string& rightTag )
{
	std::string ret;
	if ( !buffer.empty() )
	{
		std::string::size_type pos_begin = 0, pos_end = 0;
		if ( !leftTag.empty() )
		{
			pos_begin = buffer.find( leftTag );
			if ( pos_begin == std::string::npos ) {
				return ret;
			}
			pos_begin += leftTag.size();
		}
		else {
			pos_begin = 0;
		}
		if ( !rightTag.empty() )
		{
			pos_end = buffer.find( rightTag, pos_begin );
			if ( pos_end == std::string::npos ) {
				return ret;
			}
			ret = buffer.substr( pos_begin, pos_end - pos_begin );
		}
		else {
			ret = buffer.substr( pos_begin, std::string::npos );
		}
	}
	return ret;
}
bool IniUtil::stringToBoolean( const std::string& buffer, bool defaultValue )
{
	if ( buffer.empty() ) {
		return defaultValue;
	}
	std::stringstream sstream;
	sstream << buffer;
	int tmp = 0;
	sstream >> tmp;
	return (buffer.compare( std::string( "true" ) ) == 0 || tmp > 0);
}
