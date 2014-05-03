#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void starMenuCallback(Ref* sender);

	void HelloWorld::plusMinusButtonCallback(Ref* sender);

	//	test code
	void testButton(cocos2d::Size& visibleSize,cocos2d::Point& origin);

	//	drag and drop sprite
	void testDragAndDrop(cocos2d::Size& visibleSize);
	void selectSpriteForTouch(cocos2d::Point& touchLocation);
	cocos2d::Point boundLayerPos(cocos2d::Point& newPos);
	void panForTranslation(cocos2d::Point& translation);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event); 
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	//	test layer
	void testLayer();

	//	Flipbook Animations
	void testFlipbookAnimations();

private:

	//	for button test
	cocos2d::LabelTTF* label_;

	cocos2d::MenuItemImage *_plusItem;
	cocos2d::MenuItemImage *_minusItem;

	//	drag and drop sprite
	cocos2d::Sprite * background;
	cocos2d::Sprite * selSprite;
	cocos2d::Vector<cocos2d::Sprite *> movableSprites;

	//	test layer

	//	Flipbook Animations
	cocos2d::Sprite * m_pSprite1;

};

#endif // __HELLOWORLD_SCENE_H__
