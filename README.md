# NoKarma
NoKarma game engine with an entity-component architecture (composition). 

The library is divided in 3 projects:
   * NkEngine contains the basic classes to create, manage and destroy components.
   * NkCompSys contains common components that may be considered the core for several games.
   * Graphics Engine contains the start of a OpenGL wrapper.
   
The project needs to be compiled with c++11, we used VS2012.

Libraries needed:
  * OpenGL
  * Bullet Physics Engine.
  * FreeImage.
  
We are in the process of creating build files and projects for visual studio, we may use cmake or bazel to do this. 
