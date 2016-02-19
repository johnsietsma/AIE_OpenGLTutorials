#include "RenderingGeometry.h"
#include <GLFW/glfw3.h>

int main() {
	
	BaseApplication* app = new RenderingGeometry();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}
