
// MonaLisaView.h : interface of the CMonaLisaView class
//

#pragma once

typedef struct _XFORM_RETURN {
	XFORM OLD_FORM_VALUES;
	int PREV_GRAPHICS_MODE;
} XFORM_RETURN;

typedef struct _PIECE_INFO {
	int order;
	CPoint coord_center;
	float angle = 0;
	bool mirror = false;
	bool R = false;
	bool G = false;
	bool B = false;
} PIECE_INFO;

class DImage;

class CMonaLisaView : public CView
{
protected: // create from serialization only
	CMonaLisaView() noexcept;
	DECLARE_DYNCREATE(CMonaLisaView)

// Attributes
protected:
	DImage* dImage;

public:
	CMonaLisaDoc* GetDocument() const;

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

protected:
	INT BLOCK_WIDTH = 25;
	FLOAT CONST PI = 3.1415926;
	
protected:
	XFORM_RETURN Translate(CDC* pDC, float dX, float dY, bool right);
	XFORM_RETURN Scale(CDC* pDC, float sX, float sY, bool right);
	XFORM_RETURN Rotate(CDC* pDC, float angle, bool right);
	XFORM_RETURN Mirror(CDC* pDC, bool mx, bool my, bool right);

	void DrawPiece(CDC* pDC, PIECE_INFO dImageInfo);
	void DrawGrid(CDC* pDC);
	void LoadBitmapAndDraw(CDC* pDC, CString path, PIECE_INFO* arrInfo);
	void DrawCoord(CDC* pDC);
// Implementation
public:
	virtual ~CMonaLisaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MonaLisaView.cpp
inline CMonaLisaDoc* CMonaLisaView::GetDocument() const
   { return reinterpret_cast<CMonaLisaDoc*>(m_pDocument); }
#endif

