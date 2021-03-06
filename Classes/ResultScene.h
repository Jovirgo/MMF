#ifndef __NewFlipop_Result__
#define __NewFlipop_Result__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/Cocostudio.h"
#include "GamePlayScene.h"

class ResultScene: public cocos2d::Scene
{
public:
	static ResultScene* create(const GamePlayInfo& levelInfo);
    
    virtual bool init(const GamePlayInfo& levelInfo);
	void initMenu();
	void initTopBar();
	void initGamePlayInfo(const GamePlayInfo& levelInfo);

	virtual void onEnter();
	virtual void onExit();

private:
	void menuItemCallback(cocos2d::Ref* sender);
	void back();
	void replay();
	void next();

private:
	void playStarAnimation1();
	void playStarAnimation2();
	void playStarAnimation3();

	void StarFrameEventCallFunc(cocostudio::timeline::Frame* frame);

	cocos2d::Node* _rootNode;

	GamePlayInfo	_gamePlayInfo;

private:
	void adjustIncreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustIncreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustIncreasePowersEvent(cocos2d::EventCustom* event);
	void adjustDecreasePowersEvent(cocos2d::EventCustom* event);

	void updateForTimerText(float dt);

	cocos2d::ui::TextBMFont*	_coinsText;
	cocos2d::ui::TextBMFont*	_diamondsText;
	cocos2d::ui::TextBMFont*	_powersText;
	cocos2d::ui::TextBMFont*	_timerText;

	ResultScene();
	virtual ~ResultScene();
};

#endif