#ifndef __DATA_H__
#define __DATA_H__

#define VT(s)  _variant_t(s)
#define VTM		vtMissing

extern OilCrocks g_OilCrocks;
extern Pump		 g_Pump;
extern OilBox	 g_Box;

bool OpenExcelFile(LPCTSTR filename);	//从excel文件中读取数据
bool SaveExcelFile(LPCTSTR filename);	//将数据保存到excel文件中去

bool OpenExcelFile9(LPCTSTR filename);	//从excel文件中读取数据
bool SaveExcelFile9(LPCTSTR filename);	//将数据保存到excel文件中去

bool OpenExcelFile10(LPCTSTR filename);	//从excel文件中读取数据
bool SaveExcelFile10(LPCTSTR filename);	//将数据保存到excel文件中去

void PrintExcel(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);
void PrintExcel9(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);
void PrintExcel10(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);

CString GetModuleDir();  //获取当前路径
bool IsFileValid(LPCSTR path);

#endif 