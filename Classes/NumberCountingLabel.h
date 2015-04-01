//
//  NumberCountingLabel.h
//  NewFlipop
//
//  Created by 欧洲 on 15/3/9.
//
//

#ifndef __NewFlipop__NumberCountingLabel__
#define __NewFlipop__NumberCountingLabel__

#include "cocos2d.h"

class NumberCountingLabel: public cocos2d::LabelBMFont
{
public:
    CREATE_FUNC(NumberCountingLabel);
    
    virtual bool init();
    
    virtual void update(float dt);
    
protected:
    CC_SYNTHESIZE(bool, _isIncreased, IsIncreased);
    
    CC_PROPERTY(float, _countingDuration, CountingDuration);
    
    CC_PROPERTY(unsigned long, _currNumber, CurrNumber);
    CC_PROPERTY(unsigned long, _goalNumber, GoalNumber);
    
protected:
    void updateCountingDelta();
    
    int _delta;
    
    NumberCountingLabel()
    : _isIncreased(true)
    , _countingDuration(0.f)
    , _delta(0)
    , _currNumber(0)
    , _goalNumber(0)
    {}
};

#endif /* defined(__NewFlipop__NumberCountingLabel__) */
