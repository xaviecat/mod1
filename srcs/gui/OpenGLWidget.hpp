#ifndef TEST_HPP
# define TEST_HPP
# include "../core/Map.hpp"
# include "../core/Triangulator.hpp"
# include "../utils/colors.h"
# include <QKeyEvent>
# include <QMatrix4x4>
# include <QMouseEvent>
# include <QOpenGLFunctions>
# include <QOpenGLWidget>
# include <QOpenGLTexture>
# include <QPainter>
# include <QTime>
# include <QTimer>
# include <QWheelEvent>
# include <iostream>
# include <QOpenGLBuffer>
# include <QOpenGLShaderProgram>
# define SIZE_MAP 5.0

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = nullptr);
	~OpenGLWidget() override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

	void drawGizmo(QMatrix4x4 &mvpMatrix);

	void initializeUVMap();
	void initializeBuffers();
	void initializeShaders();

	void refreshMVPMatrix();

	void drawBuffers();
	void drawShaders();

	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *keyEvent) override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void rotateBy(int x, int y, int z);

	float	distance;
	float	xRot;
	float	yRot;
	float	zRot;
	float	xTrans;
	float	yTrans;
	float	zTrans;
	QPoint	lastPos;
public slots:
	virtual void timeOutSlot();
private:
	QTimer		timer;
	QTime		timeLastFrame;
	int			frameCount;
	int			lastCount;
	int			delay;

	Map				vertices;
	QString			modeName[4] = {
		"Shaders drawing",
		"Buffers drawing"
	};
	enum eRenderingMode	{SHADERS, BUFFERS};

	eRenderingMode		mode;
	int					fillMode;
	Triangulator		indexArray;
	QVector<QVector2D>	UVMap;

	QMatrix4x4			mvpMatrix;

	QOpenGLBuffer		vertexBuffer;
	QOpenGLBuffer		indexBuffer;
	QOpenGLBuffer		normalBuffer;
	QOpenGLBuffer		textureBuffer;

	QOpenGLTexture		*texture;

	QVector<QVector3D>	normales;

	//Shader zone
	QOpenGLShaderProgram	program;
	int						matrixLocation;
	int						textureLocation;

	enum eAttributeLocation	{ vertexAttribute, normalAttribute, textureAttribute};

	bool					animation = false;
	float					light_alpha = 0;
};

#endif //TEST_HPP
