#pragma once
#if 0
#include <vector>
#include "Observer.h"
class GaugeDlg;
class GaugeManager:public CObserver
{
public:
	GaugeManager(void);
	~GaugeManager(void);
	//更新数据，数据变化时调用,更新表盘
	virtual void Update(class CObservable* pObs, void* pArg = NULL);
	//增加表盘
	void addGaugeDlg(GaugeDlg* dlg);
	//删除表盘
	void delGaugeDlg(GaugeDlg* dlg);
	//保存数据
	int saveData();
	//读取数据
	int readData();
private:
	//所有表盘对话框
	std::vector<GaugeDlg*> m_vecGaugeDlg;
};
#endif
