#pragma once
#include "Ogre.h"
class MyFrameListener;

class MyApplication
{
	protected :
		Ogre::Root* _root;
		Ogre::RenderWindow* _window;
		MyFrameListener* _listener;

		std::string _pluginsFilePath;
		std::string _appName;
		std::string _defaultRessourcesPath;

		static bool _keepRunning;
	public :
		MyApplication();
		~MyApplication();

		int startUp();
		bool loadResources(std::string path);
		void finalizeRessourcesLoading();
		void createFrameListeners();
		int go();

		void update();
		static bool keepRunning();

		static void exit();

		void setAppName(std::string appName)
		{
			_appName = appName;
		}

		void setPluginsPath(std::string pluginsFilePath)
		{
			_pluginsFilePath = pluginsFilePath;
		}

		void setDefaultRessourcesPath(std::string defaultRessourcesPath)
		{
			_defaultRessourcesPath = defaultRessourcesPath;
		}
};
