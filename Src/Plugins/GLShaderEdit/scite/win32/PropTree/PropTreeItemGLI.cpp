#include "stdafx.h"
#include "PropTree.h"

#include "PropTreeItemGLI.h"


CPropTreeItemGLI::CPropTreeItemGLI():
isRenderColor(false),
renderColor(0)
{
}


CPropTreeItemGLI::~CPropTreeItemGLI()
{
}


void CPropTreeItemGLI::DrawAttribute(CDC* pDC, const RECT& rc)
{
	ASSERT(m_pProp!=NULL);

	CRect r(rc);
	
  //If rendering a color block
  if(isRenderColor)
  {
	  r.top += 1;
	  r.right = r.left + r.Height() - 1;

	  CBrush br(renderColor);
	  CBrush* pold = pDC->SelectObject(&br);
	  pDC->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);
	  pDC->SelectObject(pold);
	  pDC->DrawEdge(&r, EDGE_SUNKEN, BF_RECT);

    //Move the text render area
  	r = rc;
	  r.left += r.Height();
  }

  //Render the text
	pDC->SelectObject(m_pProp->GetNormalFont());
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(m_sAttribute, r, DT_SINGLELINE|DT_VCENTER);
}


// Set the item's attribute value
void CPropTreeItemGLI::SetItemValue(const char* newStr)
{
	m_sAttribute = newStr;
}


///////////////////////////////////////////////////////////////////////////////
//
unsigned char FloatClamp(float value)
{
  //Get the float value in the 0..255 range
  int retValue = (int)(value * 255.0f);

  //Clamp the float value between 0..255
  if(retValue < 0)
  {
    return 0;
  }
  if(retValue > 255)
  {
    return 255;
  }

  return (unsigned char)retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
void CPropTreeItemGLI::SetItemValue(unsigned int intCount, const int *intData, bool isBool)
{
  //Empty the string
  m_sAttribute = "";

  //Add an opening brace
  if(intCount > 1)
  {
    m_sAttribute += "("; 
  }

  //Copy the data
  for(unsigned int i=0; i<intCount; i++)
  {
    CString newStr;

    //If it is a boolean value
    if(isBool)
    {
      if(intData[i] > 0)
      {
        newStr = "true";
      }
      else
      {
        newStr = "false";
      }
    }
    else
    {
      newStr.Format("%d", intData[i]);
    }
    
    //Append to the string
    m_sAttribute += newStr;

    //Add a comma if necessary
    if(i != intCount-1)
    {
      m_sAttribute += ", "; 
    }
  }

  //Add a closing brace
  if(intCount > 1)
  {
    m_sAttribute += ")"; 
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void CPropTreeItemGLI::SetItemValue(unsigned int floatCount, const float *floatData, bool newRenderColor)
{
  //Empty the string
  m_sAttribute = "";

  //Assign if rendering a color
  isRenderColor = newRenderColor;

  //If there is insufficent parameters to render a color
  if(floatCount < 3)
  {
    isRenderColor = false;
  }

  //Add an opening brace
  if(floatCount > 1)
  {
    m_sAttribute += "("; 
  }

  //Copy the data
  for(unsigned int i=0; i<floatCount; i++)
  {
    CString newStr;
    newStr.Format("%g", floatData[i]);
    
    //Ensure there is a trailing .0
    if(newStr.Find('.') < 0 &&
       newStr.Find('e') < 0)
    {
      newStr += ".0";
    }

    //Append to the string
    m_sAttribute += newStr;

    //Add a comma if necessary
    if(i != floatCount-1)
    {
      m_sAttribute += ", "; 
    }
  }

  //Add a closing brace
  if(floatCount > 1)
  {
    m_sAttribute += ")"; 
  }

  //If rendering a color
  if(isRenderColor)
  {
    //Pack the render color
    renderColor =  FloatClamp(floatData[0]) |
                  (FloatClamp(floatData[1]) << 8) |
                  (FloatClamp(floatData[2]) << 16); 
  }

}


