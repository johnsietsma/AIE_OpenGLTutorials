# AIE OpenGL Tutorials

An implmentation of some the AIE tutorials for the Computer Graphics and Complex Games subjects.

The aim is to give students the tutorial code in one block so they can have a guide when they're stuck, to clear up confusion around code structure.

It is NOT for wholesale copying and pasting.

## Setup
Download and unzip the FBXLoader from the Computer Graphics subject resources page. Copy "FBXLoader.lib" and "FBXLoader_d.lib" into the dep/FBXLoader directory.

Make sure the platform dropdown in the toolbar is set to "X86".

## Code Structure

Most tutorials inherit from BaseAppliction. Modify main.cpp to create the tutorial application you'd lke to run.

The only exception is the GPGPU tutorial which run without an OpenGL window. Uncomment the GPGPU code in main.cpp to run that tutorial.

All the tutorials are standalone, they don't share any common code outside of the BaseAppliction code. You can find them in the "Tutorials" folder.

There is a tutorial called "Blank" that can be used as a template for new tutorials. It renders and grid and has a fly camera.

The tutorials are as close as I could make them to the tutorials provided to you.
