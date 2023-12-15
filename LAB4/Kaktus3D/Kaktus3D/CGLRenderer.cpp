#include "pch.h"
#include "CGLRenderer.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#define DEG_TO_RAD(x) x * 3.1415926 / 180

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//
	glClearColor(0.53, 0.65, 0.95, 0.0);
	//
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	//
	glViewport(0, 0, (GLsizei) w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45., (double)w / (double)h, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	//

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawGrid(int width)
{

	int x0 = -width / 2;
	int x1 = width / 2;

	int z0 = -width / 2;
	int z1 = width / 2;

	int d = -width / 2;
	int korak = width / 10;

	int y = 0;

	glBegin(GL_LINES);
	glColor3f(1., 1., 1.);
	for (int i = 0; i <= 10; i++) {
		glVertex3f(x0, y, d);
		glVertex3f(x1, y, d);
		d += korak;
	}
	d = -width/2;
	for (int i = 0; i <= 10; i++) {
		glVertex3f(d, y, z0);
		glVertex3f(d, y, z1);
		d += korak;
	}
	glEnd();

}

void CGLRenderer::DrawCoord()
{

	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1000.0);

	glEnd();

}

void CGLRenderer::RotateScene(CDC* pDC)
{
	if (!moving) {
		glRotatef(angleX, 1., 0., 0.);
		glRotatef(angleY, 0., 1., 0.);
		return;
	};

	if (startX == currX && startY == currY) return;

	float DeltaAngleX = ((float)currY - (float)startY) / 8.;
	float DeltaAngleY = ((float)currX - (float)startX) / 8.;


	glRotatef(angleX + DeltaAngleX, 1., 0., 0.);
	glRotatef(angleY + DeltaAngleY, 0., 1., 0.);

}

void CGLRenderer::SaveSceneAngle() {
	if (startX == currX && startY == currY) return;

	angleX = angleX + ((float)currY - (float)startY) / 8.;
	angleY = angleY + ((float)currX - (float)startX) / 8.;
}
void CGLRenderer::SetStartCoord(int x, int y)
{
	startX = x;
	startY = y;
}

void CGLRenderer::SetCurrCoord(int x, int y)
{
	currX = x;
	currY = y;
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta) 
{
	float alpha = 90.;
	float beta = 0.;

	for (int segAlpha = 0; segAlpha <= nSegAlpha; segAlpha++) {
		if (segAlpha) alpha -= (float)180 / nSegAlpha;
		float cosinus = cos(DEG_TO_RAD(alpha));
		float sinus = sin(DEG_TO_RAD(alpha));

		glBegin(GL_QUAD_STRIP);
		glColor3f(0., 0.5, 0.);
		for (int segBeta = 0; segBeta <= nSegBeta; segBeta++) {
			if (segBeta) beta += (float)360 / nSegBeta;
			float x0 = r * cosinus * cos(DEG_TO_RAD(beta));
			float y0 = r * sinus;
			float z0 = r * cosinus * sin(DEG_TO_RAD(beta));

			float x1 = r * cos(DEG_TO_RAD(alpha - (float)180 / nSegAlpha)) * cos(DEG_TO_RAD(beta));
			float y1 = r * sin(DEG_TO_RAD(alpha - (float)180 / nSegAlpha));
			float z1 = r * cos(DEG_TO_RAD(alpha - (float)180 / nSegAlpha)) * sin(DEG_TO_RAD(beta));

			glVertex3f(x0, y0, z0);
			glVertex3f(x1, y1, z1);
		}
		glEnd();
	}
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg) {
	double alpha = 0.;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., 0., 0.);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3d(r1 * cos(DEG_TO_RAD(alpha)), 0, r1 * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();

	alpha = 0.;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0., h, 0.);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3d(r2 * cos(DEG_TO_RAD(alpha)), h, r2 * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();

	alpha = 0.;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3d(r1 * cos(DEG_TO_RAD(alpha)), 0, r1 * sin(DEG_TO_RAD(alpha)));
		glVertex3d(r2 * cos(DEG_TO_RAD(alpha)), h, r2 * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg) {
	double alpha = 0.;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, 0, 0);
	while (alpha <= 360) {
		glVertex3d(r * cos(DEG_TO_RAD(alpha)), 0, r * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();

	alpha = 0.;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0, h, 0);
	while (alpha <= 360) {
		glVertex3d(r * cos(DEG_TO_RAD(alpha)), 0, r * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();
}

void CGLRenderer::DrawPot() {
	glColor3f(1.0 * 221 / 255, 1. * 126 / 255, 1. * 40 / 255);
	DrawCylinder(7, 5, 6, 8);
	glTranslatef(0., 7., 0.);

	DrawCylinder(3., 8, 8, 8);
	glTranslatef(0., 3., 0.);
}

void CGLRenderer::DrawCactus(CACTUS_NODE* NODE) {
	if (!NODE) return;

	if (NODE->isRotating)
		glColor3f(1., 1., 0.);
	else
		glColor3f(0., 1., 0.);

	if (NODE->isCylindar) {
		DrawCylinder(6, 2, 2, 6);
	}
	else {
		DrawCone(6., 2., 6);
	}
	glTranslatef(0., 6., 0.);
	glTranslatef(0., 1.2, 0.);
	DrawSphere(1.2, 20, 20);

	double initAngle = 0.;
	if (NODE->NUM_OF_EDGES > 1) {
		initAngle = 45.;
	}
	for (int child = 0; child < NODE->NUM_OF_EDGES; child++) {
		glPushMatrix();
		
		if (NODE->ROTATING_EDGE == child) {
			glRotatef(initAngle + angle, 0., 0., 1.);
			NODE->CHILDREN[child]->setIsRotating(true);
		}else
			glRotatef(initAngle, 0., 0., 1.);

		glTranslatef(0., 1.2, 0.);
		DrawCactus(NODE->CHILDREN[child]);

		if (NODE->NUM_OF_EDGES == 3)
			initAngle -= 45.;
		else
			initAngle -= 90.;
		glPopMatrix();
	}
}

void CGLRenderer::DrawFigure() {
	glPushMatrix();
	DrawPot();

	CACTUS_NODE* CENTER_SUBTREE = new CACTUS_NODE(3,-1);
	CACTUS kaktus(CENTER_SUBTREE);

	//
	CACTUS_NODE* CENTER_LEFT_SUBTREE = new CACTUS_NODE(1, -1);
	CENTER_SUBTREE->setChildren(0, CENTER_LEFT_SUBTREE);
	
	CACTUS_NODE* CENTER_CENTER_SUBTREE = new CACTUS_NODE(2, 0);
	CENTER_SUBTREE->setChildren(1, CENTER_CENTER_SUBTREE);
	
	CACTUS_NODE* CENTER_RIGHT_SUBTREE = new CACTUS_NODE(1, -1);
	CENTER_SUBTREE->setChildren(2, CENTER_RIGHT_SUBTREE);
	//
	
	CACTUS_NODE* CENTER_LEFT_LEFT_SUBTREE = new CACTUS_NODE(1, -1, false);
	CENTER_LEFT_SUBTREE->setChildren(0, CENTER_LEFT_LEFT_SUBTREE);

	CACTUS_NODE* CENTER_CENTER_LEFT_SUBTREE = new CACTUS_NODE(1, -1);
	CACTUS_NODE* CENTER_CENTER_RIGHT_SUBTREE = new CACTUS_NODE(1, -1);
	CENTER_CENTER_SUBTREE->setChildren(0, CENTER_CENTER_LEFT_SUBTREE);
	CENTER_CENTER_SUBTREE->setChildren(1, CENTER_CENTER_RIGHT_SUBTREE);

	CACTUS_NODE* CENTER_RIGHT_RIGHT_SUBTREE = new CACTUS_NODE(1, -1, false);
	CENTER_RIGHT_SUBTREE->setChildren(0, CENTER_RIGHT_RIGHT_SUBTREE);
	//

	CACTUS_NODE* CENTER_CENTER_LEFT_LEFT_SUBTREE = new CACTUS_NODE(1, -1, false);
	CENTER_CENTER_LEFT_SUBTREE->setChildren(0, CENTER_CENTER_LEFT_LEFT_SUBTREE);

	CACTUS_NODE* CENTER_CENTER_RIGHT_RIGHT_SUBTREE = new CACTUS_NODE(1, -1);
	CENTER_CENTER_RIGHT_SUBTREE->setChildren(0, CENTER_CENTER_RIGHT_RIGHT_SUBTREE);

	CACTUS_NODE* CENTER_RIGHT_RIGHT_RIGHT_SUBTREE = new CACTUS_NODE(1, -1, false);
	CENTER_RIGHT_RIGHT_SUBTREE->setChildren(0, CENTER_RIGHT_RIGHT_RIGHT_SUBTREE);
	
	DrawCactus(kaktus.START);

	glPopMatrix();
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0., -18., -82.5);

	glTranslatef(0., 18., 0.);
	RotateScene(pDC);
	glTranslatef(0., -18., 0.);

	DrawGrid(50);
	DrawCoord();
	glRotatef(90, 0., 1., 0.);
	DrawFigure();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	//..
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
