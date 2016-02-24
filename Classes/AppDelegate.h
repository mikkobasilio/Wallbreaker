#pragma once

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
public:
	AppDelegate();
	virtual ~AppDelegate();
	std::wstring s2ws(const std::string& s);
	virtual bool applicationDidFinishLaunching();
	virtual void applicationDidEnterBackground();
	virtual void applicationWillEnterForeground();
};