//
//  BoosterScene.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/2/17.
//
//

#include "BoosterScene.h"

USING_NS_CC;

bool BoosterScene::init(void)
{
    bool bRet = Scene::init();
    
    auto rootNode = CSLoader::createNode("Booster/Booster.csb");
    addChild(rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    rootNode->setPosition(Vec2(winSize.width /2,
                               winSize.height/2));
    
    return bRet;
}
