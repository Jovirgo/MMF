//
//  GameZoomMenuItem.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/1/25.
//
//

#include "GameZoomMenuItem.h"

USING_NS_CC;

const unsigned int kZoomActionTag = 0xc0c05002;

GameZoomMenuItem* GameZoomMenuItem::create(Node* normalSprite,
                                           Node* selectedSprite,
                                           Node* disabledSprite,
                                           const ccMenuCallback& callback)
{
    auto pRet = new GameZoomMenuItem();
    
    if (pRet && pRet->initWithNormalSprite(normalSprite,
                                           selectedSprite,
                                           disabledSprite,
                                           callback))
    {
        pRet->retain();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


void GameZoomMenuItem::activate()
{
    if(_enabled)
    {
        this->stopAllActions();
        this->setScale( _originalScale );
        MenuItem::activate();
    }
}

void GameZoomMenuItem::selected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::selected();
        
        Action* action = getActionByTag(kZoomActionTag);
        if (action)
        {
            this->stopAction(action);
        }
        else
        {
            _originalScale = this->getScale();
        }
        
        Action* zoomAction = ScaleTo::create(0.06f, _originalScale * 1.1f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void GameZoomMenuItem::unselected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action* zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}
