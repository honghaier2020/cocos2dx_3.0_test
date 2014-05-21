#include "HelloWorldScene.h"

#pragma warning( disable:4996)
#pragma warning( disable:4305)
USING_NS_CC;

#define  BUTTON_OK "OK"
#define  BUTTON_CANCEL "CANCEL"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
	if(1)
	{
		//	test code
		//testButton(visibleSize,origin);

		//testDragAndDrop(visibleSize);

		//testLayer();

		//testFlipbookAnimations();

		testLoadTitleMapFile();
	}
	else
	{
		/////////////////////////////
		// 2. add a menu item with "X" image, which is clicked to quit the program
		//    you may modify it.

		// add a "close" icon to exit the progress. it's an autorelease object
		auto closeItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

		closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
			origin.y + closeItem->getContentSize().height/2));

		// create menu, it's an autorelease object
		auto menu = Menu::create(closeItem, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 1);

		/////////////////////////////
		// 3. add your codes below...

		// add a label shows "Hello World"
		// create and initialize a label

		auto label = LabelTTF::create("Hello World", "Arial", 24);

		// position the label on the center of the screen
		label->setPosition(Point(origin.x + visibleSize.width/2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);

		// add "HelloWorld" splash screen"
		auto sprite = Sprite::create("HelloWorld.png");

		// position the sprite on the center of the screen
		sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::starMenuCallback(Object* pSender)
{
	if(BUTTON_OK == label_->getString())
	{
		label_->setString(BUTTON_CANCEL);
	}
	else{
		label_->setString(BUTTON_OK);
	}
	
}

void HelloWorld::plusMinusButtonCallback(Ref* sender)
{
	MenuItemToggle *toggleItem = (CCMenuItemToggle *)sender;
	if (toggleItem->selectedItem() == _plusItem){
		label_->setString("Visible button: +");
	}
	else if (toggleItem->selectedItem() == _minusItem){
		label_->setString("Visible button: -");
	}
}

void HelloWorld::testButton(cocos2d::Size& visibleSize,cocos2d::Point& origin)
{
	label_ = LabelTTF::create(BUTTON_OK, "Marker Felt", 32);
	label_->setPosition(Point(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height - 80));
	label_->setHorizontalAlignment(TextHAlignment::CENTER);
	this->addChild(label_);

	// Standard method to create a button
	auto starMenuItem = MenuItemImage::create(
		"ButtonStar.png",
		"ButtonStarSel.png", CC_CALLBACK_1(HelloWorld::starMenuCallback, this));
	starMenuItem->setPosition(Point(160, 220));
	auto starMenu = Menu::create(starMenuItem, NULL);
	starMenu->setPosition(Point::ZERO);
	this->addChild(starMenu, 1);

	_plusItem = MenuItemImage::create(
		"ButtonPlus.png",
		"ButtonPlusSel.png");
	_minusItem = MenuItemImage::create(
		"ButtonMinus.png",
		"ButtonMinusSel.png");

	MenuItemToggle *toggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorld::plusMinusButtonCallback, this), _plusItem, _minusItem, NULL);

	auto toggleMenu = Menu::create(toggleItem, NULL);
	toggleMenu->setPosition(Point(160, 290));
	this->addChild(toggleMenu, 1);
}

void HelloWorld::testDragAndDrop( cocos2d::Size& visibleSize )
{
	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB565);
	background = Sprite::create("blue-shooting-stars.png");
	background->setAnchorPoint(Point(0, 0));
	this->addChild(background);
	Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::DEFAULT);
	std::string images[] = {"bird.png", "cat.png", "dog.png", "turtle.png"};
	int images_length = 4;
	for(int i =0; i < images_length; ++i)
	{
		std::string image = images[i];
		Sprite *sprite = Sprite::create(image);
		float offsetFraction = ((float)(i+1))/(images_length+1);
		sprite->setPosition(visibleSize.width*offsetFraction, visibleSize.height/2);
		this->addChild(sprite);

		movableSprites.pushBack(sprite);
	}
	selSprite = NULL;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

bool HelloWorld::onTouchBegan( Touch *touch, Event *unused_event )
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	this->selectSpriteForTouch(touchLocation);
	return true;

}

void HelloWorld::onTouchMoved( Touch *touch, Event *unused_event )
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);

	Point oldTouchLocation = touch->getPreviousLocation();
	oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);

	Point translation = touchLocation - oldTouchLocation;
	this->panForTranslation(translation);

}

void HelloWorld::onTouchEnded( Touch *touch, Event *unused_event )
{

}

void HelloWorld::onTouchCancelled( Touch *touch, Event *unused_event )
{

}

