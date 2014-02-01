#include "Scene.h"
#include "GameObject.h"

Scene::Scene(Ogre::RenderWindow* win, Ogre::Root* root) : MyFrameListener(win, root), _movementSpeed(50.0f), _movingObject(NULL)
{
	_sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
	
	createCameras(win);
	createScene();
	
}

Scene::~Scene()
{

}

void Scene::createCameras(Ogre::RenderWindow* win)
{

}

GameObject* nGO;

void Scene::createScene()
{

}

Ogre::SceneManager* Scene::sceneManager()
{
	return _sceneManager;
}

bool Scene::frameStarted(const Ogre::FrameEvent& evt)
{

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