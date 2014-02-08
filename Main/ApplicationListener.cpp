#include "ApplicationListener.h"
#include "MyApplication.h"
#include "Scene.h"

ApplicationListener::ApplicationListener(MyApplication* app) : _application(app)
{
	size_t wHnd;
	app->getWindow()->getCustomAttribute("WINDOW", &wHnd);
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
	_keyboard = static_cast<OIS::Keyboard*>(_inputManager->createInputObject( OIS::OISKeyboard, true ));
	_mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, true));
}

int ApplicationListener::getLastSceneID()
{
	return _scenes.size();
}

void ApplicationListener::loadScene(Scene* scene)
{

	_currentScene = scene;
	if (scene != NULL)
	{
		_currentScene->load();
		_mouse->setEventCallback(_currentScene);
		_keyboard->setEventCallback(_currentScene);
	}
}


void ApplicationListener::loadScene(int sceneID)
{
	loadScene(_scenes[sceneID]);
}

void ApplicationListener::addScene(Scene* scene)
{
	_scenes.push_back(scene);
}

void ApplicationListener::setMouseViewportSize(Ogre::Viewport* viewPort)
{
	_mouse->getMouseState().width = viewPort->getActualWidth();
	_mouse->getMouseState().height = viewPort->getActualHeight();
}

ApplicationListener::~ApplicationListener()
{
	
	_inputManager->destroyInputObject( _keyboard );
	_inputManager->destroyInputObject( _mouse );

	OIS::InputManager::destroyInputSystem( _inputManager );
}

bool ApplicationListener::frameStarted(const Ogre::FrameEvent& evt)
{
		_keyboard->capture( );
		_mouse->capture( );
		
		if ( _keyboard->isKeyDown( OIS::KC_ESCAPE ) )
			MyApplication::exit();


		if (_currentScene != NULL)
			return _currentScene->update(evt);
		return true;
}


bool ApplicationListener::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool ApplicationListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	return true;
}