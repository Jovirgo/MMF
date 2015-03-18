//
//  GameZoomMenuItem.h
//  NewFlipop
//
//  Created by 欧洲 on 15/1/25.
//
//

#ifndef __NewFlipop__GameScaleMenuItem__
#define __NewFlipop__GameScaleMenuItem__

#include "cocos2d.h"

class GameZoomMenuItem: public cocos2d::MenuItemSprite
{
public:
    static GameZoomMenuItem* create(cocos2d::Node *normalSprite,
                                    cocos2d::Node *selectedSprite,
                                    cocos2d::Node *disabledSprite,
                                    const cocos2d::ccMenuCallback &callback);
    
public:
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    
protected:
    float _originalScale;
    
    GameZoomMenuItem(void) : _originalScale(1.f) {};
};

#endif /* defined(__NewFlipop__GameZoomMenuItem__) */
