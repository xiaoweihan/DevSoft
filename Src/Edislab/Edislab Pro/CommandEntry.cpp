#include "StdAfx.h"
#include "Edislab Pro.h"
#include "Edislab ProView.h"
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include "CommandEntry.h"
#include "boost/crc.hpp"
#include "Msg.h"
#include "DlgAddPage.h"
#include "DlgTabPanel.h"
#include "DlgPageNameConfig.h"
#include "Utility.h"
#include "CApplication.h"
#include "CDocuments.h"
#include "CFont0.h"
#include "CSelection.h"
#include "CParagraphFormat.h"
#include "DlgAcquirationPara.h"
#include "DlgSensorChoose.h"
#include "Global.h"
#include "SensorConfig.h"
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SerialPortService.h"
//最大页面数
const int MAX_PAGE_NUM = 4;
//表格的最大个数
const int MAX_GRID_NUM = 9;
const int MAX_DEVICE_NUM = 9;
const int MAX_DIAGRAM_NUM = 9;
//是否开始采集
static bool s_bStartCapture = false;
//处理添加页面
static void HandleAddPage(CEdislabProView* pView);
//更新添加页面
static void UpdateHandleAddPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//删除页面
static void HandleDelPage(CEdislabProView* pView);
//更新删除页面
static void UpdateHandleDelPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//页面名称
static void HandlePageName(CEdislabProView* pView);
//更新页面名称
static void UpdateHandlePageName(CEdislabProView* pView,CCmdUI* pCmdUI);
//跳转页面
static void HandleJumpPage(CEdislabProView* pView);
//更新跳转名称
static void UpdateHandleJumpPage(CEdislabProView* pView,CCmdUI* pCmdUI);
//添加表格
void HandleAddTable(CEdislabProView* pView);
//更新添加表格
void UpdateHandleAddTable(CEdislabProView* pView,CCmdUI* pCmdUI);
//添加图
void HandleAddImage(CEdislabProView* pView);
//更新添加图
void UpdateHandleAddImage(CEdislabProView* pView,CCmdUI* pCmdUI);
//添加仪表
void HandleAddDevice(CEdislabProView* pView);
//更新添加仪表
void UpdateHandleAddDevice(CEdislabProView* pView,CCmdUI* pCmdUI);
//删除元素
void HandleDelElement(CEdislabProView* pView);
//更新删除元素
void UpdateHandleDelElement(CEdislabProView* pView,CCmdUI* pCmdUI);
//开始采集
void HandleStart(CEdislabProView* pView);
void UpdateHandleStart(CEdislabProView* pView,CCmdUI* pCmdUI);
//自动识别
void HandleAutoSelect(CEdislabProView* pView);
void UpdateHandleAutoSelect(CEdislabProView* pView,CCmdUI* pCmdUI);
//手动识别
void HandleManualSelect(CEdislabProView* pView);
void UpdateHandleManualSelect(CEdislabProView* pView,CCmdUI* pCmdUI);
//输出实验报告
void HandleOutputTestReport(CEdislabProView* pView);
//删除元素
void HandleChooseDevice(CEdislabProView* pView);
//更新删除元素
void UpdateHandleChooseDevice(CEdislabProView* pView,CCmdUI* pCmdUI);
//删除元素
void HandleAcquirePara(CEdislabProView* pView);
//更新删除元素
void UpdateHandleAcquirePara(CEdislabProView* pView,CCmdUI* pCmdUI);


CCommandEntry& CCommandEntry::CreateInstance( void )
{
	static CCommandEntry s_Entry;
	return s_Entry;
}

CCommandEntry::CCommandEntry(void)
{
	InitCommandEntry();
	InitUpdateCommandEntry();
}


CCommandEntry::~CCommandEntry(void)
{
	UnInitCommandEntry();
	UnInitUpdateCommandEntry();
}

