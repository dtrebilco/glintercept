# ![](./Docs/Images/GLicon.png?raw=true) GLIntercept

##About##
GLIntercept is a OpenGL function call interceptor for Windows that will intercept and log **all** [OpenGL](http://www.opengl.org) calls. 

##Basic usage##
Select the version of GLIntercept right for the application being debugged (x86 or x64) 

_**Note:**_ You select the version of GLIntercept based on if the _application_ is x86 or x64 (64bit) - not if the operating system is 64 bit.

Then after installation, simply copy the opengl32.dll and a gliConfig.ini file from 
the install directory to the executable folder of the application you want to
intercept OpenGL calls. 

Then edit the gliConfig.ini file, enable the options required and then run the 
application.

##How it works##
GLIntercept works by overriding the call to wglGetProcAddress, wrapping the real function pointer in some assembly and then returning it to the application being debugged. 

This means that when new OpenGL extensions/versions are released, GLIntercept will automatically log the new functions.

However, while all function names are automatically logged, function parameters need to be specified. These are supplied via text files in a "C" style syntax that can be updated as needed.

eg.
`void glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );`

##History##
GLIntercept has existed since 2003 and was mainly designed as an OpenGL 1.0-2.1 debugger. So while basic function logging should work on all OpenGL versions, the more advanced features listed below may or may not work in OpenGL 3.0+. (especially when using a core OpenGL profile)

##Features##
Main GLIntercept features:

* Save all OpenGL function calls to text or [XML format](./Docs/Screenshots.md) with the option to log individual frames.
* Activate and log ARB_debug_output/GL_KHR_debug messages to the [log](./Docs/Screenshots.md)
* Run time [shader edit](./Docs/Screenshots.md). Display shader usage and edit the shaders at run time. Supports ARB VP/FP/GLSL and NV VP/FP
* [Free camera](./Docs/Screenshots.md). Fly around the geometry sent to the graphics card and enable/disable wireframe/backface-culling/view frustum render.
* Save and track textures. (1D,2D,3D,NVRect and p-buffer bound textures are supported.) Saving can be to TGA,PNG and JPG formats.
* Save and track shaders/programs.
* Save and track display lists.
* Saving of the OpenGL frame buffer (color/depth/stencil) pre and post render calls. The ability to save the "diff" of pre and post images is also available.
* [Track error states](./Docs/Screenshots.md) (logging them to the debugger output) and breaking on errors.
* Basic thread error checking.
* Function timer log.
* Resource leak tracking for contexts, display lists, shaders and textures.
* [OpenGL extension/version override](./Docs/Screenshots.md). Add/remove/replace the OpenGL extension and version strings. (Test lower end rendering paths without changing cards)
* OpenGL function stats - how many times each function is called.

##License##
*Note* Older versions of GLIntercept were licensed under the GPLv2. Newer versions (1.0+) are MIT licensed.

##Links##
Similar OpenGL tools to GLIntercept are:

* [RenderDoc](https://github.com/baldurk/renderdoc)
* [Intel GPA](https://software.intel.com/en-us/gpa)
* [APITrace](https://github.com/apitrace/apitrace)
* [GLSLDevil](http://www.vis.uni-stuttgart.de/glsldevil/index.html)
* [gDEBugger](http://www.gremedy.com/)
* [BuGLe](http://sourceforge.net/projects/bugle/)
* [AMD GPU PerfStudio](http://developer.amd.com/gpu/PerfStudio/Pages/default.aspx)
* [OpenGLXtractor (GLIntercept 0.5 with GUI)](http://members.chello.at/alexan/)

OpenGL ES / OpenVG tracing
* [tracy](https://gitorious.org/tracy) 

Direct3D
* [Visual Studio Graphics Diagnostics tools](https://msdn.microsoft.com/en-us/library/vstudio/hh315751(v=vs.120).aspx)
* PIX (part of DirectX SDK)
* [3DRipperDX](http://www.deep-shadows.com/hax/3DRipperDX.htm)



