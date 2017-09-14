#include "StdAfx.h"
#include "Edislab Pro.h"
#include "GaugeDlg.h"
#include "GaugeManager.h"
//#include "GlobalDataManager.h"
using namespace std;
GaugeManager::GaugeManager(void)
{
	//CGlobalDataManager::CreateInstance().Attach(this);
}


GaugeManager::~GaugeManager(void)
{
	//CGlobalDataManager::CreateInstance().Detach(this);
}
//更新数据，数据变化时调用,更新表盘
void GaugeManager::Update(CObservable* pObs, void* pArg)
{
	//CGlobalDataManager* dbMgr = dynamic_cast<CGlobalDataManager*>(pObs);
	//if(NULL==dbMgr)//不是数据类
	//{
	//	return;
	//}
	//更新单表记录的数据
	for(UINT i= 0; i<m_vecGaugeDlg.size(); ++i)
	{
		if(m_vecGaugeDlg[i])
		{
			m_vecGaugeDlg[i]->updateData();
		}
	}
}
//增加表盘
void GaugeManager::addGaugeDlg(GaugeDlg* dlg)
{
	bool bExist = false;
	vector<GaugeDlg*>::iterator itr = m_vecGaugeDlg.begin();
	while(itr!=m_vecGaugeDlg.end())
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
		m_vecGaugeDlg.push_back(dlg);
	}
}
//删除表盘
void GaugeManager::delGaugeDlg(GaugeDlg* dlg)
{
	vector<GaugeDlg*>::iterator itr = m_vecGaugeDlg.begin();
	while(itr!=m_vecGaugeDlg.end())
	{
		if(*itr==dlg)
		{
			m_vecGaugeDlg.erase(itr);
			break;
		}
		++itr;
	}
}
//保存数据
int GaugeManager::saveData()
{
	return 0;
}
//读取数据
int GaugeManager::readData()
{
	return 0;
}
