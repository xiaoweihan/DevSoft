// DlgSensorChoose.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgSensorChoose.h"
#include <boost/foreach.hpp>
#include "Global.h"
// CDlgSensorChoose 对话框

IMPLEMENT_DYNAMIC(CDlgSensorChoose, CBaseDialog)

CDlgSensorChoose::CDlgSensorChoose(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSensorChoose::IDD, pParent)
{

}

CDlgSensorChoose::~CDlgSensorChoose()
{
}

void CDlgSensorChoose::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_SENSOR_TYPE, m_CmbSensorType);
	DDX_Control(pDX, IDC_CMB_RANGE,       m_CmbRange);
	DDX_Control(pDX, IDC_BTN_ADD,         m_BtnAdd);
	DDX_Control(pDX, IDC_BTN_DELETE,      m_BtnDel);
	DDX_Control(pDX, IDC_BTN_DELETE_ALL,  m_BtnDelAll);
	DDX_Control(pDX, IDC_BTN_CHOOSE_CONNECTED, m_BtnChooseCon);
	DDX_Control(pDX, IDC_SENSOR_LIST,           m_ListSensor);
	DDX_Control(pDX, IDC_SELECTED_SENSOR_LIST,           m_ListChoosedSensor);
	DDX_Control(pDX, IDC_STATIC_RESOLUTION_RATIO, m_StaticResolutionRatio);
	DDX_Control(pDX, IDC_CHECK_AUTO_RECOGNIZE, m_CheckAutoChoose);
}


BEGIN_MESSAGE_MAP(CDlgSensorChoose, CDialog)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RECOGNIZE, &CDlgSensorChoose::OnBnClickedCheckAutoRecognize)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgSensorChoose::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgSensorChoose::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, &CDlgSensorChoose::OnBnClickedBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_CONNECTED, &CDlgSensorChoose::OnBnClickedBtnChooseConnected)
	ON_BN_CLICKED(ID_BTN_OK, &CDlgSensorChoose::OnBnClickedBtnOk)
	ON_CBN_SELCHANGE(IDC_CMB_SENSOR_TYPE, &CDlgSensorChoose::OnCbnSelchangeCmbSensorType)
END_MESSAGE_MAP()


// CDlgSensorChoose 消息处理程序
void CDlgSensorChoose::InitCtrls()
{

	//begin add by xiaowei.han at 2017/04/23 0:18:43
	if (m_BtnChooseCon.GetSafeHwnd())
	{
		if (g_bAutoSelect)
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		else
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		
	}
	if (m_CmbSensorType.GetSafeHwnd())
	{
		//CString strCategoryArray[] = {_T("所有"),_T("物理"),_T("化学"),_T("生物")};
		std::vector<SENSOR_TYPE_INFO> vecSensorType = CSensorConfig::CreateInstance().GetSensorTypeInfo();
		CString strName;
		int nIndex = 0;
		BOOST_FOREACH(auto& Element,vecSensorType)
		{
			strName = Element.strTypeName.c_str();
			nIndex = m_CmbSensorType.AddString(strName);
			m_CmbSensorType.SetItemData(nIndex, Element.enumType);
		}

		m_CmbSensorType.SetCurSel(nIndex);

		RefreshSensorList();
	}
	//end add by xiaowei.han at 2017/04/23 0:18:43

	if (m_ListChoosedSensor.GetSafeHwnd())
	{
		m_ListChoosedSensor.EnableItemDescription(TRUE, 1);
	}

}

void CDlgSensorChoose::OnBnClickedCheckAutoRecognize()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_CheckAutoChoose.GetCheck())
	{
		m_BtnAdd.EnableWindow(false);
		m_BtnDel.EnableWindow(false);
		m_BtnDelAll.EnableWindow(false);
		m_BtnChooseCon.EnableWindow(false);
	}
	else
	{
		m_BtnAdd.EnableWindow(true);
		m_BtnDel.EnableWindow(true);
		m_BtnDelAll.EnableWindow(true);
		m_BtnChooseCon.EnableWindow(true);
	}

	// 添加传感器识别部分代码


}


