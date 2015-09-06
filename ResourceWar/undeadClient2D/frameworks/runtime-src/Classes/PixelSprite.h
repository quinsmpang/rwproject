#ifndef _PIXEL_SPRITE_H_
#define _PIXEL_SPRITE_H_
#include "cocos2d.h"
USING_NS_CC;

/* 可获取像素点的精灵 */
class PixelSprite : public Sprite
{
public:
	static PixelSprite* create( const std::string& filename );
	void setTexture( const std::string &filename );
	GLubyte getAlphaAtPoint( const Vec2 &pt );
	bool checkHit( const Vec2 &pt );

protected:
	PixelSprite();
	virtual ~PixelSprite();
	void resetAlphaInfoFromImage( const std::string &filename );

private:
	GLubyte *m_alpha;
	int m_pixelWidth;
	int m_pixelHeight;
	std::string m_filename;
};

#endif // _BUILDING_SPRITE_H_
