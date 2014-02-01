#include "MyApplication.h"
#include "ApplicationListener.h"
#include "MainScene.h"

int main()
{
	MyApplication app("SailingByTheStars");
	
	app.go();
	
	MainScene* scene = new MainScene(&app);
	app.getAppListener()->loadScene(scene);
	
	while( MyApplication::keepRunning() )
	{
		app.update( );
	}

	return 0;
}