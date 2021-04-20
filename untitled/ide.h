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

private slots:
    void on_runBut_clicked();

    void on_stop_clicked();

    void on_atras_clicked();

    void on_delante_clicked();

    void verCorriendo(int pos);

    void imprimirMalas();

private:
    Ui::ide *ui;
};
#endif // IDE_H
