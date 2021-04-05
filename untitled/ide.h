#ifndef IDE_H
#define IDE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ide; }
QT_END_NAMESPACE

class ide : public QMainWindow
{
    Q_OBJECT

public:
    ide(QWidget *parent = nullptr);
    ~ide();

private:
    Ui::ide *ui;
};
#endif // IDE_H
