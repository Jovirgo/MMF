#include "AppDelegate.h"

#include "GamePlayScene.h"
#include "TitleScene.h"
#include "GameLevelMapScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameSize(384, 512);
#endif
    
    // set design resolution size auto adapt to 960x640 or 640x960
    Size dvSize = glview->getFrameSize();
    Size dfSize = dvSize.width > dvSize.height ? Size(1080, 720) : Size(720, 1080);
    
    float dfcmp = dfSize.width / dfSize.height; //width / height
    float dvcmp = dvSize.width / dvSize.height;
    
    float rate = 0;
    
    /*if (dvcmp > dfcmp)
    {
        rate = dfSize.height / dvSize.height;
    }
    else*/
    {
        rate = dfSize.width / dvSize.width;
    }
    
    CCLOG("rate = %f", rate);
    
    glview->setDesignResolutionSize(dvSize.width * rate, dvSize.height * rate, ResolutionPolicy::NO_BORDER);
//    glview->setDesignResolutionSize(dfSize.width, dfSize.height, ResolutionPolicy::SHOW_ALL);
	// set design resolution size end.
    
    // create a scene. it's an autorelease object
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
