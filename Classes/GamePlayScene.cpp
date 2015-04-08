//
//  GamePlayScene.cpp
//  NewFlipop
//
//  Created by OJO on 14/12/31.
//
//

#include "GamePlayScene.h"
#include "GameResources.h"  // Maybe include to a head file
#include "cocostudio/Cocostudio.h"
#include "ui/CocosGUI.h"
#include "GameData.h"
#include "GameConstant.h"
#include "ResultScene.h"
#include "GameScoreMap.h"
#include "GameBlocksCountManager.h"

USING_NS_CC;

// score label color
const Color3B COLOR_1	( 254,  72, 101);
const Color3B COLOR_2	(  63, 189, 252);
const Color3B COLOR_3	( 255, 226,  67);
const Color3B COLOR_4	( 121, 214, 107);
const Color3B COLOR_5	( 213, 104, 252);
const Color3B COLOR_6 = Color3B::BLACK;

GamePlayScene* GamePlayScene::create(const GamePlayInfo& levelInfo)
{
	auto pRet = new GamePlayScene();

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

bool GamePlayScene::init(const GamePlayInfo& levelInfo)
{
    bool bRet = Scene::init();
   
	initGamePlayInfo(levelInfo);

	initCocostudioRootNode();
	
	// blocks manager
	GameBlocksCountManager::getInstance()->resetBlocksCount();

	_blocksManager = GameBlocksManager::create(
		levelInfo.rows, 
		levelInfo.cols, 
		levelInfo.color);

    _blocksManager->retain();
    _blocksManager->setPositionY(0.f);
    _rootNode->addChild(_blocksManager);
    
    return bRet;
}

void GamePlayScene::initGamePlayInfo(const GamePlayInfo& levelInfo)
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

	// reset data
	GameData::getInstance()->resetGameDataInLevel(_gamePlayInfo.levelId);
}

void GamePlayScene::initCocostudioRootNode()
{
	// loading cocostudio
    _rootNode = CSLoader::createNode( GAMEPLAY_Csb );
    _rootNode->retain();
    addChild(_rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    _rootNode->setPosition( Vec2(winSize.width /2,
                                 winSize.height/2));

	initMenu();
    
	// init top bar
	auto topBar = _rootNode->getChildByName("TopBar");
	topBar->setPositionY( winSize.height/2 );
	topBar->setZOrder(10);

    /*
	* init fever bar and add fever event listener
	*/
    _feverBar = dynamic_cast<ui::LoadingBar*>( topBar->getChildByName("FeverBar") );
    _feverBar->retain();
    
	_eventDispatcher->addCustomEventListener( EVENT_FeverDecrease, CC_CALLBACK_1(GamePlayScene::adjustFeverDecreaseEvent, this));
	_eventDispatcher->addCustomEventListener( EVENT_FeverIncrease, CC_CALLBACK_1(GamePlayScene::adjustFeverIncreaseEvent, this));

    /*
	* init score text and add score event listener
	*/
	_scoreBar = dynamic_cast<ui::LoadingBar*>( topBar->getChildByName("ScoreBar") );
	_scoreBar->retain();

	// score star
	Size scoreBarSize = _scoreBar->getContentSize();

	for (int i = 0; i != 2; ++i)
	{
		float pecentage = float(_gamePlayInfo.scoreGoal[i])/_gamePlayInfo.scoreGoal[2];

		auto star = _scoreBar->getChildByTag(i);
		star->setPositionX( scoreBarSize.width * pecentage );
	}

    auto oldScoreText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("TxtScore"));
    oldScoreText->removeFromParent();
    
    _scoreText = NumberCountingLabel::create();
    _scoreText->retain();
    _scoreText->setPosition(oldScoreText->getPosition());
    _scoreText->setAnchorPoint(oldScoreText->getAnchorPoint());
//    _scoreText->setSystemFontName(oldScoreText->getFontName());
//    _scoreText->setSystemFontSize(oldScoreText->getFontSize());
    _scoreText->setCountingDuration(0.2f);
    topBar->addChild(_scoreText, oldScoreText->getZOrder(), oldScoreText->getName());
    
	_eventDispatcher->addCustomEventListener( EVENT_PutScore, CC_CALLBACK_1(GamePlayScene::adjustPutScoreEvent, this));
	_eventDispatcher->addCustomEventListener( EVENT_GetScore, CC_CALLBACK_1(GamePlayScene::adjustGetScoreEvent, this));
    
	/*
	* init timer bar and add timer event listener
	*/
	_playTimer = _playTimerMax = _gamePlayInfo.time;

    _timerText = dynamic_cast<ui::TextBMFont*>(topBar->getChildByName("BMFontTimer"));
    _timerText->retain();

    char szLabel[5];
    sprintf(szLabel, "%d", int(_playTimer + 0.99f));
    _timerText->setString(szLabel);

    /*
    auto timerBarBg = _rootNode->getChildByName("TimerBar");
    _timerBar = ProgressTimer::create( Sprite::create( GAMEPLAY_TimerBar ));
    _timerBar->retain();
    _timerBar->setPosition(timerBarBg->getPosition());
    _timerBar->setReverseDirection(true);
    _timerBar->setPercentage(100.f);
    _rootNode->addChild(_timerBar, timerBarBg->getZOrder()+1);
*/
	_eventDispatcher->addCustomEventListener( EVENT_PauseTimer, [this](Event*){ 
		_isTimerPause = true;
		_isFeverPause = true;
		setTouchEnabled(false);
	});

	_eventDispatcher->addCustomEventListener( EVENT_ResumeTimer, [this](Event*){
		_isTimerPause = false;
		_isFeverPause = false;
		setTouchEnabled(true);
	});

    schedule( schedule_selector(GamePlayScene::updateForPlayTimer));
	schedule( schedule_selector(GamePlayScene::updateForScore));
}

