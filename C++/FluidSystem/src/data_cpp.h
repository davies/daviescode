
float GetValue(_Worksheet &sheet,int col,int row){
	Range range;
	char grid[16];
	sprintf(grid,"%c%d",'A'+col-1,row);
	range.AttachDispatch(sheet.GetRange(VT(grid),VT(grid)) );
	COleVariant value = range.GetValue2();
		return float(value.dblVal);
}

String GetString(_Worksheet &sheet,int col,int row)
{
	Range range;
	char grid[16];
	sprintf(grid,"%c%d",'A'+col-1,row);
	range.AttachDispatch(sheet.GetRange(VT(grid),VT(grid)) );
	COleVariant value = range.GetValue2();
	if( value.vt == VT_BSTR ){
		return String( _com_util::ConvertBSTRToString(value.bstrVal) );
	}else{
		return String("");
	}
}

float ReadProperty( _Worksheet &sheet, LPCTSTR name){
	float value = 0.0;
	Range allRange = sheet.GetCells();
	LPDISPATCH found = allRange.Find(VT(name),VTM,VTM,VTM,VTM,VT(long(1)),
#ifdef EXCEL10
		VTM,
#endif
		VTM,VTM);
	if( found ){
		Range tagRange;
		tagRange.AttachDispatch(found);
		int row = tagRange.GetRow();
		int col = tagRange.GetColumn();
		value = GetValue(sheet,col+1,row);
	}
	return value;
}


Pump ReadPumpData( _Worksheet &sheet ){
	Pump pump;
	pump.efficiency = ReadProperty(sheet,"�ݻ�Ч��%")  /100;
	pump.quality = ReadProperty(sheet,"������") * 1e-6;
	pump.rotateSpeed = ReadProperty(sheet,"ת��") / 60;
	pump.count = ReadProperty(sheet,"��ʵ������");
	return pump;
}

OilBox ReadOilBoxData( _Worksheet &sheet)
{
	OilBox oilBox;
	
	oilBox.p_air = ReadProperty(sheet,"����ѹ��") * 1e6;
	oilBox.p_max = ReadProperty(sheet,"��ѹ") * 1e6;
	oilBox.p_min = ReadProperty(sheet,"���ѹ") * 1e6;
	oilBox.p_hypoMin = ReadProperty(sheet,"��ѹ") * 1e6;

	oilBox.v = ReadProperty(sheet,"�������ݻ�") * 1e-3;

	oilBox.Update();
	
	return oilBox;
}

Actions ReadActionsData( _Worksheet &sheet ,LPCTSTR name)
{
	Actions actions;
	Range allRange = sheet.GetCells();
	LPDISPATCH found = allRange.Find(VT(name),VTM,VTM,VTM,VTM,VT(long(1)),
#ifdef EXCEL10
		VTM,
#endif
		VTM,VTM);
	if( found ){
		Range nameRange;
		nameRange.AttachDispatch(found);
		int row = nameRange.GetRow();
		int col = nameRange.GetColumn();
		int count = (int)GetValue(sheet,col,row+1); //������
		actions.period = (float)GetValue(sheet,col,row+2);

		for( int i=1;i<=count;i++){
			Action action;
			action.startTime = GetValue(sheet,col+1,row+i);
			action.endTime = GetValue(sheet,col+2,row+i);
			action.vel = GetValue(sheet,col+3,row+i) * 1e-3;
			actions.push_back(action);
		}
	}

	std::sort(actions.begin(),actions.end());//����

	return actions;
}

		//��ȡ�������ڱ�
void ReadAllActionsData( _Worksheet &sheet ,OilCrocks &oilCrocks)
{
	int count = oilCrocks.size();
	for(int i=0;i<count;i++){
		oilCrocks[i].actions = ReadActionsData(sheet,oilCrocks[i].name.c_str());
	}
}

