//---------------------------------------------------------------------------

#include <windows.h>
#include "playerstruct.h"
//---------------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fwdreason, LPVOID lpvReserved)
{
        return 1;
}

//---------------------------------------------------------------------------

// player ����
// NOTICE
/*
��Borland C++ Compilerֱ�ӱ���Ϊdll����
��Microsoft Visual C++���룬�������һ��.DEF�ļ����ļ�����Ϊ 
EXPORTS
player_create @1
player_judge @2
player_destroy @3
����Ϊdll�󣬷ŵ�/playerĿ¼��

player_create����һ��ָ�룬ָ��ָ��ľ���������ѡ���Լ��������ڷ�����Դ 
player_judge�ĵ�һ��������Ϊѡ�ָղŷ��ص�ָ��
player_destroy�Ĳ���Ҳ��player_create��ѡ�ֵ�ָ�룬���ڻ�����Դ 

MAPFILESTRUCT, INITINFO, PLAYER_INFO�ľ��嶨���playerstruct.h 

plyaer_judge�ķ���ֵ������-1, 0, 1���ֱ������ת��ֱ�ߺ���ת 
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


