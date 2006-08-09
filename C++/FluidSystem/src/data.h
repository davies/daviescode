#ifndef __DATA_H__
#define __DATA_H__

#define VT(s)  _variant_t(s)
#define VTM		vtMissing

extern OilCrocks g_OilCrocks;
extern Pump		 g_Pump;
extern OilBox	 g_Box;

bool OpenExcelFile(LPCTSTR filename);	//��excel�ļ��ж�ȡ����
bool SaveExcelFile(LPCTSTR filename);	//�����ݱ��浽excel�ļ���ȥ

bool OpenExcelFile9(LPCTSTR filename);	//��excel�ļ��ж�ȡ����
bool SaveExcelFile9(LPCTSTR filename);	//�����ݱ��浽excel�ļ���ȥ

bool OpenExcelFile10(LPCTSTR filename);	//��excel�ļ��ж�ȡ����
bool SaveExcelFile10(LPCTSTR filename);	//�����ݱ��浽excel�ļ���ȥ

void PrintExcel(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);
void PrintExcel9(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);
void PrintExcel10(const OilCrocks &oilCrocks,const  OilBox &oilBox,const Pump &pump);

CString GetModuleDir();  //��ȡ��ǰ·��
bool IsFileValid(LPCSTR path);

#endif 