BOOL CDlgSensorChoose::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrls();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSensorChoose::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_ListSensor.GetCurSel();
	nIndex = m_ListSensor.GetItemData(nIndex);
	std::map<int, SENSOR_CONFIG_ELEMENT>::iterator iter = m_mapCurrentSensor.find(nIndex);
	if (m_mapCurrentSensor.cend() != iter)
	{
		CString str;
		int nNum = m_setChooseSensorID.count(iter->second.nSensorID);
		if (nNum == 0)
		{
			str.Format(_T("数据列：%s(%s)"), iter->second.strSensorSymbol, iter->second.strSensorName);
		}
		else
		{
            str.Format(_T("数据列：%s_%d(%s)"), iter->second.SensorRangeInfoArray[0].strRangeName, nNum, iter->second.strSensorName);
		}
		
		m_ListChoosedSensor.EnableItemDescription(TRUE ,1);
		nIndex = m_ListChoosedSensor.AddString(str);
	    str = iter->second.SensorRangeInfoArray[0].strRangeName.c_str();
		nIndex = str.Find('(');
		str.Left(nIndex);
	    m_ListChoosedSensor.SetItemDescription(nIndex, str);
		m_ListChoosedSensor.SetItemData(nIndex, iter->second.nSensorID);

		m_setChooseSensorID.insert(iter->second.nSensorID);
	}
}


void CDlgSensorChoose::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_ListChoosedSensor.GetCurSel();
	if (-1 != nIndex)
	{
		nIndex = m_ListChoosedSensor.GetItemData(nIndex);
		if (m_setChooseSensorID.lower_bound(nIndex) != m_setChooseSensorID.end())
		{
			m_setChooseSensorID.erase(m_setChooseSensorID.lower_bound(nIndex));
		}
		
		m_ListChoosedSensor.DeleteString(m_ListChoosedSensor.GetCurSel());
	}
}


void CDlgSensorChoose::OnBnClickedBtnDeleteAll()
{
	// TODO: 在此添加控件通知处理程序代码

	m_mapChooseSensor.clear();
	m_ListChoosedSensor.CleanUp();
	m_setChooseSensorID.clear();
}


void CDlgSensorChoose::OnBnClickedBtnChooseConnected()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSensorChoose::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 刷新表格
void CDlgSensorChoose::RefreshSensorList()
{
	m_ListSensor.CleanUp();
	m_mapCurrentSensor.clear();

	m_ListSensor.EnableItemDescription(TRUE, 1);
	std::vector<SENSOR_CONFIG_ELEMENT> vecSensorList;
	int nSensorType = m_CmbSensorType.GetCurSel();
	if (-1 == nSensorType)
	{
		return;
	}

	nSensorType = m_CmbSensorType.GetItemData(nSensorType);
	SENSOR_TYPE type;
	if (1 == nSensorType)
	{
		type = SENSOR_PHYSICS;
	} 
	else if (2 == nSensorType)
	{
		type = SENSOR_CHEMISTRY;
	}
	else if (4 == nSensorType)
	{
		type = SENSOR_BIOLOGY;
	}
	else
	{
		type = SENSOR_ALL;
	}
	
	CSensorConfig::CreateInstance().GetSensorList(vecSensorList, type);
	BOOST_FOREACH(auto &sensor, vecSensorList)
	{
		CString str(sensor.strSensorName.c_str());
		int nIndex = m_ListSensor.AddString(str);
		str = sensor.strSensorModelName.c_str();
		m_ListSensor.SetItemDescription(nIndex, str);
		m_ListSensor.SetItemData(nIndex, sensor.nSensorID);
		m_mapCurrentSensor[sensor.nSensorID] = sensor;
	}
}

void CDlgSensorChoose::RefreshRange()
{
	int nIndex = m_ListChoosedSensor.GetCurSel();
	if ( -1 != nIndex)
	{
		nIndex = m_ListChoosedSensor.GetItemData(nIndex);
		const SENSOR_CONFIG_ELEMENT &sensor = CSensorConfig::CreateInstance().GetSensorInfo(nIndex);
		if (-1 != sensor.nSensorID)
		{
			m_CmbRange.Clear();
			m_CmbRange.ShowWindow(TRUE);
			if(sensor.SensorRangeInfoArray.size() == 1)
			{
				m_CmbRange.EnableWindow(FALSE);
			}
			else
			{
				m_CmbRange.EnableWindow(FALSE);
			}

			CString str;
			int nIndex = 0;
			for (int n = 0; n < (int)sensor.SensorRangeInfoArray.size(); ++n)
			{
				str = sensor.SensorRangeInfoArray[n].strRangeName.c_str();
				nIndex = str.Find('(');
				str.Left(nIndex);
				nIndex = m_CmbRange.AddString(str);
				m_CmbRange.SetItemData(nIndex, n);

				str = sensor.SensorRangeInfoArray[n].strRangeName.c_str();
				nIndex = str.Find('(');
				str.Right(nIndex);
				str.Remove('(');
				str.Remove(')');
				m_StaticResolutionRatio.SetWindowText(str);
			}
		}
		else
		{
			m_CmbRange.ShowWindow(FALSE);
			m_StaticResolutionRatio.SetWindowText(_T(""));
		}
	}
}

void CDlgSensorChoose::OnCbnSelchangeCmbSensorType()
{
	// TODO: 在此添加控件通知处理程序代码

	RefreshSensorList();
}
