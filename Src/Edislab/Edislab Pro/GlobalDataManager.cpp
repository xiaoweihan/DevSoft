/****************************************************************************************
*版权所有:
*文件名:GlobalDataManager.cpp
*描述:全局数据管理类
*日期:2016.9
******************************************************************************************/
#include "stdafx.h"
#include "GlobalDataManager.h"
#pragma warning(push)
#pragma warning(disable:4018)
/*************************************************************************************************
函数名称:CreateInstance
功能描述:串口通信类实例化
输入参数:无
输出参数:无
返回值:实例
***************************************************************************************************/
CGlobalDataManager& CGlobalDataManager::CreateInstance()
{
	static CGlobalDataManager s_Manager;
	return s_Manager;
}

/*************************************************************************************************
函数名称:CGlobalDataManager
功能描述:构造函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CGlobalDataManager::CGlobalDataManager(void)
{
	m_bHaveData = false;

	//初始化数据容器
	Init();
}

/*************************************************************************************************
函数名称:~CGlobalDataManager
功能描述:串口通信类构造函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CGlobalDataManager::~CGlobalDataManager(void)
{
}

/*************************************************************************************************
函数名称:IsHaveData
功能描述:是否有数据
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
bool CGlobalDataManager::IsHaveData()
{
	return m_bHaveData;
}

/*************************************************************************************************
函数名称:SetHaveDataStatus
功能描述:设置为有数据状态
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
void CGlobalDataManager::SetHaveDataStatus()
{
	m_bHaveData = true;
}

/*************************************************************************************************
函数名称:SetNotHaveDataStatus
功能描述:设置成无数据状态
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
void CGlobalDataManager::SetNotHaveDataStatus()
{
	m_bHaveData = false;
}

/*************************************************************************************************
函数名称:IsHaveSensorCol
功能描述:当前组是否有传感器数据列
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
bool CGlobalDataManager::IsHaveSensorCol()
{
	if(!m_allData.size())
	{
		return false;
	}
	else
	{
		//遍历所有的组中的列
		for(int i=0; i<m_allData.size(); ++i)
		{
			for(int c=0; c<m_allData[i].vecColData.size(); ++c)
			{
				//有传感器
				if(TYPE_SENSOR==m_allData[i].vecColData[c].emType)
				{
					return true;
				}
			}
		}
		/*for (int i = 0;i < m_allData[0].vecColData.size(); ++i)
		{
		if( g_LabDataAttribution[0].vecColData[i].nColumnID > 0 
		&& g_LabDataAttribution[0].vecColData[i].nColumnID <= SENSOR_MAX_ID)
		{
		return true;
		}
		}*/
		return false;
	}
}

/*************************************************************************************************
函数名称:FindColumnID
功能描述:通过通到号找数据列ID
输入参数:nSensorChan:传感器通道
输出参数:无
返回值:数据列ID
***************************************************************************************************/
int CGlobalDataManager::FindTimeID()
{
	m_cs.Lock();
	int nResult = -1;
	if(m_allData.size())
	{
		//传感器数据更新只可能在当前组
		for(unsigned int i  = 0; i < m_allData.size();++i )
		{
			if(m_allData[i].bDefault)//当前组
			{
				for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
				{
					if(m_allData[i].vecColData[j].emType == TYPE_TIME)
					{
						nResult =  m_allData[i].vecColData[j].nColumnID;
						break;
					}
				}
			}
		}
	}
	m_cs.UnLock();

	return nResult;
}

