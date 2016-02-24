#include "Tutorials/Blank.h"
#include "Tutorials/FBXLighting.h"
#include "Tutorials/RenderingGeometry.h"
#include "Tutorials/Textures.h"

#include <GLFW/glfw3.h>

int main() {
	
	BaseApplication* app = new FBXLighting();
	if (app->startup())
		app->run();
	app->shutdown();

	return 0;
}
