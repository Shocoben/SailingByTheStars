#pragma once
#include "Ogre.h"
class ApplicationListener;

class MyApplication
{
	protected :
		Ogre::Root* _root;
		Ogre::RenderWindow* _window;
		ApplicationListener* _listener;
		static std::map<std::string, MyApplication*> appsByName;

		std::string _pluginsFilePath;
		std::string _appName;
		std::string _defaultRessourcesPath;


		static bool _keepRunning;

		
	public :
		MyApplication(const std::string& appName);
		~MyApplication();

		int startUp();
		bool loadResources(std::string path);
		void finalizeRessourcesLoading();
		void finalizeAppListener();
		int go();

		void update();
		static bool keepRunning();

		static void exit();

		ApplicationListener* getAppListener();

		Ogre::Root* getRoot();
		Ogre::RenderWindow* getWindow();

		void setPluginsPath(std::string pluginsFilePath)
		{
			_pluginsFilePath = pluginsFilePath;
		}

		void setDefaultRessourcesPath(std::string defaultRessourcesPath)
		{
			_defaultRessourcesPath = defaultRessourcesPath;
		}
};
