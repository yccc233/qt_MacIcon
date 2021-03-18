#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("translate : PNG -> ICNS");
    ui->lineEdit_load->setText(HomePath+"/icon.png");
    ui->lineEdit_save->setText(HomePath);

    per = new QProcess(this);
    connect(per,&QProcess::errorOccurred,[=]() {
        QMessageBox::warning(this,"error","process has errors;");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    per->close();
}


void MainWindow::on_pushButton_load_clicked()
{
    QString getfile = QFileDialog::getOpenFileName(this,"获取文件位置",HomePath,"文件(*.png)");
    if(getfile.isEmpty())
        return;
    ui->lineEdit_load->setText(getfile);
}

void MainWindow::on_pushButton_save_clicked()
{
    QString savefile = QFileDialog::getExistingDirectory(this,"获取文件保存位置",HomePath);
    if(savefile.isEmpty())
        return;
    ui->lineEdit_save->setText(savefile);
}

void MainWindow::on_pushButton_clicked()
{
    /*
     * 准备工作
     * 文件名、
     * 来源路径、
     * 保存路径
     */
    QString temp = ui->lineEdit_load->text();
    filename = temp.mid(temp.lastIndexOf('/')+1);
    png_source_path = temp.remove(temp.lastIndexOf('/'),filename.length()+1);
    png_save_path = ui->lineEdit_save->text();

    /*
     * 指令集，并调用shell命令
     */
    QStringList order;

    /*
     * 查看文件路径有无icns.iconset
     * 没有就新建一个
     */
    order << "-c" << "ls "+png_source_path;
    per->start(program,order);
    per->waitForFinished();
    output = per->readAllStandardOutput();
    if(!output.contains(filename))
    {
        QMessageBox::warning(this,"error","there is no '"+filename+"'");
        return;
    }
    if(!output.contains(iconset))
    {
        order.clear();
        order << "-c" << "mkdir "+png_source_path+"/"+iconset;
        per->start(program,order);
        per->waitForFinished();
    }

    workpath = png_source_path+"/"+iconset;

    for(int i = 16; i < 1024; i*=2)
    {
        temp = QString("sips -z %1 %1 %2 --out %3icon_%1.png")
                .arg(QString::number(i)).arg(png_source_path+"/"+filename)
                .arg(workpath+"/");
        order.clear();
        order << "-c" << temp;
        per->start(program,order);
        per->waitForFinished();

        temp = QString("sips -z %1 %1 %2 --out %3icon_%1@2x.png")
                .arg(QString::number(i)).arg(png_source_path+"/"+filename)
                .arg(workpath+"/");
        order.clear();
        order << "-c" << temp;
        per->start(program,order);
        per->waitForFinished();
    }
    order.clear();
    order << "-c" << QString("iconutil -c icns "+workpath+" -o "+png_save_path+"/Mac_icon.icns");
    per->start(program,order);
    per->waitForFinished();

    order.clear();
    order << "-c" << QString("iconutil -c icns "+workpath+" -o "+"src/Mac_icon副本.icns");
    per->start(program,order);
    per->waitForFinished();

    order.clear();
    order << "-c" << QString("rm -r -f "+workpath);
    per->start(program,order);
    per->waitForFinished();
    qDebug() << QString("rm -r -f "+workpath);

    QMessageBox::information(this,"complete","work done!");
}
