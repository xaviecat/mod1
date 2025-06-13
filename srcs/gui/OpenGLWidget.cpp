#include "OpenGLWidget.hpp"

#include "../core/Triangulator.hpp"

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, vertices(Map("resources/big_map2.mod1"))
	, indexArray(Triangulator(vertices))
	, vertexBuffer(QOpenGLBuffer::VertexBuffer)
	, indexBuffer(QOpenGLBuffer::IndexBuffer) {
	std::cout << C_MSG("Test parametric constructor called") << std::endl;

	distance = -2.5;
	xRot = 30;
	yRot = -40;
	zRot = 0;
	vertices_by_x = 10;// = 257;
	vertices_by_z = 10;// =257;
	quads_by_x = 9;// = 256;
	quads_by_z = 9;// = 256;
	setWindowTitle("Draw Coordinate");
	// initializeVertices();
	vertices.normalize();
	// setup display refresh
	connect(&timer, &QTimer::timeout, this, &OpenGLWidget::timeOutSlot);
	delay = 0;
	timer.start(delay);
	frameCount = 0;
	lastCount = 0;
	timeLastFrame = QTime::currentTime();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	// initializeIndexArray();
	// initializeNormales();
	normales = indexArray.normales;
	initializeBuffers();
	initializeShaders();
}

void OpenGLWidget::paintGL() {
	refreshMVPMatrix();

	switch (mode) {
	case SHADERS:
		drawShaders();
		break;
	case BUFFERS:
		drawBuffers();
		break;
	}

	drawGizmo(mvpMatrix);

	++frameCount;
	QTime	actualTime = QTime::currentTime();
	if (timeLastFrame.msecsTo(actualTime) >= 1000)
	{
		lastCount = frameCount;
		frameCount = 0;
		timeLastFrame = QTime::currentTime();
	}
}

void OpenGLWidget::paintEvent(QPaintEvent *event) {
	QOpenGLWidget::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.drawText(10, 15, QString("FPS: %1").arg(lastCount));
	painter.drawText(10, 30, "Spacebar to change transfer mode");
	// painter.drawText(10, 45, "Press T to draw on/off texture");
	// painter.drawText(10, 60, "Press F to fill on/off");
	painter.drawText(10, height()-15, QString("%1").arg(modeName[mode]));
}

void OpenGLWidget::initializeShaders() {
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/terrain.vert"))
		std::cerr << program.log().toStdString() << std::endl;
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/terrain.frag"))
		std::cerr << program.log().toStdString() << std::endl;
	if (!program.link())
		std::cerr << program.log().toStdString() << std::endl;

	matrixLocation = program.uniformLocation("mvpmatrix");
	vertexAttribute = program.attributeLocation("vertex");
	normalAttribute = program.attributeLocation("normal");

	program.bind();
	program.setUniformValue("ambiant_color", QVector4D(0.4, 0.4, 0.4, 1.0));
	program.setUniformValue("light_direction", QVector4D(cos(light_alpha), 1.0, sin(light_alpha), 1.0));
	program.release();

	vertexBuffer.bind();
	program.setAttributeBuffer(vertexAttribute, GL_FLOAT, 0, 3);
	vertexBuffer.release();

	normalBuffer.bind();
	program.setAttributeBuffer(normalAttribute, GL_FLOAT, 0, 0);
	normalBuffer.release();
}

void OpenGLWidget::drawShaders() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);
	// glPolygonMode(GL_BACK, GL_LINE);

	program.bind();
	program.setUniformValue(matrixLocation, mvpMatrix);

	vertexBuffer.bind();
	program.enableAttributeArray(vertexAttribute);
	program.setAttributeBuffer(vertexAttribute, GL_FLOAT, 0 , 3);
	vertexBuffer.release();

	normalBuffer.bind();
	program.enableAttributeArray(normalAttribute);
	program.setAttributeBuffer(normalAttribute, GL_FLOAT, 0 , 3);
	normalBuffer.release();

	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, nullptr);
	indexBuffer.release();
	program.disableAttributeArray(vertexAttribute);

	if (animation) {
		light_alpha += 0.02;
		if (light_alpha > 2 * M_PI) light_alpha -= 2 * M_PI;
		program.setUniformValue("light_direction", QVector4D(cos(light_alpha), 1.0, sin(light_alpha), 1.0));
	}

	program.release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLWidget::refreshMVPMatrix() {
	QMatrix4x4	modelMatrix;
	QMatrix4x4	viewMatrix;
	QMatrix4x4	projectionMatrix;

	viewMatrix.translate(0.0, 0.0, distance);

	modelMatrix.rotate(xRot, 1.0f, 0.0f, 0.0f);
	modelMatrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
	modelMatrix.rotate(zRot, 0.0f, 0.0f, 1.0f);

	projectionMatrix.perspective(60.0f, width() / height(), 0.1f, 500.0f);

	mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}

