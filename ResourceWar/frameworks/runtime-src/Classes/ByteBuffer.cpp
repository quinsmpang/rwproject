#include "ByteBuffer.h"

ByteBuffer::ByteBuffer()
{
	m_ReadSize = 0;
	m_WriteSize = 0;
	m_WriteHeadPtr = m_WriteBuffer;
	*(short*)m_WriteHeadPtr = 0;
	m_WritePtr = m_WriteBuffer;
	m_WritePtr += sizeof(short);
	m_WriteSize += sizeof(short);
}

ByteBuffer::ByteBuffer( void *readbuf, int readsize )
{
	m_ReadSize = readsize;
	m_pReadBuffer = (char*)readbuf;
}

ByteBuffer::~ByteBuffer()
{
}