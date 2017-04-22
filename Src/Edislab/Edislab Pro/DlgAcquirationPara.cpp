// DlgAcquirationPara.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgAcquirationPara.h"
#pragma warning(disable:4267)


// CDlgAcquirationPara 对话框

const int MAX_SAMPLE_NUM = 26000;

IMPLEMENT_DYNAMIC(CDlgAcquirationPara, CBaseDialog)

CDlgAcquirationPara::CDlgAcquirationPara(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAcquirationPara::IDD, pParent)
	, m_nGroupRadioValue(0)
{
}

CDlgAcquirationPara::~CDlgAcquirationPara()
{
}

void CDlgAcquirationPara::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FIXED_FREQUENCY, m_nGroupRadioValue);
	DDX_Control(pDX, IDC_STATIC_FREQUENCY, m_staticFrequency);
	DDX_Control(pDX, IDC_SCRBAR_FREQUENCY, m_barFrequency);
	DDX_Control(pDX, IDC_CHECK_LIMIT_TIME, m_checkLimitTime);
	DDX_Control(pDX, IDC_STATIC_LIMIT_TIME, m_staticLimitTime);
	DDX_Control(pDX, IDC_SCRBAR_LIMIT_TIME, m_barLimitTime);
	DDX_Control(pDX, IDC_CHECK_SAMPLE_START, m_checkSampleAtStart);
	DDX_Control(pDX, IDC_CHECK_MULTI_SAMPLE, m_checkMultiSample);
	DDX_Control(pDX, IDC_STATIC_MULTI_SAMPLE, m_staticMultiSample);
	DDX_Control(pDX, IDC_STATIC_SAM_NUM, m_staticSampleNum);
	DDX_Control(pDX, IDC_STATIC_WARNING, m_staticWarningInfo);
}


BEGIN_MESSAGE_MAP(CDlgAcquirationPara, CBaseDialog)
	ON_BN_CLICKED(IDC_RADIO_FIXED_FREQUENCY, &CDlgAcquirationPara::OnBnClickedRadioFixedFrequency)
	ON_BN_CLICKED(IDC_CHECK_SAMPLE_START, &CDlgAcquirationPara::OnBnClickedCheckSampleStart)
	ON_BN_CLICKED(IDC_CHECK_MULTI_SAMPLE, &CDlgAcquirationPara::OnBnClickedCheckMultiSample)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_TIME, &CDlgAcquirationPara::OnBnClickedCheckLimitTime)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgAcquirationPara 消息处理程序


void CDlgAcquirationPara::OnBnClickedRadioFixedFrequency()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_nGroupRadioValue)
	{
		// 定频率记录
		case 0:
			
			break;
		// 触发记录
		case 1:
				
			break;
		// 手动记录
		case 2:

			break;
		// 光电门触发
		case 3:

			break;
	    // 专门实验
		case 4:

			break;
		// 未知选择
		default:
			break;
	}
}


void CDlgAcquirationPara::InitCtrls()
{
	// 频率范围滚动条，36档
	InitFrequencyMap();
	m_barFrequency.SetScrollRange(0, m_mapFrequency.size() - 1, TRUE);

	//限定时间选择滚动条， 45档
	InitLimitTimeMap();
	m_barLimitTime.SetScrollRange(0, m_mapLimitTime.size() - 1, TRUE);

	m_barLimitTime.EnableWindow(FALSE);
	m_staticSampleNum.SetWindowTextW(_T("26000"));
}

void CDlgAcquirationPara::InitFrequencyMap()
{
	// 频率选择滚动条 单位：点/秒
	m_mapFrequency[0] = 0.001f;
	m_mapFrequency[1] = 0.002f;
	m_mapFrequency[2] = 0.004f;
    m_mapFrequency[3] = 0.005f;
	m_mapFrequency[4] = 0.008f;

	m_mapFrequency[5] = 0.01f;
	m_mapFrequency[6] = 0.02f;
	m_mapFrequency[7] = 0.04f;
	m_mapFrequency[8] = 0.05f;
	m_mapFrequency[9] = 0.08f;

	m_mapFrequency[10] = 0.1f;
	m_mapFrequency[11] = 0.2f;
	m_mapFrequency[12] = 0.4f;
	m_mapFrequency[13] = 0.5f;
	m_mapFrequency[14] = 0.8f;

	m_mapFrequency[15] = 1.0f;
	m_mapFrequency[16] = 2.0f;
	m_mapFrequency[17] = 4.0f;
	m_mapFrequency[18] = 5.0f;
	m_mapFrequency[19] = 8.0f;
	m_mapFrequency[20] = 10.0f;

	m_mapFrequency[21] = 20.0f;
	m_mapFrequency[22] = 40.0f;
	m_mapFrequency[23] = 50.0f;
	m_mapFrequency[24] = 100.0f;
	m_mapFrequency[25] = 200.0f;
	m_mapFrequency[26] = 500.0f;
	m_mapFrequency[27] = 1000.0f;
	m_mapFrequency[28] = 2500.0f;
	m_mapFrequency[29] = 3125.0f;
	m_mapFrequency[30] = 5000.0f;
	m_mapFrequency[31] = 6250.0f;
	m_mapFrequency[32] = 12500.0f;
	m_mapFrequency[33] = 25000.0f;
	m_mapFrequency[34] = 50000.0f;
	m_mapFrequency[35] = 100000.0f;
}

