#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "math.h"
#include "image.h"
#include "aboutdialog.h"
#include "qualityfactordialog.h"

#define RESET_ZOOM ui->horizontalSlider_3->setValue(500)

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //Apply
    int radius=ui->horizontalSlider->value();
    float saturate_v=(float)(ui->horizontalSlider_2->value())/100;
    float gamma=(float)(ui->horizontalSlider_4->value())/100;
    image=copy;
    blur(image,radius);
    saturate(image,saturate_v);
    gammaCorrect(image,gamma);
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionOpen_image_triggered()
{
    //Open Image
    QString fileName=QFileDialog::getOpenFileName(this,"Open Image","",fileExtensions);
    bool res=image.load(fileName);
    if(res){
        copy=image;
        delete scene;
        scene=new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        scene->addPixmap(QPixmap::fromImage(image));
        ui->pushButton->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->horizontalSlider_2->setEnabled(true);
        ui->horizontalSlider_4->setEnabled(true);
        ui->horizontalSlider->setValue(0);
        ui->horizontalSlider_2->setValue(0);
        ui->horizontalSlider_4->setValue(100);
        RESET_ZOOM;
    }else QMessageBox::critical(this,"Error","Unable to open image file");
}

void MainWindow::on_actionSave_Image_triggered()
{
    //Save Image
    QString fileName=QFileDialog::getSaveFileName(this,"Save Image","",fileExtensions);
    if(fileName=="") QMessageBox::critical(this,"Error","Unable to save the image file");
    else{
        QualityFactorDialog d;
        bool res=d.exec();
        if(!res) QMessageBox::critical(this,"Error","Unable to save the image file");
        else image.save(fileName,"",d.qualityFactor);
    }
}

void MainWindow::on_actionClose_Image_triggered()
{
    //Close Image
    scene->clear();
    ui->pushButton->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
    ui->horizontalSlider_2->setDisabled(true);
    ui->horizontalSlider_4->setDisabled(true);
}

void MainWindow::on_actionExit_triggered()
{
    //Exit
    QApplication::quit();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    //Zoom slider
    float scale=(float)value/500;
    ui->graphicsView->resetMatrix();
    ui->graphicsView->scale(scale,scale);
}

void MainWindow::on_pushButton_2_clicked()
{
    //Reset zoom
    RESET_ZOOM;
}

void MainWindow::on_actionAbout_triggered()
{
    //About
    AboutDialog d;
    d.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    //Reset
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_4->setValue(100);
    image=copy;
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //Blur slider
    ui->spinBox->setValue(value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    //Saturate slider
    ui->doubleSpinBox->setValue((float)value/100);
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    //Gamma slider
    ui->doubleSpinBox_2->setValue((float)value/100);
}
