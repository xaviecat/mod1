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
# include <QOpenGLVertexArrayObject>
// #define __EMSCRIPTEN__ 1
# define SIZE_MAP 5.0

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = nullptr);
	~OpenGLWidget() override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;


#ifndef __EMSCRIPTEN__
	void drawGizmo();
	void drawBuffers();
#else
	void generateBarycentricCoordinates();
	QVector<QVector3D> barycentricCoords;
	bool wireframeMode = true;
	QOpenGLBuffer barycentricBuffer;
	int wireframeModeLocation = -1;
	int wireframeColorLocation = -1;
	int wireframeWidthLocation = -1;
#endif

	void initializeUVMap();
	void initializeVAO();
	void initializeShaders();

	void refreshMVPMatrix();

	void drawShaders();

	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *keyEvent) override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void rotateBy(int x, int y, int z);

public slots:
	virtual void timeOutSlot();

private:
	// FPS Counter
	QPoint	lastPos;
	QTime	timeLastFrame;
	QTimer	timer;
	int		delay;
	int		frameCount;
	int		lastCount;

	// MVP Matrix
	QMatrix4x4	mvpMatrix;
	float		distance;
	float		xRot;
	float		yRot;
	float		zRot;
	float		xTrans;
	float		yTrans;
	float		zTrans;

	// Light animation
	bool	animation;
	float	light_alpha;

	// Rendering Mode
	QString				renderingMode[2] = {"Shaders drawing", "Buffers drawing"};
	enum eRenderingMode	{SHADERS, BUFFERS};
	eRenderingMode		mode;
	int					fillMode;

	// Mesh
	Map					vertices;
	Triangulator		indexArray;
	QVector<QVector2D>	UVMap;
	QVector<QVector3D>	normales;

	// Mesh Rendering
	QOpenGLVertexArrayObject	terrainVAO;
	QOpenGLBuffer				vertexBuffer;
	QOpenGLBuffer				indexBuffer;
	QOpenGLBuffer				textureBuffer;
	QOpenGLBuffer				normalBuffer;

	// Shaders and Attributes and Uniforms location
	QOpenGLShaderProgram	program;
	int						matrixLocation;
	int						textureLocation;
	int						lightDirectionLocation;
	int						ambientColorLocation;
	enum eAttributeLocation	{ vertexAttribute, normalAttribute, textureAttribute, barycentricAttribute};

	// Texture handling
	QOpenGLTexture	*texture;
};

#endif //TEST_HPP
