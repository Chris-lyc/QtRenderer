/********************************************************************************
** Form generated from reading UI file 'QtRenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRENDERER_H
#define UI_QTRENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtRendererClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtRendererClass)
    {
        if (QtRendererClass->objectName().isEmpty())
            QtRendererClass->setObjectName(QString::fromUtf8("QtRendererClass"));
        QtRendererClass->resize(600, 400);
        centralWidget = new QWidget(QtRendererClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtRendererClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        QtRendererClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtRendererClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtRendererClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtRendererClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtRendererClass->setStatusBar(statusBar);

        retranslateUi(QtRendererClass);

        QMetaObject::connectSlotsByName(QtRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtRendererClass)
    {
        QtRendererClass->setWindowTitle(QApplication::translate("QtRendererClass", "QtRenderer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtRendererClass: public Ui_QtRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRENDERER_H
