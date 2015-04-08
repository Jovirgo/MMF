//
//  GameLevelMapScene.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/3/21.
//
//

#include "GameLevelMapScene.h"
#include "ReadyScene.h"
#include "GameResources.h"
#include "GameData.h"
#include "PlayerStats.h"
#include "GameScoreMap.h"
#include "GameSettings.h"
#include "ShopScene.h"

USING_NS_CC;

bool GameLevelMapScene::init()
{
    bool bRet = Scene::init();
    
	// loading cococstudio resources
    _rootNode = CSLoader::createNode( MAP_Csb );
    _rootNode->retain();
    addChild(_rootNode);

    Size winSize = Director::getInstance()->getWinSize();
    _rootNode->setPosition(Vec2(winSize.width/2, 0));
    
    for (int i = 0; i != 10; ++i)
    {
        pushPlateToMapWithSize(CSLoader::createNode( MAP_PlateCsb ), MAP_PlateSize);
    }
    
	auto scrollView = dynamic_cast<ui::ScrollView*>(_rootNode->getChildByName("ScrollView"));

	Size scrollViewSize = scrollView->getContentSize();
	scrollViewSize.height = winSize.height;
	scrollView->setContentSize( scrollViewSize );
	scrollView->jumpToBottom();
	
	initTopBar();

	auto btnShop = dynamic_cast<ui::Button*>(_rootNode->getChildByName("BtnShop"));
	btnShop->addClickEventListener([](Ref*){
		Director::getInstance()->pushScene( ShopScene::create() );
	});

	GameSettings::getInstance()->playMusic( BgMusicMp3 );
    
    return bRet;
}

void GameLevelMapScene::initTopBar()
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

void GameLevelMapScene::onEnter()
{
	Scene::onEnter();

	auto scrollView = dynamic_cast<ui::ScrollView*>(_rootNode->getChildByName("ScrollView"));

	// update high score
	auto gData = GameData::getInstance();

//	if (_levelScoreMap[ gData->getLevelId() ] < gData->getScore())
//	{
//		_levelScoreMap[ gData->getLevelId() ] = gData->getScore();
//	}

	auto scoreMap = GameScoreMap::getInstance();

	// update level btn stats
	for (auto iter = _levelBtns.begin(); iter != _levelBtns.end(); ++iter)
    {
		int levelId = (*iter)->getTag();

		auto btn = dynamic_cast<ui::Button*>(*iter);

		if (levelId == 1)
			// level 1 must be visibled
		{
			btn->setVisible(true);
		}
		else
			// other level depend on last level
		{
			int scoreGoal = 0;
			sscanf( _levelInfoTable.getData(levelId-1, GameLevelInfoData::Field::ScoreGoal1), "%d", &scoreGoal);

//			if (_levelScoreMap[levelId-1] < scoreGoal)
			if (scoreMap->getScoreWithGameLevel(levelId-1) < scoreGoal)
			{
				btn->setVisible(false);
				continue;
			}
			else
			{
				btn->setVisible(true);
			}
		}

		// initial 3 stars on the button
		for (int i = 0; i != 3; ++i)
		{
			auto star = dynamic_cast<Sprite*>( btn->getChildByTag(i));

			int scoreGoal = 0;
			sscanf( _levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::ScoreGoal1), "%d", &scoreGoal);

//			if (scoreGoal <= _levelScoreMap[levelId])
			if (scoreGoal <= scoreMap->getScoreWithGameLevel(levelId))
			{
				star->setTexture( TextureCache::getInstance()->addImage( MAP_StarFull ));
			}
			else
			{
				star->setTexture( TextureCache::getInstance()->addImage( MAP_StarEmpty ));
			}
		}
    }

	// Stats bar
	char szText[20];

	snprintf(szText, 20, "%d",PlayerStats::getInstance()->getCoins());
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

	schedule( schedule_selector(GameLevelMapScene::updateForTimerText), 1.f);
}

void GameLevelMapScene::onExit()
{
	unschedule( schedule_selector(GameLevelMapScene::updateForTimerText));

	Scene::onExit();
}

