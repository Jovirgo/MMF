#include "ResultScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameData.h"
#include "GamePlayScene.h"

USING_NS_CC;

bool ResultScene::init()
{
	bool bRet = Scene::init();

    auto rootNode = CSLoader::createNode("Result/Result.csb");
    addChild(rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    rootNode->setPosition( Vec2(winSize.width /2, winSize.height/2));

	auto gData = GameData::getInstance();

	char szTextString[10];
	snprintf(szTextString, 10, "%d", gData->getScore());

	auto scoreTxt = dynamic_cast<ui::Text*>(rootNode->getChildByName("TxtScore"));
	scoreTxt->setString(szTextString);

	snprintf(szTextString, 10, "%d", gData->getComboMax());

	auto comboTxt = dynamic_cast<ui::Text*>(rootNode->getChildByName("TxtCombo"));
	comboTxt->setString(szTextString);

	snprintf(szTextString, 10, "%d", gData->getMatch());

	auto matchTxt = dynamic_cast<ui::Text*>(rootNode->getChildByName("TxtMatch"));
	matchTxt->setString(szTextString);

	// button
	auto playBtn = dynamic_cast<ui::Button*>(rootNode->getChildByName("BtnPlay"));
	playBtn->addClickEventListener([](Ref*){
		Director::getInstance()->replaceScene(GamePlayScene::create());
	});

	return bRet;
}