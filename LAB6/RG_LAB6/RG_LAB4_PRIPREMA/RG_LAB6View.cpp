
// RG_LAB4_PRIPREMAView.cpp : implementation of the CRGLAB6View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RG_LAB6.h"
#endif

#include "RG_LAB6Doc.h"
#include "RG_LAB6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592653589793238
#define DOUBLE_PI 6.2831853071795864
#define TO_RAD 0.017453292519943295
#define TO_DEG 57.29577951308232087


// CRGLAB6View

IMPLEMENT_DYNCREATE(CRGLAB6View, CView)

BEGIN_MESSAGE_MAP(CRGLAB6View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CRGLAB6View construction/destruction

CRGLAB6View::CRGLAB6View() noexcept
{
	m_glRenderer = new CGLRenderer();
}

CRGLAB6View::~CRGLAB6View()
{
	if (m_glRenderer != NULL)
	{
		delete m_glRenderer;
		m_glRenderer = NULL;
	}
}

BOOL CRGLAB6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGLAB6View drawing

void CRGLAB6View::OnDraw(CDC* pDC)
{
	CRGLAB6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer->DrawSceen(pDC);
}


// CRGLAB6View printing

BOOL CRGLAB6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGLAB6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGLAB6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGLAB6View diagnostics

#ifdef _DEBUG
void CRGLAB6View::AssertValid() const
{
	CView::AssertValid();
}

void CRGLAB6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGLAB6Doc* CRGLAB6View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLAB6Doc)));
	return (CRGLAB6Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGLAB6View message handlers


int CRGLAB6View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer->CrateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL CRGLAB6View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CRGLAB6View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer->Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CRGLAB6View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer->DestroyScene(pDC);
	ReleaseDC(pDC);
}


void CRGLAB6View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer->PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CRGLAB6View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) 
	{
		m_glRenderer->m_rotAngle -= 10;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		m_glRenderer->m_rotAngle += 10;
		Invalidate();
	}
	if (nChar == 'A') 
	{
		m_glRenderer->m_viewBeta += 10/*2 * PI / 180*/;
		Invalidate();
	}
	if (nChar == 'D') 
	{
		m_glRenderer->m_viewBeta -= 10;
		Invalidate();
	}
	if (nChar == 'W') 
	{
		m_glRenderer->m_viewAlpha += 10;
		Invalidate();
	}
	if (nChar == 'S') 
	{
		m_glRenderer->m_viewAlpha -= 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRGLAB6View::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON) 
	{
		double moveX = point.x > mousePosition.x ? 1 : -1;
		double moveY = point.y > mousePosition.y ? 1 : -1;
		if (abs(point.x - mousePosition.x) > abs(point.y - mousePosition.y))
			moveY /= 10;
		else
			moveX /= 10;
		m_glRenderer->m_viewBeta += moveX;
		m_glRenderer->m_viewAlpha += moveY;
		mousePosition = point;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
