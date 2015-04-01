//
//  GameLevelMapScene.h
//  NewFlipop
//
//  Created by 欧洲 on 15/3/21.
//
//

#ifndef __NewFlipop__GameLevelMapScene__
#define __NewFlipop__GameLevelMapScene__

#include "cocos2d.h"
#include "cocostudio/Cocostudio.h"
#include "ui/CocosGUI.h"
#include "GameLevelInfoData.h"

class GameLevelMapScene: public cocos2d::Scene
{
public:
    CREATE_FUNC(GameLevelMapScene);
    
    virtual bool init();

	virtual void onEnter();
	virtual void onExit();
    
	// add plate to the top of map
    void pushPlateToMapWithSize(cocos2d::Node* plate, const cocos2d::Size& plateSize);

private:
	// level item
    void levelItemCallback(
		cocos2d::Ref* sender, 
		cocos2d::ui::Widget::TouchEventType type);

	void goToGamePlaySceneWithLevelId(int levelId);
    
    // cocostuidio
    cocos2d::Node*	_rootNode;
	cocos2d::Vector<cocos2d::Node*>	_levelBtns;
    
	// 
	cocos2d::Vec2	_touchBeganPos;

    // total level
    int     _levelCount;
    
    // Map height
    float   _innerHeight;


private:
    GameLevelInfoData _levelInfoTable;

	std::map<int, int> _levelScoreMap;

private:
    GameLevelMapScene()
    : _rootNode(nullptr)
    , _levelCount(0)
    , _innerHeight(0.f)
    {
	}
    
    virtual ~GameLevelMapScene();
};

#endif /* defined(__NewFlipop__GameLevelMapScene__) */