/*****************************************************************************
@FunctionName : GetCommandEntryByCommanID
@FunctionDescription : 获取命令处理函数接口
@inparam  : nCommandID:命令ID
@outparam :  
@return : 命令处理函数地址
*****************************************************************************/
pCommandEntry CCommandEntry::GetCommandEntryByCommanID( unsigned int nCommandID )
{

	std::map<unsigned int,pCommandEntry>::iterator Iter = m_CommandEntryMap.find(nCommandID);

	if (Iter == m_CommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;

}

/*****************************************************************************
@FunctionName : GetUpdateCommandEntryByCommanID
@FunctionDescription : 获取命令更新处理函数接口
@inparam  : nCommandID:命令ID
@outparam :  
@return : 命令更新处理函数地址
*****************************************************************************/
pUpdateCommandEntry CCommandEntry::GetUpdateCommandEntryByCommanID( unsigned int nCommandID )
{
	std::map<unsigned int,pUpdateCommandEntry>::iterator Iter = m_UpdateCommandEntryMap.find(nCommandID);

	if (Iter == m_UpdateCommandEntryMap.end())
	{
		return NULL;
	}

	return Iter->second;
}

/*****************************************************************************
@FunctionName : InitCommandEntry
@FunctionDescription : 初始化命令处理接口数组
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitCommandEntry( void )
{
	m_CommandEntryMap.clear();

	m_CommandEntryMap[ID_ADD_PAGE] = HandleAddPage;
	m_CommandEntryMap[ID_DEL_PAGE] = HandleDelPage;
	m_CommandEntryMap[ID_SET_PAGE_NAME] = HandlePageName;
	m_CommandEntryMap[ID_GOTO_PAGE] = HandleJumpPage;
	m_CommandEntryMap[ID_ADD_TABLE] = HandleAddTable;
	m_CommandEntryMap[ID_DEL_ELEMENT] = HandleDelElement;
	m_CommandEntryMap[ID_ADD_IMAGE] = HandleAddImage;
	m_CommandEntryMap[ID_ADD_DEVICE] = HandleAddDevice;
	m_CommandEntryMap[ID_AUTO_SELECT] = HandleAutoSelect;
	m_CommandEntryMap[ID_START] = HandleStart;
	m_CommandEntryMap[ID_MANUAL_SELECT] = HandleManualSelect;
	m_CommandEntryMap[ID_OUTPUT_TEST_REPORT] = HandleOutputTestReport;

	m_CommandEntryMap[ID_SELECT_SENSOR] = HandleChooseDevice;
	m_CommandEntryMap[ID_COLLECT_PARAM] = HandleAcquirePara;
}

/*****************************************************************************
@FunctionName : InitUpdateCommandEntry
@FunctionDescription : 初始化更新命令处理接口数组
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::InitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
	m_UpdateCommandEntryMap[ID_ADD_PAGE] = UpdateHandleAddPage;
	m_UpdateCommandEntryMap[ID_DEL_PAGE] = UpdateHandleDelPage;
	m_UpdateCommandEntryMap[ID_SET_PAGE_NAME] = UpdateHandlePageName;
	m_UpdateCommandEntryMap[ID_GOTO_PAGE] = UpdateHandleJumpPage;
	m_UpdateCommandEntryMap[ID_ADD_TABLE] = UpdateHandleAddTable;
	m_UpdateCommandEntryMap[ID_DEL_ELEMENT] = UpdateHandleDelElement;
	m_UpdateCommandEntryMap[ID_ADD_IMAGE] = UpdateHandleAddImage;
	m_UpdateCommandEntryMap[ID_ADD_DEVICE] = UpdateHandleAddDevice;

	m_UpdateCommandEntryMap[ID_SELECT_SENSOR] = UpdateHandleChooseDevice;
	m_UpdateCommandEntryMap[ID_COLLECT_PARAM] = UpdateHandleAcquirePara;

	//m_UpdateCommandEntryMap[ID_START] = UpdateHandleStart;
	m_UpdateCommandEntryMap[ID_MANUAL_SELECT] = UpdateHandleManualSelect;
	m_UpdateCommandEntryMap[ID_AUTO_SELECT] = UpdateHandleAutoSelect;
}

/*****************************************************************************
@FunctionName : UnInitCommandEntry
@FunctionDescription : 清空命令处理接口数组
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitCommandEntry( void )
{
	m_CommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : UnInitUpdateCommandEntry
@FunctionDescription : 清空更新命令处理接口数组
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CCommandEntry::UnInitUpdateCommandEntry( void )
{
	m_UpdateCommandEntryMap.clear();
}

/*****************************************************************************
@FunctionName : HandleAddPage
@FunctionDescription : 处理增加页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void HandleAddPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}

	CDlgAddPage AddDlg;

	if (IDOK == AddDlg.DoModal())
	{

		//获取添加的动作
		ADD_PAGE_ELEMENT AddPageElement = AddDlg.GetAddPageElement();
		if (NEW_PAGE_COPY == AddPageElement.eumOpt)
		{
			CDlgTabPanel* pPanel = pView->GetCurrentPage();

			if (NULL != pPanel)
			{
				AddPageElement.nGridNum = pPanel->GetGridNum();
				AddPageElement.nDiagramNum = pPanel->GetDiagramNum();
				AddPageElement.nDeviceNum = pPanel->GetDeviceNum();
			}
		}		

		if (pView->AddNewTabWnd(AddPageElement.strPageName,AddPageElement.nGridNum,AddPageElement.nDiagramNum,AddPageElement.nDeviceNum))
		{
			//设置最后一个Tab为活动的Tab
			int nPageNum = pView->GetPageNum();
			pView->SetActivePage(nPageNum - 1);
		}
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleAddPage
@FunctionDescription : 更新处理增加页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleAddPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum < MAX_PAGE_NUM)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}

/*****************************************************************************
@FunctionName : HandleDelPage
@FunctionDescription : 处理删除页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void HandleDelPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}


	if (IDYES == Utility::AfxBCGPMessageBox(_T("确定要删除页面?"),MB_YESNO))
	{
		pView->DeleteCurrentPage();
	}

}

/*****************************************************************************
@FunctionName : UpdateHandleDelPage
@FunctionDescription : 更新处理删除页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleDelPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

}

/*****************************************************************************
@FunctionName : HandlePageName
@FunctionDescription : 处理页面名称设置的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void HandlePageName( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}

	CString strPageName = pView->GetCurrentPageName();


	CDlgPageNameConfig PageNameConfigDlg(strPageName);

	if (IDOK == PageNameConfigDlg.DoModal())
	{
		strPageName = PageNameConfigDlg.GetPageName();
		//设置名称
		pView->SetCurrentPageName(strPageName);
	}

}

/*****************************************************************************
@FunctionName : UpdateHandlePageName
@FunctionDescription : 处理页面名称设置的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandlePageName( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	CDlgTabPanel* pWnd = pView->GetCurrentPage();

	if (NULL == pWnd)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


/*****************************************************************************
@FunctionName : HandleJumpPage
@FunctionDescription : 处理跳转页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void HandleJumpPage( CEdislabProView* pView )
{
	if (NULL == pView)
	{
		return;
	}
}

/*****************************************************************************
@FunctionName : UpdateHandleJumpPage
@FunctionDescription : 处理跳转页面的响应
@inparam  : pView:视图指针
@outparam :  
@return : 
*****************************************************************************/
void UpdateHandleJumpPage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	int nNum = pView->GetPageNum();

	if (nNum <= 1)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//添加表格
void HandleAddTable(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddGrid();
}
//添加图
void HandleAddImage(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddDiagram();
	
}
//添加仪表
void HandleAddDevice(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	pView->AddDevice();
}
//删除元素
void HandleDelElement(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	if(pView)
	{
		CDlgTabPanel* tabPanel = pView->GetCurrentPage();
		if(tabPanel)
		{
			tabPanel->DelWnd();
		}
	}
}

void UpdateHandleAddTable( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetGridNum() >= MAX_GRID_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddImage( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetDiagramNum() >= MAX_DIAGRAM_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleAddDevice( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (pView->GetDeviceNum() >= MAX_DEVICE_NUM)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void UpdateHandleDelElement( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}

	if (0 == pView->GetGridNum() && 0 == pView->GetDeviceNum() && 0 == pView->GetDiagramNum())
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

//开始采集
void HandleStart(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	std::vector<std::string> SensorNameArray;
	CSensorIDGenerator::CreateInstance().GetAllSensorName(SensorNameArray);
	if (s_bStartCapture)
	{
		BOOST_FOREACH(auto& V,SensorNameArray)
		{
			CSerialPortService::CreateInstance().StopSensorCollect(V);
		}
	}
	else
	{
		BOOST_FOREACH(auto& V,SensorNameArray)
		{
			CSerialPortService::CreateInstance().StartSensorCollect(V);
		}
	}
	s_bStartCapture = !s_bStartCapture;
}

//自动识别
void HandleAutoSelect(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}
	
	CWnd* pWnd = AfxGetMainWnd();
	if(NULL == pWnd)
	{
		return;
	}

	g_bAutoSelect = TRUE;
	//COMIPLE.StartCom(pWnd);
	
}

//输出实验报告
void HandleOutputTestReport( CEdislabProView* pView )
{
	if (nullptr == pView)
	{
		return;
	}

	CApplication app;  
	COleVariant vTrue((short)TRUE), vFalse((short)FALSE);  
	app.CreateDispatch(_T("Word.Application"));
	app.put_Visible(FALSE); 
	//Create New Doc  
	CDocuments docs = app.get_Documents();  
	CComVariant tpl(_T("")),Visble,DocType(0),NewTemplate(false);  
	docs.Add(&tpl,&NewTemplate,&DocType,&Visble);  
	//Add Content:Text  
	CSelection sel = app.get_Selection(); 

	CParagraphFormat ParaFormat = sel.get_ParagraphFormat();
	CFont0 SelFont = sel.get_Font();
	SelFont.put_Bold(1);
	SelFont.put_Size(16);
	SelFont.put_Name(_T("宋体"));
	sel.TypeText(_T("无标题试验"));
	ParaFormat.put_Alignment(1);
	sel.TypeParagraph();
	sel.TypeParagraph();
	SelFont.put_Size(12);
	SelFont.put_Bold(0);
	ParaFormat.put_Alignment(0);
	sel.TypeText(_T("实验班级_____________实验日期_____________姓名_____________"));
	sel.TypeParagraph();
	sel.TypeParagraph();
	SelFont.put_Bold(1);
	sel.TypeText(_T("实验目的:"));

	int nTimes = 5;
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.TypeText(_T("实验目的:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.TypeText(_T("实验原理:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("实验器材:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("实验步骤:"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}

	sel.TypeText(_T("实验结果与分析"));
	for (int i = 0; i < nTimes; ++i)
	{
		sel.TypeParagraph();
	}
	sel.ReleaseDispatch();  
	ParaFormat.ReleaseDispatch();
	docs.ReleaseDispatch();  
	app.put_Visible(TRUE);  
	app.ReleaseDispatch();  
}

// 选择仪器
void HandleChooseDevice(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

    CDlgSensorChoose dlgChooseSensor;
	dlgChooseSensor.DoModal();
}

void UpdateHandleChooseDevice( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
}

// 获取参数
void HandleAcquirePara(CEdislabProView* pView)
{
	if (NULL == pView)
	{
		return;
	}

	CDlgAcquirationPara dlgSelectPara;
	dlgSelectPara.DoModal();
}

void UpdateHandleAcquirePara( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
}

void HandleManualSelect( CEdislabProView* pView )
{
	g_bAutoSelect = FALSE;
}

void UpdateHandleManualSelect( CEdislabProView* pView,CCmdUI* pCmdUI )
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
	pCmdUI->SetCheck(!g_bAutoSelect);
}

void UpdateHandleAutoSelect(CEdislabProView* pView,CCmdUI* pCmdUI)
{
	if (NULL == pView || NULL == pCmdUI)
	{
		return;
	}
	pCmdUI->SetCheck(g_bAutoSelect);
}


