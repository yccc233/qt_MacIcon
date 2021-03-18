#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QApplication>
#include <qdebug.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_load_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString HomePath = QDir::homePath();

    QString png_source_path;
    QString png_save_path;
    QString filename;
    QString iconset = "icon.iconset";
    QString program = "bash";
    QString workpath;

    QStringList orderlist;

    QString error;
    QString output;

    QProcess *per;
};
#endif // MAINWINDOW_H
