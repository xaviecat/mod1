#include "Test.hpp"

Test::Test(QWidget *parent)
	: QOpenGLWidget(parent)
	, vertexBuffer(QOpenGLBuffer::VertexBuffer)
	, indexBuffer(QOpenGLBuffer::IndexBuffer) {
	std::cout << C_MSG("Test parametric constructor called") << std::endl;

	setWindowTitle("Draw Coordinate");
	initializeVertices();

	// setup display refresh

	connect(&timer, &QTimer::timeout, this, &Test::timeOutSlot);
	delay = 0;
	timer.start(delay);
	frameCount = 0;
	lastCount = 0;
	timeLastFrame = QTime::currentTime();
}

void Test::initializeGL() {
	initializeOpenGLFunctions();

	initializeVertexArray();
	initializeIndexArray();
	initializeBuffers();

	distance = -5.0;
	xRot = 30;
	yRot = -40;
	zRot = 0;
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
}

void Test::paintGL() {
	//save initial matrix
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	QMatrix4x4	modelMatrix;
	QMatrix4x4	viewMatrix;
	QMatrix4x4	projectionMatrix;

	QVector3D	eye(0.0, 0.0, -distance);
	QVector3D	center(0.0, 0.0, 0.0);
	QVector3D	up(0.0, 1.0, 0.0);

	viewMatrix.lookAt(eye, center, up);

	modelMatrix.rotate(xRot, 1.0f, 0.0f, 0.0f);
	modelMatrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
	modelMatrix.rotate(zRot, 0.0f, 0.0f, 1.0f);

	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	projectionMatrix.perspective(60.0f, aspect, 0.1f, 500.0f);

	QMatrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mvpMatrix.constData());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (mode) {
	case VERTICES:
		drawVerticesOneByOne();
		break;
	case VERTEXARRAY:
		drawVertexArray();
		break;
	case INDEXARRAY:
		drawIndexArray();
		break;
	case BUFFERS:
		drawBuffers();
		break;
	}
	drawGizmo();

	//restore initial matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

	++frameCount;
	QTime	actualTime = QTime::currentTime();
	if (timeLastFrame.msecsTo(actualTime) >= 1000)
	{
		lastCount = frameCount;
		frameCount = 0;
		timeLastFrame = QTime::currentTime();
	}
}

void Test::paintEvent(QPaintEvent *event) {
	QOpenGLWidget::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.drawText(10, 15, QString("FPS: %1").arg(lastCount));
	painter.drawText(10, 30, "Spacebar to change transfer mode");
	// painter.drawText(10, 45, "Press T to draw on/off texture");
	// painter.drawText(10, 60, "Press F to fill on/off");
	painter.drawText(10, height()-15, QString("%1").arg(modeName[mode]));
}

void Test::drawBuffers()
{
	glColor3f(1, 1, 1);
	glEnableClientState(GL_VERTEX_ARRAY);

	vertexBuffer.bind();
	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	vertexBuffer.release();

	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, nullptr);
	indexBuffer.release();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Test::initializeBuffers()
{
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

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
	vertexBuffer.release();

	indexBuffer.create();
	indexBuffer.bind();
	indexBuffer.allocate(indexArray.constData(), indexArray.size() * sizeof(GLuint));
	indexBuffer.release();
}

void Test::drawVertexArray()
{
	glColor3f(1, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexArray.constData());
	glDrawArrays(GL_TRIANGLES, 0, vertexArray.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnd();
}

void Test::initializeVertexArray()
{
	for (int z = 0; z < quads_by_z; ++z) {
		for (int x = 0; x < quads_by_x; ++x) {
			int i = z * vertices_by_x + x;

			//first triangle
			vertexArray.push_back(vertices[i]);
			vertexArray.push_back(vertices[i + vertices_by_x]);
			vertexArray.push_back(vertices[i + 1]);

			//second triangle
			vertexArray.push_back(vertices[i + 1]);
			vertexArray.push_back(vertices[i + vertices_by_x]);
			vertexArray.push_back(vertices[i + 1 + vertices_by_x]);
		}
	}
}

void Test::drawIndexArray()
{
	glColor3f(0, 0, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
	glDrawElements(GL_TRIANGLES, indexArray.size(), GL_UNSIGNED_INT, indexArray.constData());
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnd();
}

void Test::initializeIndexArray()
{
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

void Test::drawVerticesOneByOne()
{
	glBegin(GL_TRIANGLES);
	for (int z = 0; z < quads_by_z; ++z) {
		for (int x = 0; x < quads_by_x; ++x) {
			int i = z * vertices_by_x + x;
			glColor3f(0, 1, 0);

			glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
			glVertex3f(vertices[i + vertices_by_x].x(), vertices[i + vertices_by_x].y(), vertices[i + vertices_by_x].z());
			glVertex3f(vertices[i + 1].x(), vertices[i + 1].y(), vertices[i + 1].z());

			glVertex3f(vertices[i + 1].x(), vertices[i + 1].y(), vertices[i + 1].z());
			glVertex3f(vertices[i + vertices_by_x].x(), vertices[i + vertices_by_x].y(), vertices[i + vertices_by_x].z());
			glVertex3f(vertices[i + 1 + vertices_by_x].x(), vertices[i + 1 + vertices_by_x].y(), vertices[i + 1 + vertices_by_x].z());
		}
	}
	glEnd();
}

void Test::initializeVertices()
{
	for (qsizetype z = 0; z < rawMap.size(); z++) {
		for (qsizetype x = 0; x < rawMap[z].size(); x++) {
			QVector3D	vertex;
			vertex.setX((SIZE_MAP * rawMap[z][x].x() / maxX) - SIZE_MAP / 2);
			vertex.setY(rawMap[z][x].y() / maxY);
			vertex.setZ((SIZE_MAP * rawMap[z][x].z() / maxZ) - SIZE_MAP / 2);
			vertices.push_back(vertex);
		}
	}
}

void Test::drawGizmo() {
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

void Test::resizeGL(int width, int height)
{
	glViewport(0,0, width, height);
}

void Test::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->key() == Qt::Key_Escape)
		close();
	if (keyEvent->key() == Qt::Key_Space)
		mode  = static_cast<eRenderingMode>((mode + 1) % 4);
	// if (keyEvent->key() == Qt::Key_Up)
	// 	if (delay < 50) ++delay;
	// if (keyEvent->key() == Qt::Key_Down)
	// 	if (delay > 0) --delay;
	// 		timer.start(delay);
}

void Test::wheelEvent(QWheelEvent *event)
{
	distance *= 1.0 + (1.0 * event->angleDelta().y() / 1200.0);
}

void Test::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void Test::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->position().x() - lastPos.x();
	int dy = event->position().y() - lastPos.y();

	if (event->buttons() & Qt::RightButton)
	{
		rotateBy(dy*8, 0, 0);
		rotateBy(0, dx*8, 0);
	}
	lastPos = event->pos();
}

void Test::rotateBy(int x, int y, int z)
{
	xRot += x;
	yRot += y;
	zRot += z;
}

void Test::timeOutSlot()
{
	update();
}

Test::~Test() {
	std::cout << C_MSG("Test destructor called") << std::endl;
}
