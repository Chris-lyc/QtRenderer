#include "QtRenderer.h"
#include <QFileDialog>
#include <QTextCodec>

QtRenderer::QtRenderer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    qtrenderer_widget_ = new QtRendererWidget();
    setCentralWidget(qtrenderer_widget_);
    Create_Action();
    Creat_ToolBar();
}

QtRenderer::~QtRenderer()
{}

void QtRenderer::closeEvent(QCloseEvent* e)
{}

void QtRenderer::paintEvent(QPaintEvent* paintevent)
{}

void QtRenderer::on_action_LoadModel_trigger() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Model File", "", "OBJ(*.obj)");
    QTextCodec* code = QTextCodec:: codecForName("gb18030");
    std::string strbuffer;
    strbuffer = code->fromUnicode(filePath).data();
    
    if (!filePath.isEmpty())
        qtrenderer_widget_->loadModel(strbuffer);
}

void QtRenderer::on_action_SaveImage_trigger() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG(*.png)");
    if (!fileName.isEmpty())
        qtrenderer_widget_->saveImage(fileName);
}

void QtRenderer::on_action_ShdowON_trigger() {
    qtrenderer_widget_->setUseShadow(true);
}

void QtRenderer::on_action_ShadowOFF_trigger() {
    qtrenderer_widget_->setUseShadow(false);
}

void QtRenderer::on_action_Zbuffer_trigger()
{
    qtrenderer_widget_->setZbufferType(0);
    qtrenderer_widget_->displayZbufferType();
}

void QtRenderer::on_action_ScanZbuffer_trigger()
{
    qtrenderer_widget_->setZbufferType(1);
    qtrenderer_widget_->displayZbufferType();
}

void QtRenderer::on_action_HiZ_trigger()
{
    qtrenderer_widget_->setZbufferType(2);
    qtrenderer_widget_->displayZbufferType();
}

void QtRenderer::on_action_HiZOct_trigger()
{
    qtrenderer_widget_->setZbufferType(3);
    qtrenderer_widget_->displayZbufferType();
}

void QtRenderer::on_action_MultiThreadON_trigger()
{
    qtrenderer_widget_->setMultiThread(true);
    qtrenderer_widget_->displayMultiThread();
}

void QtRenderer::on_action_MultiThreadOFF_trigger()
{
    qtrenderer_widget_->setMultiThread(false);
    qtrenderer_widget_->displayMultiThread();
}

void QtRenderer::on_action_NULLShaing_trigger()
{
    qtrenderer_widget_->setShadingType(0);
}

void QtRenderer::on_action_PhongShaing_trigger()
{
    qtrenderer_widget_->setShadingType(1);
}

void QtRenderer::on_action_ShowTime_trigger()
{
    qtrenderer_widget_->displayTime();
}


void QtRenderer::Creat_ToolBar() {
    pToolBar = addToolBar(tr("&Main"));
    pToolBar->addAction(Action_LoadModel);
    pToolBar->addAction(Action_SaveImage);
    pToolBar->addWidget(Button_Shading);
    pToolBar->addWidget(Button_ShadowMenu);
    pToolBar->addWidget(Button_ZbufferType);
    pToolBar->addWidget(Button_MultiThread);
    pToolBar->addAction(Action_ShowTime);
}

void QtRenderer::Create_Action() {
    Action_LoadModel = new QAction(tr("&Model"), this);
    connect(Action_LoadModel, &QAction::triggered, this, &QtRenderer::on_action_LoadModel_trigger);
    Action_SaveImage = new QAction(tr("&Save"), this);
    connect(Action_SaveImage, &QAction::triggered, this, &QtRenderer::on_action_SaveImage_trigger);

    Button_Shading = new QToolButton(this);
    Button_Shading->setText(tr("&Shading"));
    Menu_Shading = new QMenu(this);
    Action_NULLShaing = Menu_Shading->addAction("No shading");
    connect(Action_NULLShaing, &QAction::triggered, this, &QtRenderer::on_action_NULLShaing_trigger);
    Action_PhongShading = Menu_Shading->addAction("Phong");
    connect(Action_PhongShading, &QAction::triggered, this, &QtRenderer::on_action_PhongShaing_trigger);
    Button_Shading->setMenu(Menu_Shading);
    Button_Shading->setPopupMode(QToolButton::InstantPopup);

    Button_ShadowMenu = new QToolButton(this);
    Button_ShadowMenu->setText(tr("&Shadow"));
    Menu_ShadowMenu = new QMenu(this);
    Action_ShadowON = Menu_ShadowMenu->addAction("ON");
    connect(Action_ShadowON, &QAction::triggered, this, &QtRenderer::on_action_ShdowON_trigger);
    Action_ShadowOFF = Menu_ShadowMenu->addAction("OFF");
    connect(Action_ShadowOFF, &QAction::triggered, this, &QtRenderer::on_action_ShadowOFF_trigger);
    Button_ShadowMenu->setMenu(Menu_ShadowMenu);
    Button_ShadowMenu->setPopupMode(QToolButton::InstantPopup);

    Button_ZbufferType = new QToolButton(this);
    Button_ZbufferType->setText(tr("&Zbuffer"));
    Menu_ZbufferType = new QMenu(this);
    Action_Zbuffer = Menu_ZbufferType->addAction("Zbuffer");
    connect(Action_Zbuffer, &QAction::triggered, this, &QtRenderer::on_action_Zbuffer_trigger);
    Action_ScanZbuffer = Menu_ZbufferType->addAction("Scanline zbuffer");
    connect(Action_ScanZbuffer, &QAction::triggered, this, &QtRenderer::on_action_ScanZbuffer_trigger);
    Action_HiZ = Menu_ZbufferType->addAction("Hiz");
    connect(Action_HiZ, &QAction::triggered, this, &QtRenderer::on_action_HiZ_trigger);
    Action_HiZOct = Menu_ZbufferType->addAction("HizOct");
    connect(Action_HiZOct, &QAction::triggered, this, &QtRenderer::on_action_HiZOct_trigger);
    Button_ZbufferType->setMenu(Menu_ZbufferType);
    Button_ZbufferType->setPopupMode(QToolButton::InstantPopup);

    Button_MultiThread=new QToolButton(this);
    Button_MultiThread->setText(tr("&MultiThread"));
    Menu_MultiThread = new QMenu(this);
    Action_MultiThreadON = Menu_MultiThread->addAction("ON");
    connect(Action_MultiThreadON, &QAction::triggered, this, &QtRenderer::on_action_MultiThreadON_trigger);
    Action_MultiThreadOFF = Menu_MultiThread->addAction("OFF");
    connect(Action_MultiThreadOFF, &QAction::triggered, this, &QtRenderer::on_action_MultiThreadOFF_trigger);
    Button_MultiThread->setMenu(Menu_MultiThread);
    Button_MultiThread->setPopupMode(QToolButton::InstantPopup);

    Action_ShowTime = new QAction(tr("&TimeCost"), this);
    connect(Action_ShowTime, &QAction::triggered, this, &QtRenderer::on_action_ShowTime_trigger);
}
