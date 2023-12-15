
// Kaktus3DView.h : interface of the CKaktus3DView class
//

#pragma once
#include "CGLRenderer.h"

class CKaktus3DView : public CView
{
protected: // create from serialization only
	CKaktus3DView() noexcept;
	DECLARE_DYNCREATE(CKaktus3DView)

// Attributes
public:
	CKaktus3DDoc* GetDocument() const;
protected:
	CGLRenderer m_glRenderer;

	bool moving = false;
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

// Implementation
public:
	virtual ~CKaktus3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kaktus3DView.cpp
inline CKaktus3DDoc* CKaktus3DView::GetDocument() const
   { return reinterpret_cast<CKaktus3DDoc*>(m_pDocument); }
#endif

