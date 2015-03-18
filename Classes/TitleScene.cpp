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
#include "SettingsScene.h"
#include "BoosterScene.h"

USING_NS_CC;

bool TitleScene::init(void)
{
    bool bRet = Scene::init();
    
    auto rootNode = CSLoader::createNode("Title/Title.csb");
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
        "Title/Title_BtnPlay.png",
        "Title/Title_BtnRanking.png",
        "Title/Title_BtnStats.png",
        "Title/Title_BtnSetting.png"
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
            Director::getInstance()->pushScene(BoosterScene::create());
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
            Director::getInstance()->pushScene(SettingsScene::create());
            break;
        }
        default:
            break;
    };
}