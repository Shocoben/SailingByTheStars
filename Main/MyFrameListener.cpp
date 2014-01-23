#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win)
{
	size_t wHnd;
	win->getCustomAttribute("WINDOW", &wHnd);
	std::stringstream wHnDs;
	wHnDs << wHnd;

	OIS::ParamList pl;
	pl.insert( std::make_pair("WINDOW", wHnDs.str()) );
	#if defined OIS_WIN32_PLATFORM
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	#elif defined OIS_LINUX_PLATFORM
			pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
			pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
			pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
			pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
	#endif
	_inputManager = OIS::InputManager::createInputSystem(pl);
	_keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject( OIS::OISKeyboard, false ));
	_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, false));


}

void MyFrameListener::setMouseViewportSize(Ogre::Viewport* viewPort)
{
	_mouse->getMouseState().width = viewPort->getActualWidth();
	_mouse->getMouseState().height = viewPort->getActualHeight();
}

MyFrameListener::~MyFrameListener()
{
		_inputManager->destroyInputObject( _keyboard );
		_inputManager->destroyInputObject( _mouse );
		OIS::InputManager::destroyInputSystem( _inputManager );
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
		_keyboard->capture( );
		_mouse->capture( );
		
		if ( _keyboard->isKeyDown( OIS::KC_ESCAPE ) )
			return false;

		return true;
}

bool MyFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool MyFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}