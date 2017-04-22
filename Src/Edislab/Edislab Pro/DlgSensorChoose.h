#pragma once
#include "BaseDialog.h"

// CDlgSensorChoose 对话框

class CDlgSensorChoose : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSensorChoose)

public:
	CDlgSensorChoose(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSensorChoose();

// 对话框数据
	enum { IDD = IDD_DLG_SENSOR_CHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void InitCtrls();

private:
	// 添加按钮
	CBCGPButton m_BtnAdd;
	
	// 删除按钮
	CBCGPButton m_BtnDel;

	// 全删按钮
	CBCGPButton m_BtnDelAll;

	// 选已接按钮
	CBCGPButton m_BtnChooseCon;

	// 传感器类型下拉菜单
	CBCGPComboBox m_CmbSensorType;

	// 自动识别
	CBCGPButton m_CheckAutoChoose;

	// 量程
	CBCGPComboBox m_CmbRange;

	// 传感器列表
	CBCGPListBox m_ListSensor;
    
	// 已选传感器列表
	CBCGPListBox m_ListChoosedSensor;

	// 分辨率数值（部分传感器的分辨率参数不是文本表示的，此文本框会提供位置信息）
	CBCGPStatic m_StaticResolutionRatio;
public:
	afx_msg void OnBnClickedCheckAutoRecognize();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnDeleteAll();
	afx_msg void OnBnClickedBtnChooseConnected();
	afx_msg void OnBnClickedBtnOk();
};
