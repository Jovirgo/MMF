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
#include "GameBlocksCountManager.h"

USING_NS_CC;

const Color3B COLOR_1	( 254,  72, 101);
const Color3B COLOR_2	(  63, 189, 252);
const Color3B COLOR_3	( 255, 226,  67);
const Color3B COLOR_4	( 121, 214, 107);
const Color3B COLOR_5	( 213, 104, 252);
const Color3B COLOR_6 = Color3B::BLACK;

bool GamePlayScene::init(void)
{
    bool bRet = Scene::init();
    

	/*
	* reset data
	*/
	GameData::getInstance()->resetGameData();
	GameBlocksCountManager::getInstance()->resetBlocksCount();


    _rootNode = CSLoader::createNode("GamePlay/GamePlay.csb");
    _rootNode->retain();
    addChild(_rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    _rootNode->setPosition( Vec2(winSize.width /2,
                                 winSize.height/2));

    _blocksManager = GameBlocksManager::create(4, 4, 3);
    _blocksManager->retain();
    _blocksManager->setPositionY(-30.f);
    _rootNode->addChild(_blocksManager);

	initMenu();
    

    /*
	* init fever bar and add fever event listener
	*/
    auto feverBarBg = _rootNode->getChildByName("FeverBar");
    
    _feverBar = ProgressTimer::create(Sprite::create("GamePlay/Gameplay_Feverfull.png"));
    _feverBar->retain();
    _feverBar->setPosition(feverBarBg->getPosition());
    _feverBar->setType(ProgressTimer::Type::BAR);
    _feverBar->setBarChangeRate(Vec2(1, 0));
    _feverBar->setPercentage(0.f);
    _feverBar->setMidpoint(Vec2::ZERO);
    _rootNode->addChild(_feverBar, feverBarBg->getZOrder()+1);
    
	_eventDispatcher->addCustomEventListener("FeverDecreaseEvent", CC_CALLBACK_1(GamePlayScene::adjustFeverDecreaseEvent, this));
	_eventDispatcher->addCustomEventListener("FeverIncreaseEvent", CC_CALLBACK_1(GamePlayScene::adjustFeverIncreaseEvent, this));
    

    /*
	* init score text and add score event listener
	*/
    auto oldScoreText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtScore"));
    oldScoreText->removeFromParent();
    
    auto newScoreText = NumberCountingLabel::create();
    newScoreText->retain();
    newScoreText->setPosition(oldScoreText->getPosition());
    newScoreText->setAnchorPoint(oldScoreText->getAnchorPoint());
    newScoreText->setSystemFontName(oldScoreText->getFontName());
    newScoreText->setSystemFontSize(oldScoreText->getFontSize());
    newScoreText->setCountingDuration(0.2f);
    _rootNode->addChild(newScoreText, oldScoreText->getZOrder(), oldScoreText->getName());
    
	_eventDispatcher->addCustomEventListener("PutScoreEvent", CC_CALLBACK_1(GamePlayScene::adjustPutScoreEvent, this));
	_eventDispatcher->addCustomEventListener("GetScoreEvent", CC_CALLBACK_1(GamePlayScene::adjustGetScoreEvent, this));
    

	/*
	* init timer bar and add timer event listener
	*/
    _timerText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtTimer"));
    _timerText->retain();

    char szLabel[5];
    sprintf(szLabel, "%d", int(_playTimer + 0.99f));
    _timerText->setString(szLabel);
    
    auto timerBarBg = _rootNode->getChildByName("TimerBar");
    _timerBar = ProgressTimer::create(Sprite::create("GamePlay/Gameplay_TimeFull.png"));
    _timerBar->retain();
    _timerBar->setPosition(timerBarBg->getPosition());
    _timerBar->setReverseDirection(true);
    _timerBar->setPercentage(100.f);
    _rootNode->addChild(_timerBar, timerBarBg->getZOrder()+1);

	_eventDispatcher->addCustomEventListener("PauseTimerEvent", [this](Event*){ 
		_isTimerPause = true;
		_isFeverPause = true;
		setTouchEnabled(false);
	});

	_eventDispatcher->addCustomEventListener("ResumeTimerEvent", [this](Event*){
		_isTimerPause = false;
		_isFeverPause = false;
		setTouchEnabled(true);
	});

    schedule( schedule_selector(GamePlayScene::updateForPlayTimer));


	/*
	* start animation
	*/
	auto gameTimeLine = CSLoader::createTimeline("GamePlay/GamePlay.csb");
	gameTimeLine->play("ReadyAnimation", false);
	gameTimeLine->setLastFrameCallFunc([this](){ schedule( schedule_selector(GamePlayScene::updateForPlayTimer)); });

	_rootNode->runAction(gameTimeLine);

	setTouchEnabled(true);
    
    return bRet;
}

void GamePlayScene::initMenu()
{
	auto menuNode = dynamic_cast<Node*>(_rootNode->getChildByName("Node"));

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

		_blocksManager->autoMatchCards(3);
	});

	auto button3 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn3"));
	button3->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;

		_playTimer += 5.f;
		if (_playTimer > PLAY_TIME)
		{
			_playTimer = PLAY_TIME;
		}
	});

	auto button4 = dynamic_cast<ui::Button*>(menuNode->getChildByName("Btn4"));
	button4->addClickEventListener([this](Ref*){
		if (!_isTouchEnabled)
			return;

		_blocksManager->showRevSideHintWithDuration(0.36f);
	});
}

