#include "ReadyScene.h"
#include "GameResources.h"
#include "GameZoomMenuItem.h"
#include "PlayerStats.h"
#include "GamePlayScene.h"

USING_NS_CC;

ReadyScene* ReadyScene::create(const GamePlayInfo& levelInfo)
{
	ReadyScene* pRet = new ReadyScene();

	if (pRet && pRet->init(levelInfo))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return pRet;
	}
}

bool ReadyScene::init(const GamePlayInfo& levelInfo)
{
	bool bRet = Scene::init();
	
	initGamePlayInfo(levelInfo);
	initCocostudioRootNode();

	return bRet;
}

void ReadyScene::initCocostudioRootNode()
{
	_rootNode = CSLoader::createNode( READY_Csb );
	_rootNode->retain();
	addChild(_rootNode);

	Size winSize = Director::getInstance()->getWinSize();
	_rootNode->setPosition(winSize.width /2,
						   winSize.height/2);
	initMenu();
	initTopBar();

	char szText[10];

	snprintf(szText, 10, "%d", _gamePlayInfo.levelId);
	auto levelText = _rootNode->getChildByName("LevelText");
	dynamic_cast<ui::TextBMFont*>(levelText)->setString(szText);

	snprintf(szText, 10, "%d", _gamePlayInfo.scoreGoal[0]);
	auto targetText = _rootNode->getChildByName("TargetText");
	dynamic_cast<ui::TextBMFont*>(targetText)->setString(szText);

	auto btnExit = dynamic_cast<ui::Button*>( _rootNode->getChildByName("BtnExit") );
	btnExit->addClickEventListener([](Ref*){
		// may be change
		Director::getInstance()->popScene();
	});

	// btn tool ...
}

void ReadyScene::initMenu()
{
	char* btnEnabledImages[] = {
		READY_StartEnabled
	};

	char* btnDisabledImages[] = {
		READY_StartDisabled
	};

	// init menu
	auto oldMenu = _rootNode->getChildByName("Menu");
	auto newMenu = Menu::create();
	newMenu->setPosition(oldMenu->getPosition());

	for (int i = 0; i != oldMenu->getChildrenCount(); ++i)
	{
		auto oldItem = oldMenu->getChildByTag(i);
		auto newItem = GameZoomMenuItem::create(Sprite::create(btnEnabledImages[i]),
												Sprite::create(btnEnabledImages[i]),
												Sprite::create(btnDisabledImages[i]),
												CC_CALLBACK_1(ReadyScene::menuItemCallback, this));
		newItem->setPosition(oldItem->getPosition());
		newItem->setTag(i);
		newMenu->addChild(newItem);
	}

	_rootNode->removeChild(oldMenu);
	_rootNode->addChild(newMenu, oldMenu->getZOrder(), oldMenu->getName());
}

void ReadyScene::initTopBar()
{
	auto topBar = _rootNode->getChildByName("StatsBar");

	_coinsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("CoinsText"));
	_coinsText->retain();

	_diamondsText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("DiamondsText"));
	_diamondsText->retain();

	_powersText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("PowersText"));
	_powersText->retain();

	_timerText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("TimerText"));
	_timerText->retain();
}

void ReadyScene::initGamePlayInfo(const GamePlayInfo& levelInfo)
{
	_gamePlayInfo.levelId =		levelInfo.levelId;
	_gamePlayInfo.rows =		levelInfo.rows;
	_gamePlayInfo.cols =		levelInfo.cols;
	_gamePlayInfo.time =		levelInfo.time;
	_gamePlayInfo.color[0] =	levelInfo.color[0];
	_gamePlayInfo.color[1] =	levelInfo.color[1];
	_gamePlayInfo.color[2] =	levelInfo.color[2];
	_gamePlayInfo.color[3] =	levelInfo.color[3];
	_gamePlayInfo.color[4] =	levelInfo.color[4];
	_gamePlayInfo.bonusCoin =	levelInfo.bonusCoin;
	_gamePlayInfo.scoreGoal[0] = levelInfo.scoreGoal[0];
	_gamePlayInfo.scoreGoal[1] = levelInfo.scoreGoal[1];
	_gamePlayInfo.scoreGoal[2] = levelInfo.scoreGoal[2];
	_gamePlayInfo.bonusTool[0] = levelInfo.bonusTool[0];
	_gamePlayInfo.bonusTool[1] = levelInfo.bonusTool[1];
	_gamePlayInfo.bonusTool[2] = levelInfo.bonusTool[2];
}

void ReadyScene::onEnter()
{
	Scene::onEnter();

	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());
	_coinsText->setString(szText);
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());
	_diamondsText->setString(szText);
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowersByCalculate());
	_powersText->setString(szText);

	if (PlayerStats::getInstance()->isFullPower())
	{
		_timerText->setVisible(false);
	}
	else
	{
		int duration = PlayerStats::getInstance()->getRemainingChargingDur();
		int min = duration / 60;
		int sec = duration % 60;
		snprintf(szText, 10, "%d:%02d", min, sec);
		_timerText->setString(szText);
	}

	_eventDispatcher->addCustomEventListener(EVENT_IncreasePowers, CC_CALLBACK_1(ReadyScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreasePowersSuccess, CC_CALLBACK_1(ReadyScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseCoins, CC_CALLBACK_1(ReadyScene::adjustDecreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseCoinsSuccess, CC_CALLBACK_1(ReadyScene::adjustDecreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseDiamonds, CC_CALLBACK_1(ReadyScene::adjustIncreaseDiamondsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseDiamondsSuccess, CC_CALLBACK_1(ReadyScene::adjustDecreaseDiamondsEvent, this));

	schedule( schedule_selector(ReadyScene::updateForTimerText), 1.f);
}

void ReadyScene::onExit()
{
	unschedule( schedule_selector(ReadyScene::updateForTimerText));

	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseCoins );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseCoinsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseDiamonds );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseDiamondsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreasePowers );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreasePowersSuccess );

	Scene::onExit();
}

void ReadyScene::menuItemCallback(Ref* sender)
{
	if (PlayerStats::getInstance()->decreasePowers( READY_PowersDecrease ))
	{
		Director::getInstance()->replaceScene( GamePlayScene::create(_gamePlayInfo) );
	}
	else
	{
		// show message: low powers ...
	}
}


void ReadyScene::adjustIncreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ReadyScene::adjustDecreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ReadyScene::adjustIncreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void ReadyScene::adjustDecreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void ReadyScene::adjustIncreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ReadyScene::adjustDecreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ReadyScene::updateForTimerText(float dt)
{
	auto playerStats = PlayerStats::getInstance();

	if (playerStats->isFullPower())
	{
		_timerText->setVisible(false);
		return;
	}
	else
	{
		_timerText->setVisible(true);
	}

	// display
	char szText[10];
	
	int duration = PlayerStats::getInstance()->getRemainingChargingDur();
	int min = duration / 60;
	int sec = duration % 60;
	snprintf(szText, 10, "%d:%02d", min, sec);
	_timerText->setString(szText);

	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowersByCalculate());
	_powersText->setString(szText);
}

ReadyScene::ReadyScene()
	: _rootNode(nullptr)
	, _timerText(nullptr)
	, _coinsText(nullptr)
	, _diamondsText(nullptr)
	, _powersText(nullptr)
{
}

ReadyScene::~ReadyScene()
{
	CC_SAFE_RELEASE(_rootNode);
	CC_SAFE_RELEASE(_timerText);
	CC_SAFE_RELEASE(_coinsText);
	CC_SAFE_RELEASE(_diamondsText);
	CC_SAFE_RELEASE(_powersText);
}