void HelloWorld::selectSpriteForTouch( cocos2d::Point& touchLocation )
{
	Sprite * newSprite = NULL;
	for (Sprite *sprite : movableSprites)
	{
		if ( sprite->getBoundingBox().containsPoint(touchLocation) )
		{
			newSprite = sprite;
			break;
		}
	}
	if (newSprite != selSprite && NULL != newSprite)
	{
		if (NULL != selSprite)
		{
			selSprite->stopAllActions();
			selSprite->runAction(RotateTo::create(0.1, 0));
		}
		RotateBy * rotLeft = RotateBy::create(0.1, -14.0);
		RotateBy * rotCenter = RotateBy::create(0.1, 0.0);
		RotateBy * rotRight = RotateBy::create(0.1, 14.0);
		Sequence * rotSeq = Sequence::create(rotLeft, rotCenter, rotRight, rotCenter, NULL);
		newSprite->runAction(RepeatForever::create(rotSeq));
		selSprite = newSprite;
	}

}

Point HelloWorld::boundLayerPos(Point& newPos)
{
    Size winSize = Director::getInstance()->getWinSize();
    Point retval = newPos;

    /* if(retval.x > 0)
          retval.x = 0;
       if(retval.x < -background->getContentSize().width+winSize.width)
          retval.x = -background->getContentSize().width+winSize.width;  */
    retval.x = MIN(retval.x, 0);
    retval.x = MAX(retval.x, -background->getContentSize().width+winSize.width);

    retval.y = this->getPosition().y;
    return retval;
}

void HelloWorld::panForTranslation(Point& translation)
{
	if (selSprite)
	{
		Point newPos = selSprite->getPosition() + translation;
		selSprite->setPosition(newPos);
	}
	else
	{
		Point newPos = this->getPosition() + translation;
		this->setPosition( this->boundLayerPos(newPos) );
	}
}

void HelloWorld::testLayer()
{
	CCLayerGradient* layer1 = CCLayerGradient::create(ccc4(255, 0, 0, 255), ccc4(255, 0, 255, 255));
	layer1->setContentSize(CCSizeMake(80, 80));
	layer1->setPosition(ccp(50,50));
	addChild(layer1);
	
	CCLayerGradient* layer2 = CCLayerGradient::create(ccc4(0, 0, 0, 127), ccc4(255, 255, 255, 127));
	layer2->setContentSize(CCSizeMake(80, 80));
	layer2->setPosition(ccp(100,90));
	addChild(layer2);

	CCLayerGradient* layer3 = CCLayerGradient::create();
	layer3->setContentSize(CCSizeMake(80, 80));
	layer3->setPosition(ccp(150,140));
	layer3->setStartColor(ccc3(255, 0, 0));
	layer3->setEndColor(ccc3(255, 0, 255));
	layer3->setStartOpacity(255);
	layer3->setEndOpacity(255);
	ccBlendFunc blend;
	blend.src = GL_SRC_ALPHA;
	blend.dst = GL_ONE_MINUS_SRC_ALPHA;
	layer3->setBlendFunc(blend);
	addChild(layer3);

}

void HelloWorld::testFlipbookAnimations()
{
	if(1)
	{
		CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("grossini.png");
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache(); 
		cache->addSpriteFramesWithFile("grossini.plist");
		m_pSprite1 = CCSprite::createWithSpriteFrameName("grossini_dance_01.png"); 
		m_pSprite1->setPosition(400,400);
		spritebatch->addChild(m_pSprite1); 
		addChild(spritebatch);
		Vector<SpriteFrame*> animFrames;

		char str[100] = {0};

		for(int i = 1; i < 15; i++)
		{
			sprintf(str, "grossini_dance_%02d.png", i);
			CCSpriteFrame* frame = cache->spriteFrameByName( str );
			animFrames.pushBack(frame);
		}

		CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3f);
		m_pSprite1->runAction( CCRepeatForever::create( CCAnimate::create(animation) ) );
	}
	else
	{
		CCAnimation *animation = CCAnimation::create();

		// load image file from local file system to CCSpriteFrame, then add into CCAnimation
		for (int i = 1; i < 15; i++)
		{
			char szImageFileName[128] = {0};
			sprintf(szImageFileName, "grossini_dance_%02d.png", i);
			animation->addSpriteFrameWithFileName(szImageFileName);  
		}
	}
}

void HelloWorld::testLoadTitleMapFile()
{
	std::string file = "01.tmx"; 
	if (0)
	{
		_tileMap = CCTMXTiledMap::create(file);   
	}
	else
	{
		auto str = String::createWithContentsOfFile (FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());     
		_tileMap = TMXTiledMap::createWithXML(str->getCString(),"");  
	}
	_background = _tileMap->layerNamed("Background");         
	addChild(_tileMap, -1); 
}
