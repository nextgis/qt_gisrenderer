#include <QApplication>
#include "MainWindow.h"


int main(
      int   argc,
      char* argv[])
{
    QApplication a(argc, argv);

    QScopedPointer <MainWindow> mw1;
    mw1.reset(new MainWindow);
    mw1->setToolTip("OpenGL proba");
    mw1->setWindowTitle(QStringLiteral("Threaded OpenGlOffscreenSurface"));
    mw1->show();

    return (a.exec());
}  // main
