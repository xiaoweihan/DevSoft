#pragma once
#include "BaseDialog.h"
#include <map>
#include "Type.h"
#include "SensorConfig.h"

// CDlgAcquirationPara 对话框

class CDlgAcquirationPara : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAcquirationPara)

public:
	CDlgAcquirationPara(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAcquirationPara();

// 对话框数据
	enum { IDD = IDD_DLG_ACQUISITION_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnBnClickedRadioFixedFrequency();

private:
	void InitCtrls();

	void InitFrequencyMap();

	void InitLimitTimeMap();

	int CalSampleNum();

	void RefreshCtrl();

	int GetAverageSampleNum(float fFrequency);

private:
	// Radio Group绑定的变量
	int m_nGroupRadioValue;

	// 频率显示
	CBCGPStatic m_staticFrequency;

	// 频率选择滑动条
	/*  单位：点/秒
	  0.001点/秒， 0.002，  0.004，  0.005，  0.008，
	  0.01， 0.02,  0.04,  0.05, 0.08,  0.1, 0.2,  0.4,  0.5,  0.8,
	  1,  2,  4,  5,  8,  10,  20,  40,  50,  100,  200,  500,  1000,
	  2500,  3125,  5000,  6250,  12500,   25000,  50000,  100000,
	*/
	CScrollBar m_barFrequency;

	// 是否限定时间
	CBCGPButton m_checkLimitTime;

	// 显示限定的时间
	CBCGPStatic m_staticLimitTime;

	// 限定时间选择滚动条
	/* 数值选择范围
	   100ms, 500ms,
	   1s, 2s, 3s, 4s, 5s, 6s, 7s, 8s, 9s, 10s,
	   15s, 20s, 25s, 30s, 35s, 40s, 45s, 50s, 
	   60s, 70s, 80s, 90s, 100s, 
	   2m, 3m, 4m, 5m, 6m, 7m, 8m, 9m, 10m, 
	   12m, 14m, 16m, 18m, 20m,
	   25m,
	   30m, 45m, 60m, 90m, 
	   2h
	*/
	CScrollBar m_barLimitTime;

	// 是否在零时采样
	CBCGPButton m_checkSampleAtStart;

	// 多采样
	CBCGPButton m_checkMultiSample;

	// 多采样方式
	CBCGPStatic m_staticMultiSample;

	// 采样数目
	CBCGPStatic m_staticSampleNum;

	//提示信息
	CBCGPStatic m_staticWarningInfo;

	// 频率滚动条，位置与对应的信息
	std::map<int, float> m_mapFrequency;

	// 限定时间滚动条，位置与对应的信息
	std::map<int, float> m_mapLimitTime;
public:
// 	afx_msg void OnNMThemeChangedScrbarFrequency(NMHDR *pNMHDR, LRESULT *pResult);
// 	afx_msg void OnNMThemeChangedScrbarLimitTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckSampleStart();
	afx_msg void OnBnClickedCheckMultiSample();
	afx_msg void OnBnClickedCheckLimitTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
