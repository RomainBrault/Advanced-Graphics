#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QLabel>
#include <Sphere.hpp>
#include "mainwindow_pt2.hpp"
#include "mainwindow_pt2.hpp"
#include "ui_mainwindow_pt2.h"

#define SIZE_BUFF_HIST_TONE_MAP 500000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filemodel = nullptr;
    central_image = nullptr;
    model = nullptr;
    exposure = nullptr;

    ui->lineEdit_1->setText( QString::number( 0 ) );
    ui->lineEdit_2->setText( QString::number( 1 ) );

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
}

MainWindow::~MainWindow()
{
    delete filemodel;
    delete [] exposure;
    delete central_image;
    delete model;
    delete ui;
}

void MainWindow::on_verticalSlider_1_valueChanged(int position)
{
    float stops = static_cast< float >( position ) / 50000;
    ui->lineEdit_1->setText( QString::number( stops ) );
    if ( LatLong.isEmpty() == true ) {
        return;
    }
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( stops );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( stops );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_lineEdit_1_editingFinished()
{
    bool ok;
    float stops = ui->lineEdit_1->text( ).toFloat( &ok );
    if ( ok == false ) {
        float prev_stops = static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000;
        ui->lineEdit_1->setText( QString::number( prev_stops ) );
        return;
    }
    ui->verticalSlider_1->setValue( static_cast< int >( stops * 50000 ) );
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( stops );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( stops );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_verticalSlider_2_valueChanged(int position)
{
    float gamma = static_cast< float >( position ) / 100000;
    ui->lineEdit_2->setText( QString::number( gamma ) );
    if ( LatLong.isEmpty() == true ) {
        return;
    }
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( gamma );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 100000 );
        temp.gamma( gamma );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_lineEdit_2_editingFinished()
{
    bool ok;
    float gamma = ui->lineEdit_2->text( ).toFloat( &ok );
    if ( ok == false ) {
        float prev_gamma = static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000;
        ui->lineEdit_2->setText( QString::number( prev_gamma ) );
        return;
    }
    ui->verticalSlider_2->setValue( static_cast< int >( gamma * 100000 ) );
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( gamma );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( gamma );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_pushButton_1_clicked()
{
    ui->lineEdit_1->setText( "0" );
    ui->verticalSlider_1->setValue( 0 );
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit_2->setText( "1" );
    ui->verticalSlider_2->setValue( 100000 );
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}


QImage* MainWindow::hdrToQImage( hdr::image & im )
{
   QImage* ret = new QImage( im.getWidth(), im.getHeight(), QImage::Format_RGB888 );
   if ( ret == nullptr ) {
      return nullptr;
   }
   im.normalise( 255 );
   for ( unsigned int i = 0; i < im.getHeight( ); ++i ) {
       for ( unsigned int j = 0; j < im.getWidth( ); ++j ) {
          float r, g, b;
          im.getPixel( j, i, r, g, b );
          QRgb pix = qRgb(
                static_cast< int >( r ),
                static_cast< int >( g ),
                static_cast< int >( b )
          );
          ret->setPixel( j, i, pix);
       }
   }
   return ret;

}

void MainWindow::on_action_Open_triggered()
{
    LatLong.free( );
    delete central_image;
    central_image = nullptr;
    ui->imageLabel->setPixmap(nullptr);
    ui->lineEdit_1->setText( "0" );
    ui->verticalSlider_1->setValue( 0 );
    ui->lineEdit_2->setText( "1" );
    ui->verticalSlider_2->setValue( 100000 );

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),"*.pfm");
    LatLong.loadPFM( fileName.toStdString() );
    hdr::image temp = LatLong;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_2_clicked()
{
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
    temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_3_clicked()
{
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_1_clicked()
{
    hdr::image temp;
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_6->isChecked( ) ) {
        temp = Sphere;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        temp = Reflection;
    }
    temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
    temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
    temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_actionReset_triggered()
{
    reset( );
}

void MainWindow::on_actionSave_PPM_triggered()
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save File"),QDir::currentPath());
    central_image->save( saveFile, "PPM", 100 );
}

void MainWindow::on_actionSave_PFM_triggered()
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save File"),QDir::currentPath());
    LatLong.savePFM( saveFile.toStdString( ) );
}

void MainWindow::on_pushButton_3_clicked()
{
    reset( );
}

void MainWindow::reset( )
{
    delete [] exposure;
    exposure = nullptr;
    delete model;
    model = nullptr;
    LatLong.free( );
    delete central_image;
    central_image = nullptr;
    ui->imageLabel->setPixmap(nullptr);
    delete filemodel;
    filemodel = nullptr;
    ui->lineEdit_1->setText( "0" );
    ui->verticalSlider_1->setValue( 0 );
    ui->lineEdit_2->setText( "1" );
    ui->verticalSlider_2->setValue( 100000 );
}

void MainWindow::on_radioButton_6_clicked()
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    bool ok;
    uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
    if ( ok == false ) {
      img_size = 511;
      ui->lineEdit->setText("511");
    }
    obj::vect< float, 3 > view( 0, 0, 1 );
    obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

    hdr::image temp( img_size, img_size, 1 );
    temp.fill( 0 );
    temp.mapLatLong( s, view, LatLong,
        static_cast< float >( ui->horizontalSlider->value( ) ) / ( 100000 ) * LatLong.getWidth( ),
        static_cast< float >( ui->verticalSlider->value( ) ) / ( 100000 ) * LatLong.getWidth( )
    );

    Sphere = temp;

    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_5_clicked()
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    bool ok;
    uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
    if ( ok == false ) {
      img_size = 511;
      ui->lineEdit->setText("511");
    }
    obj::vect< float, 3 > view( 0, 0, 1 );
    obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

    hdr::image temp( img_size, img_size, 1 );
    temp.fill( 0 );
    temp.reflectanceSphere( s, view );

    Reflection = temp;

    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_4_clicked()
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    hdr::image temp = LatLong;
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    hdr::image temp;
    if ( ui->radioButton_6->isChecked( ) ) {
        bool ok;
        uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
        if ( ok == false ) {
          img_size = 511;
          ui->lineEdit->setText("511");
        }
        obj::vect< float, 3 > view( 0, 0, 1 );
        obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

        temp.create( img_size, img_size, 1 );
        temp.fill( 0 );
        temp.mapLatLong( s, view, LatLong,
            static_cast< float >( value ) / ( 100000 ) * LatLong.getWidth( ),
            static_cast< float >( ui->verticalSlider->value( ) ) / ( 100000 ) * LatLong.getHeight( )
        );
    }
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        return;
    }

    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    hdr::image temp;
    if ( ui->radioButton_6->isChecked( ) ) {
        bool ok;
        uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
        if ( ok == false ) {
          img_size = 511;
          ui->lineEdit->setText("511");
        }
        obj::vect< float, 3 > view( 0, 0, 1 );
        obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

        temp.create( img_size, img_size, 1 );
        temp.fill( 0 );
        temp.mapLatLong( s, view, LatLong,
            static_cast< float >( ui->horizontalSlider->value( ) ) / ( 100000 ) * LatLong.getWidth( ),
            static_cast< float >( value ) / ( 100000 ) * LatLong.getHeight( )
        );
    }
    if ( ui->radioButton_4->isChecked( ) ) {
        temp = LatLong;
    }
    if ( ui->radioButton_5->isChecked( ) ) {
        return;
    }

    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_pushButton_clicked()
{
    if ( LatLong.isEmpty( ) == true ) {
        return;
    }
    hdr::image temp;
    if ( ui->radioButton_6->isChecked( ) ) {
        bool ok;
        uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
        if ( ok == false ) {
          img_size = 511;
          ui->lineEdit->setText("511");
        }
        obj::vect< float, 3 > view( 0, 0, 1 );
        obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

        temp.create( img_size, img_size, 1 );
        temp.fill( 0 );
        temp.mapLatLong( s, view, LatLong,
          static_cast< float >( ui->horizontalSlider->value( ) ) / ( 100000 ) * LatLong.getWidth( ),
          static_cast< float >( ui->verticalSlider->value( ) ) / ( 100000 ) * LatLong.getHeight( )
        );
        Sphere = temp;
    }
    if ( ui->radioButton_5->isChecked() ) {
        bool ok;
        uint32_t img_size = ui->lineEdit->text( ).toUInt( &ok );
        if ( ok == false ) {
          img_size = 511;
          ui->lineEdit->setText("511");
        }
        obj::vect< float, 3 > view( 0, 0, 1 );
        obj::sphere s( img_size / 2, img_size / 2, img_size / 2 );

        temp.create( img_size, img_size, 1 );
        temp.fill( 0 );
        temp.reflectanceSphere( s, view );

        Reflection = temp;
    }
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}
