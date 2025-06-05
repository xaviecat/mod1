#ifndef TEST_HPP
# define TEST_HPP
# include "../core/Map.hpp"
# include "../utils/colors.h"
# include <QKeyEvent>
# include <QMatrix4x4>
# include <QMouseEvent>
# include <QOpenGLFunctions>
# include <QOpenGLWidget>
# include <QPainter>
# include <QTime>
# include <QTimer>
# include <QWheelEvent>
# include <iostream>
# include <QOpenGLBuffer>
# define SIZE_MAP 2.5

class Test : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit Test(QWidget *parent = 0);
	~Test() override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

	void drawGizmo();

	void initializeVertices();
	void drawVerticesOneByOne();

	void initializeVertexArray();
	void drawVertexArray();

	void initializeIndexArray();

	void initializeBuffers();
	void drawBuffers();
	void drawIndexArray();

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
	QPoint	lastPos;
public slots:
	virtual void timeOutSlot();
private:
	QTimer		timer;
	QTime		timeLastFrame;
	int			frameCount;
	int			lastCount;
	int			delay;

	QVector<QVector<QVector3D>>		rawMap ={
		{{0, 0, 0},{10, 0, 0},{20, 0, 0}, {30, 0, 0}},
		{{0, 0, 10},{10, 20, 10},{20, 15, 10}, {30, 0, 10}},
		{{0, 0, 20},{10, 5, 20},{20, 10, 20}, {30, 0, 20}},
		{{0, 0, 40},{10, 0, 40},{20, 0, 40}, {30, 0, 40}},
	};
	float			maxX = 30; //for now each dim max for square scaling
	float			maxY = 20; //for now each dim max for square scaling
	float			maxZ = 40; //for now each dim max for square scaling

	Map				vertices;
	QString			modeName[4] = {
		"Send vertices one by one",
		"Send vertex array",
		"Send index array",
		"Send buffers" };
	enum eRenderingMode {
		VERTICES,
		VERTEXARRAY,
		INDEXARRAY,
		BUFFERS
	};
	eRenderingMode	mode = VERTICES;
	Map				vertexArray;
	QVector<GLuint>	indexArray;
	QOpenGLBuffer	vertexBuffer;
	QOpenGLBuffer	indexBuffer;

	int				vertices_by_x = 4;
	int				vertices_by_z = 4;
	int				quads_by_x = 3;
	int				quads_by_z = 3;
};

#endif //TEST_HPP
