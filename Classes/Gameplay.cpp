#include "Gameplay.h"
#include <iostream>
#define PI 3.14159265
USING_NS_CC;
using namespace std;
int score;
int speed;
int x, y;
int width, height;
int moveP;
int ballAngle;

Sprite* ballSprite;
Sprite* wallSprite1;
Sprite* platformSprite;
bool leftKey, rightKey;


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

bool Gameplay::init()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	width = visibleSize.width;
	height = visibleSize.height;
	x = width / 2 + origin.x;
	y = height / 2 + origin.y;

	ballSprite = Sprite::create("balllevel1.png");
	wallSprite1 = Sprite::create("walllevel1.png");
	platformSprite = Sprite::create("platform.png");

	ballSched = schedule_selector(Gameplay::update);
	
	score = 0;
	speed = 0.1;
	
	ballSprite->setPosition(x, y - 100);
	this->addChild(ballSprite);
	
	wallSprite1->setPosition(x, y + 40);
	this->addChild(wallSprite1);
	
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
	ballAngle = 45;
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, platformSprite);
	this->schedule(ballSched, speed);
	
	return true;
}

void Gameplay::update(float delta)
{
	int ballX, ballY, platformX, platformY;
	ballX = ballSprite->getPosition().x;
	ballY = ballSprite->getPosition().y;
	platformX = platformSprite->getPositionX();
	platformY = platformSprite->getPositionY();

	

	if (ballX == width) {
		ballAngle += 90;
	}
	if (ballX == 0) {
		ballAngle -= 90;
	}

	if (ballY == height) {
		ballAngle = -(ballAngle);
	}

	if (ballY == platformY && ballX >= platformX - 74 && ballX <= platformX + 74) {
		ballAngle = 30;
	}
	ballX = ballX + 2 * cos(ballAngle*PI / 180);
	ballY = ballY + 2 * sin(ballAngle*PI / 180);
	ballSprite->setPosition(ballX, ballY);
	ballSprite->setRotation(ballSprite->getRotation() + 5);

	if (leftKey == TRUE && platformX > 74) {
		platformSprite->setPosition((platformX- 5), platformY);
	}
	if (rightKey == TRUE && platformX < width - 74) {
		platformSprite->setPosition((platformX + 5), platformY);
	}

	this->schedule(ballSched, speed);
	
}