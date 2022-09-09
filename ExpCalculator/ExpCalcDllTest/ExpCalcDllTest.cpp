// ExpCalcDllTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>

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
int main()
{
    std::cout << "这是一个算术表达式计算器\n";
    //初始化 变量map
    std::map<std::string, double> expvariable_map;
    expvariable_map.insert(pair<std::string, double >(std::string("变量v"), 100.0));

    string expstr = "(lg变量v+sin(30/180*@pi)*2)^2";

    CExpCalcObject expobj;
    CExpCalcInterface* expint = expobj.CreateObj();
    expint->CreateExpression(expstr);
    expint->SetVariableMap(&expvariable_map);

    HANDLE hOutput;
    COORD coord = { 0,8 };
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    while (1) {
        if (KEY_DOWN(VK_ESCAPE)) break;
        expvariable_map["变量v"] = expvariable_map["变量v"] + 0.1;
        SetConsoleCursorPosition(hOutput, coord);
        std::cout << "变量v" << " = " << expvariable_map["变量v"] << std::endl;
        std::cout << expstr << " = " << expint->GetClacResult() << std::endl;

        Sleep(50);
    }
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
