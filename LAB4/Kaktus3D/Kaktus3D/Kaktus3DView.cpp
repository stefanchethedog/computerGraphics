
// Kaktus3DView.cpp : implementation of the CKaktus3DView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kaktus3D.h"
#endif

#include "Kaktus3DDoc.h"
#include "Kaktus3DView.h"

#include "CGLRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKaktus3DView

IMPLEMENT_DYNCREATE(CKaktus3DView, CView)

BEGIN_MESSAGE_MAP(CKaktus3DView, CView)
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
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKaktus3DView construction/destruction

CKaktus3DView::CKaktus3DView() noexcept
{
	// TODO: add construction code here

}

CKaktus3DView::~CKaktus3DView()
{
}

BOOL CKaktus3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKaktus3DView drawing

void CKaktus3DView::OnDraw(CDC* pDC)
{
	CKaktus3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CKaktus3DView printing

BOOL CKaktus3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKaktus3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKaktus3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKaktus3DView diagnostics

#ifdef _DEBUG
void CKaktus3DView::AssertValid() const
{
	CView::AssertValid();
}

void CKaktus3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKaktus3DDoc* CKaktus3DView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKaktus3DDoc)));
	return (CKaktus3DDoc*)m_pDocument;
}
#endif //_DEBUG


// CKaktus3DView message handlers


int CKaktus3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}

void CKaktus3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

BOOL CKaktus3DView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}




void CKaktus3DView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void CKaktus3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CKaktus3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!moving) return;

	m_glRenderer.SetCurrCoord(point.x, point.y);

	Invalidate();

	CView::OnMouseMove(nFlags, point);
}


void CKaktus3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	moving = false;
	m_glRenderer.SetMoving(moving);
	m_glRenderer.SaveSceneAngle();

	CView::OnLButtonUp(nFlags, point);
}


void CKaktus3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	moving = true;
	m_glRenderer.SetMoving(moving);
	m_glRenderer.SetStartCoord(point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}


void CKaktus3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool flag = false;
	if (nChar == VK_LEFT) {
		m_glRenderer.SetAngle(10);
		flag = true;
	}
	if (nChar == VK_RIGHT) {
		m_glRenderer.SetAngle(-10);
		flag = true;
	}
	if (!flag) return;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
