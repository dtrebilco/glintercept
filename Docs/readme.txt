============================================================
  
  GLIntercept v1.3

  Copyright (c) 2014  Damian Trebilco  All rights reserved.

============================================================

Contents:
  1) Quick start guide
  2) Output
    2.1) Function Log
    2.2) Error Log
    2.3) Image Log
    2.4) Shader Log
    2.5) Frame/Render Log
    2.6) Display List Log
    2.7) Timer Log

  3) Plugins
    3.1) Shader editor
    3.2) Free Camera
    3.3) Extension/Version override
    3.4) Frame Ping
    3.5) Function Stats
    3.6) Debug Context - debug output
    3.7) C-Strike
    
  4) Known bugs
  5) Adding OpenGL extensions
  6) Contact
  7) Disclaimer

============================================================
*IMPORTANT* GLIntercept is available in 32 bit and 64 bit versions. 

You have to use the version that matches the application you are debugging - not your 
operating system. Eg. You may be using 64 bit Windows, but you still need to use
the 32 bit version of GLIntercept to debug 32 bit programs. 

Also note that the 64 bit version of GLIntercept still uses the dll name OpenGL32.dll.
  
============================================================
1) Quick start guide

After installation, simply copy the opengl32.dll and a gliConfig.ini file from 
the install directory to the executable folder of the application you want to
intercept OpenGL calls. (If the target application is in C:\Program Files\,
GLIntercept may not work correctly depending on the OS permissions) 

Then edit the gliConfig.ini file, enable the options required and then run the 
application.

Several pre-configured *.ini files are provided demostrating different options.
Rename the file to gliConfig.ini before use:

gliConfig.ini - Basic text logging with image/shader/display list dumping. 

gliConfig_XMLFrame.ini  - XML per-frame logging (on ctrl-shift-f) with 
                          image/shader/dlist/frame buffer dumping.

gliConfig_FullDebug.ini - Text logging with full debug checking and log flushing.
                          Can be slow, but useful for finding OpenGL errors 
                          or the OpenGL function the app crashed on.

gliConfig_ExtOverride.ini - Demonstrates how to use the OpenGL extension/version
                            override plugin.

gliConfig_FreeCam.ini   - Demonstrates how to use the OpenGL free camera plugin.
                          (ctrl-shift-c starts the free camera see below for 
                           other key bindings)  

gliConfig_ShaderEdit.ini - Demonstrates how to use the OpenGL shader editor plugin.
                           (ctrl-shift-s starts the shader editor)  

gliConfig_DebugContext.ini - Forced debug context with full ARB_debug_output logging.
                             XML per-frame logging on ctrl-shift-f.

gliConfig_AuthorStd.ini - Authors' preferred settings when developing.


If the interception was successful, a gliLog.txt will be generated in the same
directory as the opengl32.dll. (this is the error log). The results of the 
interception are also stored in the same directory by default. 
(ie. gliInterceptLog.txt)

It is important to note that GLIntercept is NOT safe when multiple render 
contexts are active at the same time on different threads. (ie you will 
probably experience crashes/ bad data) This restriction may be lifted in 
the future however, very few OpenGL applications use multiple render 
contexts at once. GLIntercept will currently report a warning if this is 
attempted.

============================================================
2) Output

2.1) Function Log

The GLIntercept function log simply records incoming OpenGL calls and saves 
them to a file. The logger can store functions in flat .txt files or in a .xml
format that can use a xsl file to present the data in a web browser.

Only OpenGL functions known by GLIntercept will have parameters of the function
logged. Unknown functions will have a ???? in the parameter field.

2.2) Error Log

Every execution of GLIntercept produces a gliLog.txt file in the directory of
the opengl32.dll. This file contains a record of any abnormal states or errors
that are detected during execution. If the image or shader logs are enabled, 
texture and shader memory leaks are also recorded here.

2.3) Image Log

The image log enables the saving of any OpenGL textures that are used in the 
application. Texture types include 1D,2D,3D,CUBE,NV_RECT and p-buffer bound 
textures. See the gliConfig.ini file for full options. 

2.4) Shader Log

The shader log enables the saving of OpenGL shaders/programs that are used 
in the application. Only ARB/NV/GLSL vertex and fragment programs/shaders
are currently supported. See the gliConfig.ini file for full options. 


2.5) Frame/Render Log
  
The frame log enables the saving of the OpenGL frame buffer pre and post 
render calls. The ability to save the "diff" of pre and post images is also 
available. The frame logger will not work correctly if you use 
WGL_ARB_make_current_read in your application. Also currently,
the pixel transfer options will affect the buffers read back. (ie the colors
values returned may be different from what is actually on the buffer. This may
be fixed in future versions.
See the gliConfig.ini file for full options. 

2.6) Display List Log

The display list log enables the tracking and saving of OpenGL commands that
occur inside a display list. This includes reporting memory leaks on shutdown.
See the gliConfig.ini file for full options.  

2.7) Timer Log

The timer log is a logger the writes to the function log and flags how long
each function took to execuite. WARNING - This option is only for advanced 
OpenGL users and novice OpenGL programmers should not try to optimize
programs based on these results. See gliConfig.ini file for full options 
and a listing of special conditions for this logger.

============================================================
3) Plugins

GLIntercept supports plugins loaded from external DLLs. These plugins simply
register what function updates they want to receive, and GLI will call the
plugin when the OpenGL call is made.

Each plugin is added as a line in the gliConfig.ini file and plugin specific 
options can also be provided. Each plugin typically has a config.ini file that
specifies the default global setting for all its' options. 
These files can be located:

(<install dir>\Plugins\<plugin name>\config.ini) 

Specifying any of these options in the main gliConfig.ini file will override 
the defaults.

3.1) Shader editor

The shader editor allows the editing of ARB/NV VP/FP/GLSL shaders/programs 
at run time. By pressing (by default) ctrl-shift-s during the execuition,
a shader editor (based on SciTE) will open as a seperate process with a 
listing of all currently active shaders. Edits on these shaders can then 
be performed and compiled back into the OpenGL application. 

Standard VC6 commands are used in the editor. Commands of note are:
F3 - Search
F4 - Next error
F7 - Compile
ctrl + space - Listing of all api commands (GLSL only)
ctrl + shift + space - Tool tip for the current function (GLSL only)

The fonts and color scheme of the editor can be configured by editing the
*.properties files in the plugin directory.

Note: You can also run the editor without a OpenGL application as it 
includes full syntax and tooltips as well as the GLSL validator.
(run GLISciTE.exe from the plugin folder)

Add the following lines to the gliConfig.ini to use the shader editor.
PluginData
{
  ....

  Plugins
  {
    OpenGLShaderEdit = ("GLShaderEdit/GLShaderEdit.dll")
  }
}


3.2) Free Camera

The "free" camera allows the user to "fly" around the rendered scene to view
what actual geometry is sent to the graphics card. This plugin relies on 
correct usage of the modelview matrix and its usage in all vertex programs.
(Some applications with reverse perspective matrices may need to change the 
key combinations as the controls will be flipped)

Many other options are also provided to aid in debugging. 
(see the plugins' config.ini file for full details)

Default key options of note:
 ctrl,shift,c - Enable/Disable the free camera keys
 p            - Reset camera position
 o            - Reset camera orientation
 ctrl,shift,u - Enable/Disable back face culling 
 ctrl,shift,w - Enable/Disable wire frame mode
 ctrl,shift,v - Enable/Disable rendering of the view frustum

 i,k,j,l,     - Forward/Back/Left/Right movement when in free cam mode
 Num pad keys - Pith/Roll/Yaw movement when in free cam mode
 Shift        - Movement multiplier (settable value - default = 10x)


Add the following lines to the gliConfig.ini to use the free camera plugin.
PluginData
{
  ....

  Plugins
  {
    OpenGLFreeCamera = ("GLFreeCam/GLFreeCam.dll")
  }
}


3.3) Extension/Version override

The extension override plugin allows the user to add/remove/replace OpenGL
reported extensions. This plugin also allows the overiding of 
version/vender/renderer etc strings.

In addition, the string marker interface extension GL_GREMEDY_string_marker
can be enabled/disabled via this plugin. This extension allows the insertion
of plain text strings inside the GLIntercept log. (eg "Drawing room 1")
This extension is enabled by default and can be toggled via the 
EnableStringMarker option.

See http://oss.sgi.com/projects/ogl-sample/registry/GREMEDY/string_marker.txt 
for full details on this extension.

(see the plugins' config.ini file for full details of options)

Add the following lines to the gliConfig.ini to use the extension override 
plugin.

PluginData
{
  ....

  Plugins
  {
    ExtensionOverride = ("GLExtOverride/GLExtOverride.dll")
    {
      //Override options

    }
  }
}

3.4) Frame Ping

