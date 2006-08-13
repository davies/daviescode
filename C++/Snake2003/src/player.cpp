//---------------------------------------------------------------------------

#include <windows.h>
#include "playerstruct.h"
//---------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdreason, LPVOID lpvReserved)
{
        return 1;
}

//---------------------------------------------------------------------------

// player 样本
// NOTICE
/*
用Borland C++ Compiler直接编译为dll即可
用Microsoft Visual C++编译，则请加入一个.DEF文件，文件内容为 
EXPORTS
player_create @1
player_judge @2
player_destroy @3
编译为dll后，放到/player目录下

player_create返回一个指针，指针指向的具体内容由选手自己定，用于分配资源 
player_judge的第一个参数即为选手刚才返回的指针
player_destroy的参数也是player_create中选手的指针，用于回收资源 

MAPFILESTRUCT, INITINFO, PLAYER_INFO的具体定义见playerstruct.h 

plyaer_judge的返回值可以是-1, 0, 1，分别代表左转、直走和右转 
*/
#define EXPORT extern "C" __declspec(dllexport)

EXPORT void* __stdcall player_create(MAPFILESTRUCT* map, INITINFO* info)
{
}

EXPORT int __stdcall player_judge(void*, PLAYER_INFO*) 
{
}

EXPORT void __stdcall player_destroy(void*)
{

}


