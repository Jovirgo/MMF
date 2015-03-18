//
//  AnimationReaderForCocos2d.cpp
//  TestProject
//
//  Created by 欧洲 on 15/3/13.
//
//

#include "AnimationReaderForCocos2d.h"
#include <iostream>

using namespace std;
USING_NS_CC;

Animation* AnimationReaderForCocos2d::readAnimationFromPlist(const char* szPlistName,
                                                             float delayPerUint,
                                                             int   loops,
                                                             bool  isRestoreOriginalFrame)
{
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(szPlistName);
    
    auto    fullPath = FileUtils::getInstance()->fullPathForFilename(szPlistName);
    size_t  extensionPos = fullPath.find(".plist");
    size_t  lastSlashPos = fullPath.find_last_of("/");
    
    CCASSERT(fullPath.npos != extensionPos && lastSlashPos < extensionPos, "Invalid filename");
    
    auto    datasMap = FileUtils::getInstance()->getValueMapFromFile(szPlistName);
    if (datasMap.find("frames") == datasMap.end())
    {
        CCLOG("AnimationReaderForCocos2d: No frames were found.");
        return nullptr;
    }
    
    auto    frameMap = datasMap.at("frames").asValueMap();

    auto    fileName = fullPath.substr(lastSlashPos+1, extensionPos-lastSlashPos-1);
    Vector<SpriteFrame*> frames;
    
    char*   szFrameName = new char[fileName.size() + 5];
    for (int i = 0; i != frameMap.size(); ++i)
    {   
		// frame name and file name must be same. append _00x
        sprintf(szFrameName, "%s_%03d", fileName.c_str(), i);
        
        frames.pushBack(frameCache->getSpriteFrameByName(szFrameName));
    }
    delete [] szFrameName;
    
    if (frames.size() == 0)
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(frames);
        animation->setDelayPerUnit(delayPerUint);
        animation->setLoops(loops);
        animation->setRestoreOriginalFrame(isRestoreOriginalFrame);
        return animation;
    }
}
