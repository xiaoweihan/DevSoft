/**********************************************************
*版权所有:xiaowei.han
*文件名称:CustomGrid.h
*功能描述:自定义的Grid控件
*作者:xiaowei.han
*日期:2017/04/22
**********************************************************/
#ifndef CUSTOM_GRID_H
#define CUSTOM_GRID_H
#include <vector>
#include <vector>
//定义列的属性
typedef struct _header_info
{
	//列的名称
	CString strHeadName;

	//包含列的索引
	std::vector<CString> ContainColumnIndexArray;

	_header_info()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}
	void Reset(void)
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

}HEADRER_INFO,* LP_HEADRER_INFO;

class CCustomGrid : public CBCGPGridCtrl
{
public:
	CCustomGrid();
	virtual ~CCustomGrid();
	DECLARE_DYNCREATE(CCustomGrid)

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnTrackHeader ()
	{
		return FALSE; // do not resize columns inside client area by mouse, only at header
	}

	// Merged header items:
public:
	virtual void OnPosSizeChanged();

protected:
	virtual CRect OnGetHeaderRect (CDC* pDC, const CRect& rectDraw);
	virtual void OnDrawHeader (CDC* pDC);
	virtual void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo);
public:
	virtual CBCGPGridColumnsInfo& GetColumnsInfo();
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const;
	void DrawHeaderItem (CDC* pDC, CRect rect, CBCGPHeaderItem* pHeaderItem);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	// other header implementation
	CBCGPGridColumnsInfoEx m_ColumnsEx;	
public:
	//创建的grid的ID
	static unsigned int s_GridID;

public:
	//设置行头信息(在创建Grid之前调用)
	void SetHeaderInfoArray(const std::vector<HEADRER_INFO>& HeaderInfoArray);

	//添加行头信息
	void AddHeaderInfo(const HEADRER_INFO& HeaderInfo);

	//删除行头信息
	void RemoveHeaderInfo(const CString& strHeaderName);

	//删除某一列
	void RemoveColumn(const CString& strColumnName);

	//增加某一列
	void AddColumnInfo(const CString& strHeaderName,const CString& strColumnName);

	//查询表头名称是否已经存在
	bool IsHeaderNameExist(const CString& strHeaderName);

	//判断指定表头列名是否已经存在
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//获取表头信息
	void GetHeaderInfo(std::vector<HEADRER_INFO>& HeaderInfoArray);

	//填充数据
	void FillData(void);
private:
	//创建列信息
	void CreateHeaderInfo(void);
	//创建列信息
	void CreateColumnInfo(void);
protected:
	std::vector<HEADRER_INFO> m_HeaderInfoArray;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif