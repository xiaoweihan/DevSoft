#pragma once
#include <vector>
#include "Observer.h"

class ChartFigureDlg;
class ChartXYData;
class ChartManager:public CObserver
{
public:
	ChartManager(void);
	virtual ~ChartManager(void);
	//更新数据，把全局实验数据更新到画图专用数据中
	virtual void Update(class CObservable* pObs, void* pArg = NULL);
	//获取数据，用于描绘
	//const std::vector<ChartXYData*> getChartData();
	//增加图表
	void addChartDlg(const ChartFigureDlg* dlg);
	//删除图表
	void delChartDlg(const ChartFigureDlg* dlg);
	//保存数据
	int saveData();
	//读取数据
	int readData();
private:
	//图表公有数据
	//std::vector<ChartXYData*> m_vecChartData;
	//所有图表对话框
	std::vector<const ChartFigureDlg*> m_vecChartDlg;
};