/*************************************************************************************************
函数名称:Init
功能描述:初始化数据容器
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CGlobalDataManager::Init()
{
	//默认状态下有 X、Y两列，100行。但无具体数据
	m_cs.Lock();
	//行列数据归属/属性
	GROUPDATA _group;
	_group.bDefault = true;
	_group.nGroupID = IDManager::CreateInstance()->generateID();
	//行列数据
	for (int iCol = 0; iCol < DEFAULT_COL_COUNT; ++iCol)
	{
		COLUMNDATA _column;
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		//for (int iRow = 0 ; iRow < DEFAULT_ROW_COUNT ; ++iRow)
		//{
		//	_column.data.push_back(DEFAULT_DATA);
		//}

		if(!iCol)
		{
			_column.strColumnName = DEFUALT_COLUMN_NAME_X;
		}
		else
		{
			_column.strColumnName = DEFUALT_COLUMN_NAME_Y;
		}

		_group.vecColData.push_back(_column);
	}
	m_allData.push_back(_group);
	m_cs.UnLock();
}

/*************************************************************************************************
函数名称:AppendGroup
功能描述:添加组
输入参数:无
输出参数:无
返回值:无
***************************************************************************************************/
void CGlobalDataManager::AppendGroup()
{
	m_cs.Lock();
	//新组
	GROUPDATA _group;
	//复制当前组行列属性
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			_group = m_allData[i];
			break;
		}
	}
	//修改部分信息
	_group.bDefault = false;
	_group.nGroupID = IDManager::CreateInstance()->generateID();
	_group.strGroupName = NEW_GROUP;
	//修改列数据
	for(int i=0; i<_group.vecColData.size(); ++i)
	{
		_group.vecColData[i].nColumnID = IDManager::CreateInstance()->generateID();
		_group.vecColData[i].nSensorID = -1;
		_group.vecColData[i].nSensorChanel = -1;
		//可编辑
		if(COLUMN_EDIT_FORBID==_group.vecColData[i].emEdit)
		{
			_group.vecColData[i].emEdit = COLUMN_EDIT_ONLY_NUM;
		}
		//类型
		if(TYPE_SENSOR==_group.vecColData[i].emType||TYPE_TIME==_group.vecColData[i].emType)
		{
			_group.vecColData[i].emType = TYPE_HANDWORK;
		}
		//颜色
	}
	m_cs.UnLock();
}

//添加传感器数据列
void CGlobalDataManager::AppendColumn(int nSensorID,int nSensorChanel)
{
	m_cs.Lock();
	//取得当前组
	if(m_allData.size())
	{
		COLUMNDATA _column;
	
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = _T("F(kg)\n力");
		_column.nSensorID = nSensorID;
		_column.nSensorChanel = nSensorChanel;
		_column.emType = TYPE_SENSOR;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}

//begin add by xiaowei.han 2017-3-25
//添加传感器数据列
void CGlobalDataManager::AppendColumn(int nSensorID,int nSensorChanel,const std::string& strUnit)
{
	m_cs.Lock();
	//取得当前组
	if(m_allData.size())
	{
		COLUMNDATA _column;

		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = CString(strUnit.c_str());
		_column.nSensorID = nSensorID;
		_column.nSensorChanel = nSensorChanel;
		_column.emType = TYPE_SENSOR;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}
//end add by xiaowei.han 2017-3-25

//添加时间数据列
void CGlobalDataManager::AppendTimeColumn()
{
	m_cs.Lock();
	//取得当前组
	if(m_allData.size())
	{
		COLUMNDATA _column;
	
		_column.nColumnID = IDManager::CreateInstance()->generateID();
		_column.strColumnName = _T("t(s)\n时间");
		_column.emType = TYPE_TIME;
		_column.emEdit = COLUMN_EDIT_FORBID;

		m_allData[0].vecColData.push_back(_column);
	}

	m_cs.UnLock();
}

//添加数据 一列中插一行
void CGlobalDataManager::PushData(int nColID ,int nRowPos)
{
	m_cs.Lock();
	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//因为插入的是空白数据,所以数据行数不足的可以不插入
				if(nRowPos<m_allData[i].vecColData.size())
				{
					VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
					//在指定行前插入
					m_allData[i].vecColData[c].data.insert(itrr+nRowPos, DEFAULT_DATA);
				}
				////数据不足,中间填空
				//while(m_allData[i].vecColData[c].data.size()<nRowPos)
				//{
				//	m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				//}
				//m_allData[i].vecColData[c].data[nRowPos] = szValue;
				//VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
				////在指定行前插入
				//m_allData[i].vecColData[c].data.insert(itrr+nRowPos, szValue);
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}

//通过通道号插入数据
void CGlobalDataManager::PushData(int nSensorChan,short nValue)
{
	m_cs.Lock();

	//通过通到号找数据列ID
	//int nSensorID = FindColumnID(nSensorChan);

	//if(nSensorID == -1)
	//	return;

	CString strValue;
	strValue.Format(_T("%.2f"),(float)nValue/100.);

	int nSensorID;

	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
			{
				if(m_allData[i].vecColData[j].nSensorChanel == nSensorChan)
				{
					nSensorID =  m_allData[i].vecColData[j].nColumnID;

					m_allData[i].vecColData[j].data.push_back(strValue);

					break;
				}
			}
		}
	}
	m_cs.UnLock();
}

