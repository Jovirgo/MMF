//
//  TitleScene.h
//  NewFlipop
//
//  Created by 欧洲 on 15/1/25.
//
//

#ifndef __NewFlipop__TitleScene__
#define __NewFlipop__TitleScene__

#include "cocos2d.h"

class TitleScene: public cocos2d::Scene
{
public:
    virtual bool init(void);
    
    CREATE_FUNC(TitleScene);
    
    void menuCallback(cocos2d::Ref* sender);
};

#endif /* defined(__NewFlipop__TitleScene__) */
