#include "Animal.h"

Animal* Animal::create(AimalType type)
{
	Animal* ani = new Animal();
	if(ani && ani->initBody(type))
	{
		ani->autorelease();
		return ani;
	}
	CC_SAFE_DELETE(ani);
	return NULL;
}
bool Animal::initBody(AimalType type)
{
	body = BasicEle::create();
	body->setContentSize(CCSizeMake(150,150));
	this->addChild(body);
	switch (type)
	{
	case Animal::FLYS:
		initSwings();
		break;
	case Animal::BEATS:
		initLimps();
		break;
	default:
		break;
	}
	return true;
}
bool Animal::initLimps()
{
	float bwidth = body->getContentSize().width,bheight = body->getContentSize().height;

	leftarm = BasicEle::create();
	//leftarm->setAnchorPoint(ccp(1,1));
	leftarm->setPosition(ccp(0,bheight));
	leftarm->setColor(ccc3(255,0,0));
	body->addChild(leftarm);

	rightarm = BasicEle::create();
	//rightarm->setAnchorPoint(ccp(0,1));
	rightarm->setPosition(ccp(bwidth,bheight));
	rightarm->setColor(ccc3(0,255,0));
	body->addChild(rightarm);

	leftleg = BasicEle::create();
	//leftleg->setAnchorPoint(ccp(1,0));
	leftleg->setPosition(ccp(0,0));
	leftleg->setColor(ccc3(0,0,255));
	body->addChild(leftleg);

	rightleg = BasicEle::create();
	//rightleg->setAnchorPoint(ccp(0,0));
	rightleg->setPosition(ccp(bwidth,0));
	rightleg->setColor(ccc3(255,255,0));
	body->addChild(rightleg);

	return true;
}
bool Animal::initSwings()
{
	return false;
}
void Animal::run()
{
}
void Animal::jump()
{
}
