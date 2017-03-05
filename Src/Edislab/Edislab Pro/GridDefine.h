#ifndef _GRID_DEFINE_H
#define _GRID_DEFINE_H
#include "stdafx.h"
#include "GlobalDataManager.h"
#include <vector>
using std::vector;
class CCustomLabGridCtrl;
///************************************************************************/
//控件视图定义
///************************************************************************/
#define DEFUALT_VALUE		0

//字体类型
enum GRID_FONT_TYPE
{
	NORMAL = 1,	
	BOLD,
	ITALIC
};

//控件窗口管理的结构体对象
typedef struct _ViewManager
{
	//控件窗口句柄
	CCustomLabGridCtrl* pGrid;

	//父窗口句柄
	//CWnd* hWnd;

	//控件位置
	CRect rtClient;

	//字体类型
	GRID_FONT_TYPE emFontType;

	//隐藏列
	vector<int> vecHideColIndex;

	_ViewManager(void)
	{
		pGrid = NULL;
		emFontType = NORMAL;
		vecHideColIndex.clear();
	}
}VIEWMANAGER,* LPVIEWMANAGER;



#endif