//
//  SettingsScene.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/2/16.
//
//

#include "SettingsScene.h"
#include "GameSettings.h"
#include "GameZoomMenuItem.h"

USING_NS_CC;

bool SettingsScene::init(void)
{
    bool bRet = Scene::init();
    
    auto rootNode = CSLoader::createNode("Settings/Settings.csb");
    addChild(rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    rootNode->setPosition(winSize.width /2,
                          winSize.height/2);
    
    // replace the old menu with the new one:
    auto oldMenu = rootNode->getChildByName("MenuNode");
    rootNode->removeChild(oldMenu);
    
    auto newMenu = Menu::create();
    rootNode->addChild(newMenu);
    newMenu->setPosition(oldMenu->getPosition());
    
    const char* szFileNameOfBtnTex[] = {
        "Settings/Settings_Off.png",
        "Settings/Settings_On.png",
        "Settings/Settings_Off.png",
        "Settings/Settings_On.png",
        "Settings/Settings_View.png",
        "Settings/Settings_View.png",
        "Settings/Settings_Menu.png"
    };
    
    for (int i = 0; i != oldMenu->getChildrenCount(); ++i)
    {
        auto newMenuItem = GameZoomMenuItem::create(Sprite::create(szFileNameOfBtnTex[i]),
                                                    Sprite::create(szFileNameOfBtnTex[i]),
                                                    Sprite::create(szFileNameOfBtnTex[i]),
                                                    CC_CALLBACK_1(SettingsScene::menuCallback, this));
        newMenu->addChild(newMenuItem, 0, i);
        
        auto oldMenuItem = oldMenu->getChildByTag(i);
        newMenuItem->setPosition(oldMenuItem->getPosition());
    }
    
    return bRet;
}

void SettingsScene::menuCallback(Ref* sender)
{
    switch ( dynamic_cast<Node*>(sender)->getTag() )
    {
        case 0:
        case 1:
            soundItemCallback(sender);
            break;
        case 2:
        case 3:
            musicItemCallback(sender);
            break;
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            Director::getInstance()->popScene();
            break;
        }
        default: break;
    }
}

void SettingsScene::soundItemCallback(Ref* sender)
{
    auto item = dynamic_cast<Node*>(sender);
    auto menu = item->getParent();
    
    item->setVisible(false);
    
    int tag = item->getTag();
    
    if (tag == 0)
    {
        menu->getChildByTag(1)->setVisible(true);
    }
    else
    {
        menu->getChildByTag(0)->setVisible(true);
    }
    
    GameSettings::getInstance()->setSoundEnabed(tag == 1);
}

void SettingsScene::musicItemCallback(Ref* sender)
{
    auto item = dynamic_cast<Node*>(sender);
    auto menu = item->getParent();
    
    item->setVisible(false);
    
    int tag = item->getTag();
    
    if (tag == 2)
    {
        menu->getChildByTag(3)->setVisible(true);
    }
    else
    {
        menu->getChildByTag(2)->setVisible(true);
    }
    
    GameSettings::getInstance()->setMusicEnabed(tag == 3);
}







