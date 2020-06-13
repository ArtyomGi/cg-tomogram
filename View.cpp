#include "View.h"

View::View(QWidget* parent) :
	QOpenGLWidget(parent), QOpenGLFunctions()
{
	layer = 0;
	texture = nullptr;

	setFocus();
}

void View::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(1, 1, 1, 1);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//glGenTextures(1, &VBOtexture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //цвет текстуры перекрывает цвет пикселя
}

void View::resizeGL(int nWidth, int nHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (axis)
	{
	case 0:
		glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getHeight() - 1, -1.0f, 1.0f);
		break;
	case 1:
		glOrtho(0.0f, data.getHeight() - 1, 0.0f, data.getDepth() - 1, -1.0f, 1.0f);
		break;
	case 2:
		glOrtho(0.0f, data.getWidth() - 1, 0.0f, data.getDepth() - 1, -1.0f, 1.0f);
		break;
	}
	
	//glOrtho(0.0f, nWidth - 1, 0.0f, nHeight - 1, -1.0f, 1.0f);
	glViewport(0, 0, nWidth, nHeight);
}

void View::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (visualization_state)
	{
	case VISUALIZATION_QUADS:
		switch (axis)
		{
		case 0:
			VisualizationQuadsXY();
			break;
		case 1:
			VisualizationQuadsYZ();
			break;
		case 2:
			VisualizationQuadsXZ();
			break;
		}
			
		break;
	case VISUALIZATION_QUADSTRIP:
		switch (axis)
		{
		case 0:
			VisualizationQuadStripXY();
			break;
		case 1:
			VisualizationQuadStripYZ();
			break;
		case 2:
			VisualizationQuadStripXZ();
			break;
		}
		break;
	case VISUALIZATION_TEXTURE:
		VisualizationTexture();
		break;
	}
}

int clamp(int value, int min, int max)
{
	if (value > max)
		return max;
	else if (value < min)
		return min;
	else return value;
}

void View::LoadData(QString fileName)
{
	data.readFile(fileName);

	resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
	update();
}

void View::Up()
{
	if ((layer + 1) < data.getDepth())
		layer++;
	update();
}

void View::Up10()
{
	if ((layer + 10) < data.getDepth())
		layer += 10;
	update();
}

void View::Down()
{
	if ((layer - 1) >= 0)
		layer--;
	update();
}

void View::Down10()
{
	if ((layer - 10) >= 0)
		layer -= 10;
	update();
}

float View::TransferFunction(short value)
{
	float c;

	if(userMM) c = (value - userMin) * 1.f / (userMax - userMin);
	else c = (value - data.getMin()) * 1.f / (data.getMax() - data.getMin());

	return c;
}

void View::VisualizationQuadsXY()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();

	for(int y = 0; y < (h - 1); y++)
		for (int x = 0; x < (w - 1); x++)
		{
			glBegin(GL_QUADS);

			c = TransferFunction(data[layer * w * h + y * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, y);

			c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, (y + 1));

			c = TransferFunction(data[layer * w * h + (y + 1) * w + (x + 1)]);
			glColor3f(c, c, c);
			glVertex2i((x + 1), (y + 1));

			c = TransferFunction(data[layer * w * h + y * w + (x + 1)]);
			glColor3f(c, c, c);
			glVertex2i((x + 1), y );

			glEnd();
		}
}

void View::VisualizationQuadsYZ()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();

	for (int z = 0; z < (d - 1); z++)
		for (int y = 0; y < (h - 1); y++)
		{
			glBegin(GL_QUADS);

			c = TransferFunction(data[z * w * h + y * w + layer]);
			glColor3f(c, c, c);
			glVertex2i(y, z);

			c = TransferFunction(data[(z + 1) * w * h + (y + 1) * w + layer]);
			glColor3f(c, c, c);
			glVertex2i(y, (z + 1));

			c = TransferFunction(data[(z + 1) * w * h + (y + 1) * w + (layer + 1)]);
			glColor3f(c, c, c);
			glVertex2i((y + 1), (z + 1));

			c = TransferFunction(data[z * w * h + y * w + (layer + 1)]);
			glColor3f(c, c, c);
			glVertex2i((y + 1), z);

			glEnd();
		}
}

