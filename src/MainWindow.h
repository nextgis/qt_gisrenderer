#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include "MapWidget.h"


class MainWindow
    : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow();

  private:
    MapWidget* mMapWidget;
};


#endif  // MAINWIDGET_H
