//
//  ToolStreakEffect.cpp
//  NewFlipop
//
//  Created by 欧洲 on 15/1/10.
//
//

#include "ToolStreakEffect.h"
#include "GameBlock.h"

USING_NS_CC;

bool ToolStreak::init(void)
{
    if (MotionStreak::initWithFade(0.16f, 5.f, 25.f, Color3B::WHITE, "GameBlock/Wild_effect.png"))
    {
        BlendFunc  blendFunc = {GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA};
        
        setBlendFunc(blendFunc);
        
        return true;
    }
    
    return false;
}

void ToolStreak::onEnter(void)
{
    CCMotionStreak::onEnter();
    
    schedule(schedule_selector(ToolStreak::updateForMove));
}

void ToolStreak::onExit(void)
{
    unschedule(schedule_selector(ToolStreak::updateForMove));
    
    setOwner(NULL);
    
    CCMotionStreak::onExit();
}

void ToolStreak::updateForMove(float dt)
{
    if (m_obOwner != NULL)
    {
        setPosition(m_obOwner->getPosition());
    }
}

void ToolStreak::setStreakType(int var)
{
    if (m_nStreakType == var)
    {
        return;
    }
    else
    {
        m_nStreakType = var;
    }
    
    switch (m_nStreakType) {
        case TOOL_TIME:
            setTexture(TextureCache::getInstance()->addImage("GameBlock/Timer_effect.png"));
            break;
        case TOOL_MULT:
            setTexture(TextureCache::getInstance()->addImage("GameBlock/Multiply_effect.png"));
            break;
        case TOOL_COIN:
            setTexture(TextureCache::getInstance()->addImage("GameBlock/Coin_effect.png"));
            break;
        case CARD_WILD:
            setTexture(TextureCache::getInstance()->addImage("GameBlock/Wild_effect.png"));
            break;
        default:
            break;
    }
}

int  ToolStreak::getStreakType(void)
{
    return m_nStreakType;
}

ToolStreak::ToolStreak(void)
: m_obOwner(NULL)
, m_nStreakType(CARD_WILD)
{}

ToolStreak::~ToolStreak(void)
{
}
