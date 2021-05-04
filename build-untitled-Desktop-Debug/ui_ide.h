/********************************************************************************
** Form generated from reading UI file 'ide.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IDE_H
#define UI_IDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ide
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *runBut;
    QPushButton *clearBut;
    QLabel *label_2;
    QPlainTextEdit *editor;
    QTextEdit *stdout;
    QPushButton *atras;
    QPushButton *delante;
    QTextEdit *viendo;
    QPushButton *stop;
    QTextEdit *log;
    QTextEdit *ramLiveView;
    QMenuBar *menubar;
    QMenu *menuIDE_C;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ide)
    {
        if (ide->objectName().isEmpty())
            ide->setObjectName(QString::fromUtf8("ide"));
        ide->setEnabled(true);
        ide->resize(1100, 750);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ide->sizePolicy().hasHeightForWidth());
        ide->setSizePolicy(sizePolicy);
        ide->setMinimumSize(QSize(1100, 750));
        ide->setMaximumSize(QSize(1100, 750));
        centralwidget = new QWidget(ide);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 545, 181, 21));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);
        runBut = new QPushButton(centralwidget);
        runBut->setObjectName(QString::fromUtf8("runBut"));
        runBut->setGeometry(QRect(10, 0, 89, 31));
        runBut->setCursor(QCursor(Qt::PointingHandCursor));
        clearBut = new QPushButton(centralwidget);
        clearBut->setObjectName(QString::fromUtf8("clearBut"));
        clearBut->setGeometry(QRect(650, 545, 89, 21));
        clearBut->setCursor(QCursor(Qt::PointingHandCursor));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(750, 0, 341, 31));
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setLayoutDirection(Qt::LeftToRight);
        editor = new QPlainTextEdit(centralwidget);
        editor->setObjectName(QString::fromUtf8("editor"));
        editor->setGeometry(QRect(10, 30, 731, 361));
        stdout = new QTextEdit(centralwidget);
        stdout->setObjectName(QString::fromUtf8("stdout"));
        stdout->setEnabled(true);
        stdout->setGeometry(QRect(10, 400, 731, 141));
        atras = new QPushButton(centralwidget);
        atras->setObjectName(QString::fromUtf8("atras"));
        atras->setEnabled(false);
        atras->setGeometry(QRect(120, 0, 31, 31));
        atras->setCursor(QCursor(Qt::PointingHandCursor));
        delante = new QPushButton(centralwidget);
        delante->setObjectName(QString::fromUtf8("delante"));
        delante->setEnabled(false);
        delante->setGeometry(QRect(150, 0, 31, 31));
        delante->setCursor(QCursor(Qt::PointingHandCursor));
        viendo = new QTextEdit(centralwidget);
        viendo->setObjectName(QString::fromUtf8("viendo"));
        viendo->setEnabled(false);
        viendo->setGeometry(QRect(190, 0, 451, 31));
        viendo->viewport()->setProperty("cursor", QVariant(QCursor(Qt::BusyCursor)));
        stop = new QPushButton(centralwidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setEnabled(false);
        stop->setGeometry(QRect(660, 0, 81, 31));
        stop->setCursor(QCursor(Qt::PointingHandCursor));
        log = new QTextEdit(centralwidget);
        log->setObjectName(QString::fromUtf8("log"));
        log->setGeometry(QRect(10, 570, 731, 131));
        log->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ForbiddenCursor)));
        ramLiveView = new QTextEdit(centralwidget);
        ramLiveView->setObjectName(QString::fromUtf8("ramLiveView"));
        ramLiveView->setGeometry(QRect(750, 30, 341, 671));
        ramLiveView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::WaitCursor)));
        ide->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ide);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1100, 22));
        menuIDE_C = new QMenu(menubar);
        menuIDE_C->setObjectName(QString::fromUtf8("menuIDE_C"));
        ide->setMenuBar(menubar);
        statusbar = new QStatusBar(ide);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ide->setStatusBar(statusbar);

        menubar->addAction(menuIDE_C->menuAction());

        retranslateUi(ide);

        QMetaObject::connectSlotsByName(ide);
    } // setupUi

    void retranslateUi(QMainWindow *ide)
    {
        ide->setWindowTitle(QCoreApplication::translate("ide", "ide", nullptr));
        label->setText(QCoreApplication::translate("ide", "Aplication Log", nullptr));
        runBut->setText(QCoreApplication::translate("ide", "RUN", nullptr));
        clearBut->setText(QCoreApplication::translate("ide", "CLEAR", nullptr));
        label_2->setText(QCoreApplication::translate("ide", "                     RAM LIVE VIEW", nullptr));
        atras->setText(QCoreApplication::translate("ide", "<", nullptr));
        delante->setText(QCoreApplication::translate("ide", ">", nullptr));
        stop->setText(QCoreApplication::translate("ide", "STOP", nullptr));
        menuIDE_C->setTitle(QCoreApplication::translate("ide", "IDE - C!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ide: public Ui_ide {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IDE_H
