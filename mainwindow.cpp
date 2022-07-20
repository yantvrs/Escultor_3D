#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sculptor.h>
#include <cstdlib>
#include <dialog.h>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Standard loading
    dx = 10, dy = 10, dz = 10;
    r = 0, g = 0, b = 0;
    currentObject = VOXEL;
    s = new Sculptor(dx,dy,dz);

    //Placing the sliders so they don't go beyond the sculptor's dimension
    // Dimensions
    ui->horizontalSliderDX->setMaximum(dx-1);
    ui->horizontalSliderDY->setMaximum(dy-1);
    ui->horizontalSliderDZ->setMaximum(dz-1);


    // Box
    ui->horizontalSliderBoxX->setMaximum(dx-1);
    ui->horizontalSliderBoxY->setMaximum(dy-1);
    ui->horizontalSliderBoxZ->setMaximum(dz-1);

    //Sphere (smallest of three axis)
    if(dx < dy && dx <dz){
        ui->horizontalSliderRaio->setMaximum((dx-1)/2);
    } else if(dy < dx && dy < dz){
        ui->horizontalSliderRaio->setMaximum((dy-1)/2);
    } else {
        ui->horizontalSliderRaio->setMaximum((dz-1)/2);
    }

    // Ellipsoid
    ui->horizontalSliderEllipseRx->setMaximum((dx-1)/2);
    ui->horizontalSliderEllipseRy->setMaximum((dy-1)/2);
    ui->horizontalSliderEllipseRz->setMaximum((dz-1)/2);

    // Z axix
    ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));


    actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionAddVoxel);
    actionGroup->addAction(ui->actionRemove_Voxel);
    actionGroup->addAction(ui->actionAdd_Box);
    actionGroup->addAction(ui->actionRemove_Box);
    actionGroup->addAction(ui->actionAdd_Sphere);
    actionGroup->addAction(ui->actionRemove_Sphere);
    actionGroup->addAction(ui->actionAdd_Ellipsoid);
    actionGroup->addAction(ui->actionRemove_Ellipsoid);
    ui->actionAddVoxel->setChecked(true);

    // SLOTS CONNECTION
    // connnect - connects an object, a pointer to the object that receives the signal and the name of the function that will handle the signal

    // Geometric forms slots
    connect(ui->actionAddVoxel,SIGNAL(triggered()),this,SLOT(addVoxel()));
    connect(ui->actionRemove_Voxel,SIGNAL(triggered()),this,SLOT(remove_voxel()));
    connect(ui->actionAdd_Box,SIGNAL(triggered()),this,SLOT(add_box()));
    connect(ui->actionRemove_Box,SIGNAL(triggered()),this,SLOT(remove_box()));
    connect(ui->actionAdd_Sphere,SIGNAL(triggered()),this,SLOT(add_sphere()));
    connect(ui->actionRemove_Sphere,SIGNAL(triggered()),this,SLOT(remove_sphere()));
    connect(ui->actionAdd_Ellipsoid,SIGNAL(triggered()),this,SLOT(add_ellipsoid()));
    connect(ui->actionRemove_Ellipsoid,SIGNAL(triggered()),this,SLOT(remove_ellipsoid()));

    // Buttons slots
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->horizontalSliderDZ,SIGNAL(valueChanged(int)),this,SLOT(slider_axix_z(int)));
    connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(quit()));
    connect(ui->actionNew_Sculptor,SIGNAL(triggered(bool)),this,SLOT(new_Sculptor()));
    connect(ui->actionNew_Sculptor_2,SIGNAL(triggered(bool)),this,SLOT(new_Sculptor()));

    // Color slots
    connect(ui->horizontalSliderRed,SIGNAL(valueChanged(int)),this,SLOT(setRed(int)));
    connect(ui->horizontalSliderBlue,SIGNAL(valueChanged(int)),this,SLOT(setGreen(int)));
    connect(ui->horizontalSliderGreen,SIGNAL(valueChanged(int)),this,SLOT(setBlue(int)));

    // Widget slots
    connect(ui->widgetDraw,SIGNAL(updatePosition(int,int)),this,SLOT(drawShape(int,int)));
    connect(ui->widgetDraw,SIGNAL(updatePosition(int,int)),this,SLOT(updateCoords(int,int)));

}

MainWindow::~MainWindow()
{
    delete ui; // destructor
}

void MainWindow::updateColor()
{
    QString color;
    color = "QPushButton { background-color: rgb(" +
            QString().setNum(r) + "," +
            QString().setNum(b) + "," +
            QString().setNum(g) + ");}";
    ui->pushButtonShowColor->setStyleSheet(color);
    s->setColor(r,b,g);
}

void MainWindow::quit()
{
    exit(0); // Mata tudo :X
}

void MainWindow::setRed(int r_)
{
    r = r_;
    updateColor();
}

void MainWindow::setGreen(int g_)
{
    g = g_;
    updateColor();
}

void MainWindow::setBlue(int b_)
{
    b = b_;
    updateColor();
}