OilCrocks ReadOilCrocksData( _Worksheet & sheet, int offset)
{
	OilCrocks oilCrocks;
	int count = 0;
	while( true ){
		int curr = count + offset;
		OilCrock oilCrock;
		oilCrock.name = GetString(sheet,1,curr);
		if( oilCrock.name.length() <= 0 ) break;
		
		oilCrock.D = GetValue(sheet,3,curr)*1e-3;
		oilCrock.pD = GetValue(sheet,4,curr)*1e-3;
		oilCrock.length = GetValue(sheet,6,curr)*1e-3;
		oilCrock.count = (int)GetValue(sheet,5,curr);

		count ++;
		oilCrocks.push_back(oilCrock);
	}

	return oilCrocks;
}

#ifdef EXCEL10
bool OpenExcelFile10(LPCTSTR filename)
#else
bool OpenExcelFile9(LPCTSTR filename)
#endif
{
	_Application excelApp;
	Workbooks	books;
	_Workbook	book;
	Worksheets		sheets;
	_Worksheet	sheet2;
	_Worksheet	sheet3;

	HRESULT hr = excelApp.CreateDispatch("Excel.Application",NULL);
	if (FAILED(hr)){
		AfxMessageBox("��ʼ��EXCEL������ʧ��!\n�������ҪEXCEL XP��֧��.");
		return false;
	}

	books.AttachDispatch( excelApp.GetWorkbooks() );
	book.AttachDispatch(books.Open(filename,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM
#ifdef EXCEL10
		,VTM,VTM
#endif
		));

	try{
		sheets.AttachDispatch(book.GetWorksheets());
		sheet2.AttachDispatch( sheets.GetItem(VT("sheet2")) );
		sheet3.AttachDispatch( sheets.GetItem(VT("sheet3")) );
	}
	catch(...)
	{
		AfxMessageBox("�ļ���ʽ���ԣ�������sheet2����sheet3��!");
		excelApp.Quit();
		return false;
	}

	//��ȡ�͸�����
	g_OilCrocks = ReadOilCrocksData(sheet2,5);
	ReadAllActionsData(sheet3,g_OilCrocks);

	g_Pump = ReadPumpData(sheet2);
	g_Box = ReadOilBoxData(sheet2);

	book.SetSaved(TRUE);
	excelApp.Quit();
	return true;
}

void SetValue(_Worksheet &sheet,int col,int row,float value)
{
	Range range;
	char grid[16];
	sprintf(grid,"%c%d",'A'+col-1,row);
	range.AttachDispatch(sheet.GetRange(VT(grid),VT(grid)) );
	range.SetValue2( VT(double(value)) );
}

void SetString(_Worksheet &sheet,int col,int row,String string){
	Range range;
	char grid[16];
	sprintf(grid,"%c%d",'A'+col-1,row);
	range.AttachDispatch(sheet.GetRange(VT(grid),VT(grid)) );
	range.SetValue2(VT(string.c_str()));
}

void WriteActionsData(_Worksheet &sheet,OilCrocks oilCrocks)
{
	Range allRange = sheet.GetCells();
	allRange.Clear();

	int i,j;
	int lastRow = 1;
	for(i=0;i<oilCrocks.size();i++){
		SetString(sheet,1,lastRow,oilCrocks[i].name);
		SetString(sheet,2,lastRow,"��ʼʱ��(s)");
		SetString(sheet,3,lastRow,"����ʱ��(s)");
		SetString(sheet,4,lastRow,"�ٶ�(mm/s)");
		Actions &actions = oilCrocks[i].actions;
		int count = actions.size();
		SetValue(sheet,1,lastRow+1,count);
		SetValue(sheet,1,lastRow+2,actions.period);
		for(j=0;j<count;j++){
			SetValue(sheet,2,lastRow+1+j,int(actions[j].startTime*10)/10.0);
			SetValue(sheet,3,lastRow+1+j,int(actions[j].endTime*10)/10.0);
			SetValue(sheet,4,lastRow+1+j,int(actions[j].vel*1e3));
		}
		lastRow += count+3;
	}
}

