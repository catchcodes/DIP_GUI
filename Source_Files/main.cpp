#include "DIP_GUI.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <string>
#include "opencv2/imgproc/types_c.h"
#include "function.h"
#include <QTextBrowser>
#include <qdesktopwidget.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DIP_GUI w;
    // 居中显示
    w.move((a.desktop()->width() - w.width()) / 2, (a.desktop()->height() - w.height()) / 2 - 60);
    w.setWindowTitle("DIP With GUI");

    w.show();

    return a.exec();  // 循环等待事件
}
