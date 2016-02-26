#include "Gameplay.h"
#include <iostream>
#include <vector>
#define PI 3.14159265
#define COCOS2D_DEBUG 1
USING_NS_CC;
using namespace std;

//Scene ELements
int x, y;
int width, height;

//Game Elements
int score;
int speed;
int ballAngle;
int quad;
int ballSpeed;
cocos2d::Size bodySize;
std::vector<Sprite*> wallSpritesLeft;
std::vector<Sprite*> wallSpritesRight;
std::vector<Sprite*> wallSpritesTop;
std::vector<Sprite*> bricks;
//Sprites
Sprite* ballSprite;
Sprite* wallSprite1;
Sprite* platformSprite;

//Movement
bool leftKey, rightKey;
bool isHit;
bool isHit2;

Scene* Gameplay::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Gameplay::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

std::wstring Gameplay::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

bool Gameplay::init()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	width = visibleSize.width;
	height = visibleSize.height;
	x = width / 2 + origin.x;
	y = height / 2 + origin.y;
	
	for (int x = 0; x <= 10; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(0, (x + 1) *100);
		this->addChild(wall);
		wallSpritesLeft.push_back(wall);
	}

	for (int x = 0; x <= 10; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(visibleSize.width , (x + 1) * 100);
		this->addChild(wall);
		wallSpritesRight.push_back(wall);
	}

	for (int x = 0; x <= 10; x++) {
		Sprite* wall = Sprite::create("wall2.png");
		wall->setPosition((x + 1) * 100, visibleSize.height);
		this->addChild(wall);
		wallSpritesTop.push_back(wall);
	}
	for (int x = 0; x <= 6; x++) {
		Sprite* wall = Sprite::create("walllevel1.png");
		wall->setPosition(x * 100 + 100, visibleSize.height / 2 + 200);
		this->addChild(wall);
		bricks.push_back(wall);
	}

	for (int x = 0; x <= 4; x++) {
		Sprite* wall = Sprite::create("walllevel1.png");
		wall->setPosition(x * 100 + 200, visibleSize.height / 2 + 150);
		this->addChild(wall);
		bricks.push_back(wall);
	}
	for (int x = 0; x <= 2; x++) {
		Sprite* wall = Sprite::create("walllevel1.png");
		wall->setPosition(x * 100 + 300, visibleSize.height / 2 + 100);
		this->addChild(wall);
		bricks.push_back(wall);
	}
	Sprite* wall = Sprite::create("walllevel1.png");
	wall->setPosition(400, visibleSize.height / 2 + 50);
	this->addChild(wall);
	bricks.push_back(wall);

	
	ballSprite = Sprite::create("balllevel1.png");
	platformSprite = Sprite::create("platform.png");

	ballSched = schedule_selector(Gameplay::update);
	
	score = 0;
	speed = 0.1;
	
	ballSprite->setPosition(x, y - 300);
	this->addChild(ballSprite);
	
	platformSprite->setPosition(x, y - 270);
	this->addChild(platformSprite);

	leftKey = FALSE;
	rightKey = FALSE;

	auto eventListener = EventListenerKeyboard::create();



	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftKey = TRUE;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightKey = TRUE;
			break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftKey = FALSE;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightKey = FALSE;
			break;
		}
	};

	//initial vars
	ballAngle = 60;
	quad = 90;
	ballSpeed = 6;
	bodySize = bricks.at(0)->getContentSize();
	isHit = FALSE;
	isHit2 = FALSE;
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, platformSprite);
	this->schedule(ballSched, speed);
	
	return true;
}