void GamePlayScene::adjustFeverIncreaseEvent(EventCustom *event)
{
    if (_feverStep + 1 == FEVER_MAX)
    {
		_feverStep = 0;

		enableFeverMode();
    }
    else
    {
        _feverBar->setPercentage(100.f * (++_feverStep) / FEVER_MAX);
    }
}

void GamePlayScene::adjustFeverDecreaseEvent(EventCustom *event)
{
    if (_feverStep > 0)
    {
        _feverBar->setPercentage(100.f * (--_feverStep) / FEVER_MAX);
    }
}

void GamePlayScene::adjustGetScoreEvent(EventCustom *event)
{
    char* buf = static_cast<char *>(event->getUserData());
    int scoreDelta = 0;
    sscanf(buf, "%d", &scoreDelta);
    
    GameData* instance = GameData::getInstance();
    instance->increaseScore(scoreDelta);
    
    auto scoreText = dynamic_cast<NumberCountingLabel*>(_rootNode->getChildByName("TxtScore"));
    scoreText->setGoalNumber( instance->getScore());
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
    char szScoreDelta[10];
    snprintf(szScoreDelta, 10, "%d", scoreDelta);
    
    auto scoreDeltaLabel = Label::createWithSystemFont(szScoreDelta, "Arial", 60);
    _rootNode->addChild(scoreDeltaLabel, 5);
    
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
        
        EventCustom event("GetScoreEvent");
        
        char szData[10];
        snprintf(szData, 10, "%d", scoreDelta);
        
        event.setUserData(szData);
        _eventDispatcher->dispatchEvent(&event);
    });
    
    Vec2 beginPos = _rootNode->convertToNodeSpace(Vec2(wPosX, wPosY));
    
    auto scoreText = dynamic_cast<NumberCountingLabel*>(_rootNode->getChildByName("TxtScore"));
    Vec2 endPos = scoreText->getPosition() + (scoreText->getContentSize()*0.5f);
    
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

        Director::getInstance()->replaceScene(ResultScene::create());
    }
    else
    {
        _playTimer -= dt;
    }
    
    _timerBar->setPercentage((_playTimer/PLAY_TIME)*100.f);
    
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

		_feverBar->setPercentage(100.f * _feverTimer / FEVER_DURATION);
	}
}

void GamePlayScene::enableFeverMode()
{
        _feverBar->setPercentage(100.f);

        _blocksManager->setFeverModeEnabled(true);

		schedule( schedule_selector(GamePlayScene::updateForFeverMode) );

		auto menu = _rootNode->getChildByName("Node");
		menu->runAction(MoveBy::create(0.6f, Vec2(0.f, -200.f)));
}

void GamePlayScene::disableFeverMode()
{
		_feverTimer = FEVER_DURATION;
		
        _feverBar->setPercentage(100.f * _feverStep / FEVER_MAX);

        _blocksManager->setFeverModeEnabled(false);

		unschedule( schedule_selector(GamePlayScene::updateForFeverMode) );

		auto menu = _rootNode->getChildByName("Node");
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
    CC_SAFE_RELEASE(_timerBar);
    CC_SAFE_RELEASE(_timerText);
    
	_eventDispatcher->removeCustomEventListeners("FeverDecreaseEvent");
	_eventDispatcher->removeCustomEventListeners("FeverIncreaseEvent");

	_eventDispatcher->removeCustomEventListeners("PutTimerEvent");
	_eventDispatcher->removeCustomEventListeners("GetTimerEvent");

	_eventDispatcher->removeCustomEventListeners("PutScoreEvent");
	_eventDispatcher->removeCustomEventListeners("GetScoreEvent");

	_eventDispatcher->removeCustomEventListeners("PauseTimerEvent");
	_eventDispatcher->removeCustomEventListeners("ResumeTimerEvent");

	_eventDispatcher->removeCustomEventListeners("PutCoinsEvent");
	_eventDispatcher->removeCustomEventListeners("GetCoinsEvent");
}






