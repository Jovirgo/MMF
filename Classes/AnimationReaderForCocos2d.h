//
//  AnimationReaderForCocos2d.h
//
//
//  Created by OJO on 15/3/13.
//
//

#ifndef __OJO__AnimationReaderForCocos2d__
#define __OJO__AnimationReaderForCocos2d__

#include "cocos2d.h"

class AnimationReaderForCocos2d
{
public:
    static cocos2d::Animation* readAnimationFromPlist(const char* szPlistName,
                                                      float delayPerUint = 0.02f,
                                                      int   loops = 1,
                                                      bool  isRestoreOriginalFrame = true);
};

#endif /* defined(__OJO__AnimationReaderForCocos2d__) */
