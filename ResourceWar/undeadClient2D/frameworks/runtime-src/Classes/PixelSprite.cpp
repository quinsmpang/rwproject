#include "PixelSprite.h"

PixelSprite* PixelSprite::create(  const std::string& filename )
{
	PixelSprite *sprite = new (std::nothrow) PixelSprite();
	if ( sprite && sprite->initWithFile( filename ) )
	{
		sprite->autorelease();
		sprite->resetAlphaInfoFromImage( filename );
		return sprite;
	}
	CC_SAFE_DELETE( sprite );
	return nullptr;
};

PixelSprite::PixelSprite()
{
	m_alpha = nullptr;
};

PixelSprite::~PixelSprite()
{
	CC_SAFE_DELETE_ARRAY( m_alpha );
}

void PixelSprite::setTexture( const std::string &filename )
{
	Sprite::setTexture( filename );
	resetAlphaInfoFromImage( filename );
}

void PixelSprite::resetAlphaInfoFromImage( const std::string &filename )
{
	Image * _image = new Image();
	_image->initWithImageFile( filename );
	m_pixelWidth = _image->getWidth();
	m_pixelHeight = _image->getHeight();
	CC_SAFE_DELETE_ARRAY( m_alpha );
	m_alpha = new (std::nothrow) GLubyte[m_pixelWidth * m_pixelHeight];
	auto _dataLen = _image->getDataLen();
	unsigned char *_data = _image->getData();
	unsigned int *pixels = (unsigned int *)_data;
	//if ( _image->getRenderFormat() == RGBA8888 )
	//{
	//}
	for ( auto i = 0; i < m_pixelHeight; i++ )
	{
		for ( auto j = 0; j < m_pixelWidth; j++ )
		{
			unsigned int pixel = pixels[(i * m_pixelWidth + j)];
			//c.r = *pixel & 0xff;
			//c.g = (*pixel >> 8) & 0xff;
			//c.b = (*pixel >> 16) & 0xff;
			//c.a = (*pixel >> 24) & 0xff;
			m_alpha[i * m_pixelWidth + j] = (pixel >> 24) & 0xff;
		}
	}
	CC_SAFE_DELETE( _image );
}

GLubyte PixelSprite::getAlphaAtPoint( const Vec2 &pt )
{
	Vec2 point;
	point.x = pt.x;
	point.y = getContentSize().height - pt.y;
	int x = (int)point.x - 1;
	if ( x < 0 )
	{
		x = 0;
	}
	else if ( x >= m_pixelWidth ) 
	{
		x = m_pixelWidth - 1;
	}
	int y = (int)point.y - 1;
	if ( y < 0 ) 
	{
		y = 0;
	}
	else if ( y >= m_pixelHeight ) 
	{
		y = m_pixelHeight - 1;
	}
	return m_alpha[m_pixelWidth * y + x];
}

bool PixelSprite::checkHit( const Vec2 &pt )
{
	Vec2 localLocation = convertToNodeSpace( pt );
	Rect validTouchedRect;
	validTouchedRect.size = getContentSize();
	if ( validTouchedRect.containsPoint( localLocation ) && getAlphaAtPoint( localLocation ) > 0 )
	{
		return true;
	}
	return false;
}