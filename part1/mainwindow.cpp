#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QLabel>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

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
    if ( res.isEmpty() == true ) {
        return;
    }
    hdr::image temp = res;
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
    hdr::image temp = res;
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
    if ( res.isEmpty() == true ) {
        return;
    }
    hdr::image temp = res;
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
    hdr::image temp = res;
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
    hdr::image temp = res;
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
    hdr::image temp = res;
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
    image_buffer.clear( );
    res.free( );
    delete central_image;
    central_image = nullptr;
    ui->imageLabel->setPixmap(nullptr);
    ui->lineEdit_1->setText( "0" );
    ui->verticalSlider_1->setValue( 0 );
    ui->lineEdit_2->setText( "1" );
    ui->verticalSlider_2->setValue( 100000 );

    QStringList fileName = QFileDialog::getOpenFileNames(this,tr("Open File"),QDir::currentPath(),"*.pfm");
    fileName.sort();
    for ( QStringList::Iterator it = fileName.begin( ); it != fileName.end( ); ++it ) {
        if (it->isEmpty()) {
            fileName.erase( it, it + 1 );
            continue;
        }
        image_buffer.push_back( hdr::image( ) );
        int pos = image_buffer.size() - 1;
        image_buffer[ pos ].loadPFM( it->toStdString() );
        if (image_buffer[ pos ].isEmpty( ) == true) {
            fileName.erase( it, it + 1 );
            image_buffer.pop_back();
        }
        QFileInfo cur_file( *it );
        *it = cur_file.baseName();
    }
    delete filemodel;
    filemodel = new QStringListModel(fileName);
    if ( filemodel == nullptr ) {
        return;
    }
    ui->listView->setModel(filemodel);
    delete [] exposure;
    exposure = new float[ image_buffer.size( ) ];
    if ( exposure == nullptr ) {
        return;
    }
    delete model;
    model = new QStandardItemModel( 1, image_buffer.size( ), this );
    if ( model == nullptr ) {
        delete [] exposure;
        exposure = nullptr;
        return;
    }
    for ( uint32_t i = 0; i < image_buffer.size( ); ++i ) {
        exposure[ i ] = static_cast< float >( 1 << ( 2 * i ) );
        model->removeColumn( i );
        model->setHorizontalHeaderItem( 0, new QStandardItem( QString::number( i ) ) );
        model->setItem( 0, i, new QStandardItem(  QString::number( exposure[ i ] ) ) );
    }
    ui->tableView->setModel(model);
}

void MainWindow::on_pushButton_clicked()
{
    if ( image_buffer.size( ) == 0 ) {
    	return;
    }
    res.createHDR< hdr::pol_cwf >( exposure, image_buffer.data( ), image_buffer.size( ) );
    if ( res.isEmpty() == true ) {
        return;
    }
    hdr::image temp = res;
    if ( ui->radioButton_2->isChecked( ) ) {
        temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    if ( ui->radioButton_1->isChecked( ) ) {
        temp.histEqToneMap( SIZE_BUFF_HIST_TONE_MAP );
        temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    }
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->label_4->setText( QString::number( res.dynamicRange( ) ) );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_2_clicked()
{
    hdr::image temp = res;
    temp.linearToneMap( static_cast< float >( ui->verticalSlider_1->value( ) ) / 50000 );
    temp.gamma( static_cast< float >( ui->verticalSlider_2->value( ) ) / 100000 );
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_3_clicked()
{
    hdr::image temp = res;
    delete central_image;
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
}

void MainWindow::on_radioButton_1_clicked()
{
    hdr::image temp = res;
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
    res.savePFM( saveFile.toStdString( ) );
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    delete central_image;
    res = image_buffer[ index.row() ];
    if ( res.isEmpty() == true ) {
        return;
    }
    hdr::image temp = image_buffer[ index.row() ];
    central_image = hdrToQImage( temp );
    ui->imageLabel->setPixmap(QPixmap::fromImage(*central_image));
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
    image_buffer.clear( );
    res.free( );
    delete central_image;
    central_image = nullptr;
    ui->imageLabel->setPixmap(nullptr);
    delete filemodel;
    filemodel = nullptr;
    ui->lineEdit_1->setText( "0" );
    ui->verticalSlider_1->setValue( 0 );
    ui->lineEdit_2->setText( "1" );
    ui->verticalSlider_2->setValue( 100000 );
    ui->label_4->setText( "NA" );
}

void MainWindow::on_pushButton_4_clicked()
{
  if ( ( model == nullptr ) || ( exposure == nullptr ) ) {
      return;
  }
  for ( int i = 0; i < model->columnCount(); ++i ) {
        bool ok;
        float new_exp = model->item( 0, i )->text().toFloat( &ok );
        if ( ok ) {
            exposure[ i ] = new_exp;
        }
        else {
            model->item( 0, i )->setText( "0" );
            exposure[ i ] = 0;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    delete [] exposure;
    exposure = new float[ image_buffer.size( ) ];
    if ( exposure == nullptr ) {
        return;
    }
    for ( uint32_t i = 0; i < image_buffer.size( ); ++i ) {
        model->removeColumn( i );
    }
    delete model;
    model = new QStandardItemModel( 1, image_buffer.size( ), this );
    if ( model == nullptr ) {
        delete [] exposure;
        exposure = nullptr;
        return;
    }
    for ( uint32_t i = 0; i < image_buffer.size( ); ++i ) {
        exposure[ i ] = static_cast< float >( 1 << ( 2 * i ) );
        model->removeColumn( i );
        model->setHorizontalHeaderItem( 0, new QStandardItem( QString::number( i ) ) );
        model->setItem( 0, i, new QStandardItem(  QString::number( exposure[ i ] ) ) );
    }
    ui->tableView->setModel(model);
}