void GamePlayScene::initMenu()
{
	auto menuNode = dynamic_cast<Node*>(_rootNode->getChildByName("BoosterBar"));

	Size winSize = Director::getInstance()->getWinSize();
	menuNode->setPositionY(115.f - winSize.height/2);

	auto button1 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn1"));
	button1->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;
		
		enableFeverMode();
	});

	auto button2 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn2"));
	button2->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;
		
		_blocksManager->showRevSideHintWithDuration(0.36f);
	});

	auto button3 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn3"));
	button3->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;

		_playTimer += 5.f;

		if (_playTimerMax < _playTimer)
		{
			_playTimerMax = _playTimer;
		}
	});

	auto button4 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn4"));
	button4->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;
		
		_blocksManager->autoMatchCards(3);
	});
}

void GamePlayScene::adjustFeverIncreaseEvent(EventCustom *event)
{
    if (_feverStep + 1 == GAMEPLAY_FeverStep)
    {
		_feverStep = 0;

		enableFeverMode();
    }
    else
    {
        _feverBar->setPercent(100.f * (++_feverStep) / GAMEPLAY_FeverStep);
    }
}

void GamePlayScene::adjustFeverDecreaseEvent(EventCustom *event)
{
    if (_feverStep > 0)
    {
        _feverBar->setPercent(100.f * (--_feverStep) / GAMEPLAY_FeverStep);
    }
}

void GamePlayScene::adjustGetScoreEvent(EventCustom *event)
{
    char* buf = static_cast<char *>(event->getUserData());
    int scoreDelta = 0;
    sscanf(buf, "%d", &scoreDelta);
    
    GameData* instance = GameData::getInstance();
    instance->increaseScore(scoreDelta);
    
    _scoreText->setGoalNumber( instance->getScore());
}

void GamePlayScene::adjustPutScoreEvent(EventCustom *event)
{
	/*
	* get user data and (score, scoreColor, positionX, positionY)
	*/
    char * buf = static_cast<char*>(event->getUserData());
    
    int scoreDelta = 0;
    int scoreColor = 0;
    float wPosX = 0.f;
    float wPosY = 0.f;
    
    sscanf(buf, "%d,%d,%f,%f", &scoreDelta, &scoreColor, &wPosX, &wPosY);
    
	/*
	* create a animation of the score will be get
	*/
	auto topBar = _rootNode->getChildByName("TopBar");

    char szScoreDelta[10];
    snprintf(szScoreDelta, 10, "%d", scoreDelta);
    
    auto scoreDeltaLabel = Label::createWithSystemFont(szScoreDelta, "Arial", 60);
    topBar->addChild(scoreDeltaLabel, 5);
    
    switch (scoreColor)
    {
        case CARD_1: scoreDeltaLabel->setColor(COLOR_1); break;
        case CARD_2: scoreDeltaLabel->setColor(COLOR_2); break;
        case CARD_3: scoreDeltaLabel->setColor(COLOR_3); break;
        case CARD_4: scoreDeltaLabel->setColor(COLOR_4); break;
        case CARD_5: scoreDeltaLabel->setColor(COLOR_5); break;
        case CARD_6: scoreDeltaLabel->setColor(COLOR_6); break;
        default:
            scoreDeltaLabel->setColor(Color3B::ORANGE);  break;
    }
    
    auto CallFuncOfActionFinish = CallFunc::create([=](void){
        
        EventCustom event( EVENT_GetScore );
        
        char szData[10];
        snprintf(szData, 10, "%d", scoreDelta);
        
        event.setUserData(szData);
        _eventDispatcher->dispatchEvent(&event);
    });
    
    Vec2 beginPos = topBar->convertToNodeSpace(Vec2(wPosX, wPosY));
    Vec2 endPos = _scoreText->getPosition() + (_scoreText->getContentSize()*0.5f);
    
    scoreDeltaLabel->setPosition(beginPos);
    scoreDeltaLabel->runAction( Sequence::create(MoveBy::create(0.18f, Vec2(0.f, 60.f)),
                                                 DelayTime::create(0.3f),
                                                 MoveTo::create(beginPos.getDistance(endPos)/1500.f, endPos),
                                                 RemoveSelf::create(),
                                                 CallFuncOfActionFinish,
                                                 nullptr));
}

