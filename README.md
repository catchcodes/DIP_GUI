# 数字图像处理C++


[![ ](https://img.shields.io/badge/%E6%AC%A7%E9%98%B3%E6%96%8C-catchcodes-brightgreen "my_icon")](https://github.com/catchcodes)
[![LICENSE](https://img.shields.io/badge/LICENSE-MIT-blueviolet)](https://github.com/catchcodes/DIP_GUI/blob/main/LICENSE.txt)
[![ ](https://img.shields.io/badge/QQ-%F0%9F%8C%9E%20-yellowgreen "扣扣")](https://wpa.qq.com/msgrd?v=3&uin=1994143440&site=qq&menu=yes&jumpflag=1)
![Github stars](https://img.shields.io/github/stars/catchcodes/DIP_GUI.svg)

</br>

```
 _______                     __________ 
/  .__. \ ____   ____ ___.__.\______   \
|  |  | |/  _ \ /  _ \  ||  | |    |  _/
|  |__|  ( <_> )  <_> )___  | |    |   \
 \______/\____/ \____// ____| |______  /
                      \/             \/ 
```

### Demo
![Demo](https://github.com/catchcodes/DIP_GUI/blob/master/Resource_Files/demo.gif)

### Requirements

This project is based on [Visual Studio 2022](https://visualstudio.microsoft.com/zh-hans/vs/) IDE</br>
Using [Opencv 4.7.0](https://opencv.org/releases/)&ensp;;&ensp;[Qt 5.14.2](https://download.qt.io/archive/qt/5.14/5.14.2/)&ensp;;&ensp;[Qt VS Tool 2.10.1](https://www.qt.io/blog/qt-vs-tools-for-visual-studio-2022)&ensp;;&ensp;[Python 3.10](https://www.python.org/downloads/)

### Description
![ depict](https://github.com/catchcodes/DIP_GUI/blob/master/Resource_Files/depict.png)
</br>
**功能函数**
* 直方图均衡化 : $ s_k = \displaystyle \sum_{j=0}^{k}\frac{n_j}{n}$ 
 累计频率 哪些灰度数量多，映射曲线越陡峭，均衡后越均匀
</br>

* 拉普拉斯锐化 : $ g(x,y) = f(x,y)+[8\cdot f(x,y)-\displaystyle \sum_{D8}f(i,j)] $
当邻域中心像素灰度低于它所在的领域内其它像素的平均灰度时，此中心像素的灰度应被进一步降低，当邻域中心像素灰度高于它所在的邻域内其它像素的平均灰度时，此中心像素的灰度应被进一步提高
</br>

* 频域滤波 : 
DFT: $ F(u,v) = \frac{1}{MN}\displaystyle \sum_{x=0}^{M-1} \sum_{y=0}^{N-1}f(x,y)\exp\{-j2\pi (\frac{ux}{M}+\frac{vy}{N}) \}$
</br>

**Python部分**

* 人脸特征点检测 : 
1. 调用Dlib的HOG人脸检测(将图像分割成一些 16×16 像素的小方块，计算指向性最强的梯度方向)

![HOG](https://github.com/catchcodes/DIP_GUI/blob/master/Resource_Files/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202023-03-23%20201603.png)

2. 用训练好的81个人脸关键点位置模型去寻找人脸中的81个特征点

![mark](https://github.com/catchcodes/DIP_GUI/blob/master/Resource_Files/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202023-03-19%20165113.png)
 
</br>

**UI部分**
</br>
1、用Qt Designer设计好初步界面
</br>
2、连接Button、SpinBox等信号与相应的槽函数，信号产生调用对应槽函数，在槽函数中进行相关功能函数的调用


### Function Codes
Codes of Digital-Image-Processing homework

* 直方图均衡化
  <details>
  <summary><font size="3" color="yellowgreen">代码展开</font></summary>
  <pre><code class="language-cpp">
    // 直方图均衡化的定义
    void HistEqual(Mat& gray, Mat& result)
    {
        // 哈希表统计0~255像素值的个数
        map<int, int>pixelCounter;
        for (int i = 0; i < gray.rows; i++)
        {
            for (int j = 0; j < gray.cols; j++)
            {
                int value = gray.at<uchar>(i, j);
                pixelCounter[value]++;
            }
        }
        //统计0~255像素值的频率，并计算累计频率
        map<int, double> pixel_fre;
        int pixel_sum = gray.cols * gray.rows;
        double cumul_fre = 0;
        for (int i = 0; i < 256; i++)
        {
            // 累计频率 哪些灰度数量多，映射曲线越陡峭，均衡后越均匀
            cumul_fre += double(pixelCounter[i]) / pixel_sum;
            pixel_fre[i] = cumul_fre;
        }
        //根据累计频率进行转换
        for (int i = 0; i < gray.rows; i++)
        {
            for (int j = 0; j < gray.cols; j++)
            {
                int value = gray.at<uchar>(i, j);
                double fre = pixel_fre[value];
                // 原始灰度值乘以累计频率
                result.at<uchar>(i, j) = fre * value;
            }
        }
    }
  </code></pre>
  </details>

* 拉普拉斯锐化
  <details>
  <summary><font size="3" color="yellow">代码展开</font></summary>
  <pre><code class="language-cpp">
    // 默认0填充
    void Laplacian(Mat& gray, Mat& result, int padding)
    {
        //result.convertTo(result, CV_64F);
        Mat gray_buf(gray.rows + 2, gray.cols + 2, gray.depth());
        // 0填充
        if (padding == 0)
        {
            cv::copyMakeBorder(gray, gray_buf, 1, 1, 1, 1, cv::BORDER_CONSTANT);
        }
        // 镜像填充
        else if (padding == 1)
        {
            cv::copyMakeBorder(gray, gray_buf, 1, 1, 1, 1, cv::BORDER_REFLECT);
        }
        for (int i = 0; i < gray.rows; i++)
        {
            for (int j = 0; j < gray.cols; j++)
            {
                // cv::saturate_cast<uchar>()保证范围为0~255
                // 直接访问
                result.at<uchar>(i, j) = cv::saturate_cast<uchar>(gray.at<uchar>(i, j) + 8 * gray_buf.at<uchar>(i + 1, j + 1) - gray_buf.at<uchar>(i, j) - gray_buf.at<uchar>(i, j + 1) - gray_buf.at<uchar>(i, j + 2) \
                    - gray_buf.at<uchar>(i + 1, j) - gray_buf.at<uchar>(i + 1, j + 2) - gray_buf.at<uchar>(i + 2, j) - gray_buf.at<uchar>(i + 2, j + 1) - gray_buf.at<uchar>(i + 2, j + 2));
            }
        }
    }
  </code></pre>
  </details>

* 理想低通滤波器
  <details>
  <summary><font size="3" color=#98FB98>代码展开</font></summary>
  <pre><code class="language-cpp">
	// 理想低通滤波器
	void ILPF(Mat& gray, Mat& result, int fc)
	{
		// 扩展图像矩阵，为2，3，5的倍数时运算速度快
		int m = cv:: getOptimalDFTSize(gray.rows);
		int n = cv::getOptimalDFTSize(gray.cols);
		Mat padded;
		// 零填充
		cv::copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, cv::BORDER_CONSTANT);
		padded.convertTo(padded, CV_32FC1);
		int row = padded.rows;
		int col = padded.cols;
		if (fc > MIN(row, col))	
			throw "截止频率超出图像范围";
		Mat filter = Mat::zeros(padded.size(), CV_32FC1);
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				double d = sqrt(pow((i - row / 2.0), 2) + pow((j - col / 2.0), 2));
				if (d <= fc)
				{
					filter.at<float>(i, j) = 1;
				}
			}
		}
		// imshow("滤波器", filter);
		// 实部和虚部
		Mat plane[] = {padded, Mat::zeros(padded.size(), CV_32FC1)};
		Mat complexIm;
		merge(plane, 2, complexIm); //合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
		dft(complexIm, complexIm);  //进行傅立叶变换，结果保存在自身
		split(complexIm, plane);    //分离通道
		fftshift(plane[0], plane[1]);
		Mat Real, Imag, BLUR;
		Real = plane[0].mul(filter);
		Imag = plane[1].mul(filter);
		// fftshift(Real, Imag); //效果一样 周期性
		Mat plane1[] = { Real, Imag };
		merge(plane1, 2, BLUR);//实部与虚部合并
		idft(BLUR, BLUR);
		split(BLUR, plane);                     //分离通道，主要获取通道
		magnitude(plane[0], plane[1], result);  //求幅值(模)
		normalize(result, result, 0, 1.0, NORM_MINMAX);  //归一化便于显示
	}
	// 低频移动到中心
	void fftshift(Mat plane0, Mat plane1)
	{
		// -2 : 1111_……_1110
		plane0 = plane0(Rect(0, 0, plane0.cols & -2, plane0.rows & -2));
		int cx = plane0.cols / 2;
		int cy = plane0.rows / 2;
		Mat part1_r(plane0, Rect(0, 0, cx, cy));
		Mat part2_r(plane0, Rect(cx, 0, cx, cy));
		Mat part3_r(plane0, Rect(0, cy, cx, cy));
		Mat part4_r(plane0, Rect(cx, cy, cx, cy));
		Mat temp;
		part1_r.copyTo(temp);  //左上与右下交换位置(实部)
		part4_r.copyTo(part1_r);
		temp.copyTo(part4_r);
		part2_r.copyTo(temp);  //右上与左下交换位置(实部)
		part3_r.copyTo(part2_r);
		temp.copyTo(part3_r);
		Mat part1_i(plane1, Rect(0, 0, cx, cy));  //元素坐标(cx,cy)
		Mat part2_i(plane1, Rect(cx, 0, cx, cy));
		Mat part3_i(plane1, Rect(0, cy, cx, cy));
		Mat part4_i(plane1, Rect(cx, cy, cx, cy));
		part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
		part4_i.copyTo(part1_i);
		temp.copyTo(part4_i);
		part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
		part3_i.copyTo(part2_i);
		temp.copyTo(part3_i);
	}
  </code></pre>
  </details>

* 巴特沃斯低通滤波器
  <details>
  <summary><font size="3" color=#FAFAD2>代码展开</font></summary>
  <pre><code class="language-cpp">
	// 巴特沃斯的滤波器
  	Mat filter = Mat::zeros(padded.size(), CV_32FC1);
	for (int i = 0; i < row; i++)
	{
		//float* data = filter.ptr<float>(i);
		for (int j = 0; j < col; j++)
		{
			float d = sqrt(pow((i - row / 2.0), 2) + pow((j - col / 2.0), 2));
			filter.at<float>(i, j) = 1 / (1 + pow(float(d / fc), 2 * level));
		}
	}
  </code></pre>
  </details>

* 高斯低通滤波器
  <details>
  <summary><font size="3" color=#FFDEAD>代码展开</font></summary>
  <pre><code class="language-cpp">
    // 高斯滤波器
  	Mat filter = Mat::zeros(padded.size(), CV_32FC1);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			float d = sqrt(pow((i - row / 2.0), 2) + pow((j - col / 2.0), 2));
			filter.at<float>(i, j) = exp(-pow(d, 2) / (2 * pow(fc, 2)));
		}
	}
  </code></pre>
  </details>

* 人脸检测（Python）
  <details>
  <summary><font size="3" color=#F08080>代码展开</font></summary>
  <pre><code class="language-python">
  # HOG人脸检测器寻找人脸
  def find_face(filename):
    face_detector = dlib.get_frontal_face_detector()  # 创建HOG人脸检测器
    image = cv2.imread(filename)
    detected_faces = face_detector(image, 1)
	# 存储人脸矩形框的坐标信息
    location = []
    for i, face_rect in enumerate(detected_faces):
        location.append(face_rect.left())
        location.append(face_rect.top())
        location.append(face_rect.right())
        location.append(face_rect.bottom())
    return location
  </code></pre>
  </details>

* 人脸81个特征点位置（Python）
  <details>
  <summary><font size="3" color=#FF8C00>代码展开</font></summary>
  <pre><code class="language-python">
  def find_face_landmarks(filename):
    # 人脸81个关键点模型位置 
    predictor_model = r"E:\Face_Landmarks\shape_predictor_81_face_landmarks.dat"
    face_detector = dlib.get_frontal_face_detector()  # 创建HOG人脸检测器
    face_pose_predictor = dlib.shape_predictor(predictor_model)  # 创建人脸特征点检测器
    image = cv2.imread(filename)
    detected_faces = face_detector(image, 1)
    mark = []
    for i, face_rect in enumerate(detected_faces):
        pose_landmarks = face_pose_predictor(image, face_rect)  # 获取面部的姿势
        for j in range(81):
		    # 以tuple形式存放到mark列表中，方便C++调用
            mark.append((pose_landmarks.part(j).x, pose_landmarks.part(j).y))            
    return mark
  </code></pre>
  </details>

* UI部分
  <details>
  <summary><font size="3" color=#00FA9A>代码展开</font></summary>
  <pre><code class="language-cpp">
    // 接收并响应拖拽事件
    // 过滤非图片文件
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
    // 响应拖拽事件
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
        // 保存图片路径
        filepath = url.toStdString();
    }
    void DIP_GUI::setupView()
    {
        this->setAcceptDrops(true);    //可以接收图片
    }
    // 连接信号与槽函数
    connect(ui.histbtn, &QPushButton::clicked, this, &DIP_GUI::hist);
    connect(ui.laplacebtn, &QPushButton::clicked, this, &DIP_GUI::laplace);
    connect(ui.ilpfbtn, &QPushButton::clicked, this, &DIP_GUI::ideal);
    connect(ui.blpfbtn, &QPushButton::clicked, this, &DIP_GUI::butter);
    connect(ui.glpfbtn, &QPushButton::clicked, this, &DIP_GUI::gauss);
    connect(ui.face, &QPushButton::clicked, this, &DIP_GUI::fac);
    // Lamda表达式
    connect(ui.fc_i, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_i->value(); });
    connect(ui.fc_b, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_b->value(); });
    connect(ui.fc_g, &QSpinBox::editingFinished, this, [&]() {fc = ui.fc_g->value(); });

  </code></pre>
  </details>

### ToDoList
- [ ] 人脸部分具体处理 
- [ ] 人脸眼部、鼻部、唇部、发际线进行可选性处理
- [ ] 对视频中的人脸进行滤波
    
### Bug Buddha

```
                            _ooOoo_
                           o8888888o
                           88" . "88
                           (| -_- |)
                            O\ = /O
                        ____/`---'\____
                      .   ' \\| |// `.
                       / \\||| : |||// \
                     / _||||| -:- |||||- \
                       | | \\\ - /// | |
                     | \_| ''\---/'' | |
                      \ .-\__ `-` ___/-. /
                   ___`. .' /--.--\ `. . __
                ."" '< `.___\_<|>_/___.' >'"".
               | | : `- \`.;`\ _ /`;.`/ - ` : | |
                 \ \ `-. \_ __\ /__ _/ .-` / /
         ======`-.____`-.___\_____/___.-`____.-'======
                            `=---='

         .............................................
                  佛祖镇楼                  BUG辟易
          佛曰:
                  V我50
                  v我50
                  V我50
```

