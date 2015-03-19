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

class GamePlayScene: public cocos2d::Scene
{
public:
    CREATE_FUNC(GamePlayScene);
    
    virtual bool init();
    
private:
	// 挂载 Cocostudio 的节点
    cocos2d::Node* _rootNode;
    
private:
	void initMenu();

	// 增加一个 fever 值，如果 fever 达到峰值，则启用 fever 模式。
    void adjustFeverIncreaseEvent(cocos2d::EventCustom* event);
	// 减少一个 fever 值，直到 fever 为 0。
    void adjustFeverDecreaseEvent(cocos2d::EventCustom* event);
    // 放飞分数
    void adjustPutScoreEvent(cocos2d::EventCustom* event);
	// 得到分数
    void adjustGetScoreEvent(cocos2d::EventCustom* event);
    
	// 更新游戏时间
    void updateForPlayTimer(float dt);
	// 更新疯狂模式的时间
	void updateForFeverMode(float dt);

	// 启动和关闭疯狂模式
	void enableFeverMode();
	void disableFeverMode();
    
	// 消除块的管理者
    GameBlocksManager* _blocksManager;

    // fever 值
    int     _feverStep;
	// fever 模式的计时器
    float   _feverTimer;
	// fever 模式的进度条
    cocos2d::ProgressTimer* _feverBar;
	// 顾名思义
	bool _isFeverPause;
    
    // 游戏时间的计时器
    float _playTimer;
	// 游戏时间的进度条
    cocos2d::ProgressTimer* _timerBar;
	// 游戏时间的显示标签
    cocos2d::ui::Text*      _timerText;
	// 顾名思义
	bool _isTimerPause;

	CC_PROPERTY(bool, _isTouchEnabled, TouchEnabled);
    
private:
    GamePlayScene()
    : _rootNode(nullptr)
    , _blocksManager(nullptr)
    , _feverStep(0)
    , _feverTimer(FEVER_DURATION)
    , _feverBar(nullptr)
	, _isFeverPause(true)
    , _playTimer(PLAY_TIME)
    , _timerBar(nullptr)
    , _timerText(nullptr)
	, _isTimerPause(true)
	, _isTouchEnabled(false)
    {}
    
    virtual ~GamePlayScene(void);
};

#endif /* defined(__NewFlipop__GamePlayScene__) */
