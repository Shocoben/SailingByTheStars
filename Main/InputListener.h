#pragma once
#include "OISMouse.h"
#include "OISKeyboard.h"

class ApplicationListener;

class AppMouseListener : public OIS::MouseListener
{
protected:
	ApplicationListener* _appListener;
public:
	virtual bool mouseMoved (const MouseEvent &arg)=0
	virtual bool mousePressed (const MouseEvent &arg, MouseButtonID id)=0
	virtual bool mouseReleased (const MouseEvent &arg, MouseButtonID id)=0
	AppMouseListener(ApplicationListener* appListener);
	~AppMouseListener(void);
};

class AppMouseListener
{
protected:
	ApplicationListener* _appListener;
public:

	AppMouseListener(ApplicationListener* appListener);
	~AppMouseListener(void);
};

