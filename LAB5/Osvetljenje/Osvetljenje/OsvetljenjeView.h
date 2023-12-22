
// OsvetljenjeView.h : interface of the COsvetljenjeView class
//

#pragma once
#include "CGLRenderer.h"

class COsvetljenjeView : public CView
{
protected: // create from serialization only
	COsvetljenjeView() noexcept;
	DECLARE_DYNCREATE(COsvetljenjeView)

// Attributes
public:
	COsvetljenjeDoc* GetDocument() const;
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
	virtual ~COsvetljenjeView();
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
};

#ifndef _DEBUG  // debug version in OsvetljenjeView.cpp
inline COsvetljenjeDoc* COsvetljenjeView::GetDocument() const
   { return reinterpret_cast<COsvetljenjeDoc*>(m_pDocument); }
#endif