void CDlgAcquirationPara::InitLimitTimeMap()
{
	// 限定时间滚动条 单位：秒（s）
	m_mapLimitTime[0] = 0.1f;
	m_mapLimitTime[1] = 0.5f;
	m_mapLimitTime[2] = 1.0f;
	m_mapLimitTime[3] = 2.0f;
	m_mapLimitTime[4] = 3.0f;
	m_mapLimitTime[5] = 4.0f;
	m_mapLimitTime[6] = 5.0f;
	m_mapLimitTime[7] = 6.0f;
	m_mapLimitTime[8] = 7.0f;
	m_mapLimitTime[9] = 8.0f;
	m_mapLimitTime[10] = 9.0f;
	m_mapLimitTime[11] = 10.0f;
	m_mapLimitTime[12] = 15.0f;
	m_mapLimitTime[13] = 20.0f;
	m_mapLimitTime[14] = 25.0f;
    m_mapLimitTime[15] = 30.0f;
	m_mapLimitTime[16] = 35.0f;
	m_mapLimitTime[17] = 40.0f;
	m_mapLimitTime[18] = 45.0f;
	m_mapLimitTime[19] = 50.0f;
	m_mapLimitTime[20] = 60.0f;
	m_mapLimitTime[21] = 70.0f;
	m_mapLimitTime[22] = 80.0f;
	m_mapLimitTime[23] = 90.0f;
	m_mapLimitTime[24] = 100.0f;
	m_mapLimitTime[25] = 120.0f;
	m_mapLimitTime[26] = 180.0f;
	m_mapLimitTime[27] = 240.0f;
	m_mapLimitTime[28] = 300.0f;
	m_mapLimitTime[29] = 360.0f;
	m_mapLimitTime[30] = 420.0f;
	m_mapLimitTime[31] = 480.0f;
	m_mapLimitTime[32] = 540.0f;
	m_mapLimitTime[33] = 600.0f;
	m_mapLimitTime[34] = 720.0f;
	m_mapLimitTime[35] = 840.0f;
	m_mapLimitTime[36] = 960.0f;
	m_mapLimitTime[37] = 1080.0f;
	m_mapLimitTime[38] = 1200.0f;
	m_mapLimitTime[39] = 1500.0f;
	m_mapLimitTime[40] = 1800.0f;
	m_mapLimitTime[41] = 2700.0f;
	m_mapLimitTime[42] = 3600.0f;
	m_mapLimitTime[43] = 5400.0f;
	m_mapLimitTime[44] = 7200.0f;
}

int CDlgAcquirationPara::CalSampleNum()
{
	using namespace std;
	if (BST_CHECKED != m_checkLimitTime.GetCheck())
	{
		return MAX_SAMPLE_NUM;
	}

	float fFrequency = 1.0f;
	float fTime = 1.0f;
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	if(m_mapFrequency.end() != iter)
	{
		fFrequency = iter->second;
	}

	iter = m_mapLimitTime.find(m_barLimitTime.GetScrollPos());
	if (m_mapLimitTime.end() != iter)
	{
		fTime = iter->second;
	}

	// 小于1时强制设为1，但是“在0时采样”勾选时
	int nSampleNum = (int)(fTime*fFrequency);
	if (nSampleNum < 1)
	{
		nSampleNum = 1;
	}
	else if (BST_CHECKED == m_checkSampleAtStart.GetCheck())
	{
		nSampleNum++;
	}

	return nSampleNum;
}

