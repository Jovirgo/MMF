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
    
    // init menu
    const char* szFileNameOfBtnTex[] = {
		TITLE_BtnPlay,
        TITLE_BtnRank,
        TITLE_BtnStats,
        TITLE_BtnSettings
    };

    auto oldMenu = rootNode->getChildByName("MenuNode");
    auto newMenu = Menu::create();
    newMenu->setPosition(oldMenu->getPosition());
    
    
    for (int i = 0; i != oldMenu->getChildrenCount(); ++i)
    {
        auto oldItem = oldMenu->getChildByTag(i);
        auto newItem = GameZoomMenuItem::create(Sprite::create(szFileNameOfBtnTex[i]),
                                                Sprite::create(szFileNameOfBtnTex[i]),
                                                Sprite::create(szFileNameOfBtnTex[i]),
                                                CC_CALLBACK_1(TitleScene::menuCallback, this));
        newItem->setPosition(oldItem->getPosition());
		newItem->setTag(i);
        newMenu->addChild(newItem);
    }

    rootNode->removeChild(oldMenu);
    rootNode->addChild(newMenu, oldMenu->getZOrder(), oldMenu->getName());
    
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