/****************************************************************************************
*版权所有:
*文件名:Observable.h
*描述:观察者模式--被观察者.同时也作为全局数据的操作类的基类
*日期:2016.9
******************************************************************************************/
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <afx.h>
#include <set>
using std::set;

//定义观察者
class CObserver;

//被观察者类
class CObservable
{
public:
	CObservable(void);
	virtual ~CObservable(void);

public:
	
	//注册观察者
	void Attach(CObserver* pObs);
	//注销观察者
	void Detach(CObserver* pObs);
	//注销所有观察者
	void DetachAll();
	//若状态变化，则遍历观察者，逐个通知更新
	void Notify(void* pArg = NULL);
	//测试目标状态是否变化
	bool HasChanged();
	//获取观察者数量
	int GetObserversCount();

public:
/************************全局数据操作**********************************************/
/*****数据操作有两种：	1： 传感器（接收/删除） 2： 用户Grid编辑（增、删、改）*******/

/***********************************传感器*******************************/
	//添加传感器接收到的数据
	void PushData(int nLab ,int nSensorID ,double dValue);
	void PushData(int nLab ,int nSensorID ,int dValue);
	void PushData(int nLab ,int nSensorID ,CString szValue);
	//删除传感器数据
	void PopData(int nLab ,int nSensorID);

/*********************************用户Grid编辑*******************************/
	//插入行数据
	void CreateRowData(int nLabIndex ,int nRowIndex );
	//插入列
	void CreateColumnData(int nLabIndex ,int nSensorID );
	//插入/更改某行列对应的Item值
	void InsertItemData(int nLabIndex ,int nRow ,int nCol ,CString szValue );
	//删除行数据
	void DeleteRowData(int nLabIndex ,int nRowIndex );
	//删除列数据
	void DeleteColumnData(int nLabIndex ,int nColIndex );

/*********************************实验相关**********************************/
	//获取当前实验个数
	int GetCurrLabTotal()const;
	//新增实验
	bool AddLab(int& nLabIndex);
	//删除实验
	void DeleteLab(int nLabIndex);

/***********************************操作全局数据通用函数**********************/
	
 
public:
 
	//设置状态变化!!!必须继承CObservable才能设置目标状态
	void SetChanged();
	//初始化目标为未变化状态
	void ClearChanged();

private:

	//状态
	bool m_bChanged;
	//set保证目标唯一性
	set<CObserver*> m_setObs;

	//实验个数
	int	m_nLabCount;

private:

	//查找传感器ID所对应的数据列索引
	bool FindColIndex(int nLab ,int nSensorID ,int& nCol);

	void AdapterLabData(int nLab );
};
#endif