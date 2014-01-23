#include "MyApplication.h"

int main()
{
	MyApplication app;
	
	app.go();
	while( app.keepRunning() )
	{
		app.update( );
	}
	return 0;
}