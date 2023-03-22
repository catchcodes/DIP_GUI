#include "function.h"
#include <vector>
#include <QTextBrowser>

using namespace std;

void showVector(vector<int>& x, bool console)
{
    // 创建一个字符串流
    std::stringstream ss;

    // 将向量中的元素添加到字符串流中
    for (int i = 0; i < x.size(); ++i) 
        ss << x[i] << "\n";

    // 获取字符串流中的字符串
    std::string str = ss.str();

    // 将字符串设置为Text Browser的文本
    QTextBrowser textBrowser;
    textBrowser.setText(QString::fromStdString(str));
    textBrowser.show();

    // 子系统为控制台时直接输出到控制台
    if (console)
    {
        for (vector<int>::iterator iter = x.begin(); iter != x.end(); iter++)
        {
            cout << *iter << endl;
        } 
    }
    
}