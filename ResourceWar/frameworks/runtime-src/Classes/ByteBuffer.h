#ifndef __BYTEBUFFER_H_
#define __BYTEBUFFER_H_
#include <string>
#include "utils.h"

// 为了跟C#版本相同，共用一套auto_script，封装一个一模一样的类
class ByteBuffer
{
public:
	ByteBuffer();
	ByteBuffer( void *readbuf, int readsize );
	~ByteBuffer();

public:
	void WriteSByte( unsigned char v )
	{
		LKSET_BYTE_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteByte( char v ) 
	{
		LKSET_BYTE_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteInt( int v )
	{
		LKSET_DWORD_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteUInt( unsigned int v )
	{
		LKSET_DWORD_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteShort( short v ) 
	{
		LKSET_WORD_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteUShort( unsigned short v )
	{
		LKSET_WORD_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteLong( long v ) 
	{
		LKSET_LONG_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteFloat( float v ) 
	{
		LKSET_FLOAT_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteDouble( double v ) 
	{
		LKSET_DOUBLE_SEND( m_WritePtr, &v, m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

	void WriteString( std::string v )
	{
	}

	void WriteStringWithLength( std::string v ) 
	{
		LKSET_MEM_SEND( m_WritePtr, v.c_str(), v.length(), m_WriteSize );
		*(short*)m_WriteHeadPtr = m_WriteSize-sizeof(short);
	}

public:
    int ReadByte() 
	{
		int Value;
		LKSET_BYTE_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	int ReadSByte()
	{
		int Value;
		LKSET_BYTE_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	int ReadInt() 
	{
		int Value;
		LKSET_DWORD_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	unsigned int ReadUInt()
	{
		int Value;
		LKSET_DWORD_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	short ReadShort() 
	{
		short Value;
		LKSET_WORD_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	unsigned short ReadUShort()
	{
		unsigned short Value;
		LKSET_WORD_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	long long ReadLong()
	{
		long long Value;
		LKSET_LONG_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	float ReadFloat() 
	{
		float Value;
		LKSET_FLOAT_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	double ReadDouble() 
	{
		double Value;
		LKSET_DOUBLE_RECV( &Value, m_pReadBuffer, m_ReadSize );
		return Value;
	}

	std::string ReadString() 
	{
		int len = ReadShort();
		char buffer[2048] = { 0 };
		LKSET_MEM_RECV( buffer, m_pReadBuffer, len, m_ReadSize );
		return std::string( buffer );
	}

	std::string ReadStringWithLen( unsigned short len ) 
	{
		char buffer[2048] = { 0 };
		LKSET_MEM_RECV( buffer, m_pReadBuffer, len, m_ReadSize );
		return std::string(buffer);
	}

public:
	char *m_pReadBuffer;
	int m_ReadSize;

	char m_WriteBuffer[2048];
	char *m_WriteHeadPtr;
	char *m_WritePtr;
	int m_WriteSize;
};

#endif
