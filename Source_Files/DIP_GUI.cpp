#include "DIP_GUI.h"
#include "opencv2/imgproc/types_c.h"
#include "function.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <qmimedata.h>
#include <QFileInfo>
#include <qmovie.h>
#include <qspinbox.h>

using namespace cv;

DIP_GUI::DIP_GUI(QWidget *parent)
  : QMainWindow(parent),
    fc(100),
    highpass(false)
{   
    this->setAcceptDrops(true);
    ui.setupUi(this);
    ui.fc_i->setValue(fc);
    ui.fc_b->setValue(fc);
    ui.fc_g->setValue(fc);
    // editingFinished信号下也可以正常使用上下按钮
    ui.fc_i->setMouseTracking(false);
    ui.fc_i->setMouseTracking(false);
    ui.fc_i->setMouseTracking(false);

    // 连接信号与槽函数
    connect(ui.histbtn, &QPushButton::clicked, this, &DIP_GUI::hist);
    connect(ui.laplacebtn, &QPushButton::clicked, this, &DIP_GUI::laplace);
    connect(ui.ilpfbtn, &QPushButton::clicked, this, &DIP_GUI::ideal);
    connect(ui.blpfbtn, &QPushButton::clicked, this, &DIP_GUI::butter);
    connect(ui.glpfbtn, &QPushButton::clicked, this, &DIP_GUI::gauss);
    connect(ui.face, &QPushButton::clicked, this, &DIP_GUI::fac);
    //connect(ui.high, &QRadioButton::isChecked, this, [&]() {highpass = 1; });
    //connect(ui.low, &QRadioButton::isChecked, this, [&]() {highpass = 0; });
    connect(ui.fc_i, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_i->value(); });
    connect(ui.fc_b, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_b->value(); });
    connect(ui.fc_g, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_g->value(); });
}

void DIP_GUI::dragEnterEvent(QDragEnterEvent* event)
{
    QStringList FileTypes;
    FileTypes.append("jpg");
    FileTypes.append("png");
    FileTypes.append("bmp");
    if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) 
    {
        // 对象是否可以返回URL列表，并且只有一个
        QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
        // 在FileTypes查找文件后缀是否符合
        if (FileTypes.contains(file.suffix().toLower())) 
            event->acceptProposedAction();
    }
}

void DIP_GUI::dropEvent(QDropEvent* event)
{
    // 接收文件
    QString url = event->mimeData()->urls().first().toLocalFile();
    if (url.isEmpty()) 
        return;

    // 具体将拿到的数据进行处理
    QImage img;
    img.load(url);
    // 设置图片适应QLabel大小
    img.scaled(ui.originImg->size(), Qt::KeepAspectRatio);
    ui.originImg->setScaledContents(true);
    ui.originImg->setPixmap(QPixmap::fromImage(img));
    
    filepath = url.toStdString();
}

void DIP_GUI::setupView()
{
    this->setAcceptDrops(true);    //可以接收图片
}

void DIP_GUI::hist()
{
    originImage = imread(filepath);

    //Loading GIF
    //QMovie* movie = new QMovie(QString::fromStdString("C:\\Users\\19941\\Pictures\\i\\Genshin-Loading.gif"));
    //ui.processImg->setMovie(movie);
    //movie->start();

    int channels = originImage.channels();
    Mat imageRGB[3];
    // 分离scrImage的三个Channels到imageRGB
    split(originImage, imageRGB);

    // 分别进行直方图均衡化
    for (int i = 0; i < channels; i++)
        HistEqual(imageRGB[i], imageRGB[i]);

    // 合并通道
    merge(imageRGB, 3, originImage);
    cvtColor(originImage, processedImage, COLOR_BGR2RGB);

    QImage pImage = QImage((const unsigned char*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}

void DIP_GUI::laplace()
{
    originImage = imread(filepath);
    int channels = originImage.channels();
    Mat imageRGB[3];
    // 分离scrImage的三个Channels到imageRGB
    split(originImage, imageRGB);

    // 分别进行拉普拉斯锐化
    for (int i = 0; i < channels; i++)
        Laplacian(imageRGB[i], imageRGB[i], 0);

    // 合并通道
    merge(imageRGB, 3, originImage);
    cvtColor(originImage, processedImage, COLOR_BGR2RGB);

    QImage pImage = QImage((const unsigned char*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}

void DIP_GUI::ideal()
{
    originImage = imread(filepath);
    int channels = originImage.channels();
    Mat imageRGB[3];
    // 分离scrImage的三个Channels到imageRGB
    split(originImage, imageRGB);

    highpass = true ? ui.high->isChecked() : false;

    // 分别进行理想低/高通滤波
    for (int i = 0; i < channels; i++)
        ILPF(imageRGB[i], imageRGB[i], fc, highpass);

    // 合并通道
    merge(imageRGB, 3, originImage);
    cvtColor(originImage, processedImage, COLOR_BGR2RGB);
    processedImage.convertTo(processedImage, CV_8U, 255);

    QImage pImage = QImage((const unsigned char*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}

void DIP_GUI::butter()
{
    originImage = imread(filepath);
    int channels = originImage.channels();
    Mat imageRGB[3];
    // 分离scrImage的三个Channels到imageRGB
    split(originImage, imageRGB);

    // 分别进行巴特沃斯低通滤波
    for (int i = 0; i < channels; i++)
        BLPF(imageRGB[i], imageRGB[i], fc);

    // 合并通道
    merge(imageRGB, 3, originImage);
    cvtColor(originImage, processedImage, COLOR_BGR2RGB);
    processedImage.convertTo(processedImage, CV_8U, 255);

    QImage pImage = QImage((const unsigned char*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}

void DIP_GUI::gauss()
{
    originImage = imread(filepath);
    int channels = originImage.channels();
    Mat imageRGB[3];
    // 分离scrImage的三个Channels到imageRGB
    split(originImage, imageRGB);

    // 分别进行高斯低通滤波
    for (int i = 0; i < channels; i++)
        GLPF(imageRGB[i], imageRGB[i], fc);

    // 合并通道
    merge(imageRGB, 3, originImage);
    
    cvtColor(originImage, processedImage, COLOR_BGR2RGB);
    processedImage.convertTo(processedImage, CV_8U, 255);

    QImage pImage = QImage((const uchar*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}

void DIP_GUI::fac()
{
    originImage = imread(filepath);
    faceFilter(originImage, fc);

    cvtColor(originImage, processedImage, COLOR_BGR2RGB);
    //processedImage.convertTo(processedImage, CV_8U, 255);

    QImage pImage = QImage((const uchar*)(processedImage.data), processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    pImage.scaled(ui.processImg->size(), Qt::KeepAspectRatio);
    ui.processImg->setScaledContents(true);
    ui.processImg->setPixmap(QPixmap::fromImage(pImage));
}