void WriteProperty( _Worksheet &sheet, LPCTSTR name, float value){
	Range allRange = sheet.GetCells();
	LPDISPATCH found = allRange.Find(VT(name),VTM,VTM,VTM,VTM,VT(long(1)),
#ifdef EXCEL10
		VTM,
#endif
		VTM,VTM);
	if( found ){
		Range tagRange;
		tagRange.AttachDispatch(found);
		int row = tagRange.GetRow();
		int col = tagRange.GetColumn();
		SetValue(sheet,col+1,row,value);
	}
}

void WritePumpData( _Worksheet &sheet, Pump pump)
{
	WriteProperty(sheet,"�ݻ�Ч��%",pump.efficiency * 100);
	WriteProperty(sheet,"������",pump.quality * 1e6);
	WriteProperty(sheet,"ת��",pump.rotateSpeed * 60);
	WriteProperty(sheet,"��ʵ������",pump.count);
}

void WriteOilBoxData( _Worksheet &sheet, OilBox oilBox)
{
	WriteProperty(sheet,"����ѹ��",oilBox.p_air*1e-6);
	WriteProperty(sheet,"��ѹ",oilBox.p_max*1e-6);
	WriteProperty(sheet,"���ѹ",oilBox.p_min * 1e-6);
	WriteProperty(sheet,"��ѹ",oilBox.p_hypoMin*1e-6);

	WriteProperty(sheet,"�������ݻ�",oilBox.v*1e3);
}

#ifdef EXCEL10
bool SaveExcelFile10(LPCTSTR filename)
#else
bool SaveExcelFile9(LPCTSTR filename)
#endif
{
	_Application excelApp;
	Workbooks	books;
	_Workbook	book;
	Worksheets		sheets;
	_Worksheet	sheet2;
	_Worksheet	sheet3;
	Range		range;

	if ( !excelApp.CreateDispatch("Excel.Application",NULL) ){
		AfxMessageBox("��ʼ��EXCEL������ʧ��!\n�������ҪEXCEL XP��֧��.");
		return false;
	}
	excelApp.SetVisible(false);

	books.AttachDispatch( excelApp.GetWorkbooks() );
	book.AttachDispatch(books.Open(filename,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM
#ifdef EXCEL10
		,VTM,VTM
#endif
		));

	try{
		sheets.AttachDispatch(book.GetWorksheets());
		sheet2.AttachDispatch( sheets.GetItem(VT("sheet2")) );
		sheet3.AttachDispatch( sheets.GetItem(VT("sheet3")) );
	}
	catch(...)
	{
		AfxMessageBox("�ļ���ʽ���ԣ�������sheet2����sheet3��!");
		excelApp.Quit();
		return false;
	}

	//g_OilCrocks = ReadOilCrocksData(sheet2,5);
	WriteActionsData(sheet3,g_OilCrocks);

	WritePumpData(sheet2,g_Pump);
	WriteOilBoxData(sheet2,g_Box);

	book.Save();
	excelApp.Quit();
	return true;
}

//�ƶ�chart��top��һ��λ��
void MoveChart(ChartObject &chart,int &top ){
	const int paperWidth = 700;
	const int paperHeight = 1050;
	const int space = 20;
	const int chartWidth = paperWidth;
	const int chartHeight = (paperHeight-130)/5;

	chart.SetLeft(space);
	chart.SetTop(top+space);
	chart.SetWidth(chartWidth-space*2);
	chart.SetHeight(chartHeight-space*2);
	
	top += chartHeight;
	if( (top % paperHeight) > paperHeight - chartHeight ){
		top = (top/paperHeight+1)*paperHeight + 30;
	}
}

