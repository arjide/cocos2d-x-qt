#include "IntervalTest.h"
#include "../testResource.h"

#define SID_STEP1	100
#define SID_STEP2	101
#define SID_STEP3	102

#define IDC_PAUSE	200



IntervalLayer::IntervalLayer()
{
	m_time0 = m_time1 = m_time2 = m_time3 = m_time4 = 0.0f;

	CCSize s = CCDirector::sharedDirector()->getWinSize();
	// sun
	CCParticleSystem* sun = CCParticleSun::node();
    sun->setTexture(CCTextureCache::sharedTextureCache()->addImage("Images/fire.png"));
	sun->setPosition( CCPointMake(s.width-32,s.height-32) );

	sun->setTotalParticles(130);
	sun->setLife(0.6f);
	this->addChild(sun);

	// timers
    m_label0 = CCLabelBMFont::labelWithString("0", "fonts/bitmapFontTest4.fnt");
	m_label1 = CCLabelBMFont::labelWithString("0", "fonts/bitmapFontTest4.fnt");
	m_label2 = CCLabelBMFont::labelWithString("0", "fonts/bitmapFontTest4.fnt");
	m_label3 = CCLabelBMFont::labelWithString("0", "fonts/bitmapFontTest4.fnt");
    m_label4 = CCLabelBMFont::labelWithString("0", "fonts/bitmapFontTest4.fnt");

    scheduleUpdate();
	schedule(schedule_selector(IntervalLayer::step1));
	schedule(schedule_selector(IntervalLayer::step2), 0);
	schedule(schedule_selector(IntervalLayer::step3), 1.0f);
    schedule(schedule_selector(IntervalLayer::step4), 2.0f);

    m_label0->setPosition(CCPointMake(s.width*1/6, s.height/2));
	m_label1->setPosition(CCPointMake(s.width*2/6, s.height/2));
	m_label2->setPosition(CCPointMake(s.width*3/6, s.height/2));
	m_label3->setPosition(CCPointMake(s.width*4/6, s.height/2));
    m_label4->setPosition(CCPointMake(s.width*5/6, s.height/2));

    addChild(m_label0);
	addChild(m_label1);
	addChild(m_label2);
	addChild(m_label3);
    addChild(m_label4);

	// Sprite
	CCSprite* sprite = CCSprite::spriteWithFile(s_pPathGrossini);
	sprite->setPosition( CCPointMake(40,50) );
	
	CCJumpBy* jump = CCJumpBy::actionWithDuration(3, CCPointMake(s.width-80,0), 50, 4);
	
	addChild(sprite);
	sprite->runAction( CCRepeatForever::actionWithAction(
															(CCActionInterval*)(CCSequence::actions(jump, jump->reverse(), NULL ))
														)
					 );
	// pause button
	CCMenuItem* item1 = CCMenuItemFont::itemFromString("Pause", this, menu_selector(IntervalLayer::onPause) );
	CCMenu* menu = CCMenu::menuWithItems(item1, NULL);
	menu->setPosition( CCPointMake(s.width/2, s.height-50) );

	addChild( menu );
}

IntervalLayer::~IntervalLayer()
{
    if(CCDirector::sharedDirector()->isPaused())
    {
        CCDirector::sharedDirector()->resume();
    }
}

void IntervalLayer::update(ccTime dt)
{
    m_time0 +=dt;
    char time[10] = {0};
    sprintf(time, "%2.1f", m_time0);
    m_label0->setString(time);
}

void IntervalLayer::onPause(CCObject* pSender)
{
	if(CCDirector::sharedDirector()->isPaused())
		CCDirector::sharedDirector()->resume(); 
	else
		CCDirector::sharedDirector()->pause(); 

}

void IntervalLayer::step1(ccTime dt)
{
	m_time1 +=dt;

    char str[10] = {0};
    sprintf(str, "%2.1f", m_time1);
	m_label1->setString( str );
}

void IntervalLayer::step2(ccTime dt)
{
	m_time2 +=dt;

    char str[10] = {0};
    sprintf(str, "%2.1f", m_time2);
    m_label2->setString( str ); 
}

void IntervalLayer::step3(ccTime dt)
{
	m_time3 +=dt;

    char str[10] = {0};
    sprintf(str, "%2.1f", m_time3);
    m_label3->setString( str ); 
}

void IntervalLayer::step4(ccTime dt)
{
    m_time4 +=dt;

    char str[10] = {0};
    sprintf(str, "%2.1f", m_time4);
    m_label4->setString( str ); 
}

void IntervalTestScene::runThisTest()
{
    CCLayer* pLayer = new IntervalLayer();
    addChild(pLayer);
    pLayer->release();

    CCDirector::sharedDirector()->replaceScene(this);
}