void GamePlayScene::updateForPlayTimer(float dt)
{
	if (_isTimerPause || isScheduled( schedule_selector(GamePlayScene::updateForFeverMode) ))
	{
		return;
	}

    if (_playTimer < 0.f)
    {
        _playTimer = 0.f;

		auto scoreMap = GameScoreMap::getInstance();
		auto gData = GameData::getInstance();

		if (scoreMap->getScoreWithGameLevel(gData->getLevelId())<gData->getScore())
		{
			scoreMap->setScoreWithGameLevel(gData->getLevelId(), gData->getScore());
		}

        Director::getInstance()->replaceScene( ResultScene::create(_gamePlayInfo) );
    }
    else
    {
        _playTimer -= dt;
    }
    
//    _timerBar->setPercent((_playTimer/_playTimerMax)*100.f);
    
    char szLabel[5];
    sprintf(szLabel, "%d", int(_playTimer + 0.99f));
    
    _timerText->setString(szLabel);
}

void GamePlayScene::updateForFeverMode(float dt)
{
	if (_isFeverPause)
	{
		return;
	}

	if (_feverTimer < 0.f)
	{
		disableFeverMode();
	}
	else
	{
		_feverTimer -= dt;

		_feverBar->setPercent(100.f * _feverTimer / GAMEPLAY_FeverDuration);
	}
}

void GamePlayScene::updateForScore(float dt)
{
	int textScore = _scoreText->getCurrNumber();

	for (int i = 0; i != 3; ++i)
	{
		if (textScore > _gamePlayInfo.scoreGoal[i])
		{
			auto scoreStar = dynamic_cast<Sprite*>(_scoreBar->getChildByTag(i));
			int  actionTag = 50;

			if (scoreStar->getColor() != Color3B::WHITE
				&& scoreStar->getActionByTag(actionTag) == nullptr)
			{
				// enable animation
				auto action = Spawn::create( 
					ScaleTo::create(0.1f, 1.f),
					TintTo::create(0.1f, Color3B::WHITE),
					nullptr);
				action->setTag( actionTag );
				scoreStar->runAction( action );
			}
		}
	}

	_scoreBar->setPercent(100.f * textScore/_gamePlayInfo.scoreGoal[2]);
}

void GamePlayScene::enableFeverMode()
{
	_feverBar->setPercent(100.f);

	_blocksManager->setFeverModeEnabled(true);

	schedule( schedule_selector(GamePlayScene::updateForFeverMode) );

	auto menu = _rootNode->getChildByName("BoosterBar");
	menu->runAction(MoveBy::create(0.6f, Vec2(0.f, -200.f)));
}

void GamePlayScene::disableFeverMode()
{
	_feverTimer = GAMEPLAY_FeverDuration;

	_feverBar->setPercent(100.f * _feverStep / GAMEPLAY_FeverStep);

	_blocksManager->setFeverModeEnabled(false);

	unschedule( schedule_selector(GamePlayScene::updateForFeverMode) );

	auto menu = _rootNode->getChildByName("BoosterBar");
	menu->runAction(MoveBy::create(0.6f, Vec2(0.f, 200.f)));
}

void GamePlayScene::setTouchEnabled(bool var)
{
	if (_isTouchEnabled == var)
	{
		return;
	}
	else
	{
		_isTouchEnabled = var;
	}

	_blocksManager->setTouchEnabled(_isTouchEnabled);
}

bool GamePlayScene::getTouchEnabled()
{
	return _isTouchEnabled;
}

GamePlayScene::~GamePlayScene(void)
{
    CC_SAFE_RELEASE(_rootNode);
    CC_SAFE_RELEASE(_blocksManager);
    CC_SAFE_RELEASE(_feverBar);
//    CC_SAFE_RELEASE(_timerBar);
    CC_SAFE_RELEASE(_timerText);
	CC_SAFE_RELEASE(_scoreBar);
	CC_SAFE_RELEASE(_scoreText);
    
	_eventDispatcher->removeCustomEventListeners( EVENT_FeverDecrease );
	_eventDispatcher->removeCustomEventListeners( EVENT_FeverIncrease );

	_eventDispatcher->removeCustomEventListeners( EVENT_PutScore );
	_eventDispatcher->removeCustomEventListeners( EVENT_GetScore );

	_eventDispatcher->removeCustomEventListeners( EVENT_PauseTimer );
	_eventDispatcher->removeCustomEventListeners( EVENT_ResumeTimer );

}






