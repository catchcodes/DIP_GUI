#include "function.h"
#include <Python.h>

using namespace std;

void runPython(vector<int>& location_x, vector<int>& location_y)
{
    // 设置python.exe的路径
    Py_SetPythonHome(L"C:\\Users\\19941\\AppData\\Local\\Programs\\Python\\Python310");
    Py_Initialize(); //调用Py_Initialize()进行初始化
    if (!Py_IsInitialized())
    {
        cout << "初始化失败!" << endl;
    }
    else
    {
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('.\\Py_Files')"); //指定face.py所在的目录

        PyObject* pModule = nullptr;  // 声明模块face.py
        // PyObject* pFunc = nullptr; // 声明函数find_face
        PyObject* pFuncl = nullptr;   // 声明函数find_face_landmark

        pModule = PyImport_ImportModule("face"); //这里是要调用的文件名face.py
        if (pModule == NULL)
        {
            cout << "没找到该Python文件" << endl;
        }
        else
        {
            // pFunc = PyObject_GetAttrString(pModule, "find_face");//这里是要调用的函数名
            pFuncl = PyObject_GetAttrString(pModule, "find_face_landmarks");

            // 不能用C++的String，存在格式上的问题！
            PyObject* args = Py_BuildValue("(s)", ".\\Resource_Files\\1.jpg");//给python函数参数赋值
            // PyObject* face = PyObject_CallObject(pFunc, args);  // 面部矩形框
            PyObject* mark = PyObject_CallObject(pFuncl, args);    // 调用函数，获取特征点列表
            PyObject* x = nullptr;
            PyObject* y = nullptr;

            for (int n = 0; n < int(PyList_GET_SIZE(mark)); n++)
            {
                PyObject* location = PyList_GetItem(mark, n);
                x = PyTuple_GetItem(location, 0);
                y = PyTuple_GetItem(location, 1);
                int int_x = 1;
                int int_y = 1;

                PyArg_Parse(x, "i", &int_x);
                PyArg_Parse(y, "i", &int_y);
                location_x.push_back(int_x);  // 特征点的x坐标
                location_y.push_back(int_y);  // 特征点的y坐标
            }
        }
        Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
    }
}