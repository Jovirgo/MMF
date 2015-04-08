#include "ResultScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameZoomMenuItem.h"
#include "GameData.h"
#include "GameResources.h"
#include "GamePlayScene.h"
#include "PlayerStats.h"
#include "ReadyScene.h"
#include "GameLevelInfoData.h"

USING_NS_CC;

ResultScene* ResultScene::create(const GamePlayInfo& levelInfo)
{
	auto pRet = new ResultScene();

	if (pRet && pRet->init(levelInfo))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}

	return pRet;
}

bool ResultScene::init(const GamePlayInfo& levelInfo)
{
	bool bRet = Scene::init();

    _rootNode = CSLoader::createNode( RESULT_Csb );
	_rootNode->retain();
    addChild(_rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    _rootNode->setPosition( Vec2(winSize.width /2, 
								 winSize.height/2));
	
	initMenu();
	initTopBar();

	// data ...
	int score = GameData::getInstance()->getScore();
	int stars = 0;

	for (int i = 0; i != 3; ++i)
	{
		if (score > levelInfo.scoreGoal[i])
		{
			++stars;
		}
		else
		{
			break;
		}
	}

	char szText[20];
	snprintf(szText, 20, "%d", score);

	auto scoreText = dynamic_cast<ui::TextBMFont*>( _rootNode->getChildByName("ScoreText"));
	scoreText->setString(szText);

	switch (stars)
	{
	case 1: 
		{
			playStarAnimation1();

			snprintf(szText, 20, "%d", levelInfo.bonusCoin);
			auto bonusText = dynamic_cast<ui::TextBMFont*>(_rootNode->getChildByName("BonusText"));
			bonusText->setString(szText);

			PlayerStats::getInstance()->increaseCoins( levelInfo.bonusCoin );

			break;
		}
	case 2: 
		{
			playStarAnimation2(); 

			snprintf(szText, 20, "%d", levelInfo.bonusCoin);
			auto bonusText = dynamic_cast<ui::TextBMFont*>(_rootNode->getChildByName("BonusText"));
			bonusText->setString(szText);

			PlayerStats::getInstance()->increaseCoins( levelInfo.bonusCoin );

			break;
		}
	case 3: 
		{
			playStarAnimation3(); 

			snprintf(szText, 20, "%d", levelInfo.bonusCoin);
			auto bonusText = dynamic_cast<ui::TextBMFont*>(_rootNode->getChildByName("BonusText"));
			bonusText->setString(szText);

			PlayerStats::getInstance()->increaseCoins( levelInfo.bonusCoin );

			break;
		}
	case 0:
	default: break;
	}

	initGamePlayInfo(levelInfo);

	return bRet;
}


void ResultScene::initMenu()
{
	char* btnEnabledImages[] = {
		RESULT_BackEnabled,
		RESULT_ReplayEnabled,
		RESULT_NextEnabled
	};

	char* btnDisabledImages[] = {
		RESULT_BackDisabled,
		RESULT_ReplayDisabled,
		RESULT_NextDisabled,
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
												CC_CALLBACK_1(ResultScene::menuItemCallback, this));
		newItem->setPosition(oldItem->getPosition());
		newItem->setTag(i);
		newMenu->addChild(newItem);
	}

	if (GameData::getInstance()->getScore() < _gamePlayInfo.scoreGoal[0])
	{
		dynamic_cast<GameZoomMenuItem*>( newMenu->getChildByTag(2) )->setEnabled(false);
	}

	_rootNode->removeChild(oldMenu);
	_rootNode->addChild(newMenu, oldMenu->getZOrder(), oldMenu->getName());
}

void ResultScene::initTopBar()
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

void ResultScene::initGamePlayInfo(const GamePlayInfo& levelInfo)
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

void ResultScene::onEnter()
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

	_eventDispatcher->addCustomEventListener(EVENT_IncreasePowers, CC_CALLBACK_1(ResultScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreasePowersSuccess, CC_CALLBACK_1(ResultScene::adjustIncreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseCoins, CC_CALLBACK_1(ResultScene::adjustDecreasePowersEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseCoinsSuccess, CC_CALLBACK_1(ResultScene::adjustDecreaseCoinsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_IncreaseDiamonds, CC_CALLBACK_1(ResultScene::adjustIncreaseDiamondsEvent, this));
	_eventDispatcher->addCustomEventListener(EVENT_DecreaseDiamondsSuccess, CC_CALLBACK_1(ResultScene::adjustDecreaseDiamondsEvent, this));
	
	schedule( schedule_selector(ResultScene::updateForTimerText), 1.f);
}

void ResultScene::onExit()
{
	unschedule( schedule_selector(ResultScene::updateForTimerText));

	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseCoins );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseCoinsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreaseDiamonds );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreaseDiamondsSuccess );
	_eventDispatcher->removeCustomEventListeners( EVENT_IncreasePowers );
	_eventDispatcher->removeCustomEventListeners( EVENT_DecreasePowersSuccess );

	Scene::onExit();
}

