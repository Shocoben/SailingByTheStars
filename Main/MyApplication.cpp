#include "MyApplication.h"
#include "Scene.h"

bool MyApplication::_keepRunning = true;

bool MyApplication::keepRunning()
{
	return _keepRunning;
}

void MyApplication::exit()
{
	_keepRunning = false;
}


MyApplication::MyApplication()
{
		_root = NULL;
		_window = NULL;
		_listener = NULL;
		_defaultRessourcesPath = "myResources_d.cfg";
		_pluginsFilePath = "plugins_d.cfg";
		_appName = "Ogre3D App";
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
	createFrameListeners();

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

void MyApplication::createFrameListeners
()
{
	_listener = new Scene(_window, _root);
	_root->addFrameListener(_listener);
}

void MyApplication::update()
{
	Ogre::WindowEventUtilities::messagePump();
	_keepRunning = _root->renderOneFrame();

}