void View::VisualizationQuadsXZ()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();

	for (int z = 0; z < (d - 1); z++)
		for (int x = 0; x < (w - 1); x++)
		{
			glBegin(GL_QUADS);

			c = TransferFunction(data[z * w * h + layer * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, z);

			c = TransferFunction(data[(z + 1) * w * h + (layer + 1) * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, (z + 1));

			c = TransferFunction(data[(z + 1) * w * h + (layer + 1) * w + (x + 1)]);
			glColor3f(c, c, c);
			glVertex2i((x + 1), (z + 1));

			c = TransferFunction(data[z * w * h + layer * w + (x + 1)]);
			glColor3f(c, c, c);
			glVertex2i((x + 1), z);

			glEnd();
		}
}

void View::VisualizationQuadStripXY()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();

	for (int y = 0; y < (h - 1); y++)
	{
		glBegin(GL_QUAD_STRIP);

		c = TransferFunction(data[layer * w * h + y * w + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, y);

		c = TransferFunction(data[layer * w * h + (y + 1) * w + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, (y + 1));

		for (int x = 1; x < w; x++)
		{
			c = TransferFunction(data[layer * w * h + (y + 1) * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, (y + 1));

			c = TransferFunction(data[layer * w * h + y * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, y);
		}

		glEnd();
	}
}

void View::VisualizationQuadStripXZ()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();

	for (int z = 0; z < (d - 1); z++)
	{
		glBegin(GL_QUAD_STRIP);

		c = TransferFunction(data[z * w * h + layer * w + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, z);

		c = TransferFunction(data[z * w * h + (layer + 1) * w + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, (z + 1));

		for (int x = 1; x < w; x++)
		{
			c = TransferFunction(data[z * w * h + (layer + 1) * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, (z + 1));

			c = TransferFunction(data[z * w * h + layer * w + x]);
			glColor3f(c, c, c);
			glVertex2i(x, z);
		}

		glEnd();
	}
}

void View::VisualizationQuadStripYZ()
{
	float c;

	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();

	for (int z = 0; z < (d - 1); z++)
	{
		glBegin(GL_QUAD_STRIP);

		c = TransferFunction(data[z * w * h + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, z);

		c = TransferFunction(data[z * w * h + 0]);
		glColor3f(c, c, c);
		glVertex2i(0, (z + 1));

		for (int y = 1; y < h; y++)
		{
			c = TransferFunction(data[z * w * h + (y + 1) * w + layer]);
			glColor3f(c, c, c);
			glVertex2i(y, (z + 1));

			c = TransferFunction(data[z * w * h + y * w + layer]);
			glColor3f(c, c, c);
			glVertex2i(y, z);
		}

		glEnd();
	}
}

//void View::Load2DTexture()
//{
//	glBindTexture(GL_TEXTURE_2D, VBOtexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}

void View::genTextureImageXY()
{
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();
	QColor col;
	float c;

	textureImage = QImage(w, h, QImage::Format_RGB32);

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			c = int(255*TransferFunction(data[layer * w * h + w * y + x]));
			col.setRgb(c, c, c);
			textureImage.setPixelColor(x, y, col);
		}

	if (texture == nullptr)
	{
		texture = new QOpenGLTexture(textureImage);
		texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		texture->setMagnificationFilter(QOpenGLTexture::Linear);
	}
	else
	{
		//texture->destroy();
		//texture->create();
		//texture->setSize(textureImage.width(), textureImage.height(), textureImage.depth());
		////texture->setFormat(textureImage.format());
		//texture->allocateStorage();
		//texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		//texture->setMagnificationFilter(QOpenGLTexture::Linear);
		texture->setData(textureImage);
	}
}

void View::genTextureImageYZ()
{
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();
	QColor col;
	float c;

	textureImage = QImage(w, h, QImage::Format_RGB32);

	for (int z = 0; z < d - 1; z++)
		for (int y = 0; y < h - 1; y++)
		{
			c = int(255 * TransferFunction(data[z * w * h + w * y + layer]));
			col.setRgb(c, c, c);
			textureImage.setPixelColor(y, z, col);
		}

	if (texture == nullptr)
	{
		texture = new QOpenGLTexture(textureImage);
		texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		texture->setMagnificationFilter(QOpenGLTexture::Linear);
	}
	else
	{
		//texture->destroy();
		//texture->create();
		//texture->setSize(textureImage.width(), textureImage.height(), textureImage.depth());
		////texture->setFormat(textureImage.format());
		//texture->allocateStorage();
		//texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		//texture->setMagnificationFilter(QOpenGLTexture::Linear);
		texture->setData(textureImage);
	}
}

void View::genTextureImageXZ()
{
	int w = data.getWidth();
	int h = data.getHeight();
	int d = data.getDepth();
	QColor col;
	float c;

	textureImage = QImage(w, h, QImage::Format_RGB32);

	for (int z = 0; z < d - 1; z++)
		for (int x = 0; x < w - 1; x++)
		{
			c = int(255 * TransferFunction(data[z * w * h + w * layer + x]));
			col.setRgb(c, c, c);
			textureImage.setPixelColor(x, z, col);
		}

	if (texture == nullptr)
	{
		texture = new QOpenGLTexture(textureImage);
		texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		texture->setMagnificationFilter(QOpenGLTexture::Linear);
	}
	else
	{
		//texture->destroy();
		//texture->create();
		//texture->setSize(textureImage.width(), textureImage.height(), 1);
		////texture->setFormat(textureImage.format());
		//texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
		//texture->allocateStorage();
		//texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
		//texture->setMagnificationFilter(QOpenGLTexture::Linear);
		texture->setData(textureImage);
	}
}

void View::VisualizationTexture()
{
	glEnable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, VBOtexture);
	texture->bind();

	glBegin(GL_QUADS);
	//glColor(1, 1, 1);

	glTexCoord2f(0, 0);
	glVertex2i(0, 0);

	glTexCoord2f(0, 1);
	glVertex2i(0, data.getHeight());

	glTexCoord2f(1, 1);
	glVertex2i(data.getWidth(), data.getHeight());

	glTexCoord2f(1, 0);
	glVertex2i(data.getWidth(), 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void View::changeLayer()
{
	if (visualization_state == VISUALIZATION_TEXTURE)
	{
		switch (axis)
		{
		case 0:
			genTextureImageXY();
			break;
		case 1:
			genTextureImageYZ();
			break;
		case 2:
			genTextureImageXZ();
			break;
		}
		
		//Load2DTexture();
	}
}

void View::keyPressEvent(QKeyEvent* event)
{
	if (event->nativeVirtualKey() == Qt::Key_U)
	{
		Up();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_D)
	{
		Down();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_F)
	{
		Up10();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_B)
	{
		Down10();
		changeLayer();
	}
	else if (event->nativeVirtualKey() == Qt::Key_N)
	{
		visualization_state = (visualization_state + 1) % 3;
		qDebug() << "New state is " << visualization_state;
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			switch (axis)
			{
			case 0:
				genTextureImageXY();
				break;
			case 1:
				genTextureImageYZ();
				break;
			case 2:
				genTextureImageXZ();
				break;
			}
			//Load2DTexture();
		}
	}
	else if (event->nativeVirtualKey() == Qt::Key_L)
	{
		QString fileName = QFileDialog::getOpenFileName(this,
			"Load new file", "~/", "Binary Files (*.bin)");
		LoadData(fileName);
	}
	else if (event->nativeVirtualKey() == Qt::Key_A)
	{
		axis = (axis + 1) % 3;
		qDebug() << "New axis is " << axis;
		switch (axis)
		{
		case 0:
			resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
			break;
		case 1:
			resize(clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getDepth(), MIN_WIN_SIZE, MAX_WIN_SIZE));
			break;
		case 2:
			resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getDepth(), MIN_WIN_SIZE, MAX_WIN_SIZE));
			break;
		}
	}

	update();
}

void View::setQuads(bool checked)
{
	if (checked)
	{
		visualization_state = 0;
		qDebug() << "New state is " << visualization_state;
		update();
	}
}

void View::setQuadstips(bool checked)
{
	if (checked)
	{
		visualization_state = 1;
		qDebug() << "New state is " << visualization_state;
		update();
	}
}

void View::setTexture(bool checked)
{
	if (checked)
	{
		visualization_state = 2;
		qDebug() << "New state is " << visualization_state;
		switch (axis)
		{
		case 0:
			genTextureImageXY();
			break;
		case 1:
			genTextureImageYZ();
			break;
		case 2:
			genTextureImageXZ();
			break;
		}
		update();
	}
}

void View::axisSetXY(bool checked)
{
	if (checked)
	{
		//axis = (axis + 1) % 3;
		axis = 0;
		qDebug() << "New axis is " << axis;
		resize(clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getDepth(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			switch (axis)
			{
			case 0:
				genTextureImageXY();
				break;
			case 1:
				genTextureImageYZ();
				break;
			case 2:
				genTextureImageXZ();
				break;
			}
			//Load2DTexture();
		}
		update();
	}
}

void View::axisSetYZ(bool checked)
{
	if (checked)
	{
		//axis = (axis + 1) % 3;
		axis = 1;
		qDebug() << "New axis is " << axis;
		resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getHeight(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			switch (axis)
			{
			case 0:
				genTextureImageXY();
				break;
			case 1:
				genTextureImageYZ();
				break;
			case 2:
				genTextureImageXZ();
				break;
			}
			//Load2DTexture();
		}
		update();
	}
}

void View::axisSetXZ(bool checked)
{
	if (checked)
	{
		//axis = (axis + 1) % 3;
		axis = 2;
		qDebug() << "New axis is " << axis;
		resize(clamp(data.getWidth(), MIN_WIN_SIZE, MAX_WIN_SIZE), clamp(data.getDepth(), MIN_WIN_SIZE, MAX_WIN_SIZE));
		if (visualization_state == VISUALIZATION_TEXTURE)
		{
			switch (axis)
			{
			case 0:
				genTextureImageXY();
				break;
			case 1:
				genTextureImageYZ();
				break;
			case 2:
				genTextureImageXZ();
				break;
			}
			//Load2DTexture();
		}
		update();
	}
}

void View::loadNewData(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"Load new file", "~/", "Binary Files (*.bin)");
	LoadData(fileName);
}

void View::toggleUserMM(int state)
{
	if (state == Qt::Checked) userMM = true;
	else userMM = false;
	update();
}

void View::setMM(bool checked)
{
	bool ok = false;

	int min = QInputDialog::getInt(this,
		"Set Min", "Enter new min value", data.getMin(), data.getMin(), data.getMax(), 1, &ok);
	if (!ok) return;

	int max = QInputDialog::getInt(this,
		"Set Max", "Enter new max value", data.getMax(), data.getMin(), data.getMax(), 1, &ok);
	if (!ok) return;
	
	userMin = min;
	userMax = max;
}