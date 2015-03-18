//
//  SettingsScene.h
//  NewFlipop
//
//  Created by 欧洲 on 15/2/16.
//
//

#ifndef __NewFlipop__SettingsScene__
#define __NewFlipop__SettingsScene__

#include "cocos2d.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

class SettingsScene: public cocos2d::Scene
{
public:
    CREATE_FUNC(SettingsScene);
    
    virtual bool init(void);
    
private:
    void menuCallback(cocos2d::Ref* sender);
    void soundItemCallback(cocos2d::Ref* sender);
    void musicItemCallback(cocos2d::Ref* sender);
};

#endif /* defined(__NewFlipop__SettingsScene__) */