void GameLevelMapScene::pushPlateToMapWithSize(Node* plate, const Size& plateSize)
{
    auto scrollView = dynamic_cast<ui::ScrollView*>(_rootNode->getChildByName("ScrollView"));
    scrollView->addChild(plate);
    
    
    // update _innerHeight
    plate->setPosition( Vec2(plateSize.width /2, 
							 plateSize.height/2 + _innerHeight));
    _innerHeight += plateSize.height;
    
    
    // update setInnerContainerSize
    auto innerSize = scrollView->getContentSize();
    if (innerSize.height < _innerHeight)
    {
        innerSize.height = _innerHeight;
    }
    scrollView->setInnerContainerSize(innerSize);
    
    
    // initial level button
    auto levels = plate->getChildByName("Levels");

    for (auto iter = levels->getChildren().begin(); iter != levels->getChildren().end(); ++iter)
    {
		auto button = dynamic_cast<ui::Button*>(*iter);
		button->setSwallowTouches(false);
		button->setTag(button->getTag() + _levelCount);
		button->addTouchEventListener( CC_CALLBACK_2(GameLevelMapScene::levelItemCallback, this));

		char szNumber[5];
		snprintf(szNumber, 5, "%d", button->getTag());

		auto text = dynamic_cast<ui::TextBMFont*>(button->getChildByName("Text"));
		text->setString(szNumber);

		_levelBtns.pushBack(button);
    }

	// initial locked level
	auto lockedLevels = plate->getChildByName("LockedLevels");

	for (auto iter = lockedLevels->getChildren().begin(); iter != lockedLevels->getChildren().end(); ++iter)
	{
		char szNumber[5];
		snprintf(szNumber, 5, "%d", (*iter)->getTag()+_levelCount);

		auto text = dynamic_cast<ui::TextBMFont*>((*iter)->getChildByName("Text"));
		text->setString(szNumber);
	}

    _levelCount += levels->getChildrenCount();
}

void GameLevelMapScene::goToGamePlaySceneWithLevelId(int levelId)
{
	if (levelId < _levelInfoTable.getRows())
	{
		GamePlayInfo info;

		sscanf(_levelInfoTable.getData(levelId, GameLevelInfoData::Field::LevelID), "%d", &info.levelId);
		sscanf(_levelInfoTable.getData(levelId, GameLevelInfoData::Field::Rows), "%d", &info.rows);
		sscanf(_levelInfoTable.getData(levelId, GameLevelInfoData::Field::Cols), "%d", &info.cols);
		sscanf(_levelInfoTable.getData(levelId, GameLevelInfoData::Field::Time), "%d", &info.time);

		sscanf(_levelInfoTable.getData(levelId, GameLevelInfoData::Field::BonusCoin), "%d", &info.bonusCoin);

		for (int i = 0; i != 5; ++i)
		{
			sscanf(_levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::Color1), "%d", info.color+i);
		}

		for (int i = 0; i != 3; ++i)
		{
			sscanf(_levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::ScoreGoal1), "%d", info.scoreGoal+i);
			sscanf(_levelInfoTable.getData(levelId, i+GameLevelInfoData::Field::BonusTool1), "%d", info.bonusTool+i);
		}

		auto scene = ReadyScene::create(info);
		Director::getInstance()->pushScene(scene);
	}
	else
	{
		CCLOG("No level in levelInfoTable");
	}
}

void GameLevelMapScene::levelItemCallback(Ref* sender, ui::Widget::TouchEventType type)
{
	auto button = dynamic_cast<ui::Button*>(sender);
	auto parent = button->getParent();

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		{
			_touchBeganPos = parent->convertToWorldSpace( button->getPosition());
			break;
		}
	case ui::Widget::TouchEventType::MOVED:
		{
			auto pos = parent->convertToWorldSpace( button->getPosition());

			if (pos.distance(_touchBeganPos) > TOUCH_Sensitivity)
			{
				button->setHighlighted(false);
			}
			break;
		}
	case ui::Widget::TouchEventType::ENDED:
		{
			_touchBeganPos = Vec2::ZERO;

			goToGamePlaySceneWithLevelId(button->getTag());
			break;
		}
	default:
		break;
	}
}

void GameLevelMapScene::updateForTimerText(float dt)
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

GameLevelMapScene::~GameLevelMapScene()
{
	_levelBtns.erase(_levelBtns.begin(), _levelBtns.end());
	
    CC_SAFE_RELEASE(_timerText);
    CC_SAFE_RELEASE(_coinsText);
    CC_SAFE_RELEASE(_diamondsText);
    CC_SAFE_RELEASE(_powersText);
    CC_SAFE_RELEASE(_rootNode);
}

