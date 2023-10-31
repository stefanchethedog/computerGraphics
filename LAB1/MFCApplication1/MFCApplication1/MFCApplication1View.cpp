
// MFCApplication1View.cpp : implementation of the CMFCApplication1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <list>


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMFCApplication1View construction/destruction


CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: add construction code here

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CMFCApplication1View::DrawBackground(CDC* pDC) {
	CBrush backgroundBrush(RGB(220,220,220));
	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);
	CPen gridPen(PS_SOLID, 1, RGB(210, 210, 210));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

}

void CMFCApplication1View::DrawGrid(CDC* pDC) {
	int NUMBER_OF_SQUARES = 20;
	int WIDTH_OF_SQUARE_IN_PIXELS = 500 / 20;
	int NUMBER_OF_LINES = NUMBER_OF_SQUARES - 1;

	CPen gridPen(PS_SOLID, 1, RGB(240, 240, 240));
	CPen* oldPen = pDC->SelectObject(&gridPen);


	for (int COLUMN = 0; COLUMN < NUMBER_OF_LINES; COLUMN++) {
		int x1 = (COLUMN + 1) * WIDTH_OF_SQUARE_IN_PIXELS;
		int y1 = 1;
		int x2 = x1;
		int y2 = 499;

		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}

	for (int ROW = 0; ROW < NUMBER_OF_LINES; ROW++) {
		int y1 = (ROW + 1) * WIDTH_OF_SQUARE_IN_PIXELS;
		int x1 = 1;
		int y2 = y1;
		int x2 = 499;

		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}
	
	pDC->SelectObject(oldPen);
}

void CMFCApplication1View::DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3) {
	CPoint p1(x1, y1);
	CPoint p2(x2, y2);
	CPoint p3(x3, y3);

	POINT niz[] = { p1, p2, p3 };
	pDC->Polygon(niz,3);
}

void CMFCApplication1View::DrawQuadtrilateral(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {

	CPoint p1(x1, y1);
	CPoint p2(x2, y2);
	CPoint p3(x3, y3);
	CPoint p4(x4, y4);

	POINT niz[] = { p1,p2,p3,p4 };

	pDC->Polygon(niz, 4);
}

void CMFCApplication1View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle = 0.0) {
	float DEFAULT_ANGLE = 2*M_PI / n;
	float CURRENT_ANGLE = DEFAULT_ANGLE + rotAngle;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = 0x00FF00FF;

	CPen borderPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 3, &logBrush);
	CPen* oldPen = pDC->SelectObject(&borderPen);

	CPoint* POINTS_LIST = new CPoint[n + 1];

	for (int i = 0; i < n + 1; i++) {
		double cosinus = cos(CURRENT_ANGLE);
		double sinus = sin(CURRENT_ANGLE);
		CPoint tmpPoint(cx + r * cosinus, cy + r * sinus);
		POINTS_LIST[i] = tmpPoint;
		CURRENT_ANGLE += DEFAULT_ANGLE;
	}

	pDC->Polyline(POINTS_LIST, n + 1);

	pDC->SelectObject(oldPen);
}

// CMFCApplication1View drawing

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBackground(pDC);

	int FIGURE_BORDER_COLOR = 0x00FF00FF;
	int PURPLE_SQUARE_COLOR = RGB(153, 0, 204);
	int YELLOW_TRIANGLE_COLOR = RGB(255, 255, 0);
	int ORANGE_TRIANGLE_COLOR = RGB(255, 153, 51);
	int RED_TRIANGLE_COLOR = RGB(255, 0, 0);
	int GREEN_TRIANGLE_COLOR = RGB(50, 205, 50);
	int DELT_PINK_COLOR = RGB(255, 153, 204);

	LOGBRUSH logBrush;

	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = FIGURE_BORDER_COLOR;

	CPen borderPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 5, &logBrush);
	CPen* oldPen = pDC->SelectObject(&borderPen);
	
	/**
	*  PURPLE SQUARE
	**/
	CBrush* newBrush = new CBrush(PURPLE_SQUARE_COLOR);
	CBrush* oldBrush = pDC->SelectObject(newBrush);
	pDC->Rectangle(3.25 * 25, 25, 7.9 * 25, 5.375 * 25);

	/**
	*  SRAFIRANI BELI
	**/
	delete newBrush;
	newBrush = new CBrush(HS_DIAGCROSS, RGB(102, 153, 255));
	pDC->SelectObject(newBrush);
	DrawTriangle(pDC, 7.9 * 25, 25,7.9 * 25, 10 * 25, 16.9 * 25, 10 * 25);

	/**
	*  YELLOW TRIANGLE
	**/
	delete newBrush;
	newBrush = new CBrush(YELLOW_TRIANGLE_COLOR);
	pDC->SelectObject(newBrush);
	DrawTriangle(pDC, 3.25 * 25, 5.375 * 25, 7.9 * 25, 5.375 * 25, 3.25 * 25, 10 * 25);

	/**
	*  ORANGE TRIANGLE
	**/
	delete newBrush;
	newBrush = new CBrush(ORANGE_TRIANGLE_COLOR);
	pDC->SelectObject(newBrush);
	DrawTriangle(pDC, 7.9 * 25, 5.375 * 25, 3.25 * 25, 10 * 25, 7.9 * 25, 14.675 * 25);

	/**
	*  RED TRIANGLE
	**/
	delete newBrush;
	newBrush = new CBrush(RED_TRIANGLE_COLOR);
	pDC->SelectObject(newBrush);
	DrawTriangle(pDC, 7.9 * 25, 10 * 25, 7.9 * 25, 14.675 * 25, 13 * 25, 14.675 * 25);


	/**
	*  GREEN TRIANGLE
	**/
	delete newBrush;
	newBrush = new CBrush(GREEN_TRIANGLE_COLOR);
	pDC->SelectObject(newBrush);
	DrawTriangle(pDC,7.9 * 25, 10 * 25, 16.9 * 25, 10 * 25, 16.9 * 25, 19 * 25 );

	/**
	*  PINK DELT
	**/
	delete newBrush;
	newBrush = new CBrush(DELT_PINK_COLOR);
	pDC->SelectObject(newBrush);
	DrawQuadtrilateral(pDC, 7.9 * 25, 14.675 * 25, 12.5 * 25, 14.675 * 25, 16.9 * 25, 19 * 25,  12.25 * 25 ,19*25);
	

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	DrawRegularPolygon(pDC,10.5 * 25 , 7.5 * 25 , 1.5  *25,4);
	DrawRegularPolygon(pDC, 4.5 * 25, 6.75 * 25, 0.75 * 25, 8);
	DrawRegularPolygon(pDC, 6 * 25, 10 * 25, 25, 6);
	DrawRegularPolygon(pDC, 14.25 * 25, 12.5 * 25, 1.5 * 25, 5);
	DrawRegularPolygon(pDC, 9.25*25 , 13.25 * 25, 0.75 * 25, 7);



	if (ShowGrid) DrawGrid(pDC);
}


// CMFCApplication1View printing

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCApplication1View diagnostics

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View message handlers


void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar != VK_SPACE) return;

	ShowGrid = !ShowGrid;
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
