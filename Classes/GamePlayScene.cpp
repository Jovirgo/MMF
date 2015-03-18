//
//  GamePlayScene.cpp
//  NewFlipop
//
//  Created by OJO on 14/12/31.
//
//

#include "GamePlayScene.h"
#include "GameResources.h"  // Maybe include to a head file
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "GameData.h"
#include "GameConstant.h"

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
    
    _rootNode = CSLoader::createNode("GamePlay/GamePlay.csb");
    _rootNode->retain();
    addChild(_rootNode);
    
    Size winSize = Director::getInstance()->getWinSize();
    _rootNode->setPosition( Vec2(winSize.width /2,
                                 winSize.height/2));
    
    // ...

    _blocksManager = GameBlocksManager::create(4, 4, 3);
    _blocksManager->retain();
    _blocksManager->setPositionY(-30.f);
    _rootNode->addChild(_blocksManager);
    
    // fever bar
    auto feverBarBg = _rootNode->getChildByName("FeverBar");
    
    _feverBar = ProgressTimer::create(Sprite::create("GamePlay/Gameplay_Feverfull.png"));
    _feverBar->retain();
    _feverBar->setPosition(feverBarBg->getPosition());
    _feverBar->setType(ProgressTimer::Type::BAR);
    _feverBar->setBarChangeRate(Vec2(1, 0));
    _feverBar->setPercentage(0.f);
    _feverBar->setMidpoint(Vec2::ZERO);
    _rootNode->addChild(_feverBar, feverBarBg->getZOrder()+1);
    
    
    auto feverListener_1 = EventListenerCustom::create("FeverDecreaseEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustFeverDecreaseEvent, this));
    auto feverListener_2 = EventListenerCustom::create("FeverIncreaseEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustFeverIncreaseEvent, this));
    /*auto feverListener_3 = EventListenerCustom::create("FeverDisabledEvent", [=](EventCustom* event){
        _blocksManager->setFeverModeEnabled(false);
    });*/
    
    _eventDispatcher->addEventListenerWithFixedPriority(feverListener_1, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(feverListener_2, 1);
    // _eventDispatcher->addEventListenerWithFixedPriority(feverListener_3, 1);
    
    
    // score text
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
    
    auto scoreListener_1 = EventListenerCustom::create("PutScoreEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustPutScoreEvent, this));
    auto scoreListener_2 = EventListenerCustom::create("GetScoreEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustGetScoreEvent, this));
    
    _eventDispatcher->addEventListenerWithFixedPriority(scoreListener_1, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(scoreListener_2, 1);
    
    
    // timer bar
    schedule( schedule_selector(GamePlayScene::updateForPlayTimer));
    
    _timerText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtTimer"));
    _timerText->retain();
    
    auto timerBarBg = _rootNode->getChildByName("TimerBar");
    
    _timerBar = ProgressTimer::create(Sprite::create("GamePlay/Gameplay_TimeFull.png"));
    _timerBar->retain();
    _timerBar->setPosition(timerBarBg->getPosition());
    _timerBar->setReverseDirection(true);
    _timerBar->setPercentage(100.f);
    _rootNode->addChild(_timerBar, timerBarBg->getZOrder()+1);
    
    auto timerListener_1 = EventListenerCustom::create("PutTimerEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustPutTimerEvent, this));
    auto timerListener_2 = EventListenerCustom::create("GetTimerEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustGetTimerEvent, this));
    
    _eventDispatcher->addEventListenerWithFixedPriority(timerListener_1, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(timerListener_2, 1);

	_eventDispatcher->addCustomEventListener("PauseTimerEvent", [this](Event*){ 
		_isTimerPause = true;
		_isFeverPause = true;
	});

	_eventDispatcher->addCustomEventListener("ResumeTimerEvent", [this](Event*){
		_isTimerPause = false;
		_isFeverPause = false;
	});
    
    // coins text
    
    auto coinsListener_1 = EventListenerCustom::create("PutCoinsEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustPutCoinsEvent, this));
    auto coinsListener_2 = EventListenerCustom::create("GetCoinsEvent",
                                                       CC_CALLBACK_1(GamePlayScene::adjustGetCoinsEvent, this));
    
    _eventDispatcher->addEventListenerWithFixedPriority(coinsListener_1, 1);
    _eventDispatcher->addEventListenerWithFixedPriority(coinsListener_2, 1);
    
    return bRet;
}

void GamePlayScene::adjustFeverIncreaseEvent(EventCustom *event)
{
    if (_feverStep + 1 == FEVER_MAX)
    {
		_feverStep = 0;

        _feverBar->setPercentage(100.f);

        _blocksManager->setFeverModeEnabled(true);

		schedule( schedule_selector(GamePlayScene::updateForFeverMode) );
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
    
//    char szScore[10];
//    snprintf(szScore, 10, "%d", instance->getScore());
//    _scoreText->setString(szScore);
    auto scoreText = dynamic_cast<NumberCountingLabel*>(_rootNode->getChildByName("TxtScore"));
    scoreText->setGoalNumber( instance->getScore());
}

void GamePlayScene::adjustPutScoreEvent(EventCustom *event)
{
    char * buf = static_cast<char*>(event->getUserData());
    
    int scoreDelta = 0;
    int scoreColor = 0;
    float wPosX = 0.f;
    float wPosY = 0.f;
    
    sscanf(buf, "%d,%d,%f,%f", &scoreDelta, &scoreColor, &wPosX, &wPosY);
    
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

void GamePlayScene::adjustPutCoinsEvent(EventCustom* event)
{
    char* szData = static_cast<char*>(event->getUserData());
    
    float wPosX = 0.f;
    float wPosY = 0.f;
    sscanf(szData, "%f,%f", &wPosX, &wPosY);
    
    auto sprite = Sprite::createWithSpriteFrameName("CoinAnimation_000");
    _rootNode->addChild(sprite);
    
    auto CallFuncOfActionFinish = CallFunc::create([=](void){
        
        EventCustom event("GetCoinsEvent");
        _eventDispatcher->dispatchEvent(&event);
    });
    
    Vec2 beginPos = _rootNode->convertToNodeSpace(Vec2(wPosX, wPosY));
    
    auto coinsText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtCoins"));
    Vec2 endPos = coinsText->getPosition() - (coinsText->getContentSize()*0.5f);
    
    sprite->setPosition(beginPos);
    sprite->runAction( Sequence::create(ScaleTo::create(0.1f, 0.5f),
                                        MoveTo::create(beginPos.getDistance(endPos)/1500.f, endPos),
                                        RemoveSelf::create(),
                                        CallFuncOfActionFinish,
                                        nullptr));
}

void GamePlayScene::adjustGetCoinsEvent(EventCustom* event)
{
    GameData* instance =  GameData::getInstance();
    instance->increaseCoins(1);
    
    char szCoins[10];
    snprintf(szCoins, 10, "%d", instance->getCoins());
    
    auto coinsText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtCoins"));
    coinsText->setString(szCoins);
}

void GamePlayScene::adjustPutTimerEvent(EventCustom* event)
{
     ++_activeTimerCount;
    
    char* szData = static_cast<char*>(event->getUserData());
    
    float wPosX = 0.f;
    float wPosY = 0.f;
    sscanf(szData, "%f,%f", &wPosX, &wPosY);
    
    auto sprite = Sprite::create("GameBlock/Tool_time0.png");
    _rootNode->addChild(sprite);
    
    auto CallFuncOfActionFinish = CallFunc::create([=](void){
        
        EventCustom event("GetTimerEvent");
        _eventDispatcher->dispatchEvent(&event);
    });
    
    Vec2 beginPos = _rootNode->convertToNodeSpace(Vec2(wPosX, wPosY));
    
    auto coinsText = dynamic_cast<ui::Text*>(_rootNode->getChildByName("TxtTimer"));
    Vec2 endPos = coinsText->getPosition();
    
    sprite->setPosition(beginPos);
    sprite->runAction( Sequence::create(ScaleTo::create(0.1f, 0.5f),
                                        MoveTo::create(beginPos.getDistance(endPos)/1500.f, endPos),
                                        RemoveSelf::create(),
                                        CallFuncOfActionFinish,
                                        nullptr));
}

void GamePlayScene::adjustGetTimerEvent(EventCustom* event)
{
    --_activeTimerCount;
    
    _playTimer += 5.f;
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
        
        if (_activeTimerCount == 0)
        {
			_playTimer = PLAY_TIME;

 //           unschedule( schedule_selector(GamePlayScene::updateForPlayTimer));
        }
        else
        {
        }
    }
    else
    {
        _playTimer -= dt;
    }
    
    _timerBar->setPercentage((_playTimer/60.f)*100.f);
    
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
		_feverTimer = FEVER_DURATION;
		
        _feverBar->setPercentage(0.f);

        _blocksManager->setFeverModeEnabled(false);

		unschedule( schedule_selector(GamePlayScene::updateForFeverMode) );
	}
	else
	{
		_feverTimer -= dt;

		_feverBar->setPercentage(100.f * _feverTimer / FEVER_DURATION);
	}
}

GamePlayScene::~GamePlayScene(void)
{
    CC_SAFE_RELEASE(_rootNode);
    CC_SAFE_RELEASE(_blocksManager);
    CC_SAFE_RELEASE(_feverBar);
    CC_SAFE_RELEASE(_timerBar);
    CC_SAFE_RELEASE(_timerText);
}






