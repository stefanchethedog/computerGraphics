#pragma once

struct CACTUS_NODE;

struct CACTUS {
	CACTUS_NODE* START;

	CACTUS(CACTUS_NODE* STARTING_NODE) {
		START = STARTING_NODE;
	}
};

struct CACTUS_NODE {
	int NUM_OF_EDGES;
	int ROTATING_EDGE;
	bool isCylindar;
	bool isRotating;
	CACTUS_NODE** CHILDREN;

	CACTUS_NODE(int n, int r, bool cyl = true) {
		NUM_OF_EDGES = n;
		ROTATING_EDGE = r;
		isCylindar = cyl;
		isRotating = false;

		CHILDREN = new CACTUS_NODE * [n];
		for (int i = 0; i < n; i++) {
			CHILDREN[i] = NULL;
		}
	}
	void setChildren(int k, CACTUS_NODE* CHILD) {
		if (k < 0 && k >= NUM_OF_EDGES) return;
		CHILDREN[k] = CHILD;
	}
	void setIsRotating(bool rotates = true) {
		this->isRotating = rotates;
	}
};


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

	bool moving = false;
public:
	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);

	void DrawGrid(int width);
	void DrawCoord();
	void RotateScene(CDC* pDC);
	void SaveSceneAngle();

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawPot();
	void DrawCactus(CACTUS_NODE* NODE);
	void DrawFigure();

	void SetStartCoord(int x, int y);
	void SetCurrCoord(int x, int y);
	void SetMoving(bool moving) { this->moving = moving; }
	void SetAngle(float deltaAngle) { this->angle += deltaAngle; }
};

