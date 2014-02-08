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

ApplicationListener* Scene::getAppListener()
{
	return _appListener;
}

Camera* Scene::getMainCamera()
{
	return _mainCamera;
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

bool Scene::mouseMoved (const OIS::MouseEvent &arg)
{
	return true;
}

bool Scene::mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	std::cout << "Scene Mouse Pressed" << std::endl;
	return true;
}

bool Scene::mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}

bool Scene::keyReleased (const OIS::KeyEvent &arg)
{
	return true;
}

bool Scene::keyPressed (const OIS::KeyEvent &arg)
{
	return true;
}