void CDlgAcquirationPara::RefreshCtrl()
{
	using namespace std;
	int nSampleNum = CalSampleNum();
	CString strText;
	// 采样数目
	if (nSampleNum > MAX_SAMPLE_NUM)
	{
		strText.Format(_T("%d(>%d)"), nSampleNum, MAX_SAMPLE_NUM);
	}
	else
	{
		strText.Format(_T("%d"), nSampleNum);
	}
	m_staticSampleNum,SetWindowTextW(strText);

	// 多采样
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	strText = _T("");
	if (iter != m_mapFrequency.end())
	{
		if (BST_CHECKED == m_checkMultiSample.GetCheck())
		{
			strText.Format(_T("每采%d点求平均值"), GetAverageSampleNum(iter->second));
		}
	}
	m_staticMultiSample.SetWindowText(strText);

	// 最下方警告信息
	strText = _T("");
	if (nSampleNum > MAX_SAMPLE_NUM)
	{
		strText = _T("采样数目已经超出系统建议的26000个点，将在超出能力时，自动停止实验。");
	}
	m_staticWarningInfo.SetWindowText(strText);
}

int CDlgAcquirationPara::GetAverageSampleNum(float fFrequency)
{
	if (fFrequency <= 4.0f)
	{
		return 50;
	}
	else if (fFrequency <= 5.0f)
	{
		return 40;
	}
	else if (fFrequency <= 8.0f)
	{
		return 25;
	}
	else if (fFrequency <= 10.0f)
	{
		return 20;
	}
	else if (fFrequency <= 20.0f)
	{
		return 10;
	}
	else if (fFrequency <= 40.0f)
	{
		return 5;
	}
	else if (fFrequency <= 50.0f)
	{
		return 4;
	}
	else if (fFrequency <= 100.0f)
	{
		return 2;
	}
	else 
	{
		return 1;
	}
}

void CDlgAcquirationPara::OnBnClickedCheckSampleStart()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshCtrl();
}


void CDlgAcquirationPara::OnBnClickedCheckMultiSample()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshCtrl();
}


void CDlgAcquirationPara::OnBnClickedCheckLimitTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == m_checkLimitTime.GetCheck())
	{
		m_barLimitTime.EnableWindow(TRUE);
	}
	else
	{
		m_barLimitTime.EnableWindow(FALSE);
	}
}


BOOL CDlgAcquirationPara::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAcquirationPara::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = pScrollBar->GetScrollPos();    // 获取水平滚动条当前位置   

	switch (nSBCode)    
	{    
		// 如果向左滚动一列，则pos减1   
	case SB_LINEUP:    
		pos -= 1;    
		break;    
		// 如果向右滚动一列，则pos加1   
	case SB_LINEDOWN:    
		pos  += 1;    
		break;    
		// 如果向左滚动一页，则pos减10   
	case SB_PAGEUP:    
		pos -= 10;    
		break;    
		// 如果向右滚动一页，则pos加10   
	case SB_PAGEDOWN:    
		pos  += 10;    
		break;    
		// 如果滚动到最左端，则pos为1   
	case SB_TOP:    
		pos = 1;    
		break;    
		// 如果滚动到最右端，则pos为100   
	case SB_BOTTOM:    
		pos = 100;    
		break;      
		// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值   
	case SB_THUMBPOSITION:    
		pos = nPos;    
		break;    
		// 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值   
	default:     
		return;    
	}    

	// 设置滚动块位置   
	pScrollBar->SetScrollPos(pos);   

	using namespace std;
    CString strStatic;
	map<int, float>::iterator iter = m_mapFrequency.find(m_barFrequency.GetScrollPos());
	if (iter != m_mapFrequency.end())
	{

		strStatic.Format(_T("%f点/秒，即%f秒/点"), iter->second, 1.0f/iter->second);
		m_staticFrequency.SetWindowText(strStatic);

		strStatic = _T("");
		if (BST_CHECKED == m_checkMultiSample.GetCheck())
		{
			strStatic.Format(_T("每采%d点求平均值"), GetAverageSampleNum(iter->second));
		}

		m_staticMultiSample.SetWindowText(strStatic);
	}

	strStatic = _T("");
	if(m_checkLimitTime.GetCheck())
	{
		/*map<int, float>::iterator*/ iter = m_mapLimitTime.find(m_barLimitTime.GetScrollPos());
		if (m_mapLimitTime.end() != iter)
		{
			// 三种时间单位
			if (iter->second < 1.0f)
			{
				strStatic.Format(_T("%f毫秒"), iter->second * 100);
			}
			else if (iter->second < 101.0f)
			{
				strStatic.Format(_T("%f秒"), iter->second);
			}
			else if (iter->second < 5401.0f )
			{
				strStatic.Format(_T("%f分"), iter->second/60.0f);
			}
			else
			{
				strStatic.Format(_T("%f小时"), iter->second/3600.0f);
			}
		}
	}

	m_staticLimitTime.SetWindowText(strStatic);

	RefreshCtrl();

	CBaseDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
