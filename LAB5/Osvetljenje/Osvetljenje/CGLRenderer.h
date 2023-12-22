#pragma once
class CGLRenderer
{
protected:
	HGLRC m_hrc;

	float angle = 0.;
	float angleX = 0.;
	float angleY = 0.;

	int startX = 0;
	int startY = 0;
	int currX = 0;
	int currY = 0;

	int prevX = 0;
	int prevY = 0;

	bool isNormalShowing = true;

	bool moving = false;
public:
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);

	void DrawGrid(int width);
	void DrawCoord();
	void RotateScene();
	void SaveSceneAngle();

	void SetupSceneLighting();

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawBox(double w, double h, double d);
	void DrawLayer(float h, float r1, float r2, int nSeg, bool purple = false);

	void DrawTable();
	void DrawFigure();

	void SetStartCoord(int x, int y);
	void SetCurrCoord(int x, int y);
	void SetMoving(bool moving) { this->moving = moving; }
	void SetAngle(float deltaAngle) { this->angle += deltaAngle; }
	void ToggleIsNormalShowing(bool is) { this->isNormalShowing = !this->isNormalShowing; }
};
