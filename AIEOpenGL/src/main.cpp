#include "Tutorials/AdvancedTexturing.h"
#include "Tutorials/Blank.h"
#include "Tutorials/GPGPU.h"
#include "Tutorials/FBXLighting.h"
#include "Tutorials/RenderingGeometry.h"
#include "Tutorials/Textures.h"
#include "Tutorials/UtilitySystems.h"

#include <GLFW/glfw3.h>

int main() {
	
	/*BaseApplication* app = new UtilitySystems();
	if (app->startup())
		app->run();
	app->shutdown();*/

    GPGPU gpgpu;
    gpgpu.runCPU();
    gpgpu.runGPU();

	return 0;
}
