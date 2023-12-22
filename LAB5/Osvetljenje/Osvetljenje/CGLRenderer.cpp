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
	glClearColor(1., 1., 1., 0.0);
	//
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	//
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45., (double)w / (double)h, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	//

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawBox(double w, double h, double d) {
	//Donja baza

	glBegin(GL_QUADS);
	glNormal3f(0., -1., 0.);
	//D PL
	glVertex3f(-w / 2, -h / 2, d / 2);
	//D PD
	glVertex3f(w / 2, -h / 2, d / 2);
	//D ZD
	glVertex3f(w / 2, -h / 2, -d / 2);
	//D ZL
	glVertex3f(-w / 2, -h / 2, -d / 2);
	glEnd();

	//gornja baza
	glBegin(GL_QUADS);

	glNormal3f(0., 1., 0.);
	//G PL
	glVertex3f(-w / 2, h / 2, d / 2);
	//G PD
	glVertex3f(w / 2, h / 2, d / 2);
	//G ZD
	glVertex3f(w / 2, h / 2, -d / 2);
	//G ZL
	glVertex3f(-w / 2, h / 2, -d / 2);
	glEnd();

	//omotac

	glBegin(GL_QUADS);
	//prednja
	glNormal3f(0., 0., 1.);
	glVertex3f(-w / 2, -h / 2, d / 2);
	glVertex3f(-w / 2, h / 2, d / 2);
	glVertex3f( w / 2, h / 2, d / 2);
	glVertex3f(w / 2, -h / 2, d / 2);

	//desna
	glNormal3f(1., 0., 0.);
	glVertex3f(w / 2, -h / 2, d / 2);
	glVertex3f(w / 2, h / 2, d / 2);
	glVertex3f(w / 2, h / 2, -d / 2);
	glVertex3f(w / 2, -h / 2, -d / 2);

	//zadnja
	glNormal3f(0., 0., -1.);
	glVertex3f(-w / 2, -h / 2, -d / 2);
	glVertex3f(-w / 2, h / 2, -d / 2);
	glVertex3f(w / 2, h / 2,-d / 2);
	glVertex3f(w / 2, -h / 2, -d / 2);

	//leva
	glNormal3f(-1., 0., 0.);
	glVertex3f(-w / 2, -h / 2, d / 2);
	glVertex3f(-w / 2, h / 2, d / 2);
	glVertex3f(-w / 2, h / 2, -d / 2);
	glVertex3f(-w / 2, -h / 2, -d / 2);

	glEnd();
}

void CGLRenderer::DrawTable() {
	GLfloat mat_amb[] = { 1., 1., 1., 1.0 };
	GLfloat mat_dif[] = { 1., 1., 1., 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess =128.;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	DrawSphere(10, 100, 16);
	DrawCylinder(20, 5, 5, 8);
	glTranslatef(0, 20, 0);
	DrawBox(25., 7.5, 25.);
}

void CGLRenderer::SetupSceneLighting() {
	glEnable(GL_LIGHTING);
	GLfloat lmodel_ambient[] = { .2 , .2 , .2 , 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	float light_ambient[] = { 0.25, 0.25, 0.25, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat global_light_position[] = { cos(DEG_TO_RAD(60)) , 0.0 , sin(DEG_TO_RAD(60)), 0.0};


	glLightfv(GL_LIGHT0, GL_POSITION, global_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);


	GLfloat light1_ambient[] = { 0.2, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 50.0, 50.0, 1.0 };
	GLfloat spot_direction1[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 22.5);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHT1);

	GLfloat light2_ambient[] = { 0.2, 0.0, 0.2, 1.0 };
	GLfloat light2_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat light2_specular[] = { 1.0, 0.0, 1.0, 1.0 };
	GLfloat light2_position[] = { 0.0, 50.0, -50.0, 1.0 };
	GLfloat spot_direction2[] = { 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light1_position);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 22.5);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHT2);


	GLfloat light3_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light3_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light3_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light3_position[] = { 0.0, 100.0, 0.0, 1.0 };
	GLfloat spot_direction3[] = { -1.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light1_position);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction3);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHT3);
}


void CGLRenderer::DrawFigure()
{

	glTranslatef(0., 3.25, 0.);
	DrawLayer(2, 9, 7, 25);
	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 5, 25, true);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 5, 25);
	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 5, 25, true);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 7, 25);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 5, 25, true);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 7, 25);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 9, 25, true);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 9, 7, 25);

	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 5, 25, true);


	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 7, 25);


	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 5, 25, true);


	glTranslatef(0, 2, 0.);
	DrawLayer(2, 5, 7, 25);


	glTranslatef(0, 2, 0.);
	DrawLayer(2, 7, 9, 25, true);

}


void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	glTranslatef(0, -20, -100);
	glTranslatef(0, 20, 0);
	RotateScene();
	glTranslatef(0, -20, 0);
	glTranslatef(0, 20, 100);

	glTranslatef(0, -20, -100);
	
	SetupSceneLighting();
	DrawCoord();
	
	DrawTable();
	DrawFigure();


	glFlush();
	SwapBuffers(pDC->m_hDC);
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
	for (int i = 0; i <= 10; i++) {
		glVertex3f(x0, y, d);
		glVertex3f(x1, y, d);
		d += korak;
	}
	d = -width / 2;
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

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1000.0);


	glEnd();

}

