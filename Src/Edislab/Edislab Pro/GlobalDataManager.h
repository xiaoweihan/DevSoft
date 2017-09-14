/****************************************************************************************
*版权所有:
*文件名:GlobalDataManager.h
*描述:全局数据管理类
*日期:2016.9
******************************************************************************************/

#ifndef GLOBAL_DATA_MANAGER_H
#define GLOBAL_DATA_MANAGER_H

#include "Observable.h"
#include "Global.h"
#include "Lock.h"

#define GLOBAL_DATA		CGlobalDataManager::CreateInstance()
//全局数据管理类
class CGlobalDataManager :public CObservable
{
public:
	static CGlobalDataManager& CreateInstance();
private:
	CGlobalDataManager(void);
	virtual ~CGlobalDataManager(void);

public:
	//数据操作方法

	//初始化数据
	void Init();

	//添加组
	void AppendGroup();

	//添加传感器数据列
	void AppendColumn(int nSensorID,int nSensorChanel);
	void AppendColumn(int nSensorID,int nSensorChanel,const std::string& strUnit);
	//添加时间数据列
	void AppendTimeColumn();

	//添加数据
	void PushData(int nColID ,int nRowPos);
	void PushData(int nSensorChan,short nValue);
	//void PushData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue);
	void PushData(int nRowPos );
	void PushData(int nSensorChan,float fValue);
	//修改数据
	void ModifyData(int nColID ,int nRowPos ,CString szValue);
	void ModifyData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue);
	//删除数据
	void PopAllData();
	void PopGroupData(const int nGroupID);
	void PopData(int nColID);
	void PopData(int nColID ,int nRowPos);
	void PopData(int nColID ,int nStartRowPos ,int nEndRowPos);

	//删除数据容器
	void DeleteAllContainer();
	void DeleteContainer(int nColID);

	//取得最大行数
	int GetMaxRow();

	//取得每个Item数据
	void GetItemData(int nCol,int nRow,CString& strValue);

	VECTOR<GROUPDATA> getAllData(){ return m_allData;}

	//通过通到号找数据列ID
	int FindTimeID();

private:
	//是否有数据
	bool IsHaveData();
	//设置为有数据状态
	void SetHaveDataStatus();
	//设置成无数据状态
	void SetNotHaveDataStatus();

	//当前组是否有传感器数据列
	bool IsHaveSensorCol();

private:
	//保护全局数据的临界区
	CLock m_cs;

	bool m_bHaveData;
	//全局数据
	VECTOR<GROUPDATA> m_allData; //每列存的数据量可能不一样
};

#endif
