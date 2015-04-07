
#ifndef __GLI_SHADER_DEBUG_H_
#define __GLI_SHADER_DEBUG_H_

class SciTEBase;
class CPropTree;
class CPropTreeItem;
class CPropTreeItemGLI;

#include <vector>

using namespace std;

//DT_TODO: Comments, class checks

//The states the debugger can be in
enum GLIDebugState
{
  DBS_None,   //No debugging state
  DBS_Init,   //Debugging is init, but no pixel/vertex has been selected
  DBS_Pixel,  //A debugging pixel has been selected

};

//@
//  Summary:
//    This structure contains the data saved from the frame buffer.
//  
struct FrameBufferData
{
  //@
  //  Summary:
  //    The default constructor.
  //  
  FrameBufferData();

  int  bufferType;           // The type of buffer the data comes from (eg. the RGBA or depth component)
  int  drawBuffer;           // If a specific draw buffer is used (eg. multiple color attachments)

  uint bufferWidth;          // The width of the buffer
  uint bufferHeight;         // The height of the buffer
  uint numPixelValues;       // The number of float values in a single pixel 

  vector<float> preBuffer;   // The pre-render buffer
  vector<float> postBuffer;  // The post-render buffer
};
typedef vector<FrameBufferData> FrameBufferDataArray;


//@
//  Summary:
//    This class provides a interface to the shader debug dialog.
//  
class GLIShaderDebug
{
public:

  //@
  //  Summary:
  //    To initialize the main window etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  static bool Init(HINSTANCE hInstance, HWND parentWnd, SciTEBase *scite);

  //@
  //  Summary:
  //    To get the shader data instance (if any).
  //  
  //  Returns:
  //    If a instance is available it is returned.
  //
  static GLIShaderDebug *GetInstance();

  //@
  //  Summary:
  //    Destructor, only to be called internally
  //  
  virtual ~GLIShaderDebug();

  //@
  //  Summary:
  //    To initialize the property control on dialog startup
  // 
  void InitPropertyControl(HWND initDialogWindow);

  //@
  //  Summary:
  //    To start debugging mode with the provided network data.
  //  
  //  Parameters:
  //    bufferSize  - The size of the provided buffer.
  //
  //    buffer      - The network buffer.
  //
  //  Returns:
  //    If a debug mode switch was successful, true is returned. Else false
  //    is returned.
  //
  bool DebugBegin(uint bufferSize, void *buffer);

  //@
  //  Summary:
  //    To end the debug mode.
  //  
  //  Returns:
  //    If debug mode has exited, true is returned. Else false
  //    is returned.
  //
  bool DebugEnd();

protected:

  //Structure to hold data about the uniforms in the program
  struct UniformData
  {
    UniformData(); 

    string name;            // The name of the uniform
    uint type;              // The type of the uniform
    uint numTypeElements;   // The number of elements in the type 
    bool isFloatType;       // Flag indicating a float or int based type 

    vector<int>   intUniformData;   // The float uniform data (if a float type)
    vector<float> floatUniformData; // The int uniform data   (if a int type)
  };
  typedef vector<UniformData> UniformDataArray;


  SciTEBase   *scite;                             // Handle to the scite instance
  HWND         dialogWindow;                      // The handle to the dialog window

  CPropTree   *propTree;                          // The property tree handle

  GLIDebugState debugState;                       // The state of the debugger
  
  uint          debugUID;                         // The current debug shader unique ID
  uint          debugGLID;                        // The current debug shader OpenGL ID
  string        debugSource;                      // The current debug shader source

  UniformDataArray      uniformValues;            // The uniform data array
  FrameBufferDataArray  frameBufferArray;         // The frame buffer data array


  //@
  //  Summary:
  //    Constructor
  //  
  GLIShaderDebug(SciTEBase *scite);

  //@
  //  Summary:
  //    To update the display of uniforms/watch values with the current data.
  //  
  void UpdateVariableDisplay();

  //@
  //  Summary:
  //    To open (or bring to focus) the current shader that is being debugged 
  //    in the main scite window.
  //  
  void OpenDebugSource();

  //@
  //  Summary:
  //    To add the passed uniform data to the passed display tree.
  //  
  //  Parameters:
  //    pRoot  - The display root to add the data to.
  //
  //    uniformValue - The uniform data to display.
  //
  void AddVariableDisplay(CPropTreeItem* pRoot, const UniformData &uniformValue);

  //@
  //  Summary:
  //    To add the passed matrix uniform data to the passed display tree.
  //  
  //  Parameters:
  //    pRoot  - The display root to add the matrix data to.
  //
  //    uniformValue - The uniform data to display.
  //
  //    arrayOffset  - The offset into the uniform data to extract the matrix value from.
  //
  void AddVariableMatrixDisplay(CPropTreeItemGLI* pRoot, const UniformData &uniformValue, uint arrayOffset);
};



#endif // __GLI_SHADER_DEBUG_H_
