#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <HDRimage.hpp>
#include <QMainWindow>
#include <vector>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QImage>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
void on_verticalSlider_1_valueChanged(int position);

void on_lineEdit_1_editingFinished();

void on_verticalSlider_2_valueChanged(int position);

void on_lineEdit_2_editingFinished();

void on_pushButton_1_clicked();

void on_pushButton_2_clicked();

void on_pushButton_3_clicked();

void on_action_Open_triggered();

void on_radioButton_2_clicked();

void on_radioButton_3_clicked();

void on_radioButton_1_clicked();

void on_actionReset_triggered();

void on_actionSave_PPM_triggered();

void on_actionSave_PFM_triggered();

void on_horizontalSlider_valueChanged(int value);

void on_radioButton_6_clicked();

void on_radioButton_4_clicked();

void on_verticalSlider_valueChanged(int value);

void on_pushButton_clicked();

void on_radioButton_5_clicked();

private:
  static QImage* hdrToQImage( hdr::image & );
         void    reset( );

  Ui::MainWindow *ui;
  QStandardItemModel *model;
  hdr::image LatLong;
  hdr::image Sphere;
  hdr::image Reflection;
  QImage* central_image;
  QStringListModel* filemodel;
  float *exposure;
};

#endif // MAINWINDOW_HPP
