#define WINVER 0x0500 

#include <afxwin.h>
#include <afxext.h>

#include "GLIBitmapView.h"


IMPLEMENT_DYNCREATE(GLIBitmapView, CScrollView)

BEGIN_MESSAGE_MAP(GLIBitmapView, CScrollView)
	//{{AFX_MSG_MAP(CMfctest_pureView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//
GLIBitmapView::GLIBitmapView()
{
	// TODO: add construction code here

}

///////////////////////////////////////////////////////////////////////////////
//
GLIBitmapView::~GLIBitmapView()
{
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL GLIBitmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnDraw(CDC* pDC)
{
	//CMfctest_pureDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

