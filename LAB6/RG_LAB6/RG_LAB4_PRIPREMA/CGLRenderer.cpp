#include "pch.h"
#include "CGLRenderer.h"

#define PI 3.141592653589793238
#define DOUBLE_PI 6.2831853071795864
#define TO_RAD 0.017453292519943295
#define TO_DEG 57.29577951308232087

CGLRenderer::CGLRenderer()
{
	m_hrc = NULL;
	m_rotAngle = 0.0;
	m_viewR = 30.0;
	m_viewAlpha = 45.0;
	m_viewBeta = 45.0;
	m_lookingAtX = 0.0;
	m_lookingAtY = 0.0;
	m_lookingAtZ = 0.0;
	m_cameraCoordX = 0.0;
	m_cameraCoordY = 0.0;
	m_cameraCoordZ = 0.0;
	m_upX = 0.0;
	m_upY = 1.0;
	m_upZ = 0.0;
	CalculateCameraPosition();
}

CGLRenderer::~CGLRenderer()
{
	m_grassTex.DeleteTexture();
	m_objectsTex.DeleteTexture();
}

bool CGLRenderer::CrateGLContext(CDC* pDC)
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

	if (bResult == FALSE) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glClearColor(0.79, 0.79, 0.79, 0.0);

	m_grassTex.LoadTextureFromFile("./res/FloorTexture.jpg", true);
	m_objectsTex.LoadTextureFromFile("./res/ObjectTextureGrid.png");

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	wglMakeCurrent(NULL, NULL);

	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::CalculateCameraPosition()
{
	m_cameraCoordX = m_viewR * cos(m_viewAlpha * TO_RAD) * cos(m_viewBeta * TO_RAD);
	m_cameraCoordY = m_viewR * sin(m_viewAlpha * TO_RAD);
	m_cameraCoordZ = m_viewR * cos(m_viewAlpha * TO_RAD) * sin(m_viewBeta * TO_RAD);
}

void CGLRenderer::DrawSceen(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	CalculateCameraPosition();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_cameraCoordX, m_cameraCoordY, m_cameraCoordZ,
			  m_lookingAtX, m_lookingAtY, m_lookingAtZ,
			  m_upX, m_upY, m_upZ);

	DrawAxis(2);
	DrawGround(50, 50, 3, 4);
	DrawTruck();

	glFlush();

	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(width);

	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(200.0, 0.0, 0.0);
	
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 200.0, 0.0);
	
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 200.0);

	glEnd();

	glLineWidth(1);
}

