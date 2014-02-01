#include "MyApplication.h"
#include "ApplicationListener.h"

std::map<std::string, MyApplication*> createMap()
{
	std::map<std::string, MyApplication*> map;
	return map;
}
std::map<std::string, MyApplication*> MyApplication::appsByName = createMap();

bool MyApplication::_keepRunning = true;
bool MyApplication::keepRunning()
{
	return _keepRunning;
}

void MyApplication::exit()
{
	_keepRunning = false;
}

MyApplication::MyApplication(const std::string& appName)
{
		_root = NULL;
		_window = NULL;
		_listener = NULL;
		_defaultRessourcesPath = "myResources_d.cfg";
		_pluginsFilePath = "plugins_d.cfg";
		_appName = appName;
		
		//appsByName[_appName] = this;
}

ApplicationListener* MyApplication::getAppListener()
{
	return _listener;
}

MyApplication::~MyApplication()
{
	delete _root;
}

int MyApplication::go()
{
	int s = startUp();
	if (s < 0)
		return s;
	loadResources(_defaultRessourcesPath);
	finalizeRessourcesLoading();
	finalizeAppListener();

	return 0;
}

int MyApplication::startUp()
{
	_root = new Ogre::Root(_pluginsFilePath);
	if (!_root->showConfigDialog())
	{
		return -1;
	}
	_window = _root->initialise(true, _appName);
	
	return 0;
}

bool MyApplication::loadResources(std::string path)
{
	if (path.empty())
		return false;
	//Loading stuff
	Ogre::ConfigFile cf;
	cf.load(path);
	Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();

	Ogre::String sectionName, typeName, dataPath;

	
	while(sectionIter.hasMoreElements())
	{
		sectionName = sectionIter.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = sectionIter.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			dataPath = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataPath, typeName, sectionName);
		}
	}

	return true;
}

void MyApplication::finalizeRessourcesLoading()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApplication::finalizeAppListener()
{
	_listener = new ApplicationListener(this);
	_root->addFrameListener(_listener);
}

void MyApplication::update()
{
	Ogre::WindowEventUtilities::messagePump();
	_keepRunning = _root->renderOneFrame();

}

Ogre::Root* MyApplication::getRoot()
{
	return _root;
};

Ogre::RenderWindow* MyApplication::getWindow()
{
	return _window;
}
