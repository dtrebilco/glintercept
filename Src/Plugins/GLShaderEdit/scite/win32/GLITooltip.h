
#ifndef __GLI_TOOLTIP_H_
#define __GLI_TOOLTIP_H_

#include "SciTEWin.h"

typedef unsigned int uint;

class SCiTEBase;


//@
//  Summary:
//    This class displays tooltips for the GLI dialog.
//    There can only be one instance of this class.
//    (This class is a bit hacky - Tooltips in raw win32 is complicated)
//  
class GLIToolTip
{
public:

  //@
  //  Summary:
  //    Create the tooltip window for teh specified dialog. 
  //    (only one instance at a time can exist)
  //  
  GLIToolTip(HWND dialogWnd, HINSTANCE hInstance);

  //@
  //  Summary:
  //    Destyroys the tooltip window
  //  
  virtual ~GLIToolTip();


};



#endif // __GLI_TOOLTIP_H_
