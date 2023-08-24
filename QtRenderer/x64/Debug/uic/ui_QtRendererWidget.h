/********************************************************************************
** Form generated from reading UI file 'QtRendererWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRENDERERWIDGET_H
#define UI_QTRENDERERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtRendererWidget
{
public:
    QLabel *FPSLabel;

    void setupUi(QWidget *QtRendererWidget)
    {
        if (QtRendererWidget->objectName().isEmpty())
            QtRendererWidget->setObjectName(QString::fromUtf8("QtRendererWidget"));
        QtRendererWidget->resize(400, 300);
        FPSLabel = new QLabel(QtRendererWidget);
        FPSLabel->setObjectName(QString::fromUtf8("FPSLabel"));
        FPSLabel->setGeometry(QRect(10, 10, 72, 15));

        retranslateUi(QtRendererWidget);

        QMetaObject::connectSlotsByName(QtRendererWidget);
    } // setupUi

    void retranslateUi(QWidget *QtRendererWidget)
    {
        QtRendererWidget->setWindowTitle(QApplication::translate("QtRendererWidget", "Form", nullptr));
        FPSLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QtRendererWidget: public Ui_QtRendererWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRENDERERWIDGET_H
