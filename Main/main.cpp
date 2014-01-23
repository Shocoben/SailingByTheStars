#include "MyApplication.h"

int main()
{
	MyApplication app;
	
	app.go();
	while( MyApplication::keepRunning() )
	{
		app.update( );
	}
	return 0;
}