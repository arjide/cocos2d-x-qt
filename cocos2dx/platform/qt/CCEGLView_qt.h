/**
* Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* For the applicable distribution terms see the license text file included in
* the distribution.
*/

#ifndef CCEGLVIEWQT_H_
#define CCEGLVIEWQT_H_

#include "CCEGLViewProtocol.h"
#include "CCCommon.h"
#include "CCGeometry.h"

#include <QtGui>
#include <QWidget>

#include <EGL/egl.h>

/*
#ifdef Q_OS_SYMBIAN
#define MS_USE_TOUCH_EVENTS
#define MAX_TOUCH_POINTS 5
#endif
*/

//#ifdef Q_WS_MAEMO_6
#define MS_USE_TOUCH_EVENTS
#define MAX_TOUCH_POINTS 5
//#endif

bool initExtensions();

NS_CC_BEGIN

class CCSet;
class CCTouch;
class EGLTouchDelegate;

class CC_DLL CCEGLView: public QWidget, public CCEGLViewProtocol
{
    Q_OBJECT
public:
    CCEGLView(QWidget* parent = NULL);
    virtual ~CCEGLView();

    virtual void end();
    virtual bool isOpenGLReady();
    virtual void swapBuffers();
    virtual void setIMEKeyboardState(bool bOpen);

    /*
    virtual bool    isIpad();
    virtual CCRect  getViewPort();
    virtual CCSize  getSize();
    virtual void    setFrameSize(float width, float height);
    virtual CCSize  getFrameSize();
    virtual void    setDesignResolutionSize(float width, float height);
    virtual void    setTouchDelegate(EGLTouchDelegate * pDelegate);
    virtual float   getScreenScaleFactor();
    virtual bool    canSetContentScaleFactor();
    virtual void    setContentScaleFactor(float contentScaleFactor);
    virtual void    setViewPortInPoints(float x , float y , float w , float h);
    virtual void    setScissorInPoints(float x , float y , float w , float h);
    virtual float   getMainScreenScale();
    virtual void    setViewName(const char* pszViewName);
    const char*     getViewName();

    virtual void    handleTouchesBegin(int num, int ids[], float xs[], float ys[]);
    virtual void    handleTouchesMove(int num, int ids[], float xs[], float ys[]);
    virtual void    handleTouchesEnd(int num, int ids[], float xs[], float ys[]);
    virtual void    handleTouchesCancel(int num, int ids[], float xs[], float ys[]);
    */

    bool Create();

    void release();


    /**
     @brief	get the shared main open gl window
     */
    static CC_DLL CCEGLView& sharedOpenGLView();

protected: // From QWidget (and other base classes)
    void inputMethodEvent(QInputMethodEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:
    bool initEGL();
    void releaseEGL();
    bool handleKeyEvent(QEvent*);
    void updateVkbOrientation();
#ifdef MS_USE_TOUCH_EVENTS
    bool handleTouchEvent(QEvent*);
    bool event(QEvent*);
#else
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
#endif

private:
#ifdef MS_USE_TOUCH_EVENTS
    CCTouch *m_pTouch[MAX_TOUCH_POINTS];
#else
    CCTouch *m_pTouch;
#endif

    bool bIsInit;

    EGLDisplay  m_eglDisplay;
    EGLConfig   m_eglConfig;
    EGLSurface  m_eglSurface;
    EGLContext  m_eglContext;
};

NS_CC_END

using namespace cocos2d;

#endif /* CCEGLVIEWLINUX_H_ */