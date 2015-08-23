#ifndef __INIUTIL_H_
#define __INIUTIL_H_
#include <map>
#include <vector>
#include <string>

class IniUtil
{
public:
	IniUtil();
	virtual ~IniUtil();
	static IniUtil* createFromFile( const std::string& filePath );
	static IniUtil* createFromStream( const std::stringstream& sstream );

	bool loadFromFile( const std::string& filePath );
	bool loadFromStream( const std::stringstream& sstream );
	void flush( const std::string& filePath );
	std::string getFilePath() const { return this->m_filePath;}
	bool isSectionExist( const std::string& section ) const;
	bool deleteSection( const std::string& section );
	void getSectionNames( std::vector<std::string>& sectionVector );
	void setString( const std::string& section, const std::string& key, const std::string& value );
	std::string getString( const std::string& section, const std::string& key, const std::string& defaultValue );
	void setInteger( const std::string& section, const std::string& key, int value );
	int getInteger( const std::string& section, const std::string& key, int defaultValue );
	void setDouble( const std::string& section, const std::string& key, double value, int pre );
	double getDouble( const std::string& section, const std::string& key, double defaultValue );
	void setBoolean( const std::string& section, const std::string& key, bool value );
	bool getBoolean( const std::string& section, const std::string& key, bool defalutValue );

protected:
	void trimString( std::string& buffer, const std::string& trim, bool isAll );
	void parseLine( const std::string& buffer );
	std::string parseSection( const std::string& buffer, const std::string& leftTag, const std::string& rightTag );
	bool stringToBoolean( const std::string& buffer, bool defaultValue );

	typedef std::map<std::string, std::string> KVtype;
	typedef std::map<std::string, KVtype> SKVtype;

	std::string m_filePath;
	std::string m_currentSection;
	SKVtype m_propsMap;
};

#endif
