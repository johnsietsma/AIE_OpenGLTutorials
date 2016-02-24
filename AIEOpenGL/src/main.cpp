#include "Blank.h"
#include "FBXLighting.h"
#include "RenderingGeometry.h"
#include "Textures.h"

#include <GLFW/glfw3.h>

int main() {
	
	BaseApplication* app = new FBXLighting();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}
