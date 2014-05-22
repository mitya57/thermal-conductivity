#include <QToolBar>
#include <QSlider>
#include <QMainWindow>
#include "drawarea.h"

class MainWindow: public QMainWindow {
Q_OBJECT

public:
    MainWindow(unsigned sliderSteps);
    DrawArea drawArea;

public slots:
    void processSliderUpdate(int newValue);

private:
    QToolBar toolBar;
    QSlider slider;
};
