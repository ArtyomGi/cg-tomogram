#pragma once
#pragma comment (lib, "opengl32.lib")
#include <QGLWidget>
#include <QtOpenGL>

#include <qimage.h>
#include <algorithm>

#include "Data.h"

class View : public QOpenGLWidget, protected QOpenGLFunctions
{

	Q_OBJECT
private:
	Data data;
	int layer;

	bool userMM = false;
	int userMin, userMax;

	int axis = 0;

	void Up();
	void Down();
	void Up10();
	void Down10();

	static const int MAX_WIN_SIZE = 1000;
	static const int MIN_WIN_SIZE = 250;

	int visualization_state = 0;
	static const int VISUALIZATION_QUADS = 0;
	static const int VISUALIZATION_QUADSTRIP = 1;
	static const int VISUALIZATION_TEXTURE = 2;

	void changeLayer();

	float TransferFunction(short value);

	void VisualizationQuadsXY();
	void VisualizationQuadsYZ();
	void VisualizationQuadsXZ();

	void VisualizationQuadStripXY();
	void VisualizationQuadStripYZ();
	void VisualizationQuadStripXZ();

	void VisualizationTexture();

	void genTextureImageXY();
	void genTextureImageYZ();
	void genTextureImageXZ();

	QImage textureImage;
	QOpenGLTexture* texture;

protected:
	void initializeGL() override;
	void resizeGL(int nWidth, int nHeight) override;
	void paintGL() override;

public:
	View(QWidget* parent);
	~View() {};

	void LoadData(QString fileName);

public slots:
	void keyPressEvent(QKeyEvent* event) override;

	void setQuads(bool checked);
	void setQuadstips(bool checked);
	void setTexture(bool checked);

	void axisSetXY(bool checked);
	void axisSetYZ(bool checked);
	void axisSetXZ(bool checked);

	void loadNewData(bool checked);

	void toggleUserMM(int state);
	void setMM(bool checked);
};