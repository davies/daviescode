#include "stdafx.h"
#include <comdef.h>
#include <comutil.h>
#include "excel10.h"
#include "excel9.h"
#include "simu.h"
#include "data.h"

#pragma comment(lib, "comsupp.lib")

using namespace EXCEL9;

OilCrocks g_OilCrocks;
Pump	  g_Pump;
OilBox	  g_Box;

CString GetExcelVersion(){
	_Application excelApp;
	CString strVersion;
	HRESULT hr = excelApp.CreateDispatch("Excel.Application",NULL);
	if (FAILED(hr)){
		AfxMessageBox("初始化EXCEL服务器失败!\n该软件需要EXCEL XP/2000的支持.");
	}else{
		strVersion = excelApp.GetVersion();
		excelApp.Quit();
	}
	return strVersion;
}

bool OpenExcelFile(LPCTSTR filename)
{
	CString strVersion = GetExcelVersion();
	
	if( strVersion == "9.0" ){
		return OpenExcelFile9(filename);
	}else if (strVersion == "10.0" || strVersion == "11.0"){
		return OpenExcelFile10(filename);
	}else{
		AfxMessageBox("初始化EXCEL服务器失败!\n该软件需要EXCEL XP/2000的支持.");
		return false;
	}
}

bool SaveExcelFile(LPCTSTR filename)
{
	CString strVersion = GetExcelVersion();

	if( strVersion == "9.0" ){
		return SaveExcelFile9(filename);
	}else if (strVersion == "10.0" || strVersion == "11.0"){
		return SaveExcelFile10(filename);
	}else{
		AfxMessageBox("初始化EXCEL服务器失败!\n该软件需要EXCEL XP/2000的支持.");
		return false;
	}
}

void PrintExcel(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump)
{
	CString strVersion = GetExcelVersion();

	if( strVersion == "9.0" ){
		PrintExcel9(oilCrocks,oilBox,pump);
	}else if (strVersion == "10.0" || strVersion == "11.0"){
		PrintExcel10(oilCrocks,oilBox,pump);
	}else{
		AfxMessageBox("初始化EXCEL服务器失败!\n该软件需要EXCEL XP/2000的支持.");
	}
}


//获取当前路径
CString GetModuleDir()
{
	TCHAR pFileName[MAX_PATH];
	GetModuleFileName( NULL, pFileName, MAX_PATH );
	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind( _T('\\') );
	if( nPos < 0 )
		return CString("");
	else
		return csFullPath.Left( nPos );
}

// IsFileValid returns TRUE if the specified file exists, FALSE otherwise.
bool IsFileValid(LPCSTR path)
{
	unsigned int attribs;
	attribs = GetFileAttributes(path);
	if (attribs == 0xFFFFFFFF)
		return FALSE;
	else
		return (attribs & FILE_ATTRIBUTE_DIRECTORY ? FALSE : TRUE);
}
