//
//  TitleScene.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/1/25.
//
//

#include "TitleScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameZoomMenuItem.h"
#include "GameResources.h"
#include "GameLevelMapScene.h"



USING_NS_CC;

bool TitleScene::init(void)
{
    bool bRet = Scene::init();
    
    auto rootNode = CSLoader::createNode( TITLE_Csb );
    addChild(rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    rootNode->setPosition(Vec2(winSize.width /2,
                               winSize.height/2));
    
    // replace the old menu with the new one:
    auto oldMenu = rootNode->getChildByName("MenuNode");
    rootNode->removeChild(oldMenu);
    
    auto newMenu = Menu::create();
    rootNode->addChild(newMenu);
    newMenu->setPosition(oldMenu->getPosition());
    
    const char* szFileNameOfBtnTex[] = {
		TITLE_BtnPlay,
        TITLE_BtnRank,
        TITLE_BtnStats,
        TITLE_BtnSettings
    };
    
    for (int i = 0; i != oldMenu->getChildrenCount(); ++i)
    {
        auto newMenuItem = GameZoomMenuItem::create(Sprite::create(szFileNameOfBtnTex[i]),
                                                    Sprite::create(szFileNameOfBtnTex[i]),
                                                    Sprite::create(szFileNameOfBtnTex[i]),
                                                    CC_CALLBACK_1(TitleScene::menuCallback, this));
        newMenu->addChild(newMenuItem, 0, i);
        
        auto oldMenuItem = oldMenu->getChildByTag(i);
        newMenuItem->setPosition(oldMenuItem->getPosition());
    }
    
    // replace end
    
    return bRet;
}

void TitleScene::menuCallback(Ref* sender)
{
    int itemTag = dynamic_cast<Node*>(sender)->getTag();
    
    switch (itemTag)
    {
        case 0: // play
        {
			Director::getInstance()->replaceScene( GameLevelMapScene::create() );
            break;
        }
        case 1: // ranking
        {
            break;
        }
        case 2: // stats
        {
            break;
        }
        case 3: // setting
        {
            break;
        }
        default:
            break;
    };
}