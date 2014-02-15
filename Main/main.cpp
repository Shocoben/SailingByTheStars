#include "MyApplication.h"
#include "ApplicationListener.h"
#include "MainScene.h"

int main()
{
	MyApplication app("SailingByTheStars", "media/terraint.xml");
	int startR = app.go();

	if (startR == 0)
	{
		MainScene* scene = new MainScene(&app, app.getRootXMLNode()->first_node("Levelone"));
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