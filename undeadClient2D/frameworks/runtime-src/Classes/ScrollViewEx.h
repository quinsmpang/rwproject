/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2010 Sangwoo Im
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __SCROLLVIEWEX_H__
#define __SCROLLVIEWEX_H__

#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScrollViewEx;

/**
 * ScrollView support for cocos2d-x.
 * It provides scroll view functionalities to cocos2d projects natively.
 */
class ScrollViewEx : public ScrollView
{
public:
    static ScrollViewEx* create(Size size, Node* container = NULL);
    static ScrollViewEx* create();
    ScrollViewEx();
    virtual ~ScrollViewEx();

    void setTouchEnabled(bool enabled);
	bool isTouchEnabled() const;


    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    
protected:
	void deaccelerateScrolling( float dt );
	bool _enable;
};

// end of GUI group
/// @}


#endif /* __SCROLLVIEWEX_H__ */
