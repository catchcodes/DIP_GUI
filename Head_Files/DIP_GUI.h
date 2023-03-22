#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DIP_GUI.h"
#include <opencv2/core/mat.hpp>
#include <string.h>
#include <vector>

using cv::Mat;
using std::string;

class DIP_GUI : public QMainWindow
{
    Q_OBJECT

public:
    DIP_GUI(QWidget *parent = nullptr);
    Mat originImage;
    Mat processedImage;
    int fci;
    int fcb;
    int fcg;
    int fcf;
    bool highpass;
    string filepath;
    string oldFilePath;
    std::vector<int> location_x;
    std::vector<int> location_y;

private:
    Ui::DIP_GUIClass ui;

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private:
    void setupView();

private slots:
    void hist();
    void laplace();
    void ideal();
    void butter();
    void gauss();
    void fac();
};
