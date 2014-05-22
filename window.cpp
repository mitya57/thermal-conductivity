#include "window.h"

MainWindow::MainWindow(unsigned sliderSteps):
  toolBar("Toolbar", this),
  slider(Qt::Horizontal, this) {
    resize(drawArea.size());
    slider.setRange(0, sliderSteps);
    toolBar.addWidget(&slider);
    addToolBar(&toolBar);
    setCentralWidget(&drawArea);
    connect(&slider, SIGNAL(valueChanged(int)),
            this,    SLOT(processSliderUpdate(int)));
}

void MainWindow::processSliderUpdate(int newValue) {
    drawArea.stepT = newValue;
    drawArea.repaint();
}
