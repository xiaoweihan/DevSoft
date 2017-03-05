#include "stdafx.h"
#include "GridDataManager.h"


CGridDataManager::CGridDataManager(void)
{
	m_vecGridColumnInfo.clear();
}


CGridDataManager::~CGridDataManager(void)
{
}

void CGridDataManager::GetGridItemData(int nRow,int nColumn,_variant_t& ItemValue)
{
	ItemValue = (LPCTSTR)m_vecGridData[nRow][nColumn];
}

void CGridDataManager::InitGroupInfo()
{
	m_GridGroupInfo.bDefault = true;
}

void CGridDataManager::InitColumnInfo()
{
	COLUMNDATA X_Data;
	X_Data.strColumnName = DEFUALT_COLUMN_NAME_X;
	m_vecGridColumnInfo.push_back(X_Data);

	COLUMNDATA Y_Data;
	Y_Data.strColumnName = DEFUALT_COLUMN_NAME_Y;
	m_vecGridColumnInfo.push_back(Y_Data);

	for(int i = 0; i < 2; ++i)
	{
		vector<CString> vec;
		for(int j = 0; j < INIT_ROW_NUM; ++j)
		{
			CString strTemp;
			strTemp.Empty();
			vec.push_back(strTemp);
		}
		m_vecGridData.push_back(vec);
	}
}

void CGridDataManager::SetGroupInfo(GROUPDATA GroupData)
{
	m_GridGroupInfo.strGroupName = GroupData.strGroupName;
	m_GridGroupInfo.strGroupName = GroupData.strGroupName;
	m_GridGroupInfo.sysAddTime = GroupData.sysAddTime;
	m_GridGroupInfo.bDefault = GroupData.bDefault;
}

void CGridDataManager::SetColumnInfo(int nColIndex,COLUMNDATA ColData)
{
	int nCount = m_vecGridColumnInfo.size();
	if(nColIndex < 0||nColIndex >= nCount)
		return;

	m_vecGridColumnInfo[nColIndex].strColumnName = ColData.strColumnName;
	m_vecGridColumnInfo[nColIndex].strColumnUnit = ColData.strColumnUnit;
	m_vecGridColumnInfo[nColIndex].nColColor = ColData.nColColor;
	m_vecGridColumnInfo[nColIndex].emPointSize = ColData.emPointSize;
	m_vecGridColumnInfo[nColIndex].emType = ColData.emType;
	m_vecGridColumnInfo[nColIndex].emDecimalPoint = ColData.emDecimalPoint;
	m_vecGridColumnInfo[nColIndex].emEdit = ColData.emEdit;
}

GROUPDATA &CGridDataManager::GetGridGroupInfo()
{
	return m_GridGroupInfo;
}

vector <COLUMNDATA>& CGridDataManager::GetGridColummnInfo()
{
	return m_vecGridColumnInfo;
}

vector<vector<CString>>& CGridDataManager::GetGridData()
{
	return m_vecGridData;
}