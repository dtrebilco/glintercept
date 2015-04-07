
#ifndef __GLI_BITMAP_VIEW_H_
#define __GLI_BITMAP_VIEW_H_

//DT_TODO: Comments, class checks

class GLIBitmapView : public CScrollView
{
public:

	GLIBitmapView();
	virtual ~GLIBitmapView();

  DECLARE_DYNCREATE(GLIBitmapView)

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


// Generated message map functions
protected:
	//{{AFX_MSG(CMfctest_pureView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



#endif // __GLI_BITMAP_VIEW_H_