#ifdef EXCEL10
void PrintExcel10(const OilCrocks &oilCrocks,const OilBox &oilBox,const Pump &pump)
#else
void PrintExcel9(const OilCrocks &oilCrocks,const OilBox &oilBox,const Pump &pump)
#endif
{
	//�Ƚ��з���
	Flux pumpFlux,allFlux;
	OilCount oilCount = Simulation(oilCrocks,oilBox,pump,pumpFlux,allFlux);

	_Application excelApp;
	Workbooks	books;
	_Workbook	book;
	Worksheets sheets;
	_Worksheet	printSheet,dataSheet;
	ChartObject actionsChartObject,curChartObject;
	_Chart		chart;
	ChartTitle  chartTitle;
	Range		range;

	HRESULT hr= excelApp.CreateDispatch("Excel.Application",NULL);
	if ( FAILED(hr) ){
		AfxMessageBox("��ʼ��EXCEL������ʧ��!");
		return ;
	}
	
	//Get filename
	CString filename(GetModuleDir()+CString("\\print.plt"));
	if( ! IsFileValid(filename) ){
		AfxMessageBox("�Ҳ�����ӡģ���ļ�print.xls,��ŵ���ǰĿ¼��");
		return;
	}
	books.AttachDispatch( excelApp.GetWorkbooks() );
	book.AttachDispatch(books.Open(filename,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM,VTM
#ifdef EXCEL10
		,VTM,VTM
#endif
		));

	sheets.AttachDispatch( book.GetSheets() );
	printSheet.AttachDispatch( sheets.GetItem(VT("print")) );
	dataSheet.AttachDispatch( sheets.GetItem(VT("data")) );

	WritePumpData(printSheet,g_Pump);
	WriteOilBoxData(printSheet,g_Box);

	int i;
	int crockCount = oilCrocks.size();

	Range sourceRange;
	int top = 130;
	actionsChartObject.AttachDispatch( printSheet.ChartObjects(VT("actionChart")) );
	CString str1,str2;
	for(i=0;i<crockCount;i++){
		str1.Format("A%d",i+1);
		actionsChartObject.SetName(str1);
		actionsChartObject.Copy();
		printSheet.Paste( VTM,VTM );  //ճ��ͼ��
		actionsChartObject.SetName("actions");

		//�ƶ�ͼ�����ʵ�λ��
		curChartObject.AttachDispatch( printSheet.ChartObjects(VT(str1)) );
		MoveChart(curChartObject,top);

		//��������Դ
		const Actions &actions = oilCrocks[i].actions;
		int actionCount = actions.size();
		str1.Format("A%d",i*50+1);
		str2.Format("B%d",(i+1)*50);
		sourceRange.AttachDispatch( dataSheet.GetRange( VT(str1),VT(str2) ) );
		sourceRange.Clear();

		sourceRange.SetItem( VT(long(1)),VT(long(1)),VT(double(0)) ); //��ʼ��
		sourceRange.SetItem( VT(long(1)),VT(long(2)),VT(double(0)) );

		for(long j=0;j<actionCount;j++){
			sourceRange.SetItem( VT(long(j*4+2)),VT(long(1)),VT(actions[j].startTime) );
			sourceRange.SetItem( VT(long(j*4+2)),VT(long(2)),VT(double(0)) );
			sourceRange.SetItem( VT(long(j*4+3)),VT(long(1)),VT(actions[j].startTime) );
			sourceRange.SetItem( VT(long(j*4+3)),VT(long(2)),VT(actions[j].vel*1e3) );
			sourceRange.SetItem( VT(long(j*4+4)),VT(long(1)),VT(actions[j].endTime) );
			sourceRange.SetItem( VT(long(j*4+4)),VT(long(2)),VT(actions[j].vel*1e3) );
			sourceRange.SetItem( VT(long(j*4+5)),VT(long(1)),VT(actions[j].endTime) );
			sourceRange.SetItem( VT(long(j*4+5)),VT(long(2)),VT(double(0)) );
		}

		sourceRange.SetItem( VT(long(actionCount*4+2)),VT(long(1)),VT(double(actions.period)) ); //��ʼ��
		sourceRange.SetItem( VT(long(actionCount*4+2)),VT(long(2)),VT(double(0)) );

		//��������Դ
		chart.AttachDispatch( curChartObject.GetChart() );
		chart.SetSourceData(VT(sourceRange),VTM);
		//���ñ���
		chartTitle.AttachDispatch(chart.GetChartTitle());
		chartTitle.SetCaption( CString(oilCrocks[i].name.c_str())+CString("�Ķ������ڱ�") );
	}
	actionsChartObject.Delete();

	//�����͸�������ͼ
	curChartObject.AttachDispatch( printSheet.ChartObjects(VT("flux")) );
	MoveChart(curChartObject,top);
	str2.Format("H%d",allFlux.size()*2+10);
	sourceRange.AttachDispatch( dataSheet.GetRange(VT("G1"),VT(str2)) );
	sourceRange.Clear();
	float lastValue = 0.0;
	for(i=0;i<allFlux.size();i++){
		sourceRange.SetItem( VT(long(i*2+1)),VT(long(1)),VT(allFlux[i].time) );
		sourceRange.SetItem( VT(long(i*2+1)),VT(long(2)),VT(lastValue*1e3*60) );
		sourceRange.SetItem( VT(long(i*2+2)),VT(long(1)),VT(allFlux[i].time) );
		sourceRange.SetItem( VT(long(i*2+2)),VT(long(2)),VT(allFlux[i].value*1e3*60) );
		lastValue = allFlux[i].value;
	}
//	chart.AttachDispatch( curChartObject.GetChart() );
//	chart.SetSourceData( VT(sourceRange),VTM );

	//�����ͱ�����
	curChartObject.AttachDispatch( printSheet.ChartObjects(VT("pump")) );
	MoveChart(curChartObject,top);
	str2.Format("J%d",pumpFlux.size()*2+10);
	sourceRange.AttachDispatch( dataSheet.GetRange(VT("I1"),VT(str2)) );
	sourceRange.Clear();
	lastValue = 0.0;
	for(i=0;i<pumpFlux.size();i++){
		sourceRange.SetItem( VT(long(i*2+1)),VT(long(1)),VT(pumpFlux[i].time) );
		sourceRange.SetItem( VT(long(i*2+1)),VT(long(2)),VT(lastValue*1e3*60) );
		sourceRange.SetItem( VT(long(i*2+2)),VT(long(1)),VT(pumpFlux[i].time) );
		sourceRange.SetItem( VT(long(i*2+2)),VT(long(2)),VT(pumpFlux[i].value*1e3*60) );
		lastValue = pumpFlux[i].value;
	}

	//��������������ͼ
	curChartObject.AttachDispatch( printSheet.ChartObjects(VT("oilCount")) );
	MoveChart(curChartObject,top);
	str2.Format("D%d",oilCount.size()+5);
	sourceRange.AttachDispatch( dataSheet.GetRange(VT("C1"),VT(str2)) );
	sourceRange.Clear();
	for(i=0;i<oilCount.size();i++){
		sourceRange.SetItem( VT(long(i+1)),VT(long(1)),VT(oilCount[i].time) );
		sourceRange.SetItem( VT(long(i+1)),VT(long(2)),VT(oilCount[i].value*1e3) );
	}

	//����������ѹ��ͼ
	curChartObject.AttachDispatch( printSheet.ChartObjects(VT("press")) );
	MoveChart(curChartObject,top);
	str2.Format("F%d",oilCount.size()+5);
	sourceRange.AttachDispatch( dataSheet.GetRange(VT("E1"),VT(str2)) );
	sourceRange.Clear();
	//�����������е�������С��<2.5%�������е�ѹ���������н������Թ�ϵ
	for(i=0;i<oilCount.size();i++){
		sourceRange.SetItem( VT(long(i+1)),VT(long(1)),VT(oilCount[i].time) );
		float p = oilBox.p_air / (1 - oilCount[i].value / oilBox.v) ;
		sourceRange.SetItem( VT(long(i+1)),VT(long(2)),VT( p*1e-6) );
	}
	
	printSheet.PrintOut(VTM,VTM,VT(long(1)),VTM,VT(true),VTM,VTM,VTM);
	book.SetSaved(TRUE);

	excelApp.Quit();
}