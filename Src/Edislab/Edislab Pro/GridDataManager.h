/*******************************************************************************/
/*描述:Grid数据管理类
/*作者:
/*日期:2015.8
/*******************************************************************************/

#ifndef GRID_DATA_MANAGER_H
#define GRID_DATA_MANAGER_H

#include <vector>
using std::vector;
#include "GridDefine.h"

class CGridDataManager
{
public:
	CGridDataManager(void);
	virtual ~CGridDataManager(void);

public:

	void InitGroupInfo();

	void SetGroupInfo(GROUPDATA ColData);

	void InitColumnInfo();

	void SetColumnInfo(int nColIndex,COLUMNDATA ColData);

	//获得Grid Item数据
	void GetGridItemData(int nRow,int nColumn,_variant_t& ItemValue);

	void AddColumnInfo();

	GROUPDATA& GetGridGroupInfo();

	vector <COLUMNDATA>& GetGridColummnInfo();

	vector<vector<CString>>& GetGridData();

private:
	//Grid行列数据（不含头）
	vector<vector<CString>> m_vecGridData;

/**********************************************************************/
//目前支持单一组

	//Grid组数据信息
	GROUPDATA	m_GridGroupInfo;
	//Grid列数据信息（不含Item value数值）
	vector <COLUMNDATA> m_vecGridColumnInfo;
/**********************************************************************/
};

#endif