void CGLRenderer::RotateScene()
{
	if (!moving) {
		glRotatef(angleX, 1., 0., 0.);
		glRotatef(angleY, 0., 1., 0.);
		return;
	};

	if (startX == currX && startY == currY) return;

	float DeltaAngleX = ((float)currY - (float)prevY) / 8;
	float DeltaAngleY = ((float)currX - (float)prevX) / 8;

	angleX += DeltaAngleX;
	angleY += DeltaAngleY;

	glRotatef(angleX, 1., 0., 0.);
	glRotatef(angleY, 0., 1., 0.);

	prevX = currX;
	prevY = currY;

}

void CGLRenderer::SaveSceneAngle() {
	//if (startX == currX && startY == currY) return;

	//angleX = angleX + ((float)currY - (float)startY) / 8;
	//angleY = angleY + ((float)currX - (float)startX) / 8;
}
void CGLRenderer::SetStartCoord(int x, int y)
{
	prevX = x;
	prevY = y;
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

	for (int segAlpha = 0; segAlpha <= nSegAlpha ; segAlpha++) {
		float cosinus_alpha = cos(DEG_TO_RAD(alpha));
		float sinus_alpha = sin(DEG_TO_RAD(alpha));

		glBegin(GL_QUAD_STRIP);
		for (int segBeta = 0; segBeta <= nSegBeta; segBeta++) {
			float cosinus_beta = cos(DEG_TO_RAD(beta));
			float sinus_beta = sin(DEG_TO_RAD(beta));

			float x0 = r * cosinus_alpha * cosinus_beta;
			float y0 = r * sinus_alpha;
			float z0 = r * cosinus_alpha * sinus_beta;

			float nx0 = x0 / r;
			float ny0 = y0 / r;
			float nz0 = z0 / r;

			float x1 = r * cos(DEG_TO_RAD(alpha - (float)180 / nSegAlpha)) * cosinus_beta;
			float y1 = r * sin(DEG_TO_RAD(alpha - (float)180 / nSegAlpha));
			float z1 = r * cos(DEG_TO_RAD(alpha - (float)180 / nSegAlpha)) * sinus_beta;

			float nx1 = x1 / r;
			float ny1 = y1 / r;
			float nz1 = z1 / r;

			glNormal3f(nx0, ny0, nz0);
			glVertex3f(x0, y0, z0);

			glNormal3f(nx1, ny1, nz1);
			glVertex3f(x1, y1, z1);

			beta += (float)360 / nSegBeta;
		}
		glEnd();
		alpha -= (float)180 / nSegAlpha;

	}
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg) {
	double alpha = 0.;
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0., -1., 0.);
	glVertex3d(0., 0., 0.);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3d(r1 * cos(DEG_TO_RAD(alpha)), 0, r1 * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();

	alpha = 0.;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0., 1., 0.);
	glVertex3d(0., h, 0.);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3d(r2 * cos(DEG_TO_RAD(alpha)), h, r2 * sin(DEG_TO_RAD(alpha)));
		alpha += (double)360 / nSeg;
	}
	glEnd();

	alpha = 0.;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		float nx = cos(DEG_TO_RAD(alpha));
		float ny = 0;
		float nz = sin(DEG_TO_RAD(alpha));

		glNormal3f(nx, ny, nz);
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

void CGLRenderer::DrawLayer(float h, float r1, float r2, int nSeg, bool purple) {
	float alpha = 0;

	GLfloat mat_amb[] = { 1., 1., 1., 1.0 };
	GLfloat mat_dif[] = { purple, 0.0, 1.0, 1.0 }; 
	GLfloat mat_specular[] = { 1., 1., 1., 1.0 };
	GLfloat mat_shininess = 128.;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {

		float L = sqrt(h * h + (r1 - r2) * (r1 - r2));
		float nx = h / L * cos(DEG_TO_RAD(alpha));
		float ny = (r1 - r2) / L;
		float nz = h / L * sin(DEG_TO_RAD(alpha));

		glNormal3f(nx, ny, nz);
		glVertex3f(r1 * cos(DEG_TO_RAD(alpha)), 0, r1* sin(DEG_TO_RAD(alpha)) );
		glVertex3f(r2 * cos(DEG_TO_RAD(alpha)), h, r2 * sin(DEG_TO_RAD(alpha)));

		alpha += (float)360 / nSeg;
	}
	glEnd();

	alpha = 0.;

	if (isNormalShowing) {
		glBegin(GL_LINES);
		for (int i = 0; i <= nSeg; i++) {
			float L = sqrt(h * h + (r1 - r2) * (r1 - r2));
			float nx = h / L * cos(DEG_TO_RAD(alpha));
			float ny = (r1 - r2) / L;
			float nz = h / L * sin(DEG_TO_RAD(alpha));

			glVertex3f(r1 * cos(DEG_TO_RAD(alpha)), 0, r1 * sin(DEG_TO_RAD(alpha)));
			glVertex3f(r1 * cos(DEG_TO_RAD(alpha)) + nx, ny, r1 * sin(DEG_TO_RAD(alpha)) + nz);

			glVertex3f(r2 * cos(DEG_TO_RAD(alpha)), h, r2 * sin(DEG_TO_RAD(alpha)));
			glVertex3f(r2 * cos(DEG_TO_RAD(alpha)) + nx, h + ny, r2 * sin(DEG_TO_RAD(alpha)) + nz);
			alpha += (float)360 / nSeg;


		}
		glEnd();
	}
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