// ExpCalcDllTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>

#include "../CExpCalcDll/CExpCalcObject.h"



#ifdef _DEBUG
#if defined(_AMD64_)    //64位Debug
#pragma comment (lib,"../bin/x64/ExpCalc_d_x64.lib")
#else                   //32位Debug
#pragma comment (lib,"../bin/Win32/ExpCalc_d_x86.lib")
#endif
#else
#if defined(_AMD64_)    //64位Release
#pragma comment (lib,"../bin/x64/ExpCalc_x64.lib")
#else                   //32位Release
#pragma comment (lib,"../bin/Win32/ExpCalc_x86.lib")
#endif
#endif  //_DEBUG

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

mutex mut;

//线程使用类调用
class CCalc {
public:
    CCalc() {
        start = false;
        m_expstr = "(lg[x]+sin(30/180*@pi)*2)^2";
        expint = expobj.CreateObj();
        expint->CreateExpression(m_expstr);
    };
    ~CCalc() {
    };

    void SetVariableMap(std::map<std::string, double>* datamap)
    {
        expint->SetVariableMap(datamap);
    }

    void stop()
    {
        start = false;
    }

    void run() {
        start = true;
        HANDLE hOutput;
        COORD coord = { 0,25 };
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        while (start) {
            {//显示互斥，否则会显示混乱， 把需要互斥的地方括起来，使用std::lock_guard类模板实现互斥元，对象在构造时锁，析构时解锁
                std::lock_guard<std::mutex> mtx_locker(mut);
                SetConsoleCursorPosition(hOutput, coord);
                std::cout << "子线程：" << m_expstr << " = " << expint->GetClacResult() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
private:
    CExpCalcObject expobj;
    CExpCalcInterface* expint;
    string m_expstr;
    bool start;
};


int main()
{
    std::cout << "这是一个算术表达式计算器\n";
    //初始化 变量map
    std::map<std::string, double> expvariable_map;
    expvariable_map.insert(pair<std::string, double >(std::string("变量v"), 0.0));
    expvariable_map.insert(pair<std::string, double >(std::string("x"), 0.0));
    expvariable_map.insert(pair<std::string, double >(std::string("3.1415926"), 3.1415926));

    //定义一个表达式
    string expstr = "(lg变量v+sin(30/180*@pi)*2)^2";
    CExpCalcObject expobj;
    CExpCalcInterface* expint = expobj.CreateObj();
    expint->CreateExpression(expstr);
    expint->SetVariableMap(&expvariable_map);

    //开启一个计算线程
    CCalc calc;
    calc.SetVariableMap(&expvariable_map);
    thread test(&CCalc::run, &calc);
    //test.detach();  //detach()：线程退出时自动释放资源。  join()：阻塞主线程，等待线程退出。

    HANDLE hOutput;
    COORD coord = { 0,15 };
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    while (1) {
        if (KEY_DOWN(VK_ESCAPE)) { //Esc -- 退出
            calc.stop();
            break;
        }
        expvariable_map["变量v"] = expvariable_map["变量v"] + 0.1;
        expvariable_map["x"] = expvariable_map["变量v"];
        {//显示互斥，否则会显示混乱
            std::lock_guard<std::mutex> mtx_locker(mut);
            SetConsoleCursorPosition(hOutput, coord);
            //显示变量表中的变量。
            std::cout << "变量标签名  值" << endl;
            std::cout << "-----------------" << endl;
            std::cout << "变量v" << " = " << expvariable_map["变量v"] << std::endl;
            std::cout << "[x]" << " = " << expvariable_map["x"] << std::endl;
            std::cout << "[3.1415926]" << " = " << expvariable_map["3.1415926"] << std::endl;
            std::cout << "-----------------" << endl;
            std::cout << std::endl;

            std::cout << "表达式计算" << endl;
            std::cout << "-----------------" << endl;
            std::cout <<"主线程：" << expstr << " = " << expint->GetClacResult() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    //等待线程退出
    test.join();
    return 0;
}
