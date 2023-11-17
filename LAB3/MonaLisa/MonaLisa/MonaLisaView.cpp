
// MonaLisaView.cpp : implementation of the CMonaLisaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MonaLisa.h"
#endif

#include "MonaLisaDoc.h"
#include "MonaLisaView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

#include "../../../DImage/DImage.h"

// CMonaLisaView

IMPLEMENT_DYNCREATE(CMonaLisaView, CView)

BEGIN_MESSAGE_MAP(CMonaLisaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMonaLisaView construction/destruction

CMonaLisaView::CMonaLisaView() noexcept
{	
	dImage = new DImage[9];
	for (int i = 1; i <= 9; i++) {
		std::string s = "./res/mona";
		std::string res = s + std::to_string(i) + ".dib";
		dImage[i-1].Load(CString(res.c_str()));
	}

}

CMonaLisaView::~CMonaLisaView()
{
}

BOOL CMonaLisaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMonaLisaView drawing
void CMonaLisaView::DrawGrid(CDC* pDC) {
	int NUMBER_OF_LINES = 500 / BLOCK_WIDTH;

	CPen gridPen(PS_SOLID, 1, RGB(0, 0, 0));
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


XFORM_RETURN CMonaLisaView::Translate(CDC* pDC, float dX, float dY, bool right) {
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

XFORM_RETURN CMonaLisaView::Rotate(CDC* pDC, float angle, bool right) {
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

XFORM_RETURN CMonaLisaView::Scale(CDC* pDC, float sX, float sY, bool right) {
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

XFORM_RETURN CMonaLisaView::Mirror(CDC* pDC, bool mx, bool my, bool right) {
	XFORM form;
	XFORM_RETURN XFormOld;

	XFormOld.PREV_GRAPHICS_MODE = pDC->GetGraphicsMode();
	BOOL b = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&(XFormOld.OLD_FORM_VALUES));


	form.eDx = 0;
	form.eDy = 0;
	form.eM11 = mx?-1:1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = my?-1:1;

	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return XFormOld;
}

void CMonaLisaView::DrawCoord(CDC* pDC) {
	pDC->MoveTo(-30, 0);
	pDC->LineTo(150, 0);

	pDC->MoveTo(0, -30);
	pDC->LineTo(0, 150);
}

void CMonaLisaView::DrawPiece(CDC* pDC, PIECE_INFO dImageInfo) {
	CBitmap* bmpImage;
	bmpImage = dImage[dImageInfo.order - 1].GetBitmap();

	CBitmap bmpMask;
	BITMAP bm;
	bmpImage->GetBitmap(&bm);

	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	CDC* SrcDC = new CDC();
	SrcDC->CreateCompatibleDC(pDC);
	CDC* DstDC = new CDC();
	DstDC->CreateCompatibleDC(pDC);

	CBitmap* pOldSrcBmp = SrcDC->SelectObject(bmpImage);
	CBitmap* pOldDstBmp = DstDC->SelectObject(&bmpMask);

	//pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCCOPY);   OK OK OK

	COLORREF clrTopLeft = SrcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = SrcDC->SetBkColor(clrTopLeft);
	DstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCCOPY);

	COLORREF clrSaveDstText = SrcDC->SetTextColor(RGB(255, 255, 255));
	SrcDC->SetBkColor(RGB(0, 0, 0));
	SrcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, DstDC, 0, 0, SRCAND);

	//pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCCOPY);

	DstDC->SetTextColor(clrSaveDstText);
	SrcDC->SetBkColor(clrSaveBk);
	SrcDC->SelectObject(pOldSrcBmp);
	DstDC->SelectObject(pOldDstBmp);
	delete SrcDC;
	delete DstDC;

	CDC* MemDC = new CDC();


	MemDC->CreateCompatibleDC(pDC);

	CBitmap* bmpOldT = MemDC->SelectObject(&bmpMask);
	pDC->BitBlt(-dImageInfo.coord_center.x, -dImageInfo.coord_center.y, bm.bmWidth, bm.bmHeight , MemDC,  0, 0, SRCAND);

	BYTE* BUFFER;
	int BUFFER_SIZE = bm.bmWidth * bm.bmHeight * bm.bmBitsPixel / 8;
	BUFFER = new BYTE[BUFFER_SIZE];

	bmpImage->GetBitmapBits(BUFFER_SIZE, BUFFER);
	int gray = 0;

	for (int i = 0; i < BUFFER_SIZE && !dImageInfo.R && !dImageInfo.B && !dImageInfo.G; i++) {
		if (i % 4 == 3) continue;

		gray += BUFFER[i];
		if (i % 4 == 2) {
			if (gray == 0) continue;

			gray = gray / 3 + 64;
			if (gray > 255) gray = 255;

			BUFFER[i - 2] = gray;
			BUFFER[i - 1] = gray;
			BUFFER[i] = gray;

			gray = 0;
		}
	}

	int intensity = 0;
	for (int i = 0; i < BUFFER_SIZE && (dImageInfo.R || dImageInfo.G || dImageInfo.B); i++) {
		if (i % 4 == 3) continue;

		intensity += BUFFER[i];
		if (i % 4 == 2) {
			if (intensity == 0) continue;
			
			intensity = 64 + intensity / 3;
			if (intensity > 255) intensity = 255;

			BUFFER[i - 2] = dImageInfo.B ? intensity : 0;
			BUFFER[i - 1] = dImageInfo.G ? intensity : 0;
			BUFFER[i] = dImageInfo.R ? intensity : 0;

			intensity = 0;
		}
	}
	bmpImage->SetBitmapBits(BUFFER_SIZE, BUFFER);

	MemDC->SelectObject(bmpImage);
	pDC->BitBlt(- dImageInfo.coord_center.x, -dImageInfo.coord_center.y, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCPAINT);
	
	//pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, FinalDC, 0, 0, SRCCOPY);

	MemDC->SelectObject(bmpOldT);
	delete MemDC;

}

void CMonaLisaView::LoadBitmapAndDraw(CDC* pDC, CString path, PIECE_INFO* arrInfo) {
	XFORM old;
	Translate(pDC, 25, 25, false);
	//da zamenim pDC sa memDC

	for (int i = 0; i < 9; i++) {
		int w = dImage[arrInfo[i].order - 1].Width();
		int h = dImage[arrInfo[i].order - 1].Height();
		

		if (i % 3 == 0) {
			pDC->GetWorldTransform(&old);
		}

		XFORM_RETURN beforeRotate = Rotate(pDC, arrInfo[i].angle, false);

		DrawPiece(pDC, arrInfo[i]);

		pDC->SetWorldTransform(&(beforeRotate.OLD_FORM_VALUES));
		//DrawCoord(pDC);

		Translate(pDC, 6 * BLOCK_WIDTH, 0, false);

		if (i % 3 == 2) {
			pDC->SetWorldTransform(&old);
			Translate(pDC, 0, 6 * BLOCK_WIDTH, false);
		}
	}
	//pDC->BitBlt(0,0, 3*w, 3*h, memDC, 0, 0, SRCCOPY);
}

void CMonaLisaView::OnDraw(CDC* pDC)
{
	CMonaLisaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	DrawGrid(pDC);

	PIECE_INFO redosled[9];

	//Rotate(pDC, PI / 2, true);
	//Translate(pDC, 20 * BLOCK_WIDTH, 0, true);

	//Mirror(pDC, true, false, true);
	//Translate(pDC, 20 * BLOCK_WIDTH, 0, true);
	
	redosled[0].order = 1;
	redosled[0].coord_center = CPoint(90, 235);
	redosled[0].angle = PI / 2 + PI / 8 + PI / 64;

	//Tacke centara koord.
	redosled[1].order = 9;
	redosled[1].coord_center = CPoint(32, 86);
	redosled[1].angle = PI / 8 - PI / 32;

	redosled[2].order = 4;
	redosled[2].coord_center = CPoint(170, 42);
	redosled[2].angle = PI/8 - PI / 2 - PI/64 - PI / 128 + PI / 256;
	
	redosled[3].order = 7;
	redosled[3].coord_center = CPoint(145, 240);
	redosled[3].angle = 3 * PI / 4 + PI / 32 + PI / 64 + PI / 128;


	redosled[4].order = 2;
	redosled[4].coord_center = CPoint(100, 35);
	redosled[4].angle = -PI / 8 - PI / 16 + PI / 128 + PI / 256;

	redosled[5].order = 6;
	redosled[5].coord_center = CPoint(26 , 96);
	redosled[5].angle =  PI / 8 + PI / 512;

	redosled[6].order = 5;
	redosled[6].coord_center = CPoint(215, 190);
	redosled[6].angle = PI + PI / 16 + PI / 64;

	redosled[7].order = 3;
	redosled[7].coord_center = CPoint(77, 42);
	redosled[7].angle = -PI / 8 + PI / 64 + PI / 128;


	redosled[8].order = 8;
	redosled[8].coord_center = CPoint(30, 180);
	redosled[8].angle = 3. / 8. * PI + PI / 64 + PI / 256 - PI / 512;

	LoadBitmapAndDraw(pDC,CString("./res/mona1.dib"), redosled);
	// TODO: add draw code for native data here
}


// CMonaLisaView printing

BOOL CMonaLisaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMonaLisaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMonaLisaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMonaLisaView diagnostics

#ifdef _DEBUG
void CMonaLisaView::AssertValid() const
{
	CView::AssertValid();
}

void CMonaLisaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMonaLisaDoc* CMonaLisaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonaLisaDoc)));
	return (CMonaLisaDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonaLisaView message handlers
