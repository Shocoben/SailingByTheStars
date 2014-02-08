#include "MyApplication.h"
#include "ApplicationListener.h"
#include "MainScene.h"

int main()
{
	MyApplication app("SailingByTheStars");
	int startR = app.go();

	if (startR == 0)
	{
		MainScene* scene = new MainScene(&app, "media/terraint.xml");
		app.getAppListener()->loadScene(scene);
	
		while( MyApplication::keepRunning() == true )
		{
			app.update( );
		}
	}
	else
	{
		return startR;
	}
	return 0;
}