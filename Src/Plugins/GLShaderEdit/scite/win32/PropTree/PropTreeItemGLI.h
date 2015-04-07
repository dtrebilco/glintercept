
#ifndef _PROPTREEITEMGLI_H
#define _PROPTREEITEMGLI_H

#include "PropTreeItemStatic.h"

class PROPTREE_API CPropTreeItemGLI : public CPropTreeItemStatic
{
public:
	CPropTreeItemGLI();
	virtual ~CPropTreeItemGLI();

public:
	// The attribute area needs drawing
	virtual void DrawAttribute(CDC* pDC, const RECT& rc);

	// Set the item's attribute value
	virtual void SetItemValue(const char* newStr);
  void SetItemValue(unsigned int floatCount, const float *floatData, bool newRenderColor);
  void SetItemValue(unsigned int intCount, const int *intData, bool isBool);

protected:

  bool     isRenderColor;      // Flag indicating if a render color is used
  COLORREF renderColor;        // The color to render if rendering a color
};


#endif // _PROPTREEITEMGLI_H