//begin add by xiaowei.han 2017-3-25
void CGlobalDataManager::PushData(int nSensorChan,float fValue)
{
	m_cs.Lock();

	//通过通到号找数据列ID
	//int nSensorID = FindColumnID(nSensorChan);

	//if(nSensorID == -1)
	//	return;

	CString strValue;
	strValue.Format(_T("%.2f"),fValue);

	int nSensorID;

	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		if(m_allData[i].bDefault)
		{
			for(unsigned int j  = 0; j < m_allData[i].vecColData.size();++j )
			{
				if(m_allData[i].vecColData[j].nSensorChanel == nSensorChan)
				{
					nSensorID =  m_allData[i].vecColData[j].nColumnID;

					m_allData[i].vecColData[j].data.push_back(strValue);

					break;
				}
			}
		}
	}
	m_cs.UnLock();
}
//end add by xiaowei.han 2017-3-25

//一列添加多行 没有此功能 一次只能增加一行
//void CGlobalDataManager::PushData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue)
//{
//}
//所有列添加一行
void CGlobalDataManager::PushData(int nRowPos)
{
	m_cs.Lock();
	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			//因为插入的是空白数据,所以数据行数不足的可以不插入
			if(nRowPos<m_allData[i].vecColData.size())
			{
				VEC_STRING::iterator itrr = m_allData[i].vecColData[c].data.begin();
				//在指定行前插入
				m_allData[i].vecColData[c].data.insert(itrr+nRowPos, DEFAULT_DATA);
			}
		}
	}
	m_cs.UnLock();
}

