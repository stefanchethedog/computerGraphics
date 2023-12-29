
// RG_LAB4_PRIPREMAView.h : interface of the CRGLAB6View class
//

#pragma once

#include "CGLRenderer.h"


class CRGLAB6View : public CView
{
protected: // create from serialization only
	CRGLAB6View() noexcept;
	DECLARE_DYNCREATE(CRGLAB6View)

// Attributes
public:
	CRGLAB6Doc* GetDocument() const;
	CGLRenderer* m_glRenderer;
	POINT mousePosition = { 0, 0 };

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
	virtual ~CRGLAB6View();
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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in RG_LAB4_PRIPREMAView.cpp
inline CRGLAB6Doc* CRGLAB6View::GetDocument() const
   { return reinterpret_cast<CRGLAB6Doc*>(m_pDocument); }
#endif