void OpenGLWidget::initializeBuffers() {
	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
	vertexBuffer.release();

	indexBuffer.create();
	indexBuffer.bind();
	indexBuffer.allocate(indexArray.constData(), indexArray.size() * sizeof(GLuint));
	indexBuffer.release();

	normalBuffer.create();
	normalBuffer.bind();
	normalBuffer.allocate(normales.constData(), normales.size() * sizeof(QVector3D));
	normalBuffer.release();

}

void OpenGLWidget::drawBuffers() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, fillMode);
	glPolygonMode(GL_BACK, GL_LINE);

	glColor3f(0.0, 0.5, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);

	vertexBuffer.bind();
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	vertexBuffer.release();

	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, nullptr);
	indexBuffer.release();

	glDisableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLWidget::initializeNormales() {
	normales.resize(vertices.size());

	for (int z = 0; z < quads_by_z; ++z) {
		for (int x = 0; x < quads_by_x; ++x) {
			int i = z * vertices_by_x + x;

			int i1 = i;
			int i2 = i + vertices_by_x;
			int i3 = i + 1;

			QVector3D v1 = vertices[i2] - vertices[i1];
			QVector3D v2 = vertices[i3] - vertices[i1];
			QVector3D normal1 = QVector3D::crossProduct(v1, v2).normalized();

			normales[i1] += normal1;
			normales[i2] += normal1;
			normales[i3] += normal1;

			int i4 = i + 1;
			int i5 = i + vertices_by_x;
			int i6 = i + 1 + vertices_by_x;

			QVector3D v3 = vertices[i5] - vertices[i4];
			QVector3D v4 = vertices[i6] - vertices[i4];
			QVector3D normal2 = QVector3D::crossProduct(v3, v4).normalized();

			normales[i4] += normal2;
			normales[i5] += normal2;
			normales[i6] += normal2;
		}
	}

	for (int i = 0; i < normales.size(); ++i) {
		normales[i].normalize();
	}
}

void OpenGLWidget::initializeIndexArray() {
	for (int z = 0; z < quads_by_z; ++z) {
		for (int x = 0; x < quads_by_x; ++x) {
			int i = z * vertices_by_x + x;

			//first triangle
			indexArray.push_back(i);
			indexArray.push_back(i + vertices_by_x);
			indexArray.push_back(i + 1);

			//second triangle
			indexArray.push_back(i + 1);
			indexArray.push_back(i + vertices_by_x);
			indexArray.push_back(i + 1 + vertices_by_x);
		}
	}
}

void OpenGLWidget::initializeVertices() {
	for (qsizetype z = 0; z < vertices_by_z; z++) {
		for (qsizetype x = 0; x < vertices_by_x; x++) {
			QVector3D	vertex;
			vertex.setX((SIZE_MAP * rawMap[z][x].x() / vertices_by_x) - SIZE_MAP / 2);
			if (x == 0 || x == vertices_by_x - 1 || z == 0 || z == vertices_by_z - 1)
				vertex.setY(0);
			else
				vertex.setY(2.0 * rawMap[z][x].y() / 255);
			vertex.setZ((SIZE_MAP * rawMap[z][x].z() / vertices_by_z) - SIZE_MAP / 2);
			vertices.push_back(vertex);
		}
	}
}

void OpenGLWidget::drawGizmo(QMatrix4x4 &mvpMatrix) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mvpMatrix.constData());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBegin(GL_LINES); // x line (red)
	{
		glColor3d(1,0,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES); // y line (blue)
	{
		glColor3d(0,0,1);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES); // z line (green)
	{
		glColor3d(0,1,0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	}
	glEnd();
}

void OpenGLWidget::resizeGL(int width, int height) {
	glViewport(0,0, width, height);
}

void OpenGLWidget::keyPressEvent(QKeyEvent *keyEvent) {
	if (keyEvent->key() == Qt::Key_Escape)
		close();
	if (keyEvent->key() == Qt::Key_Space)
		mode  = static_cast<eRenderingMode>((mode + 1) % 2);
	if (keyEvent->key() == Qt::Key_F)
		fillMode = fillMode == GL_LINE ? GL_FILL : GL_LINE;
	if (keyEvent->key() == Qt::Key_L)
		animation = !animation;
	// if (keyEvent->key() == Qt::Key_Up)
	// 	if (delay < 50) ++delay;
	// if (keyEvent->key() == Qt::Key_Down)
	// 	if (delay > 0) --delay;
	// 		timer.start(delay);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
	distance *= 1.0 + (1.0 * event->angleDelta().y() / 1200.0);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
	lastPos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
	int dx = event->position().x() - lastPos.x();
	int dy = event->position().y() - lastPos.y();

	if (event->buttons() & Qt::RightButton)
	{
		rotateBy(dy*8, 0, 0);
		rotateBy(0, dx*8, 0);
	}
	lastPos = event->pos();
}

void OpenGLWidget::rotateBy(int x, int y, int z) {
	xRot += x;
	yRot += y;
	zRot += z;
}

void OpenGLWidget::timeOutSlot() {
	update();
}

OpenGLWidget::~OpenGLWidget() {
	std::cout << C_MSG("Test destructor called") << std::endl;
}
