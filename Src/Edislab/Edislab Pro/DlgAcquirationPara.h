#pragma once
#include "BaseDialog.h"

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
};
