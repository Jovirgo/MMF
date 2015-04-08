#ifndef __NewFlipop_ReadyScene__
#define __NewFlipop_ReadyScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/Cocostudio.h"
#include "GamePlayScene.h"

class ReadyScene: public cocos2d::Scene
{
public:
	static ReadyScene* create(const GamePlayInfo& levelInfo);

	virtual bool init(const GamePlayInfo& levelInfo);
	void initCocostudioRootNode();
	void initMenu();
	void initTopBar();
	void initGamePlayInfo(const GamePlayInfo& levelInfo);

	virtual void onEnter();
	virtual void onExit();

private:
	void menuItemCallback(cocos2d::Ref* sender);

private:
	void adjustIncreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseCoinsEvent(cocos2d::EventCustom* event);
	void adjustIncreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustDecreaseDiamondsEvent(cocos2d::EventCustom* event);
	void adjustIncreasePowersEvent(cocos2d::EventCustom* event);
	void adjustDecreasePowersEvent(cocos2d::EventCustom* event);

	void updateForTimerText(float dt);

	cocos2d::ui::TextBMFont*	_timerText;
	cocos2d::ui::TextBMFont*	_coinsText;
	cocos2d::ui::TextBMFont*	_diamondsText;
	cocos2d::ui::TextBMFont*	_powersText;
	
private:
	cocos2d::Node* _rootNode;

	GamePlayInfo	_gamePlayInfo;

	ReadyScene();
	virtual ~ReadyScene();
};

#endif