//修改数据 修改单个单元格数据
void CGlobalDataManager::ModifyData(int nColID ,int nRowPos ,CString szValue)
{
	m_cs.Lock();
	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//数据不足,中间填空
				while(m_allData[i].vecColData[c].data.size()<=nRowPos)
				{
					m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				}
				m_allData[i].vecColData[c].data[nRowPos] = szValue;
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//修改一列中的多行数据
void CGlobalDataManager::ModifyData(int nColID ,int nStartRowPos ,int nEndRowPos ,VEC_STRING vecValue)
{
	m_cs.Lock();
	//遍历所有的组中的列 找到ID一样的列
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bMod = false;
		for(int c=0; c<m_allData[i].vecColData.size(); ++c)
		{
			if(nColID == m_allData[i].vecColData[c].nColumnID)
			{
				//数据不足,中间填空
				while(m_allData[i].vecColData[c].data.size()<=nEndRowPos)
				{
					m_allData[i].vecColData[c].data.push_back(DEFAULT_DATA);
				}
				for(int v=0; v<vecValue.size(); ++v)
				{
					int index = nStartRowPos+v;
					if(index<=nEndRowPos)
					{
						m_allData[i].vecColData[c].data[index] = vecValue[v];
					}
					else
					{
						break;
					}
				}
				bMod = true;
				break;
			}
		}
		if(bMod)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//删除数据
void CGlobalDataManager::PopAllData()
{
	m_cs.Lock();
	//遍历删除所有数据组
	VECTOR<GROUPDATA>::iterator itr = m_allData.begin();
	while(itr!=m_allData.end())
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin(); 
		while(itrc!=itr->vecColData.end())
		{
			int id = itrc->nColumnID;
			itr->vecColData.erase(itrc);
			//删除ID
			IDManager::CreateInstance()->delID(id);
		}
		//删除组
		int id = itr->nGroupID;
		m_allData.erase(itr);
		IDManager::CreateInstance()->delID(id);
	}
}
//删除一列
void CGlobalDataManager::PopData(int nColID)
{
	m_cs.Lock();
	for(int i=0; i<m_allData.size(); ++i)
	{
		bool bDel = false;
		for(VECTOR<COLUMNDATA>::iterator itr = m_allData[i].vecColData.begin(); 
			itr!=m_allData[i].vecColData.end(); ++itr)
		{
			if(nColID==itr->nColumnID)
			{
				//删除列
				m_allData[i].vecColData.erase(itr);
				//删除ID
				IDManager::CreateInstance()->delID(nColID);
				bDel = true;
				break;
			}
		}
		if(bDel)
		{
			break;
		}
	}
	m_cs.UnLock();
}
//删除一组
void CGlobalDataManager::PopGroupData(const int nGroupID)
{
	m_cs.Lock();
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		if(nGroupID==itr->nGroupID)
		{
			VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin(); 
			while(itrc!=itr->vecColData.end())
			{
				int id = itrc->nColumnID;
				itr->vecColData.erase(itrc);
				//删除ID
				IDManager::CreateInstance()->delID(id);
			}
			//删除组
			m_allData.erase(itr);
			IDManager::CreateInstance()->delID(nGroupID);
			break;
		}
	}
}
//删除一个数据
void CGlobalDataManager::PopData(int nColID ,int nRowPos)
{
	m_cs.Lock();
	bool bFind = false;
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin();
		//遍历列
		while(itrc!=itr->vecColData.end())
		{
			//找到列
			if(nColID==itrc->nColumnID)
			{
				VEC_STRING::iterator itrr = itrc->data.begin();
				int row = 0;
				while(itrr!=itrc->data.end())
				{
					//找到行
					if(row==nRowPos)
					{
						itrc->data.erase(itrr);
						bFind = true;
						break;
					}
					++row;
				}
				break;
			}
		}
		if(bFind)
		{
			break;
		}
	}
	m_cs.UnLock();
}
void CGlobalDataManager::PopData(int nColID ,int nStartRowPos ,int nEndRowPos)
{
	m_cs.Lock();
	bool bFind = false;
	for(VECTOR<GROUPDATA>::iterator itr = m_allData.begin(); itr!=m_allData.end(); ++itr)
	{
		VECTOR<COLUMNDATA>::iterator itrc = itr->vecColData.begin();
		//遍历列
		while(itrc!=itr->vecColData.end())
		{
			//找到列
			if(nColID==itrc->nColumnID)
			{
				VEC_STRING::iterator itrr = itrc->data.begin();
				int row = 0;
				while(itrr!=itrc->data.end())
				{
					//找到行
					if(row>=nStartRowPos && row<=nEndRowPos)
					{
						itrc->data.erase(itrr);
						bFind = true;
					}
					++row;
					if(row>nEndRowPos)
					{
						break;
					}
				}
				break;
			}
		}
		if(bFind)
		{
			break;
		}
	}
	m_cs.UnLock();
}

//删除数据容器
void CGlobalDataManager::DeleteAllContainer()
{
}
void CGlobalDataManager::DeleteContainer(int nColID)
{
}

//取得每个Item数据
void CGlobalDataManager::GetItemData(int nCol,int nRow,CString& strValue)
{
	m_cs.Lock();

	strValue.Empty();

	int nGroupCount = static_cast<int>(m_allData.size());

	int nFind = 0;
	int nColumnID = 0;

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		int nColumnCount = static_cast<int>(m_allData[nGroup].vecColData.size());
		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			if(nFind == nCol)
			{
				nColumnID = m_allData[nGroup].vecColData[nColumn].nColumnID;

				if(m_allData[nGroup].vecColData[nColumn].data.size() > nRow)
				{
					strValue = m_allData[nGroup].vecColData[nColumn].data[nRow];
				}

				break;
			}
			nFind++;
		}
	}

	m_cs.UnLock();
}

//取得最大行数
int CGlobalDataManager::GetMaxRow()
{
	m_cs.Lock();

	int nMaxRow = 0;

	int nGroupCount = static_cast<int>(m_allData.size());

	for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
	{
		int nColumnCount = static_cast<int>(m_allData[nGroup].vecColData.size());
		for (int nCol = 0; nCol < nColumnCount; nCol++)
		{
			int nSize = static_cast<int>(m_allData[nGroup].vecColData[nCol].data.size());
			nMaxRow = max(nMaxRow,nSize);
		}
	}

	m_cs.UnLock();

	return nMaxRow;
}
#pragma warning(pop)
