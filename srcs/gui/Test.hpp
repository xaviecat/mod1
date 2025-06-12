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
# include <QOpenGLShaderProgram>
# define SIZE_MAP 5.0

class Test : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit Test(QWidget *parent = nullptr);
	~Test() override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

	void drawGizmo(QMatrix4x4 &mvpMatrix);

	void initializeVertices();
	void initializeIndexArray();
	void initializeNormales();
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
	QPoint	lastPos;
public slots:
	virtual void timeOutSlot();
private:
	QTimer		timer;
	QTime		timeLastFrame;
	int			frameCount;
	int			lastCount;
	int			delay;

	QVector<QVector<QVector3D>>		rawMap = {
		{{0, 90, 0}, {1, 87, 0}, {2, 90, 0}, {3, 90, 0}, {4, 87, 0}, {5, 77, 0}, {6, 69, 0}, {7, 62, 0}, {8, 59, 0}, {9, 60, 0}},
		{{0, 89, 1}, {1, 88, 1}, {2, 90, 1}, {3, 89, 1}, {4, 84, 1}, {5, 75, 1}, {6, 67, 1}, {7, 57, 1}, {8, 54, 1}, {9, 56, 1}},
		{{0, 85, 2}, {1, 83, 2}, {2, 83, 2}, {3, 82, 2}, {4, 150, 2}, {5, 180, 2}, {6, 150, 2}, {7, 60, 2}, {8, 56, 2}, {9, 54, 2}},
		{{0, 79, 3}, {1, 75, 3}, {2, 76, 3}, {3, 76, 3}, {4, 200, 3}, {5, 220, 3}, {6, 200, 3}, {7, 64, 3}, {8, 58, 3}, {9, 55, 3}},
		{{0, 76, 4}, {1, 70, 4}, {2, 67, 4}, {3, 69, 4}, {4, 220, 4}, {5, 240, 4}, {6, 220, 4}, {7, 60, 4}, {8, 58, 4}, {9, 56, 4}},
		{{0, 76, 5}, {1, 64, 5}, {2, 60, 5}, {3, 60, 5}, {4, 240, 5}, {5, 255, 5}, {6, 240, 5}, {7, 57, 5}, {8, 57, 5}, {9, 57, 5}},
		{{0, 71, 6}, {1, 61, 6}, {2, 54, 6}, {3, 52, 6}, {4, 220, 6}, {5, 240, 6}, {6, 220, 6}, {7, 55, 6}, {8, 57, 6}, {9, 61, 6}},
		{{0, 58, 7}, {1, 53, 7}, {2, 48, 7}, {3, 48, 7}, {4, 200, 7}, {5, 220, 7}, {6, 200, 7}, {7, 60, 7}, {8, 62, 7}, {9, 66, 7}},
		{{0, 55, 8}, {1, 54, 8}, {2, 53, 8}, {3, 58, 8}, {4, 150, 8}, {5, 180, 8}, {6, 150, 8}, {7, 64, 8}, {8, 66, 8}, {9, 72, 8}},
		{{0, 53, 9}, {1, 54, 9}, {2, 57, 9}, {3, 59, 9}, {4, 57, 9}, {5, 58, 9}, {6, 66, 9}, {7, 69, 9}, {8, 67, 9}, {9, 71, 9}},
	};

	QVector<QVector3D>				vertices;
	QString			modeName[4] = {
		"Shaders drawing",
		"Buffers drawing"
	};
	enum eRenderingMode {
		SHADERS,
		BUFFERS
	};

	eRenderingMode		mode = SHADERS;
	int					fillMode = GL_FILL;
	Map					vertexArray;
	QVector<GLuint>		indexArray;
	QVector<QVector2D>	textures;

	QMatrix4x4			mvpMatrix;

	QOpenGLBuffer		vertexBuffer;
	QOpenGLBuffer		indexBuffer;
	QOpenGLBuffer		normalBuffer;

	QVector<QVector3D>	normales;
	qsizetype				vertices_by_x;
	qsizetype				vertices_by_z;
	qsizetype				quads_by_x;
	qsizetype				quads_by_z;
	//Shader zone
	QOpenGLShaderProgram	program;
	int						matrixLocation;
	int						vertexAttribute;
	int						normalAttribute;

	bool					animation = false;
	float					light_alpha = 0;
};

#endif //TEST_HPP
