#pragma once
#include <vector>
#include <set>
#include <map>
#include "DataDefine.h"
#include "Global.h"
class ChartManager;
class ChartXYData
{
public:
	ChartXYData();
	~ChartXYData();
	void setXYData(std::vector<CMeDPoint>& data);
	const std::vector<CMeDPoint>& getData();
	void setColor(CMeColor color);
	CMeColor getColor();
	void setID(int id);
	int getID();
private:
	std::vector<CMeDPoint> mData;
	CMeColor mColor;
	int ID;
	//bool mVisible;
};

class MarkPoint
{
public:
	MarkPoint();
	~MarkPoint();
	void paint();
};
enum ChartType{
	E_CHART_LINE,				//线
	E_CHART_STRIP,				//条状
	E_CHART_PIE,				//饼状
	E_CHART_STRIP_ADD,			//叠加条状图
	E_CHART_AREA,				//面积图
	E_CHART_POLAR_COORDINATE,	//极座标
	E_CHART_RADAR,				//雷达
	E_CHART_RADAR_FILL,			//雷达填充
	E_CHART_YINYANG,			//阴阳图
	E_CHART_HILO,				//小矿脉
	E_CHART_HILO_OC,			//小矿脉开合
	E_CHART_BUBBLE,				//泡泡
};
enum LineStyle{
	E_LINE_DOT,
	E_LINE_LINE,
	E_LINE_DOT_LINE,
};
enum MoveStyle{
	E_Y_SCROLL,
	E_Y_SHOWALL,
	E_Y_HANDLE,
};
class ChartXY
{
public:
	ChartXY(HDC hDC);
	~ChartXY();
	
	enum MouseInArea{
		E_AREA_NULL,
		E_AREA_X,
		E_AREA_Y,
		E_AREA_VIEW,
	};
public:
	void paintEvent();
	void mousePressEvent(CMeDPoint msPos);
	void mouseReleaseEvent(CMeDPoint msPos);
	void mouseMoveEvent(CMeDPoint msPos);
	void wheelEvent(CMeDPoint msPos, int deltaWh);
	void resize(CSize size);
	void showAll();
	void zoomIn(CMeDPoint cPt);
	void ZoomOut(CMeDPoint cPt);

	//virtual bool eventFilter(QObject *obj, QEvent *e);
protected:
	void drawAxesXY();
	void drawLine();
	void drawStrip();
	void drawSpripAdd();
	void drawPie();
	void drawArea();
	void drawPolarCoordinate();
	void drawRadar();
	void drawRadarFill();
	void drawYinYang();
	void drawHilo();
	void drawHiloOC();
	void drawBubble();

	CMeDPoint screen2xy(CMeDPoint pt);
	CMeDPoint xy2screen(CMeDPoint pt);

	void calcRecView();

public:
	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void getXRange(double& min, double& max);
	void getYRange(double& min, double& max);
	//void addChartData(ChartXYData* data);
	//const ChartXYData* chartData(int index);
	//void removeChartData(int index);
	//void removeAllChartData();
	int calNumDigit(double num);
	//数据理器
	void setChartMgr(ChartManager* mgr);
	const ChartManager* getChartMgr();
	//update visible and column
	void setVisible(int id, bool bShow);
	bool getVisible(int id);

	std::map<int, bool> getMapVisible() { return m_mapVisible; }
	void setMapVisible(std::map<int, bool> m) { m_mapVisible = m; }

	void setXID(int id);
	int getXID();
	//更新数据
	void updateData(class CGlobalDataManager* dbMgr);
	void refreshData();

	LineStyle getLineStyle() { return m_eLineStyle; }
	void setLineStyle(LineStyle e) { m_eLineStyle = e; }
	MoveStyle getMoveStyle() { return m_eMoveStyle; }
	void setMoveStyle(MoveStyle e) { m_eMoveStyle = e; }
	ChartType getChartType() { return m_eChartType; }
	void setChartType(ChartType e) { m_eChartType = e; }

private:
	ChartType m_eChartType;
	MoveStyle m_eMoveStyle;
	LineStyle m_eLineStyle;
	CRect m_recView;
	//范围
	double m_minX;
	double m_maxX;
	double m_minY;
	double m_maxY;
	bool m_bMouseLBtn;
	CMeDPoint m_oldMsPt;
	//鼠标所在区域
	MouseInArea m_eMouseArea;
	CRect m_recXAxes;
	CRect m_recYAxes;
	HDC m_hDC;
	CDC* m_pDC;
	CSize m_size;
	CDC* m_pMemDC;
	ChartManager* m_chartMgr;
	//不显示的列 ID
	std::map<int, bool> m_mapVisible;
	int m_nXID;
	CString m_strX;
	CString m_strY;
	//
	class CGlobalDataManager* m_dbMgr;
	std::vector<GROUPDATA> m_allData;
	//数据值 更改时要重新计算
	std::vector<ChartXYData> m_vecLineData;
};


