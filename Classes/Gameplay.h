#pragma once
#include "cocos2d.h"
using namespace std;
class Gameplay :  public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Gameplay);
	// implement the "static create()" method manually

	void update(float) override;
private:
	cocos2d::SEL_SCHEDULE ballSched;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

};

