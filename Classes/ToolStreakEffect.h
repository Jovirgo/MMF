//
//  ToolStreakEffect.h
//  NewFlipop
//
//  Created by 欧洲 on 15/1/10.
//
//

#ifndef __NewFlipop__ToolStreakEffect__
#define __NewFlipop__ToolStreakEffect__

#include "cocos2d.h"

class ToolStreak: public cocos2d::MotionStreak
{
public:
    CREATE_FUNC(ToolStreak);
    
    virtual bool init(void);
    
    virtual void onEnter(void);
    virtual void onExit(void);
    
protected:
    void updateForMove(float dt);
    
    /* owner */
    CC_SYNTHESIZE_RETAIN(cocos2d::Node *, m_obOwner, Owner);
    
protected:
    /* type */
    CC_PROPERTY(int, m_nStreakType, StreakType);
    
    ToolStreak(void);
    virtual ~ToolStreak(void);
};

#endif /* defined(__NewFlipop__ToolStreakEffect__) */
