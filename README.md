# AIE OpenGL Tutorials

## Setup
Download and unzip the FBXLoader. Copy "FBXLoader.lib" and "FBXLoader_d.lib" into the dep/FBXLoader directory.

Make sure the platform dropdown in the toolbar is set to "X86".

## Code Structure

Most tutorials inherit from BaseAppliction. Modify main.cpp to create the tutorial application you'd lke to run.

The only exception is the GPGPU tutorial which run without an OpenGL window. Uncomment the GPGPU code in main.cpp to run that tutorial.

All the tutorials are standalone, they don't share any common code outside of the BaseAppliction code. You can find them in the "Tutorials" folder.

There is a tutorial called "Blank" that can be used as a template for new tutorials.
