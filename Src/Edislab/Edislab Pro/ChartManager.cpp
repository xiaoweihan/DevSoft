#include "StdAfx.h"
#include "Edislab Pro.h"
#include "ChartManager.h"
#include "ChartFigureDlg.h"
#include "GlobalDataManager.h"
using namespace std;

ChartManager::ChartManager(void)
{
	CGlobalDataManager::CreateInstance().Attach(this);
}


ChartManager::~ChartManager(void)
{
	CGlobalDataManager::CreateInstance().Detach(this);
}

//更新数据，把全局实验数据更新到画图专用数据中
void ChartManager::Update(CObservable* pObs, void* pArg)
{
	CGlobalDataManager* dbMgr = dynamic_cast<CGlobalDataManager*>(pObs);
	if(NULL==dbMgr)//不是数据类
	{
		return;
	}
	//更新单表记录的数据
	for(UINT i= 0; i<m_vecChartDlg.size(); ++i)
	{
		if(m_vecChartDlg[i])
		{
			m_vecChartDlg[i]->updateData(dbMgr);
		}
	}
}
//获取数据，用于描绘
//const std::vector<ChartXYData*> ChartManager::getChartData()
//{
//	return m_vecChartData;
//}
//增加图表
void ChartManager::addChartDlg(const ChartFigureDlg* dlg)
{
	bool bExist = false;
	vector<const ChartFigureDlg*>::iterator itr = m_vecChartDlg.begin();
	while(itr!=m_vecChartDlg.end())
	{
		if(*itr==dlg)
		{
			bExist = true;
			break;
		}
		++itr;
	}
	if(!bExist)
	{
		m_vecChartDlg.push_back(dlg);
	}
}
//删除图表
void ChartManager::delChartDlg(const ChartFigureDlg* dlg)
{
	//m_chartDlg.erase(dlg);
	vector<const ChartFigureDlg*>::iterator itr = m_vecChartDlg.begin();
	while(itr!=m_vecChartDlg.end())
	{
		if(*itr==dlg)
		{
			m_vecChartDlg.erase(itr);
			break;
		}
		++itr;
	}
}
//保存数据
int ChartManager::saveData()
{
	//更新单表记录的数据
	for(UINT i= 0; i<m_vecChartDlg.size(); ++i)
	{
		if(m_vecChartDlg[i])
		{
			m_vecChartDlg[i]->saveData();
		}
	}
	return 0;
}
//读取数据
int ChartManager::readData()
{
	//读取数据并创建对应的对话框
	return 0;
}
