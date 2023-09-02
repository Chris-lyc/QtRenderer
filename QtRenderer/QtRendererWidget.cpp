#include "QtRendererWidget.h"
#include <QPainter>
#include <QFont>

QtRendererWidget::QtRendererWidget(QWidget* parent) :QWidget(parent),
width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), depth(DEFAULT_DEPTH), model(nullptr), lastFrameTime(0),
lightDir(Vec3f(2, 2, 2)), lastPos({ 0,0 }), ratio(0.f),
useShadow(false),
ui(new Ui::QtRendererWidget) {
	ui->setupUi(this);
	setFixedSize(width, height);
	ui->FPSLabel->setStyleSheet("background:transparent");
	ui->FPSLabel->setVisible(false);
	camera = new Camera(width * 1.f / height);
	initDevice();
	connect(&timer, &QTimer::timeout, this, &QtRendererWidget::render);
	timer.start(1);
}

void QtRendererWidget::initDevice() {
	RenderInstance::init(width, height, depth);
}

void QtRendererWidget::paintEvent(QPaintEvent* paintevent) {
	QPainter frameBufferPainter;
	frameBufferPainter.begin(this);
	frameBufferPainter.drawImage(0, 0, RenderInstance::getInstance().getBuffer());
}

void QtRendererWidget::loadModel(QString filePath) {
	model = new Model(filePath.toStdString());
	ui->FPSLabel->setVisible(true);
}

void QtRendererWidget::saveImage(QString filePath) {
	RenderInstance::getInstance().saveImage(filePath);
}

void QtRendererWidget::setUseShadow(bool use) {
	this->useShadow = use;
}

Matrix QtRendererWidget::viewport(int x, int y, int w, int h) {
	Matrix ViewPort = Matrix::identity();
	ViewPort[0][3] = x + w / 2.f;
	ViewPort[1][3] = y + h / 2.f;
	ViewPort[2][3] = DEFAULT_DEPTH / 2.f;

	ViewPort[0][0] = w / 2.f;
	ViewPort[1][1] = h / 2.f;
	ViewPort[2][2] = DEFAULT_DEPTH / 2.f;
	return ViewPort;
}

void QtRendererWidget::wheelEvent(QWheelEvent* event) {
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;
	QPoint res;
	if (!numPixels.isNull()) {
		res = numPixels;
	}
	else if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		res = numSteps;
	}
	ratio += res.y();
}

void QtRendererWidget::mousePressEvent(QMouseEvent* event) {
	currentButtons = event->buttons();
	currentPos = event->pos();
	lastPos = { 0,0 };
}
void QtRendererWidget::mouseReleaseEvent(QMouseEvent* event) {
	currentButtons = event->buttons();
}

void QtRendererWidget::mouseMoveEvent(QMouseEvent* event) {
	currentPos = event->pos();
}

void QtRendererWidget::processInput() {
	if (currentButtons == Qt::LeftButton || currentButtons == Qt::RightButton) {
		if (!lastPos.isNull()) {
			Vec2f motion = { (float)(currentPos - lastPos).x(),(float)(currentPos - lastPos).y() };
			motion.x = (motion.x / width);
			motion.y = (motion.y / height);
			if (currentButtons == Qt::LeftButton)
			{
				camera->rotateAroundTarget(motion);
			}
			if (currentButtons == Qt::RightButton)
			{
				camera->moveTarget(motion);
			}
		}
		lastPos = currentPos;
	}
	if (std::abs(ratio) > 1e-6) {
		camera->scale(ratio);
		ratio = 0.f;
	}
}

void QtRendererWidget::render() {
	RenderInstance::getInstance().clearBuffer();
	int nowTime = QTime::currentTime().msecsSinceStartOfDay();
	if (lastFrameTime != 0)
	{
		int deltaTime = nowTime - lastFrameTime;
		ui->FPSLabel->setText(QStringLiteral("FPS : ") + QString::number(1000.0 / deltaTime, 'f', 0));
	}
	lastFrameTime = nowTime;
	if (model == nullptr)return;
	processInput();
	RenderInstance::getInstance().setUseShadow(useShadow);
	if (useShadow) {
		Vec3f camPos = camera->getPosition();
		camera->setPosition(lightDir);
		RenderInstance::getInstance().depthShader->Model_ = Matrix::identity();
		RenderInstance::getInstance().depthShader->View_ = camera->getViewMatrix();
		//RenderInstance::getInstance().depthShader->Projection_ = Matrix::identity();
		RenderInstance::getInstance().depthShader->Projection_ = camera->getPerspectiveMatrix();
		//RenderInstance::getInstance().depthShader->Projection_ = camera.getOrthoMatrix();
		//RenderInstance::getInstance().depthShader->Projection_ = ortho(-1.f, 1.f, -1.f, 1.f, -NEAR, -FAR);
		RenderInstance::getInstance().depthShader->ViewPort_ = viewport(0, 0, width, height);
		RenderInstance::getInstance().depthShader->eye = camera->getPosition();
		RenderInstance::getInstance().depthShader->light_dir = lightDir;
		camera->setPosition(camPos);
	}
	RenderInstance::getInstance().colorShader->Model_= Matrix::identity();
	RenderInstance::getInstance().colorShader->View_ = camera->getViewMatrix();
	RenderInstance::getInstance().colorShader->Projection_ = camera->getPerspectiveMatrix();
	//RenderInstance::getInstance().colorShader->Projection_ = ortho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, NEAR, FAR);
	RenderInstance::getInstance().colorShader->ViewPort_ = viewport(0, 0, width, height);
	RenderInstance::getInstance().colorShader->eye = camera->getPosition();
	RenderInstance::getInstance().colorShader->light_dir = lightDir;
	model->draw();
	update();
}