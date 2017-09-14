/****************************************************************************************
*版权所有:
*文件名:Observable.cpp
*描述:观察者模式--被观察者.同时也作为全局数据结构的操作类
*日期:2016.9
******************************************************************************************/
#include "stdafx.h"
#include "Observable.h"

#include "Observer.h"
#include "Global.h"
//#include "GlobalDataManager.h"
/*************************************************************************************************
函数名称:CObservable
功能描述:构造函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CObservable::CObservable(void)
{
	m_bChanged = false;

	m_nLabCount = 0;
}

/*************************************************************************************************
函数名称:~CObservable
功能描述:析构函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CObservable::~CObservable(void)
{
	m_setObs.clear();
}

/*************************************************************************************************
函数名称:Attach
功能描述:注册观察者
输入参数:pObs:观察者指针
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::Attach(CObserver* pObs)
{
	//判断参数有效性
    if (NULL == pObs)
	{
		return;
	}
	
	//加入观察者列表
    m_setObs.insert(pObs);
}

/*************************************************************************************************
函数名称:Detach
功能描述:注销观察者
输入参数:pObs:观察者指针
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::Detach(CObserver* pObs)
{
	//判断参数有效性
    if (NULL == pObs)
	{
		return;
	}

	//从观察者列表中移除
    m_setObs.erase(pObs);
}

/*************************************************************************************************
函数名称:DetachAll
功能描述:注销所有观察者
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::DetachAll()
{
	//清空观察者列表
    m_setObs.clear();
}

/*************************************************************************************************
函数名称:SetChanged
功能描述:设置状态变化
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::SetChanged()
{
    m_bChanged = true;
}

/*************************************************************************************************
函数名称:ClearChanged
功能描述:初始化目标为未变化状态
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::ClearChanged()
{
    m_bChanged = false;
}

/*************************************************************************************************
函数名称:HasChanged
功能描述:状态是否变化
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
bool CObservable::HasChanged()
{
    return m_bChanged;
}

/*************************************************************************************************
函数名称:GetObserversCount
功能描述:获取观察者数量
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
int CObservable::GetObserversCount()
{
    return static_cast<int>(m_setObs.size());
}

/*************************************************************************************************
函数名称:Notify
功能描述:通知更新
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::Notify(void* pArg /* = NULL */)
{
    if (!HasChanged())
	{
		return;
	}
    
    ClearChanged();
 
    set<CObserver*>::iterator itr = m_setObs.begin();
    for (; itr != m_setObs.end(); itr++)
    {
        (*itr)->Update(this, pArg);
    }
}

/*************************************************************************************************
函数名称:GetCurrLabTotal
功能描述:获取当前实验总个数
输入参数:无
输出参数:无
返回值:实验总个数
***************************************************************************************************/
int CObservable::GetCurrLabTotal()const
{
	return m_nLabCount;
}

/*************************************************************************************************
函数名称:AddLab
功能描述:新增实验
输入参数:nLabIndex:新增实验索引
输出参数:无
返回值:true:成功/false:失败
***************************************************************************************************/
bool CObservable::AddLab(int& nLabIndex)
{
	if(m_nLabCount >= LAB_MAX_COUNT)
	{
		return false;
	}

	m_nLabCount++;

	//新增实验数据
	//EnterCriticalSection(m_cs);

	//g_LabData

	//LeaveCriticalSection(m_cs);

	return true;
}

/*************************************************************************************************
函数名称:DeleteLab
功能描述:删除实验
输入参数:实验索引
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::DeleteLab(int nLabIndex)
{
	//判断参数的有效性
	if(nLabIndex<=0 || nLabIndex>m_nLabCount)
	{
		return;
	}

	//清除实验数据
	//EnterCriticalSection(m_cs);

	//清除数据
	//g_LabData[nLabIndex].clear();
	//清除索引
	//g_LabDataIndex[nLabIndex].clear();

	//LeaveCriticalSection(m_cs);
}

/*************************************************************************************************
函数名称:PushData
功能描述:添加数据
输入参数:nLab:实验号  nSensorID:传感器ID dValue: 添加的数据值
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::PushData(int nLabIndex ,int nSensorID ,double dValue)
{
	//判断参数的有效性
	if(nLabIndex<=0 || nLabIndex>m_nLabCount)
	{
		return;
	}

	//传感器ID(1~70)
	if(nSensorID<=0 || nSensorID>SENSOR_MAX_ID)
	{
		return;
	}

	//添加传感器接收的数据
	//EnterCriticalSection(m_cs);

	//查找传感器ID所对应的数据列索引
	int nColIndex = 0;
	if(FindColIndex(nLabIndex, nSensorID,nColIndex))
	{
		//已存在
		//g_LabDataIndex[nLabIndex][g_LabDataRowCount[nLabIndex]];
	}
	else
	{
		//不存在，插入列，并适配数据
		CreateColumnData(nLabIndex,nSensorID);


		//先适配再插入数据
		AdapterLabData(nLabIndex);

	}
	

	//LeaveCriticalSection(m_cs);

}

/*************************************************************************************************
函数名称:CreateColumnData
功能描述:插入列
输入参数:nLabIndex:实验号 nSensorID:传感器ID
输出参数:无
返回值:无
***************************************************************************************************/
void CObservable::CreateColumnData(int nLabIndex ,int nSensorID)
{
	//添加列数据
	VEC_STRING vecNewData;
	//g_LabData[nLabIndex].push_back(vecNewData);

	//PAIRINDEX newIndex;

	////更新索引
	//newIndex.nColIndex = g_LabData[nLabIndex].size();
	//newIndex.nID = nSensorID;

	////添加索引
	//g_LabDataIndex.push_back(newIndex);
}

/*************************************************************************************************
函数名称:FindColIndex
功能描述:查找传感器ID所对应的数据列索引
输入参数:nLab:实验号 nSensorID:传感器ID nCol:数据列索引
输出参数:无
返回值:true:已存在此传感器/false:不存在
***************************************************************************************************/
bool CObservable::FindColIndex(int nLab ,int nSensorID ,int& nCol)
{
	//for ( auto iter=g_LabDataIndex[nLab].begin(); iter!=g_LabDataIndex[nLab].end(); ++iter)
	//{
	//	if(iter->nID == nSensorID)
	//	{
	//		nCol = iter->nColIndex;
	//		return true;
	//	}
	//}

	//nCol = ++(g_LabDataIndex[nLab].size());

	return false;
}

/*************************************************************************************************
函数名称:AdapterLabData
功能描述:实验数据适配，使数据量统一
输入参数:nLab:实验号
输出参数:无
返回值:
***************************************************************************************************/
void CObservable::AdapterLabData(int nLab)
{
	//
	VEC_STRING vecNewData;
	//g_LabData[nLabIndex].push_back(vecNewData);

	//

}
