//
//  GamePlayScene.h
//  NewFlipop
//
//  Created by OJO on 14/12/31.
//
//

#ifndef __NewFlipop__GamePlayScene__
#define __NewFlipop__GamePlayScene__

#include "cocos2d.h"
#include "GameBlocksManager.h"
#include "ui/CocosGUI.h"
#include "NumberCountingLabel.h"



struct GamePlayInfo
{
	// read data from LevelInfo.csv
	int	levelId;
	int	rows;
	int cols;
	int time;

	// 1 mean have this color, 0 is No
	int color[5];

	int bonusCoin;

	// 3 differenc degree goal and bonus
	int scoreGoal[3];
	int bonusTool[3];
};

class GamePlayScene: public cocos2d::Scene
{
public:
	static GamePlayScene* create(const GamePlayInfo& levelInfo);
    
    virtual bool init(const GamePlayInfo& levelInfo);
    
	void initGamePlayInfo(const GamePlayInfo& levelInfo);
	void initCocostudioRootNode();
	void initMenu();

private:
    cocos2d::Node* _rootNode;
    
private:

    void adjustFeverIncreaseEvent(cocos2d::EventCustom* event);
    void adjustFeverDecreaseEvent(cocos2d::EventCustom* event);

    void adjustPutScoreEvent(cocos2d::EventCustom* event);
    void adjustGetScoreEvent(cocos2d::EventCustom* event);
    
    void updateForPlayTimer(float dt);
	void updateForFeverMode(float dt);
	void updateForScore(float dt);

	void enableFeverMode();
	void disableFeverMode();
    
    GameBlocksManager* _blocksManager;

    int     _feverStep;
    float   _feverTimer;
    cocos2d::ui::LoadingBar* _feverBar;
	bool _isFeverPause;
    
	float _playTimerMax;
    float _playTimer;
//    cocos2d::ProgressTimer* _timerBar;
    cocos2d::ui::TextBMFont*      _timerText;
	bool _isTimerPause;

	cocos2d::ui::LoadingBar* _scoreBar;
	NumberCountingLabel*	_scoreText;

	GamePlayInfo _gamePlayInfo;

	CC_PROPERTY(bool, _isTouchEnabled, TouchEnabled);
    
private:
    GamePlayScene()
    : _rootNode(nullptr)
    , _blocksManager(nullptr)
    , _feverStep(0)
    , _feverTimer(GAMEPLAY_FeverDuration)
    , _feverBar(nullptr)
	, _isFeverPause(true)
	, _playTimerMax(60.f)
    , _playTimer(_playTimerMax)
//    , _timerBar(nullptr)
    , _timerText(nullptr)
	, _isTimerPause(true)
	, _scoreBar(nullptr)
	, _scoreText(nullptr)
	, _isTouchEnabled(false)
    {}
    
    virtual ~GamePlayScene(void);
};

#endif /* defined(__NewFlipop__GamePlayScene__) */