void ResultScene::menuItemCallback(Ref* sender)
{
	auto item = dynamic_cast<GameZoomMenuItem*>(sender);

	switch (item->getTag())
	{
		case 0: back();		break;
		case 1: replay();	break;
		case 2: next();		break;
		default:
			break;
	}
}

void ResultScene::back()
{
	Director::getInstance()->popScene();
}

void ResultScene::replay()
{
	Director::getInstance()->replaceScene( ReadyScene::create(_gamePlayInfo) );
}

void ResultScene::next()
{
	GameLevelInfoData levelInfoTable;

	int levelId = _gamePlayInfo.levelId + 1;

	if (levelId < levelInfoTable.getRows())
	{
		GamePlayInfo newInfo;

		sscanf(levelInfoTable.getData(levelId, GameLevelInfoData::Field::LevelID), "%d", &newInfo.levelId);
		sscanf(levelInfoTable.getData(levelId, GameLevelInfoData::Field::Rows), "%d", &newInfo.rows);
		sscanf(levelInfoTable.getData(levelId, GameLevelInfoData::Field::Cols), "%d", &newInfo.cols);
		sscanf(levelInfoTable.getData(levelId, GameLevelInfoData::Field::Time), "%d", &newInfo.time);

		sscanf(levelInfoTable.getData(levelId, GameLevelInfoData::Field::BonusCoin), "%d", &newInfo.bonusCoin);

		for (int i = 0; i != 5; ++i)
		{
			sscanf(levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::Color1), "%d", newInfo.color+i);
		}

		for (int i = 0; i != 3; ++i)
		{
			sscanf(levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::ScoreGoal1), "%d", newInfo.scoreGoal+i);
			sscanf(levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::BonusTool1), "%d", newInfo.bonusTool+i);
		}

		Director::getInstance()->replaceScene( ReadyScene::create(newInfo) );
	}
}

void ResultScene::playStarAnimation1()
{
	auto timeline = CSLoader::createTimeline( RESULT_Csb );
	timeline->play("StarAnimation1", false);
	timeline->setFrameEventCallFunc( CC_CALLBACK_1(ResultScene::StarFrameEventCallFunc, this));

	_rootNode->runAction(timeline);
}

void ResultScene::playStarAnimation2()
{
	auto timeline = CSLoader::createTimeline( RESULT_Csb );
	timeline->play("StarAnimation2", false);
	timeline->setFrameEventCallFunc( CC_CALLBACK_1(ResultScene::StarFrameEventCallFunc, this));

	_rootNode->runAction(timeline);
}

void ResultScene::playStarAnimation3()
{
	auto timeline = CSLoader::createTimeline( RESULT_Csb );
	timeline->play("StarAnimation3", false);
	timeline->setFrameEventCallFunc( CC_CALLBACK_1(ResultScene::StarFrameEventCallFunc, this));

	_rootNode->runAction(timeline);
}

void ResultScene::StarFrameEventCallFunc(cocostudio::timeline::Frame* frame)
{
	/*
    auto evnt = dynamic_cast<cocostudio::timeline::EventFrame*>(frame); 
    if( !evnt) 
        return; 

	std::string str = evnt->getEvent();

	if (str == "")
    {
    }
	*/
}

void ResultScene::adjustIncreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ResultScene::adjustDecreaseCoinsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getCoins());

	_coinsText->setString(szText);
}

void ResultScene::adjustIncreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void ResultScene::adjustDecreaseDiamondsEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getDiamonds());

	_diamondsText->setString(szText);
}

void ResultScene::adjustIncreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ResultScene::adjustDecreasePowersEvent(EventCustom* event)
{
	char szText[20];
	snprintf(szText, 20, "%d", PlayerStats::getInstance()->getPowers());

	_powersText->setString(szText);
}

void ResultScene::updateForTimerText(float dt)
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

ResultScene::ResultScene()
	: _rootNode(nullptr)
	, _coinsText(nullptr)
	, _diamondsText(nullptr)
	, _powersText(nullptr)
	, _timerText(nullptr)
{}

ResultScene::~ResultScene()
{
	CC_SAFE_RELEASE(_rootNode);
	CC_SAFE_RELEASE(_coinsText);
	CC_SAFE_RELEASE(_diamondsText);
	CC_SAFE_RELEASE(_powersText);
	CC_SAFE_RELEASE(_timerText);
}