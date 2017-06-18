/**********************************************************
*版权所有:xiaowei.han
*文件名称:GridDisplayColumnInfo.h
*功能描述:Grid显示列信息数据管理器
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#pragma once
#include "GridGroupType.h"

class CGridColumnGroupManager
{
public:
	static CGridColumnGroupManager& CreateInstance();

	void GetGridDisplayInfo(std::vector<COLUMN_GROUP_INFO>& DisplayInfoArray);

	//查询表头名称是否已经存在
	bool IsHeaderNameExist(const CString& strHeaderName);

	//判断指定表头列名是否已经存在
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//添加列信息
	void AddDisplayColumnInfo(const COLUMN_GROUP_INFO& GridColumnInfo);

	//在某个头中添加一列信息
	void AddDisplayColumnInfo(const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo);

    //移除某一列
	void RemoveColumnInfo(const CString& strHeaderName,const CString& strColumnName);
	
	//移除某一表头
	void RemoveHeader(const CString& strHeaderName);

	//修改组名
	void ModifyHeaderInfo(const CString& strOldName,const CString& strNewName);
protected:
	CGridColumnGroupManager(void);
	~CGridColumnGroupManager(void);
private:
	void InitGridDisplayInfo();
protected:
	//Grid的分组显示信息
	std::vector<COLUMN_GROUP_INFO> m_HeaderInfoArray;
private:
	static CGridColumnGroupManager s_obj;

};

