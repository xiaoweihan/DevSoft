/***********************************************************************************/
/*描述:Grid控件工厂(多视图Grid组合)
/*作者:
/*日期:2015.8
/************************************************************************************/
#ifndef GRID_CTRL_FACTORY_H
#define GRID_CTRL_FACTORY_H

#include <windef.h>
#include "GridViewManager.h"
#include "GridDataManager.h"

class CGridControlFactory
{
public:
	CGridControlFactory(void);
	virtual ~CGridControlFactory(void);

public:
	//为窗口添加Grid控件
	CWnd* AddGridCtrl(CWnd* _Parent);
	//移除窗口Grid控件
	void RemoveGridCtrl(CWnd* pWnd);

	//添加实验组（复制当前组）
	//void AddLabGroup();
	//添加实验列（栏/新栏）
	void AddLabColumn();

private:
	//初始化Grid信息
	void InitGridInfo();
	//复制Grid信息
	void CopyGridInfo();

private:

	CGridViewManager*	m_ViewManager;
	CGridDataManager*	m_DataManager;
};

#endif