
// StefanovKaktusView.h : interface of the CStefanovKaktusView class
//

#pragma once

typedef struct _XFORM_RETURN {
	XFORM OLD_FORM_VALUES;
	int PREV_GRAPHICS_MODE;
} XFORM_RETURN;
struct CACTUS_NODE;

struct CACTUS {
	CACTUS_NODE* START;

	CACTUS( CACTUS_NODE* STARTING_NODE) {
		START = STARTING_NODE;
	}
};

struct CACTUS_NODE {
	int NUM_OF_EDGES;
	int ROTATING_EDGE;
	FLOAT CX;
	FLOAT CY;
	CACTUS_NODE** CHILDREN;
	CString* NAME;

	CACTUS_NODE(int n, int r, CString name): CX(0.0), CY(0.0) {
		NUM_OF_EDGES = n;
		ROTATING_EDGE = r;
		NAME = new CString(name);

		CHILDREN = new CACTUS_NODE * [n];
		for (int i = 0; i < n; i++) {
			CHILDREN[i] = NULL;
		}
	}
	void setChildren(int k, CACTUS_NODE* CHILD) {
		if (k < 0 && k >= NUM_OF_EDGES) return;
		CHILDREN[k] = CHILD;
	}
};

struct TAKEN {
	CString NAME;
	int BRANCH_ID;
};

class CStefanovKaktusView : public CView
{
protected: // create from serialization only
	CStefanovKaktusView() noexcept;
	DECLARE_DYNCREATE(CStefanovKaktusView)

	// Attributes
public:
	CStefanovKaktusDoc* GetDocument() const;

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

	FLOAT PI = 3.1415926;
	BOOL showGrid = false;
	FLOAT WHOLE_CACTUS_ANGLE = 0;
	FLOAT BRANCH_ANGLE = 0;
	TAKEN TAKEN_BY;

	HENHMETAFILE CACTUS_REGULAR_META;
	HENHMETAFILE CACTUS_SELECTED_META;

protected:
	XFORM_RETURN Translate(CDC* pDC, float dX, float dY, bool right);
	XFORM_RETURN Scale(CDC* pDC, float sX, float sY, bool right);
	XFORM_RETURN Rotate(CDC* pDC, float angle, bool right);

	void DrawAxes(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void DrawCircle(CDC* pDC);
	
	void DrawName(CDC* pDC);
	void DrawPot(CDC* pDC);
	
	void DrawSubtree(CDC* pDC, CACTUS_NODE* NODE);
	void DrawTree(CDC* pDC, CACTUS NODE);
	void DrawFigure(CDC* pDC);
	// Implementation
public:
	virtual ~CStefanovKaktusView();
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

#ifndef _DEBUG  // debug version in StefanovKaktusView.cpp
inline CStefanovKaktusDoc* CStefanovKaktusView::GetDocument() const
{
	return reinterpret_cast<CStefanovKaktusDoc*>(m_pDocument);
}
#endif

