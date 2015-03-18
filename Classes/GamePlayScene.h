//
//  GamePlayScene.h
//  NewFlipop
//
//  Created by 欧洲 on 14/12/31.
//
//

#ifndef __NewFlipop__GamePlayScene__
#define __NewFlipop__GamePlayScene__

#include "cocos2d.h"
#include "GameBlocksManager.h"
#include "ui/CocosGUI.h"
#include "NumberCountingLabel.h"

class GamePlayScene: public cocos2d::Scene
{
public:
    CREATE_FUNC(GamePlayScene);
    
    virtual bool init();
    
    cocos2d::Node* _rootNode;
    
    GameBlocksManager* _blocksManager;
    
private:
    void adjustFeverIncreaseEvent(cocos2d::EventCustom* event);
    void adjustFeverDecreaseEvent(cocos2d::EventCustom* event);
    
    void adjustPutScoreEvent(cocos2d::EventCustom* event);
    void adjustGetScoreEvent(cocos2d::EventCustom* event);
    
    void adjustPutCoinsEvent(cocos2d::EventCustom* event);
    void adjustGetCoinsEvent(cocos2d::EventCustom* event);
    
    void adjustPutTimerEvent(cocos2d::EventCustom* event);
    void adjustGetTimerEvent(cocos2d::EventCustom* event);
    
    void updateForPlayTimer(float dt);
	void updateForFeverMode(float dt);
    
    // fever mode
    int     _feverStep;
    float   _feverTimer;
    cocos2d::ProgressTimer* _feverBar;
	bool _isFeverPause;
    
    // play time
    int   _activeTimerCount;
    float _playTimer;
    cocos2d::ProgressTimer* _timerBar;
    cocos2d::ui::Text*      _timerText;
	bool _isTimerPause;
    
private:
    GamePlayScene()
    : _rootNode(nullptr)
    , _blocksManager(nullptr)
    , _feverStep(0)
    , _feverTimer(FEVER_DURATION)
    , _feverBar(nullptr)
	, _isFeverPause(true)
    , _activeTimerCount(0)
    , _playTimer(PLAY_TIME)
    , _timerBar(nullptr)
    , _timerText(nullptr)
	, _isTimerPause(true)
    {}
    
    virtual ~GamePlayScene(void);
};

#endif /* defined(__NewFlipop__GamePlayScene__) */