void CGLRenderer::DrawGround(double width, double length, int repS, int repT)
{
	glEnable(GL_TEXTURE_2D);
	m_grassTex.SelectTexture();

	glBegin(GL_QUADS);
	{
		glTexCoord2f(repS, repT);
		glVertex3f(width / 2, 0, length / 2);

		glTexCoord2f(repS, 0);
		glVertex3f(width / 2, 0, -length / 2);

		glTexCoord2f(0, 0);
		glVertex3f(-width / 2, 0, -length / 2);

		glTexCoord2f(0, repT);
		glVertex3f(-width / 2, 0, length / 2);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	glEnable(GL_TEXTURE_2D);
	m_objectsTex.SelectTexture();

	//double alpha = DOUBLE_PI / nSegAlpha;
	//double beta = DOUBLE_PI / nSegAlpha;
	//glBegin(GL_QUAD_STRIP);
	//for (int i = 0; i < nSegAlpha; i++)
	//{
	//	for (int j = 0; j < nSegBeta; j++)
	//	{
	//		//glTexCoord2f(j / 16, t / 16);
	//		glVertex3f(r * cos(alpha * i) * cos(beta * j),
	//			r + r * sin(alpha * i),
	//			r * cos(alpha * i) * sin(beta * j));
	//		//glTexCoord2f(j / 16, (t - 1) / 16);
	//		glVertex3f(r * cos(alpha * ((double)i + 1)) * cos(beta * j),
	//			r + r * sin(alpha * ((double)i + 1)),
	//			r * cos(alpha * ((double)i + 1)) * sin(beta * j));
	//	}
	//}
	//glEnd();

	double alpha = PI / 2;
	double beta = 0;

	double dAlpha = PI / nSegAlpha;
	double dBeta = DOUBLE_PI / nSegBeta;

	int t = 9;
	float s1, t1, s2, t2;
	

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSegAlpha; i++)
	{
		beta = 0;
		for (int j = 0; j <= nSegBeta; j++)
		{
			s1 = 1 - beta / DOUBLE_PI;
			t1 = 1 - (alpha + PI / 2) / DOUBLE_PI;
			glTexCoord2f(s1, t1);
			glVertex3f(r * cos(alpha) * cos(beta),
				r + r * sin(alpha),
				r * cos(alpha) * sin(beta));
	
			s2 = 1 - beta / DOUBLE_PI;
			t2 = 1 - (alpha - dAlpha + PI / 2) / DOUBLE_PI;
			glTexCoord2f(s2, t2);
			glVertex3f(r * cos(alpha - dAlpha) * cos(beta),
				r + r * sin(alpha - dAlpha),
				r * cos(alpha - dAlpha) * sin(beta));
			beta += dBeta;
		}
		alpha -= dAlpha;
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg, double s, double t, double rTex)
{
	double alpha = DOUBLE_PI / nSeg;

	glBegin(GL_TRIANGLE_FAN);
	//glTexCoord2f(s, t);
	//glVertex3f(0, 0, 0);
	for (int i = 0; i < nSeg; i++)
	{
		glTexCoord2f(rTex * cos(i) + s, rTex * sin(i) + t);
		glVertex3f(r1 * cos(alpha * i), 0, r1 * sin(alpha * i));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	//glTexCoord2f(s, t);
	//glVertex3f(0, 0, 0);
	for (int i = 0; i < nSeg; i++)
	{
		glTexCoord2f(s + rTex * cos(i), t + rTex * sin(i));
		glVertex3f(r2 * cos(alpha * i), h, r2 * sin(alpha * i));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg + 1; i++)
	{
		glVertex3f(r1 * cos(alpha * i), 0, r1 * sin(alpha * i));
		glVertex3f(r2 * cos(alpha * i), h, r2 * sin(alpha * i));
	}
	glEnd();
}

void CGLRenderer::DrawWheel(double h, double r, int nStep, double s, double t, double rTex)
{
	double dW = PI / (double)nStep,
		hHalf = h / 2;

	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(s, t);
		glVertex3f(0, 0, 0);
		for (double i = 0; i < (DOUBLE_PI + dW); i += dW)
		{
			glTexCoord2f(rTex * sin(i) + s, rTex * cos(i) + t);
			glVertex3f(r * sin(i), hHalf, r * cos(i));
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(s, t);
		glVertex3f(0, 0, 0);
		for (double i = 0; i < (DOUBLE_PI + dW); i += dW)
		{
			glTexCoord2f(s + rTex * sin(i), t + rTex * cos(i));
			glVertex3f(r * sin(i), -hHalf, r * cos(i));
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor3f(.75, .75, .55);
	glBegin(GL_QUAD_STRIP);
	{
		for (double i = 0; i < (DOUBLE_PI + dW); i += dW)
		{
			double x = sin(i),
				y = 1,
				z = cos(i);

			glVertex3f(r * x, hHalf, r * z);
			glVertex3f(r * x, -hHalf, r * z);
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawWheels(double h, double r, double angle)
{
	glEnable(GL_TEXTURE_2D);
	m_objectsTex.SelectTexture();

	glPushMatrix();
	{
		glTranslatef(1.35 * r, 2 * h, -r);
		glRotatef(angle, 0, 1, 0);
		DrawWheel(h, r, 8, 6.0 / 16, 1.5/16, 1.5 / 16);
		//DrawCylinder(h, r, r, 8, 6.0 / 16, 1.5 / 16, 1.5 / 16);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(-2 * r, 2 * h, -r);
		glRotatef(angle, 0, 1, 0);
		DrawWheel(h, r, 8, 6.0 / 16, 1.5/16, 1.5 / 16);
		//DrawCylinder(h, r, r, 8, 6.0 / 16, 1.5 / 16, 1.5 / 16);
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTruckBody(double a, double texTileSize, double width)
{
	glTranslatef(-5.5 * a, 1.5 * a, width/2);

	DrawTruckSide(a, texTileSize, width);
	DrawTruckGrayPart(a, width);
	glTranslatef(0.0, 0.0, -width);
	DrawTruckSide(a, texTileSize, width);
}

void CGLRenderer::DrawTruckGrayPart(double a, double width)
{
	glBegin(GL_QUADS);
	{
		glColor3f(0.75, 0.75, 0.75);
		// Front of the car

		glVertex3f(0, 0, -width);
		glVertex3f(0, 0, 0);

		glVertex3f(0, 5 * a, 0);
		glVertex3f(0, 5 * a, -width);


		// Wind shield

		glVertex3f(0, 5 * a, -width);
		glVertex3f(0, 5 * a, 0);

		glVertex3f(2 * a, 7 * a, 0);
		glVertex3f(2 * a, 7 * a, -width);

		// Roof

		glVertex3f(2 * a, 7 * a, -width);
		glVertex3f(2 * a, 7 * a, 0);

		glVertex3f(4 * a, 7 * a, 0);
		glVertex3f(4 * a, 7 * a, -width);


		// Back shield

		glVertex3f(4 * a, 7 * a, -width);
		glVertex3f(4 * a, 7 * a, 0);

		glVertex3f(4 * a, 3 * a, 0);
		glVertex3f(4 * a, 3 * a, -width);

		// Propshaft

		glVertex3f(4 * a, 3 * a, -width);
		glVertex3f(4 * a, 3 * a, 0);

		glVertex3f(11 * a, 3 * a, 0);
		glVertex3f(11 * a, 3 * a, -width);

		// Back

		glVertex3f(11 * a, 3 * a, -width);
		glVertex3f(11 * a, 3 * a, 0);

		glVertex3f(11 * a, 0, 0);
		glVertex3f(11 * a, 0, -width);

		// Bottom - back

		glVertex3f(11 * a, 0, -width);
		glVertex3f(11 * a, 0, 0);
		glVertex3f(9 * a, 0, 0);
		glVertex3f(9 * a, 0, -width);

		glVertex3f(8 * a, 2 * a, -width);
		glVertex3f(8 * a, 2 * a, 0);
		glVertex3f(7 * a, 2 * a, 0);
		glVertex3f(7 * a, 2 * a, -width);

		glVertex3f(6 * a, 0, -width);
		glVertex3f(6 * a, 0, 0);
		glVertex3f(4 * a, 0, 0);
		glVertex3f(4 * a, 0, -width);

		glVertex3f(3 * a, 2 * a, -width);
		glVertex3f(3 * a, 2 * a, 0);
		glVertex3f(2 * a, 2 * a, 0);
		glVertex3f(2 * a, 2 * a, -width);

		glVertex3f(a, 0, -width);
		glVertex3f(a, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -width);


		glVertex3f(9 * a, a, -width);
		glVertex3f(9 * a, a, 0);
		glVertex3f(8 * a, 2 * a, 0);
		glVertex3f(8 * a, 2 * a, -width);

		glVertex3f(4 * a, a, -width);
		glVertex3f(4 * a, a, 0);
		glVertex3f(3 * a, 2 * a, 0);
		glVertex3f(3 * a, 2 * a, -width);



		glVertex3f(7 * a, 2 * a, -width);
		glVertex3f(7 * a, 2 * a, 0);
		glVertex3f(6 * a, a, 0);
		glVertex3f(6 * a, a, -width);

		glVertex3f(2 * a, 2 * a, -width);
		glVertex3f(2 * a, 2 * a, 0);
		glVertex3f(a, a, 0);
		glVertex3f(a, a, -width);


		glVertex3f(9 * a, 0, -width);
		glVertex3f(9 * a, 0, 0);
		glVertex3f(9 * a, a, 0);
		glVertex3f(9 * a, a, -width);

		glVertex3f(4 * a, 0, -width);
		glVertex3f(4 * a, 0, 0);
		glVertex3f(4 * a, a, 0);
		glVertex3f(4 * a, a, -width);


		glVertex3f(6 * a, 0, -width);
		glVertex3f(6 * a, 0, 0);
		glVertex3f(6 * a, a, 0);
		glVertex3f(6 * a, a, -width);

		glVertex3f(a, 0, -width);
		glVertex3f(a, 0, 0);
		glVertex3f(a, a, 0);
		glVertex3f(a, a, -width);
	}
	glEnd();
}

void CGLRenderer::DrawTruckSide(double a, double texTileSize, double width)
{
	glEnable(GL_TEXTURE_2D);
	m_objectsTex.SelectTexture();

	//kabina
	glBegin(GL_POLYGON);
	{
		glTexCoord2f(4 * texTileSize, 4 * texTileSize);
		glVertex3f(4 * a, 3 * a, 0);

		glTexCoord2f(4 * texTileSize, 0);
		glVertex3f(4 * a, 7 * a, 0);

		glTexCoord2f(2 * texTileSize, 0);
		glVertex3f(2 * a, 7 * a, 0);

		glTexCoord2f(0, 2 * texTileSize);
		glVertex3f(0, 5 * a, 0);

		glTexCoord2f(0, 4 * texTileSize);
		glVertex3f(0, 3 * a, 0);
	}
	glEnd();
	

	//prikolica
	glBegin(GL_QUADS);
	{
		glTexCoord2f(5 * texTileSize, 5 * texTileSize);
		glVertex3f(5 * a, 2 * a, 0);

		glTexCoord2f(5 * texTileSize, 4 * texTileSize);
		glVertex3f(5 * a, 3 * a, 0);

		glTexCoord2f(0, 4 * texTileSize);
		glVertex3f(0, 3 * a, 0);

		glTexCoord2f(0, 5 * texTileSize);
		glVertex3f(0, 2 * a, 0);


		glTexCoord2f(texTileSize, 7 * texTileSize);
		glVertex3f(a, 0, 0);

		glTexCoord2f(texTileSize, 5 * texTileSize);
		glVertex3f(a, 2 * a, 0);

		glTexCoord2f(0, 5 * texTileSize);
		glVertex3f(0, 2 * a, 0);

		glTexCoord2f(0, 7 * texTileSize);
		glVertex3f(0, 0, 0);


		glTexCoord2f(5 * texTileSize, 7 * texTileSize);
		glVertex3f(5 * a, 0, 0);

		glTexCoord2f(5 * texTileSize, 5 * texTileSize);
		glVertex3f(5 * a, 2 * a, 0);

		glTexCoord2f(4 * texTileSize, 5 * texTileSize);
		glVertex3f(4 * a, 2 * a, 0);

		glTexCoord2f(4 * texTileSize, 7 * texTileSize);
		glVertex3f(4 * a, 0, 0);


		// Drugi deo
		glTexCoord2f(11 * texTileSize, 5 * texTileSize);
		glVertex3f(11 * a, 2 * a, 0);

		glTexCoord2f(11 * texTileSize, 4 * texTileSize);
		glVertex3f(11 * a, 3 * a, 0);

		glTexCoord2f(5 * texTileSize, 4 * texTileSize);
		glVertex3f(5 * a, 3 * a, 0);

		glTexCoord2f(5 * texTileSize, 5 * texTileSize);
		glVertex3f(5 * a, 2 * a, 0);


		glTexCoord2f(6 * texTileSize, 7 * texTileSize);
		glVertex3f(6 * a, 0, 0);

		glTexCoord2f(6 * texTileSize, 5 * texTileSize);
		glVertex3f(6 * a, 2 * a, 0);

		glTexCoord2f(5 * texTileSize, 5 * texTileSize);
		glVertex3f(5 * a, 2 * a, 0);

		glTexCoord2f(5 * texTileSize, 7 * texTileSize);
		glVertex3f(5 * a, 0, 0);


		glTexCoord2f(11 * texTileSize, 7 * texTileSize);
		glVertex3f(11 * a, 0, 0);

		glTexCoord2f(11 * texTileSize, 5 * texTileSize);
		glVertex3f(11 * a, 2 * a, 0);

		glTexCoord2f(9 * texTileSize, 5 * texTileSize);
		glVertex3f(9 * a, 2 * a, 0);

		glTexCoord2f(9 * texTileSize, 7 * texTileSize);
		glVertex3f(9 * a, 0, 0);
	}
	glEnd();

	//zubici tockovi
	glBegin(GL_TRIANGLES);
	{
		glTexCoord2f(2 * texTileSize, 4 * texTileSize);
		glVertex3f(2 * a, 2 * a, 0);

		glTexCoord2f(texTileSize, 4 * texTileSize);
		glVertex3f(a, 2 * a, 0);

		glTexCoord2f(texTileSize, 5 * texTileSize);
		glVertex3f(a, a, 0);

		glTexCoord2f(4 * texTileSize, 5 * texTileSize);
		glVertex3f(4 * a, a, 0);

		glTexCoord2f(4 * texTileSize, 4 * texTileSize);
		glVertex3f(4 * a, 2 * a, 0);

		glTexCoord2f(3 * texTileSize, 4 * texTileSize);
		glVertex3f(3 * a, 2 * a, 0);

		// Drugi deo
		glTexCoord2f(7 * texTileSize, 4 * texTileSize);
		glVertex3f(7 * a, 2 * a, 0);

		glTexCoord2f(6 * texTileSize, 4 * texTileSize);
		glVertex3f(6 * a, 2 * a, 0);

		glTexCoord2f(6 * texTileSize, 5 * texTileSize);
		glVertex3f(6 * a, a, 0);


		glTexCoord2f(9 * texTileSize, 5 * texTileSize);
		glVertex3f(9 * a, a, 0);

		glTexCoord2f(9 * texTileSize, 4 * texTileSize);
		glVertex3f(9 * a, 2 * a, 0);

		glTexCoord2f(8 * texTileSize, 4 * texTileSize);
		glVertex3f(8 * a, 2 * a, 0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawTruck()
{
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		DrawWheels(1, 1.5, -90);

		glScalef(1, -1, 1);
		DrawWheels(1, 1.5, 90);
	}
	glPopMatrix();

	glPushMatrix();
	{
		DrawTruckBody(1.0, 1.0 / 16, 5);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(2.0, 4.5, 0.0);
		glScalef(1.65, 1.0, 1.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		DrawSphere(2.0, 16, 16);
	}
	glPopMatrix();
}