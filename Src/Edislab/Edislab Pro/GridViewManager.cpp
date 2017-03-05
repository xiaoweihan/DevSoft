/*******************************************************************************/
/*描述:Grid控件视图管理类
/*作者:
/*日期:2015.8
/*******************************************************************************/
#include "stdafx.h"
#include "GridViewManager.h"
#include "CustomLabGridCtrl.h"
#include "GlobalDataManager.h"


CGridViewManager::CGridViewManager(void)
{
	CGlobalDataManager::CreateInstance().Attach(this);

	InitializeCriticalSection(&m_cs);

	m_GridViewList.clear();
}


CGridViewManager::~CGridViewManager(void)
{
	DeleteCriticalSection(&m_cs);
}

void CGridViewManager::AddGridView(CCustomLabGridCtrl* pGridView)
{
	if (NULL == pGridView)
		return;

	EnterCriticalSection(&m_cs);
	list<VIEWMANAGER>::iterator Iter = m_GridViewList.begin();
	while (Iter != m_GridViewList.end())
	{
		if (Iter->pGrid == pGridView)
		{
			break;
		}
		++Iter;
	}

	if (Iter == m_GridViewList.end())
	{
		VIEWMANAGER ViewManager;
		ViewManager.pGrid = pGridView;
		m_GridViewList.push_back(ViewManager);
	}

	LeaveCriticalSection(&m_cs);
}

void CGridViewManager::AddGridViewAttibute(CCustomLabGridCtrl* pGridView,GRID_FONT_TYPE type,vector<int> vecCol)
{
	if (NULL == pGridView)
		return;

	if(type < NORMAL || type > ITALIC)
		return;

	EnterCriticalSection(&m_cs);
	list<VIEWMANAGER>::iterator Iter = m_GridViewList.begin();
	while (Iter != m_GridViewList.end())
	{
		if (Iter->pGrid == pGridView)
		{
			Iter->emFontType = type;
			Iter->vecHideColIndex.clear();
			Iter->vecHideColIndex = vecCol;
			break;
		}
		++Iter;
	}
	LeaveCriticalSection(&m_cs);
}

void CGridViewManager::RemoveGridView(CCustomLabGridCtrl* pGridView)
{
	EnterCriticalSection(&m_cs);
	list<VIEWMANAGER>::iterator Iter = m_GridViewList.begin();
	while (Iter != m_GridViewList.end())
	{
		if (Iter->pGrid == pGridView)
		{
			break;
		}
		++Iter;
	}

	if (Iter != m_GridViewList.end())
	{
		m_GridViewList.erase(Iter);
	}
	LeaveCriticalSection(&m_cs);
}

void CGridViewManager::GetGridViewList(list<VIEWMANAGER>& ViewList)
{
	ViewList.clear();

	EnterCriticalSection(&m_cs);
	ViewList.assign(m_GridViewList.begin(),m_GridViewList.end());
	LeaveCriticalSection(&m_cs);
}

int CGridViewManager::GetGridViewCount()const
{
	return static_cast<int>(m_GridViewList.size());
}

void CGridViewManager::RefreshAll(CCustomLabGridCtrl* pGrid)
{
	EnterCriticalSection(&m_cs);

	list<VIEWMANAGER>::iterator Iter = m_GridViewList.begin();
	while (Iter != m_GridViewList.end())
	{
		if(Iter->pGrid != pGrid)
			Iter->pGrid->Refresh();
		++Iter;
	}

	LeaveCriticalSection(&m_cs);
}

void CGridViewManager::Update(CObservable* pObs, void* pArg)
{
	int nParam = (int)pArg;
	EnterCriticalSection(&m_cs);

	//取得行数
	int nMaxRow = GLOBAL_DATA.GetMaxRow();

	list<VIEWMANAGER>::iterator Iter = m_GridViewList.begin();
	while (Iter != m_GridViewList.end())
	{
		if(nParam == 1)
		{
			Iter->pGrid->UpdataGrid();
		}

		if(nMaxRow > 100)
		{
			Iter->pGrid->SetVirtualRows(nMaxRow);

			Iter->pGrid->AdjustLayout ();
		}

		Iter->pGrid->Refresh();
		++Iter;
	}

	LeaveCriticalSection(&m_cs);
}