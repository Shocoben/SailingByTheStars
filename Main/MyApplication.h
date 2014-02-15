#pragma once
#include "Ogre.h"
#include "RapidXML\rapidxml.hpp"
class ApplicationListener;
using namespace rapidxml;

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
		xml_document<> _docXML;
		xml_node<>* _rootXMLNode;
		

		char* _xmlChar;
		
	public :
		MyApplication(const std::string& appName, const std::string& xmlFile);
		~MyApplication();

		int startUp();
		bool loadResources(std::string path);
		void finalizeRessourcesLoading();
		void finalizeAppListener();
		int go();

		void update();
		static bool keepRunning();

		static void exit();
		void createColourCube();

		const xml_node<>* getRootXMLNode();

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
