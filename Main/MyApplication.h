#include "Scene.h"

class MyApplication
{
	protected :
		Ogre::Root* _root;
		Ogre::RenderWindow* _window;
		MyFrameListener* _listener;

		std::string _pluginsFilePath;
		std::string _appName;
		std::string _defaultRessourcesPath;

		bool _keepRunning;
	public :
		MyApplication();
		~MyApplication();

		int startUp();
		bool loadResources(std::string path);
		void finalizeRessourcesLoading();
		void createFrameListeners();
		int go();

		void update();
		bool keepRunning()
		{
			return _keepRunning;
		}

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