void MainWindow::drawShape(int hClick, int vClick)
{
    switch(currentObject){
        case(VOXEL):
            s->putVoxel(vClick,hClick,ui->horizontalSliderDZ->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(NOVOXEL):
            s->cutVoxel(vClick,hClick,ui->horizontalSliderDZ->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(BOX):
            s->putBox(vClick-ui->horizontalSliderBoxY->value()/2,
                         vClick+ui->horizontalSliderBoxY->value()/2,
                         hClick-ui->horizontalSliderBoxX->value()/2,
                         hClick+ui->horizontalSliderBoxX->value()/2,
                         ui->horizontalSliderDZ->value()-ui->horizontalSliderBoxZ->value()/2,
                         ui->horizontalSliderDZ->value()+ui->horizontalSliderBoxZ->value()/2);
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(NOBOX):
            s->cutBox(vClick-ui->horizontalSliderBoxY->value()/2,
                         vClick+ui->horizontalSliderBoxY->value()/2,
                         hClick-ui->horizontalSliderBoxX->value()/2,
                         hClick+ui->horizontalSliderBoxX->value()/2,
                         ui->horizontalSliderDZ->value()-ui->horizontalSliderBoxZ->value()/2,
                         ui->horizontalSliderDZ->value()+ui->horizontalSliderBoxZ->value()/2);
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(SPHERE):
            s->putSphere(vClick,hClick,ui->horizontalSliderDZ->value(),ui->horizontalSliderRaio->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(NOSPHERE):
            s->cutSphere(vClick,hClick,ui->horizontalSliderDZ->value(),ui->horizontalSliderRaio->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(ELLIPSOID):
            s->putEllipsoid(vClick,hClick,ui->horizontalSliderDZ->value(),
                          ui->horizontalSliderEllipseRx->value(),
                          ui->horizontalSliderEllipseRy->value(),
                          ui->horizontalSliderEllipseRz->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
        case(NOELLIPSOID):
            s->cutEllipsoid(vClick,hClick,ui->horizontalSliderDZ->value(),
                          ui->horizontalSliderEllipseRx->value(),
                          ui->horizontalSliderEllipseRy->value(),
                          ui->horizontalSliderEllipseRz->value());
            ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
            break;
    }
}

void MainWindow::updateCoords(int vClick, int hClick)
{
    ui->horizontalSliderDX->setValue(vClick);
    ui->horizontalSliderDY->setValue(hClick);
}

void MainWindow::new_Sculptor()
{
    Dialog *D;
    D = new Dialog();
    D->exec();
    if(D->result() == QDialog::Accepted){
        dx = D->dXDialog().toInt();
        dy = D->dYDialog().toInt();
        dz = D->dZDialog().toInt();
        if(s != NULL)
           delete s;
        s = new Sculptor(dx,dy,dz);

        ui->horizontalSliderDX->setMaximum(dx-1);
        ui->horizontalSliderDY->setMaximum(dy-1);
        ui->horizontalSliderDZ->setMaximum(dz-1);

        // Placing the sliders so they don't go beyond the sculptor's dimension
        // Box
        ui->horizontalSliderBoxX->setMaximum(dx-1);
        ui->horizontalSliderBoxY->setMaximum(dy-1);
        ui->horizontalSliderBoxZ->setMaximum(dz-1);

        //Sphere (smallest of three axis)
        if(dx < dy && dx <dz){
            ui->horizontalSliderRaio->setMaximum((dx-1)/2);
        } else if(dy < dx && dy < dz){
            ui->horizontalSliderRaio->setMaximum((dy-1)/2);
        } else {
            ui->horizontalSliderRaio->setMaximum((dz-1)/2);
        }

        // Ellipsoid
        ui->horizontalSliderEllipseRx->setMaximum((dx-1)/2);
        ui->horizontalSliderEllipseRy->setMaximum((dy-1)/2);
        ui->horizontalSliderEllipseRz->setMaximum((dz-1)/2);
    }

    ui->widgetDraw->loadMatriz(s->getPlano(ui->horizontalSliderDZ->value(),XY));
    ui->horizontalSliderRed->setValue(0);
    ui->horizontalSliderGreen->setValue(0);
    ui->horizontalSliderBlue->setValue(0);
}

void MainWindow::save()
{
    QFileDialog D;
    QString selectedFilter;
    QString filename = D.getSaveFileName(this,tr("Save"),
                                              QDir::currentPath(),
                                              tr("Arquivo OFF(*.off)"),
                                              &selectedFilter);
    if(filename.isNull()){
        return;
    }
    s->writeOFF(filename.toStdString());
}

void MainWindow::addVoxel()
{
    currentObject = VOXEL;
}

void MainWindow::remove_voxel()
{
    currentObject = NOVOXEL;
}

void MainWindow::add_box()
{
    currentObject = BOX;
}

void MainWindow::remove_box()
{
    currentObject = NOBOX;
}

void MainWindow::add_sphere()
{
    currentObject = SPHERE;
}

void MainWindow::remove_sphere()
{
    currentObject = NOSPHERE;
}

void MainWindow::add_ellipsoid()
{
    currentObject = ELLIPSOID;
}

void MainWindow::remove_ellipsoid()
{
    currentObject = NOELLIPSOID;
}

void MainWindow::slider_axis_z(int value)
{
    ui->widgetDraw->loadMatriz(s->getPlano(value,XY));
}

