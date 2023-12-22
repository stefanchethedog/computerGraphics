
// OsvetljenjeView.cpp : implementation of the COsvetljenjeView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Osvetljenje.h"
#endif

#include "OsvetljenjeDoc.h"
#include "OsvetljenjeView.h"

#include "CGLRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COsvetljenjeView

IMPLEMENT_DYNCREATE(COsvetljenjeView, CView)

BEGIN_MESSAGE_MAP(COsvetljenjeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// COsvetljenjeView construction/destruction

COsvetljenjeView::COsvetljenjeView() noexcept
{
	// TODO: add construction code here

}

COsvetljenjeView::~COsvetljenjeView()
{
}

BOOL COsvetljenjeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COsvetljenjeView drawing

void COsvetljenjeView::OnDraw(CDC* pDC)
{
	COsvetljenjeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}


// COsvetljenjeView printing

BOOL COsvetljenjeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COsvetljenjeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COsvetljenjeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COsvetljenjeView diagnostics

#ifdef _DEBUG
void COsvetljenjeView::AssertValid() const
{
	CView::AssertValid();
}

void COsvetljenjeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COsvetljenjeDoc* COsvetljenjeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COsvetljenjeDoc)));
	return (COsvetljenjeDoc*)m_pDocument;
}
#endif //_DEBUG

int COsvetljenjeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}

void COsvetljenjeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);

}

BOOL COsvetljenjeView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void COsvetljenjeView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}



void COsvetljenjeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void COsvetljenjeView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!moving) return;

	m_glRenderer.SetCurrCoord(point.x, point.y);

	Invalidate();

	CView::OnMouseMove(nFlags, point);
}


void COsvetljenjeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	moving = false;
	m_glRenderer.SetMoving(moving);
	m_glRenderer.SaveSceneAngle();

	CView::OnLButtonUp(nFlags, point);
}


void COsvetljenjeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	moving = true;
	m_glRenderer.SetMoving(moving);
	m_glRenderer.SetStartCoord(point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}


// COsvetljenjeView message handlers
