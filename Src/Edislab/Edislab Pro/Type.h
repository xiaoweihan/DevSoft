#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include "Macro.h"
class CSplitBar;
class CEdislabProView;
typedef std::vector<CWnd*> WidgetArray;
typedef std::vector<CWnd*>::iterator WidgetArrayIter;
typedef std::vector<CSplitBar*> SplitBarArray;
typedef std::vector<CSplitBar*>::iterator SplitBarArrayIter;

//分隔条的交点信息
typedef struct _split_intersect_rect
{
	CRect InterSectRect;

	//由哪2个矩形相交
	//水平分隔条
	CRect* pRect1;
	//垂直分隔条
	CRect* pRect2;
	//垂直分隔条的索引
	int nVerIndex;
	//水平分隔条的索引
	int nHorIndex;

	_split_intersect_rect(void)
	{
		pRect1 = NULL;
		pRect2 = NULL;
		nVerIndex = -1;
		nHorIndex = -1;
	}

}SPLIT_INTERSECT_RECT,* LP_SPLIT_INTERSECT_RECT;

//鼠标状态
enum MOUSE_STATE
{
	MOVE_NO_STATE = 0,
	//水平移动
	MOVE_HOR,
	//垂直移动
	MOVE_VER,
	//水平和垂直移动
	MOVE_ALL

};

//记录移动分隔条的信息
typedef struct _split_bar_info
{
	//分隔条类型
	MOUSE_STATE Type;
	//占用的索引
	unsigned int nIndex;

	_split_bar_info(void)
	{
		Type = MOVE_NO_STATE;
		nIndex = -1;
	}

}SPLIT_BAR_INFO,* LP_SPLIT_BAR_INFO;


enum NEW_PAGE_OPT
{
	NEW_PAGE_EMPTY = 0,
	NEW_PAGE_COPY,
	NEW_PAGE_CUSTOM
};

//新增元素的配置的结构体
typedef struct _add_page_element
{
	//新建页的选项
	NEW_PAGE_OPT eumOpt;
	//页的名称
	CString strPageName;
	
	int nGridNum;

	int nDiagramNum;

	int nDeviceNum;

	_add_page_element(void)
	{
		eumOpt = NEW_PAGE_COPY;
		nGridNum = 1;
		nDeviceNum = 1;
		nDiagramNum = 1;
		strPageName = DEFAULT_PAGE_NAME;
	}

}ADD_PAGE_ELEMENT,* LP_ADD_PAGE_ELEMENT;



//函数指针定义
typedef void (*pCommandEntry)(CEdislabProView* pView);
//更新函数指针定义
typedef void (*pUpdateCommandEntry)(CEdislabProView* pView,CCmdUI* pCmdUI);

#endif