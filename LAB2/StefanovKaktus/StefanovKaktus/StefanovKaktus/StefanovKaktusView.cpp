
// StefanovKaktusView.cpp : implementation of the CStefanovKaktusView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "StefanovKaktus.h"
#endif

#include "StefanovKaktusDoc.h"
#include "StefanovKaktusView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "math.h"
// CStefanovKaktusView

IMPLEMENT_DYNCREATE(CStefanovKaktusView, CView)

BEGIN_MESSAGE_MAP(CStefanovKaktusView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CStefanovKaktusView construction/destruction

CStefanovKaktusView::CStefanovKaktusView() noexcept
{
	CACTUS_REGULAR_META = GetEnhMetaFile(_T("./res/cactus_part.emf"));
	CACTUS_SELECTED_META = GetEnhMetaFile(_T("./res/cactus_part_light.emf"));
	TAKEN_BY.BRANCH_ID = -1;
	TAKEN_BY.NAME = "";
}

CStefanovKaktusView::~CStefanovKaktusView()
{
}

BOOL CStefanovKaktusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CStefanovKaktusView drawing
XFORM_RETURN CStefanovKaktusView::Translate(CDC* pDC, float dX, float dY, bool right) {
	XFORM form;
	XFORM_RETURN XFormOld;

	XFormOld.PREV_GRAPHICS_MODE = pDC->GetGraphicsMode();
	BOOL b = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&(XFormOld.OLD_FORM_VALUES));

	form.eDx = dX;
	form.eDy = dY;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;

	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return XFormOld;
}

XFORM_RETURN CStefanovKaktusView::Rotate(CDC* pDC, float angle, bool right) {
	XFORM form;
	XFORM_RETURN XFormOld;

	XFormOld.PREV_GRAPHICS_MODE = pDC->GetGraphicsMode();
	BOOL b = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&(XFormOld.OLD_FORM_VALUES));

	FLOAT SINUS = sin(angle);
	FLOAT COSINUS = cos(angle);

	form.eDx = 0;
	form.eDy = 0;
	form.eM11 = COSINUS;
	form.eM12 = SINUS;
	form.eM21 = -1 * SINUS;
	form.eM22 = COSINUS;

	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return XFormOld;
}

XFORM_RETURN CStefanovKaktusView::Scale(CDC* pDC, float sX, float sY, bool right) {
	XFORM form;
	XFORM_RETURN XFormOld;

	XFormOld.PREV_GRAPHICS_MODE = pDC->GetGraphicsMode();
	BOOL b = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&(XFormOld.OLD_FORM_VALUES));


	form.eDx = 0;
	form.eDy = 0;
	form.eM11 = sX;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = sY;

	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return XFormOld;
}

void CStefanovKaktusView::DrawAxes(CDC* pDC) {
	pDC->MoveTo(-1 * BLOCK_WIDTH, 0);
	pDC->LineTo(5 * BLOCK_WIDTH, 0);

	pDC->MoveTo(0, -1 * BLOCK_WIDTH);
	pDC->LineTo(0, 5 * BLOCK_WIDTH);
}

void CStefanovKaktusView::DrawCircle(CDC* pDC) {
	CBrush brush(RGB(0, 200, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Ellipse(-10, -10, 10, 10);

	pDC->SelectObject(oldBrush);
}

void CStefanovKaktusView::DrawGrid(CDC* pDC) {
	int NUMBER_OF_LINES = 500 / BLOCK_WIDTH;

	CPen gridPen(PS_SOLID, 1, RGB(240, 240, 240));
	CPen* oldPen = pDC->SelectObject(&gridPen);


	for (int COLUMN = 0; COLUMN < NUMBER_OF_LINES; COLUMN++) {
		int x1 = (COLUMN + 1) * BLOCK_WIDTH;
		int y1 = 1;
		int x2 = x1;
		int y2 = 499;

		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}

	for (int ROW = 0; ROW < NUMBER_OF_LINES; ROW++) {
		int y1 = (ROW + 1) * BLOCK_WIDTH;
		int x1 = 1;
		int y2 = y1;
		int x2 = 499;

		pDC->MoveTo(x1, y1);
		pDC->LineTo(x2, y2);
	}

	pDC->SelectObject(oldPen);
}

void CStefanovKaktusView::DrawBackground(CDC* pDC) {
	CBrush newBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldBrush);
}

void CStefanovKaktusView::DrawPot(CDC* pDC) {
	CBrush newBrush(RGB(222, 148, 0));
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	pDC->Rectangle(7.5 * BLOCK_WIDTH, 2 * BLOCK_WIDTH, 12.5 * BLOCK_WIDTH, 2.8 * BLOCK_WIDTH);
	
	POINT pts[4] = {
		CPoint(8.2 * BLOCK_WIDTH,0),
		CPoint(11.8 * BLOCK_WIDTH, 0),
		CPoint(12 * BLOCK_WIDTH, 2 * BLOCK_WIDTH),
		CPoint(8 * BLOCK_WIDTH, 2 * BLOCK_WIDTH)
	};
	pDC->Polygon(pts, 4);

	pDC->SelectObject(oldBrush);
}

void CStefanovKaktusView::DrawName(CDC* pDC) {
	CFont forecolor_font;
	forecolor_font.CreateFontW(1.4 * BLOCK_WIDTH, 0, 1800, 1800, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

	CFont shadow_font;
	shadow_font.CreateFontW(1.4 * BLOCK_WIDTH, 0, 1800, 1800, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SelectObject(&forecolor_font);

	CString BR_INDEKSA_IME_PREZIME("18395 Stefan Stankovic");
	TextOut(pDC->m_hDC, 18.8 * BLOCK_WIDTH, 18.95 * BLOCK_WIDTH, BR_INDEKSA_IME_PREZIME, BR_INDEKSA_IME_PREZIME.GetLength());

	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SelectObject(&forecolor_font);

	TextOut(pDC->m_hDC, 18.9 * BLOCK_WIDTH, 18.9 * BLOCK_WIDTH, BR_INDEKSA_IME_PREZIME, BR_INDEKSA_IME_PREZIME.GetLength());
}

void CStefanovKaktusView::DrawTree(CDC* pDC, CACTUS NODE) {
	DrawSubtree(pDC, NODE.START);
}

void CStefanovKaktusView::DrawSubtree(CDC* pDC, CACTUS_NODE* NODE) {
	if (!NODE) return;
	Translate(pDC, NODE->CX, NODE->CY, false);

	//DrawAxes(pDC);
	FLOAT INITIAL_ANGLE = PI / 2;
	FLOAT ANGLE_INCREMENT = 0;

	INT32 NUM_OF_EDGES = NODE->NUM_OF_EDGES;
	INT32 ROTATING_EDGE = NODE->ROTATING_EDGE;
	if (NUM_OF_EDGES == 2) {
		ANGLE_INCREMENT = PI * 6 / (7 * (FLOAT)NUM_OF_EDGES);
	}
	if (NUM_OF_EDGES == 3) {
		ANGLE_INCREMENT = PI * 2 / (3 * (FLOAT)NUM_OF_EDGES);
		INITIAL_ANGLE = PI / (FLOAT)NUM_OF_EDGES - PI / 2;
	}

	if (NUM_OF_EDGES == 2) {
		Rotate(pDC, -ANGLE_INCREMENT / 2, false);
	}
	if (NUM_OF_EDGES == 3) {
		Rotate(pDC, -ANGLE_INCREMENT, false);
	}

	float SCALE_UP = 1;
	if (NUM_OF_EDGES == 3) {
		SCALE_UP = 0.4;
	}
	else if (NUM_OF_EDGES == 2) {
		SCALE_UP = 0.7;
	}
	else if (NUM_OF_EDGES == 1) {
		SCALE_UP = 1.2;
	}

	//TODO: solve bug for 4-6 or arrows
	for (int i = 0; i < NODE->NUM_OF_EDGES; i++) {

		XFORM_RETURN old = Translate(pDC, 0, 1.0, false);

		if (i == ROTATING_EDGE && (TAKEN_BY.BRANCH_ID == -1 || TAKEN_BY.NAME == *(NODE->NAME))) {
			Rotate(pDC, WHOLE_CACTUS_ANGLE, false);
			TAKEN_BY.BRANCH_ID = i;
			TAKEN_BY.NAME = *(NODE->NAME);
		}
		else if (i == ROTATING_EDGE) {
			Rotate(pDC, BRANCH_ANGLE, false);
		}

		XFORM_RETURN beforeScale = Scale(pDC, SCALE_UP, 1, false);
		PlayEnhMetaFile(pDC->m_hDC, (i == ROTATING_EDGE) ? CACTUS_SELECTED_META : CACTUS_REGULAR_META, CRect(-25, 0, 25, 70));

		pDC->SetWorldTransform(&(beforeScale.OLD_FORM_VALUES));
		Translate(pDC, 0, -1.0, false);

		if (NODE->CHILDREN[i]) {
			NODE->CHILDREN[i]->CX = 0;
			NODE->CHILDREN[i]->CY = 70;
			DrawSubtree(pDC, NODE->CHILDREN[i]);
		}
		pDC->SetWorldTransform(&(old.OLD_FORM_VALUES));

		Rotate(pDC, ANGLE_INCREMENT, false);
	}

	DrawCircle(pDC);
}
void CStefanovKaktusView::DrawFigure(CDC* pDC){
	/*Layer 0*/
	CACTUS_NODE* CACTY = new CACTUS_NODE(1, -1, CString('0'));

	/*Layer 1*/
	CACTUS_NODE* CENTER_SUBTREE0 = new CACTUS_NODE(2, 1, CString('1'));

	/*Layer 2*/
	CACTUS_NODE* CENTER_SUBTREE1 = new CACTUS_NODE(3, -1, CString('2'));

	/*Layer 3*/
	CACTUS_NODE* CENTER_SUBTREE2 = new CACTUS_NODE(2, -1, CString('3'));
	CACTUS_NODE* RIGHT_SUBTREE2 = new CACTUS_NODE(3, 1, CString('4'));

	CACTY->setChildren(0, CENTER_SUBTREE0);

	CENTER_SUBTREE0->setChildren(1, CENTER_SUBTREE1);

	CENTER_SUBTREE1->setChildren(1, CENTER_SUBTREE2);
	CENTER_SUBTREE1->setChildren(2, RIGHT_SUBTREE2);

	CACTUS KAKTUS(CACTY);
	CACTY->CX = 250;
	CACTY->CY = 3 * BLOCK_WIDTH;

	DrawTree(pDC, KAKTUS);
}

void CStefanovKaktusView::OnDraw(CDC* pDC)
{
	CStefanovKaktusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*Rotate(pDC, PI , true);
	Translate(pDC, 500, 500, true);*/

	DrawBackground(pDC);
	
	XFORM beforeAnyTransformations;
	pDC->GetWorldTransform(&beforeAnyTransformations);

	DrawFigure(pDC);
	
	pDC->SetWorldTransform(&beforeAnyTransformations);
	
	DrawPot(pDC);

	if (showGrid) DrawGrid(pDC);	//TODO: implement this function

	//Fontovi
	DrawName(pDC);
}


// CStefanovKaktusView printing

BOOL CStefanovKaktusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStefanovKaktusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStefanovKaktusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CStefanovKaktusView diagnostics

#ifdef _DEBUG
void CStefanovKaktusView::AssertValid() const
{
	CView::AssertValid();
}

void CStefanovKaktusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStefanovKaktusDoc* CStefanovKaktusView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStefanovKaktusDoc)));
	return (CStefanovKaktusDoc*)m_pDocument;
}
#endif //_DEBUG


// CStefanovKaktusView message handlers


void CStefanovKaktusView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar != VK_SPACE && nChar != VK_LEFT && nChar != VK_RIGHT && nChar != VK_NUMPAD4 && nChar != VK_NUMPAD6) return;

	if (nChar == VK_SPACE) showGrid = !showGrid;

	if (nChar == VK_LEFT) WHOLE_CACTUS_ANGLE -= PI / 32;
	if (nChar == VK_RIGHT) WHOLE_CACTUS_ANGLE += PI / 32;

	if (nChar == VK_NUMPAD4) BRANCH_ANGLE -= PI / 32;
	if (nChar == VK_NUMPAD6) BRANCH_ANGLE += PI / 32;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
