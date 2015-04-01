//
//  NumberCountingLabel.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/3/9.
//
//

#include "NumberCountingLabel.h"

USING_NS_CC;

bool NumberCountingLabel::init()
{
    bool bRet = LabelBMFont::initWithString("0", "fonts/Score.fnt");
    
//    setSystemFontName("Arial");
//    setSystemFontSize(32);
    
    scheduleUpdate();
    
    return bRet;
}

void NumberCountingLabel::update(float dt)
{
    if (_currNumber == _goalNumber)
    {
        return;
    }
    
    unsigned long delta = _delta * dt;
    
    if (_isIncreased)
    {
        if (_currNumber + delta < _goalNumber)
        {
            setCurrNumber(_currNumber + delta);
            return;
        }
    }
    else
    {
        if (_currNumber - delta > _goalNumber)
        {
            setCurrNumber(_currNumber - delta);
            return;
        }
    }
    
    setCurrNumber(_goalNumber);
}

void NumberCountingLabel::setCountingDuration(float var)
{
    if (var < 0.001f)
    {
        _countingDuration = 0.f;
        return;
    }
    else
    {
        _countingDuration = var;
    }
    
    updateCountingDelta();
}

float NumberCountingLabel::getCountingDuration()
{
    return _countingDuration;
}

void NumberCountingLabel::setCurrNumber(unsigned long var)
{
    if (_currNumber == var)
    {
        return;
    }
    else
    {
        _currNumber = var;
    }
    
    char szString[15];
    snprintf(szString, 15, "%lu", _currNumber);
    setString(szString);
    
    if ((_goalNumber > _currNumber) != _isIncreased)
    {
        _goalNumber = _currNumber;
    }
}

unsigned long NumberCountingLabel::getCurrNumber()
{
    return _currNumber;
}

void NumberCountingLabel::setGoalNumber(unsigned long var)
{
    _goalNumber = var;
    
    if ((_goalNumber > _currNumber) != _isIncreased)
    {
        _currNumber = _goalNumber;
    }
    
    updateCountingDelta();
}

unsigned long NumberCountingLabel::getGoalNumber()
{
    return _goalNumber;
}

void NumberCountingLabel::updateCountingDelta()
{
    if (_countingDuration < 0.001f)
    {
        _delta = 0;
        return;
    }
    
    if (_goalNumber > _currNumber)
    {
        _delta = (_goalNumber-_currNumber)/_countingDuration;
    }
    else
    {
        _delta = (_currNumber-_goalNumber)/_countingDuration;
    }
}