void Gameplay::update(float delta)
{
	//initializiations
	int ballX, ballY, platformX, platformY;
	ballX = ballSprite->getPosition().x;
	ballY = ballSprite->getPosition().y;
	platformX = platformSprite->getPositionX();
	platformY = platformSprite->getPositionY();
	
	//QUADS
	//0 = Up Right
	//90 = Up Left
	//180 = Down Left
	//270 = Down Right

	//bumangga sa right wall
	for (int x = 0; x < wallSpritesRight.size(); x++) {
		
		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesRight.at(x)->getBoundingBox())) {
			switch (quad) {
			case 0: quad = 90;
				break;
			case 270: quad = 180;
				break;
			}
		}
	}

	//bumangga sa left wall
	for (int x = 0; x < wallSpritesLeft.size(); x++) {
		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesLeft.at(x)->getBoundingBox())) {
			switch (quad) {
			case 90: quad = 0;
				break;
			case 180: quad = 270;
				break;
			}
		}
	}

	//bumangga sa top wall
	for (int x = 0; x < wallSpritesTop.size(); x++) {
		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesTop.at(x)->getBoundingBox())) {
			switch (quad) {
			case 0: quad = 270;
				break;
			case 90: quad = 180;
				break;
			}
		}
	}

	//bumangga sa platform
	int platformWidth = platformSprite->getContentSize().width / 5;
	if (isHit == FALSE) {
		if (ballSprite->getBoundingBox().intersectsRect(platformSprite->getBoundingBox())) {
			//first hati ng platform
			isHit = TRUE;
			if (ballX >= 0 && ballX < platformX - 74 + platformWidth * 1) {
				quad = 90;
				ballAngle = ballAngle / 4;
			}
			else if (ballX >= platformX - 74 + platformWidth * 1 && ballX < platformX - 74 + platformWidth * 2) {
				quad = 90;
				ballAngle = ballAngle / 2;
			}
			else if (ballX >= platformX - 74 + platformWidth * 2 && ballX < platformX - 74 + platformWidth * 3) {
				switch (quad) {
				case 180: quad = 90;
					break;
				case 270: quad = 0;
					break;
				}
			}
			else if (ballX >= platformX - 74 + platformWidth * 3 && ballX < platformX - 74 + platformWidth * 4) {
				quad = 0;
				ballAngle = ballAngle / 2;
			}
			else if (ballX >= platformX - 74 + platformWidth * 4 && ballX <= platformX - 74 + platformWidth * 5) {
				quad = 0;
				ballAngle = ballAngle / 4;
			}
		}
		

		}
	if (!(ballSprite->getBoundingBox().intersectsRect(platformSprite->getBoundingBox()))) {
		isHit = FALSE;
	}
	
	//bumangga sa wall
	for (int x = 0; x < bricks.size(); x++) {
		if (isHit2 == FALSE) {
			if (ballSprite->getBoundingBox().intersectsRect(bricks.at(x)->getBoundingBox())) {
				isHit2 = TRUE;
				if (ballY >= bricks.at(x)->getPositionY() + bodySize.height / 2) {
					switch (quad) {
					case 270: quad = 0;
						break;
					case 180: quad = 90;
						break;
					}
				}
				else if (ballY <= bricks.at(x)->getPositionY() - bodySize.height / 2) {
					switch (quad) {
					case 0: quad = 270;
						break;
					case 90: quad = 180;
						break;
					}
				}
				else if (ballX <= bricks.at(x)->getPositionX() - bodySize.width / 2) {
					switch (quad) {
					case 0: quad = 90;
						break;
					case 270: quad = 180;
						break;
					}
				}
				else if (ballX >= bricks.at(x)->getPositionX() + bodySize.width / 2) {
					switch (quad) {
					case 90: quad = 0;
						break;
					case 180: quad = 270;
						break;
					}
				}
				this->removeChild(bricks.at(x));
				bricks.erase(bricks.begin() + x);
			}
		}

		isHit2 = FALSE;
	}

	//angle of reflection
	ballAngle = 180 - ballAngle - 90;
	//pagcompute sa paggalaw
	ballX = ballX + ballSpeed * cos((ballAngle + quad) * PI / 180);
	ballY = ballY + ballSpeed * sin((ballAngle + quad) * PI / 180);
	ballSprite->setPosition(ballX, ballY);


	//etc movements
	ballSprite->setRotation(ballSprite->getRotation() + 5);

	if (leftKey == TRUE && platformX > 74) {
		platformSprite->setPosition((platformX - 5), platformY);
	}
	if (rightKey == TRUE && platformX < width - 74) {
		platformSprite->setPosition((platformX + 5), platformY);
	}

	this->schedule(ballSched, speed);
}