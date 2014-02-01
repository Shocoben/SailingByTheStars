#include "MyApplication.h"
#include "ApplicationListener.h"
#include "Scene.h"
#include "GameObject.h"

Scene::Scene( MyApplication* app ) : _application(app), _appListener(app->getAppListener()), _listenerID(_appListener->getLastSceneID())
{
	_appListener->addScene(this);
}

Scene::~Scene()
{
	
}

GameObject* nGO;

Ogre::SceneManager* Scene::sceneManager()
{
	return _sceneManager;
}

int Scene::getLastGOListLength()
{
	return _gameObjectsList.size();
}

void Scene::addToScene(GameObject* gameObject)
{
	_gameObjectsList.push_back(gameObject);
}


void Scene::removeFromScene(GameObject* gameObject)
{
	std::vector<GameObject*>::iterator itGO = _gameObjectsList.begin() + gameObject->getSceneID();
	if (gameObject->getSceneID() < _gameObjectsList.size())
	{
		for(std::vector<GameObject*>::iterator it = itGO + 1; it != _gameObjectsList.end(); ++it) 
		{
			GameObject* cObj = *it;
			cObj->objectHaveBeenErasedFromlist(gameObject->getSceneID());
		}
	}
	_gameObjectsList.erase(itGO);


}