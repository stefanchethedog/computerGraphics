
// MFCApplication1View.h : interface of the CMFCApplication1View class
//

#pragma once


class CMFCApplication1View : public CView
{
protected: // create from serialization only
	CMFCApplication1View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication1View)

// Attributes
public:
	CMFCApplication1Doc* GetDocument() const;
	BOOL showGrid = false;
	LONG BLOCK_WIDTH = 25;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DrawBackground(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);
	virtual void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	virtual void DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3);
	virtual void DrawQuadtrilateral(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

// Implementation
public:
	virtual ~CMFCApplication1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in MFCApplication1View.cpp
inline CMFCApplication1Doc* CMFCApplication1View::GetDocument() const
   { return reinterpret_cast<CMFCApplication1Doc*>(m_pDocument); }
#endif

