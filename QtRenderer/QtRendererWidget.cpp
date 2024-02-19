#include "QtRendererWidget.h"
#include <QPainter>
#include <QTime>

QtRendererWidget::QtRendererWidget(QWidget* parent) : QWidget(parent),
                                                      width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), depth(DEFAULT_DEPTH), model(nullptr), lastFrameTime(0),
                                                      lightDir(Vec3f(2, 2, 2)), lastPos({ 0,0 }), ratio(0.f),
                                                      useShadow(false), zbufferType(0), multiThread(true),shadingType(0),
                                                      ui(new Ui::QtRendererWidget), deltaTime(0)
{
	ui->setupUi(this);
	setFixedSize(width, height);
	ui->FPSLabel->setStyleSheet("background:transparent");
	ui->FPSLabel->setVisible(false);
	camera = new Camera(width * 1.f / height);
	initDevice();
	displayZbufferType();
	displayMultiThread();
	connect(&timer, &QTimer::timeout, this, &QtRendererWidget::render);
	timer.start(1);
}

void QtRendererWidget::displayZbufferType()
{
	if(zbufferType==0)
	{
		std::cout<<"use zbuffer"<<std::endl;
	}
	else if(zbufferType==1)
	{
		std::cout<<"use scanline zbuffer"<<std::endl;
	}
	else if(zbufferType==2)
	{
		std::cout<<"use hierarchy zbuffer"<<std::endl;
	}
	else if(zbufferType==3)
	{
		std::cout<<"use hierarchy zbuffer + octTree"<<std::endl;
	}
}

void QtRendererWidget::displayMultiThread()
{
	if(multiThread)
	{
		std::cout<<"open multiThread"<<std::endl;
	}
	else
	{
		std::cout<<"close multiThread"<<std::endl;
	}
}

void QtRendererWidget::displayTime()
{
	std::cout<<"Last frame time cost: "<<deltaTime<<" ms"<<std::endl;
}


void QtRendererWidget::initDevice() {
	RenderInstance::init(width, height, depth);
}

void QtRendererWidget::paintEvent(QPaintEvent* paintevent) {
	QPainter frameBufferPainter;
	frameBufferPainter.begin(this);
	frameBufferPainter.drawImage(0, 0, RenderInstance::getInstance().getBuffer());
}

void QtRendererWidget::loadModel(std::string filePath) {
	model = new Model(filePath);
	model->displayMeshNumber();
	ui->FPSLabel->setVisible(true);

	RenderInstance& instance=RenderInstance::getInstance();
	instance.model=model;
	instance.setTextureToShader();
}

void QtRendererWidget::saveImage(QString filePath) {
	RenderInstance::getInstance().saveImage(filePath);
}

void QtRendererWidget::setUseShadow(bool use) {
	this->useShadow = use;
	RenderInstance::getInstance().setUseShadow(use);
}

void QtRendererWidget::setZbufferType(int type)
{
	this->zbufferType=type;
	RenderInstance::getInstance().setZbufferType(type);
}

void QtRendererWidget::setMultiThread(bool multiThread)
{
	this->multiThread=multiThread;
	RenderInstance::getInstance().setMultiThread(multiThread);	
}

void QtRendererWidget::setShadingType(int type)
{
	this->shadingType=type;
	RenderInstance::getInstance().setShadingType(type);
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
	RenderInstance& instance=RenderInstance::getInstance();
	instance.clearBuffer();
	
	int nowTime = QTime::currentTime().msecsSinceStartOfDay();
	if (lastFrameTime != 0)
	{
		deltaTime = nowTime - lastFrameTime;
		ui->FPSLabel->setText(QStringLiteral("FPS : ") + QString::number(1000.0 / deltaTime, 'f', 0));
	}
	lastFrameTime = nowTime;

	if (model == nullptr)return;
	
	processInput();
	
	// instance.setUseShadow(useShadow);
	// instance.setZbufferType(zbufferType);

	if(zbufferType==2||zbufferType==3)
	{
		instance.clearZPyramid();
	}

	instance.camera=camera;
	
	if (useShadow) {
		Vec3f camPos = camera->getPosition();
		camera->setPosition(lightDir);
		
		instance.depthShader->Model_=Matrix::identity();
		instance.depthShader->View_ = camera->getViewMatrix();
		instance.depthShader->Projection_ = camera->getPerspectiveMatrix();
		instance.depthShader->ViewPort_ = viewport(0, 0, width, height);
		instance.depthShader->eye = camera->getPosition();
		instance.depthShader->light_dir = lightDir;
		
		camera->setPosition(camPos);
	}
	
	instance.colorShader->Model_= Matrix::identity();
	instance.colorShader->View_ = camera->getViewMatrix();
	instance.colorShader->Projection_ = camera->getPerspectiveMatrix();
	instance.colorShader->ViewPort_ = viewport(0, 0, width, height);
	instance.colorShader->eye = camera->getPosition();
	instance.colorShader->light_dir = lightDir;
	
	// model->draw();
	instance.render();
	
	update();
}