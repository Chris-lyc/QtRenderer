#include "QtRenderer.h"
#include <QFileDialog>

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
    if (!filePath.isEmpty())
        qtrenderer_widget_->loadModel(filePath);
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

void QtRenderer::Creat_ToolBar() {
    pToolBar = addToolBar(tr("&Main"));
    pToolBar->addAction(Action_LoadModel);
    pToolBar->addAction(Action_SaveImage);
    pToolBar->addWidget(Button_ShadowMenu);
}

void QtRenderer::Create_Action() {
    Action_LoadModel = new QAction(tr("&Model"), this);
    connect(Action_LoadModel, &QAction::triggered, this, &QtRenderer::on_action_LoadModel_trigger);
    Action_SaveImage = new QAction(tr("&Save"), this);
    connect(Action_SaveImage, &QAction::triggered, this, &QtRenderer::on_action_SaveImage_trigger);

    Button_ShadowMenu = new QToolButton(this);
    Button_ShadowMenu->setText(tr("&Shadow"));
    Menu_ShadowMenu = new QMenu(this);
    Action_ShadowON = Menu_ShadowMenu->addAction("ON");
    connect(Action_ShadowON, &QAction::triggered, this, &QtRenderer::on_action_ShdowON_trigger);
    Action_ShadowOFF = Menu_ShadowMenu->addAction("OFF");
    connect(Action_ShadowOFF, &QAction::triggered, this, &QtRenderer::on_action_ShadowOFF_trigger);
    Button_ShadowMenu->setMenu(Menu_ShadowMenu);
    Button_ShadowMenu->setPopupMode(QToolButton::InstantPopup);
}
