#pragma once

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>

#include "CGLTexture.h"


class CGLRenderer
{
private:
	HGLRC m_hrc;
public:
	double m_rotAngle;
	double m_viewR;
	double m_viewAlpha;
	double m_viewBeta;
	double m_cameraCoordX;
	double m_cameraCoordY;
	double m_cameraCoordZ;
	double m_lookingAtX;
	double m_lookingAtY;
	double m_lookingAtZ;
	double m_upX;
	double m_upY;
	double m_upZ;
	void CalculateCameraPosition();
	CGLTexture m_grassTex;
	CGLTexture m_objectsTex;
	CGLRenderer();
	~CGLRenderer();
	bool CrateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawSceen(CDC* pDC);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg, double s, double t, double rTex);
	void DrawAxis(double width);
	void DrawGround(double width, double lenght, int repS, int repT);
	void DrawWheel(double h, double r, int nStep, double s, double t, double rTex);
	void DrawWheels(double h, double r, double angle);
	void DrawTruckBody(double a, double texA, double width);
	void DrawTruckGrayPart(double a, double width);
	void DrawTruckSide(double a, double texA, double width);
	void DrawTruck();
};