The "ping" plugin will attempt to force a refresh of the 
OpenGL windows if the framerate is too low. This is mostly useful
for OpenGL apps that do not continually update. (ie. only update via
windows "paint" messages.)

Other functionality of GLIntercept requires a continued framerate
to function correctly (ie frame-grabber and shader editor) Use this
plugin when these functionality is required in applications that do
not continually update.

(see the plugins' config.ini file for full details)

Add the following lines to the gliConfig.ini to use the plugin.
PluginData
{
  ....

  Plugins
  {
    PingPlugin = ("GLFramePing/GLFramePing.dll")
    {
      //Other options

    }
  }
}


3.5) Function Stats

The function statistics plugin will write to the GLIntercept log
(gliLog.txt) the call counts for each OpenGL function called (on shutdown).

(see the plugins' config.ini file for full details)

Add the following lines to the gliConfig.ini to use the plugin.
PluginData
{
  ....

  Plugins
  {
    FunctionStats = ("GLFuncStats/GLFuncStats.dll");
    {
      //Other options

    }
  }
}

3.6) Debug Context - debug output

The Debug Context plugin was written to allow a user to force a debug context
in OpenGL 3.0+ applications and report the logging reported by the extension 
ARB_debug_output. Config settings control what messages are reported and where
the messages are logged. There is also an option to issue a debug breakpoint
on each active message logged.

(see the plugins' config.ini file for full details)

Add the following lines to the gliConfig.ini to use the plugin.
PluginData
{
  ....

  Plugins
  {
    DebugContext = ("GLDebugContext/GLDebugContext.dll")
    {
      //Other options controlling debug logging 

    }
  }
} 



3.7) C-Strike

The C-Strike plugin was written to toggle rendering of different parts of
origional Half-Life and CounterStrike games. This was written to "blue screen"
composite game replays for video post production. 

(see the plugins' config.ini file for full details)

Add the following lines to the gliConfig.ini to use the plugin.
PluginData
{
  ....

  Plugins
  {
    OpenGLCStrike = ("GLCStrike/GLCStrike.dll")
    {
      //Other options

    }
  }
}   

============================================================
4) Known bugs

NOTE: GLIntercept will currently not handle multiple render contexts running 
on multiple threads at the same time. See the above note in section 1.

Also note that GLIntercept was written as a OpenGL 1.0-2.x debugger - 
running with a pure context in OpenGL 3.0+ may make some features break.

In GLIntercept, the advanced loggers (ie.image,shader) assume that all 
function calls are valid. For this reason, ensure the application is 
free of OpenGL errors for correct usage of these loggers.

Possible bugs are expected in the advanced loggers if "unusual" work is done
in display lists. (ie texture/shader creation or general operations that will
not get compiled into the list) This is especially true if the "compile and
execute" option is used when creating the display list. 

GLIntercept will also have trouble determining if a display list performs 
render calls if it calls another display list internally to perform the 
render. (This will affect the frame logger)

GLIntercept also relies on a conformant OpenGL implementation. 

Generated xml logs that use xsl will not display correctly in all browsers.
 - Firefox 3.0-13.0 - Works
 - Internet Explorer 9.0 - Works with allowing scripts on popup.
 - Google Chrome - Unable to run local scripts   

  A IEViewLog.hta application has been provided for IE users to use instead 
  of opening the main log file.

============================================================
5) Adding OpenGL extensions

While GLIntercept does cover most common extensions, adding new extensions 
is trivial.

To add a new OpenGL extension, access the GLFunctions folder in the install 
directory and add a file containing the new tokens/functions to the folder. 
The syntax of these files is very close to standard C so reference the 
existing files for syntax. Then add a #include directive to the gliIncludes.h 
file. 

GLIntercept will log any parse errors in the file to the gliLog.txt file next
time you run a OpenGL app. (no re-compiling is necessary)

============================================================
6) Contact

Bug reports/feature requests can be reported at:
https://github.com/dtrebilco/glintercept/issues

Any questions cane be sent to:
dtrebilco@techie.com

This account is usually only checked on weekends so allow about a week if you
expect a reply.

============================================================
7) Disclaimer

  GLIntercept from version 1.0+ uses the MIT License :

  Copyright (c) 2014 Damian Trebilco
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

