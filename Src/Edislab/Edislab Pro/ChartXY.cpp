#include "stdafx.h"
#include "ChartXY.h"
#define _USE_CTRL_MATH_DEFINES
#include <math.h>
#include <boost/foreach.hpp>
#include "Global.h"
#include "ChartManager.h"
#include "GridColumnGroupManager.h"
//#include "SensorIDGenerator.h"
#include "SensorManager.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "Utility.h"
#pragma warning(push)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4258)
using namespace std;
#define VIEW_X_EDGE (20)
#define VIEW_Y_EDGE (20)
#define AXES_WIDTH  (10)

ChartXYData::ChartXYData()
{
}
ChartXYData::~ChartXYData()
{
}
void ChartXYData::setXYData(std::vector<CMeDPoint>& data)
{
	mData = data;
}
const std::vector<CMeDPoint>& ChartXYData::getData()
{
	return mData;
}
void ChartXYData::setColor(CMeColor color)
{
	mColor = color;
}
CMeColor ChartXYData::getColor()
{
	return mColor;
}

#if 0
void ChartXYData::setID(int id)
{
	ID = id;
}
int ChartXYData::getID()
{
	return ID;
}
#endif

ChartXY::ChartXY(HDC hDC)
	: m_hDC(hDC)
	, m_minX(0)
	, m_maxX(100)
	, m_minY(0)
	, m_maxY(100)
	, m_eChartType(E_CHART_LINE)
	, m_eMouseArea(E_AREA_NULL)
	, m_eMoveStyle(E_X_SCROLL)
	, m_eLineStyle(E_LINE_LINE)
	, m_eOpeMode(E_OPE_DRAG)
	, m_bMouseLBtn(false)
	, m_bCheckVal(false)
	, m_bQieLine(false)
	, m_bStatistics(false)
{
	m_pDC = CDC::FromHandle(m_hDC);
	m_pMemDC = NULL;
	m_chartMgr = NULL;
	//begin ldh 0610
	std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
	for(int i = 0; i < ColumnGroupArray.size(); ++i)
	{
		for(int g = 0; g < ColumnGroupArray[i].ColumnArray.size(); ++g)
		{
			CString strTempName = ColumnGroupArray[i].ColumnArray[g].strColumnName;
			std::string strSensorName = Utility::WideChar2MultiByte(strTempName.GetBuffer(0));
			SENSOR_TYPE_KEY KeyID;
			if (CSensorManager::CreateInstance().QuerySensorIDByName(strSensorName,KeyID))
			{

				if (m_nXID.nSensorID < 0 && m_nXID.nSensorSerialID < 0)
				{
					m_nXID = KeyID;
				}

				if (m_nXID.nSensorID != KeyID.nSensorID || m_nXID.nSensorSerialID != KeyID.nSensorSerialID)
				{
					m_mapVisible[KeyID] = true;
				}
			}
			
		}

		if (!m_mapVisible.empty())
		{
			break;
		}
	}
	//end ldh 0610
	refreshData();
}

ChartXY::~ChartXY()
{
	//m_nXID = -1;
}

void ChartXY::setXID(const SENSOR_TYPE_KEY& id)
{
	m_nXID = id;
}

SENSOR_TYPE_KEY ChartXY::getXID()
{
	return m_nXID;
}
void ChartXY::paintEvent()
{
	m_pDC = CDC::FromHandle(m_hDC);
	if(!m_pDC)
	{
		return;
	}
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(m_hDC, m_size.cx, m_size.cy);
	m_pMemDC->SelectObject(hMemBitmap);

	CRgn rgn;
	CRect rc(0, 0, m_size.cx, m_size.cy);
	rc.InflateRect(-3, -3, -3, -3);

	rgn.CreateRectRgnIndirect(rc);
	m_pMemDC->SelectClipRgn(&rgn);
	//填充背景 
	m_pMemDC->FillSolidRect(0, 0, m_size.cx, m_size.cy, RGB(255, 255, 255));
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
		{
			drawLine();
		}
		break;
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
	DeleteObject(hMemBitmap);
	BitBlt(m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), m_pMemDC->m_hDC, rc.left, rc.top, SRCCOPY);
	//ReleaseDC(pMemDC);
	m_pMemDC->DeleteDC();
	delete m_pMemDC;
	m_pMemDC = NULL;
}
void ChartXY::mousePressEvent(CMeDPoint msPos)
{
	m_bMouseLBtn = true;
	m_oldMsPt = msPos;
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
			if(m_recXAxes.PtInRect(m_oldMsPt))
			{
				m_eMouseArea = E_AREA_X;
				//setCursor(QCursor(Qt::SizeHorCursor));
				
			}else if(m_recYAxes.PtInRect(m_oldMsPt))
			{
				m_eMouseArea = E_AREA_Y;
				//setCursor(QCursor(Qt::SizeVerCursor));
			}
			else
			{
				m_eMouseArea = E_AREA_VIEW;
				//setCursor(QCursor(Qt::ClosedHandCursor));
			}
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
}
void ChartXY::mouseReleaseEvent(CMeDPoint msPos)
{
	m_bMouseLBtn = false;
	m_oldMsPt = msPos;
	if(m_recXAxes.PtInRect(m_oldMsPt))
	{
		m_eMouseArea = E_AREA_X;
		//setCursor(QCursor(Qt::SizeHorCursor));

	}else if(m_recYAxes.PtInRect(m_oldMsPt))
	{
		m_eMouseArea = E_AREA_Y;
		//setCursor(QCursor(Qt::SizeVerCursor));
	}
	else
	{
		m_eMouseArea = E_AREA_VIEW;
		//setCursor(QCursor(Qt::OpenHandCursor));
	}
}
void ChartXY::mouseMoveEvent(CMeDPoint msPos)
{
	CMeDPoint pt = msPos;
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
			bool bPaint = false;
			if(!m_bMouseLBtn)
			{
				if(m_recXAxes.PtInRect(m_oldMsPt))
				{
					m_eMouseArea = E_AREA_X;
					//setCursor(QCursor(Qt::SizeHorCursor));

				}else if(m_recYAxes.PtInRect(m_oldMsPt))
				{
					m_eMouseArea = E_AREA_Y;
					//setCursor(QCursor(Qt::SizeVerCursor));
				}
				else
				{
					m_eMouseArea = E_AREA_VIEW;
					//setCursor(QCursor(Qt::OpenHandCursor));
				}
			}
			if(m_bMouseLBtn&&E_OPE_DRAG==m_eOpeMode)
			{
				CMeDPoint xyOld = screen2xy(m_oldMsPt);
				switch(m_eMouseArea)
				{
				case E_AREA_VIEW:
					{
						if(m_recView.PtInRect(pt))
						{
							CMeDPoint xyCur = screen2xy(pt);
							CMeDPoint delta = xyCur-xyOld;
							m_minX -= delta.x;
							m_maxX -= delta.x;
							m_minY -= delta.y;
							m_maxY -= delta.y;
						}
					}
					break;
				case E_AREA_X:
					{
						if(pt.x>m_recView.left)
						{
							double oldX = m_oldMsPt.x-m_recView.left;
							double newX = pt.x-m_recView.left;
							if(oldX<1&&oldX>=0) oldX = 1;
							if(oldX>-1&&oldX<=0) oldX = -1;
							if(newX<1&&newX>=0) newX = 1;
							if(newX>-1&&newX<=0) newX = -1;
							double ratio = abs(newX/oldX);
							//double xDelta = xyOld.x()-m_minX;
							m_maxX = m_minX + (m_maxX-m_minX)/ratio;
						}else
						{
							m_maxX = 110;
							if(m_minX>m_maxX)
								m_minX = 0;
						}
					}
					break;
				case E_AREA_Y:
					{
						if(pt.y<m_recView.bottom)
						{
							double oldY = m_oldMsPt.y-m_recView.bottom;
							double newY = pt.y-m_recView.bottom;
							if(oldY<1&&oldY>=0) oldY = 1;
							if(oldY>-1&&oldY<=0) oldY = -1;
							if(newY<1&&newY>=0) newY = 1;
							if(newY>-1&&newY<=0) newY = -1;
							double ratio = newY/oldY;
							//double xDelta = xyOld.x()-m_minX;
							m_maxY = m_minY + (m_maxY-m_minY)/ratio;
						}else
						{
							m_maxY = 10;
							if(m_minY>m_maxY)
								m_minY = 0;
						}
					}
					break;
				}
				bPaint = true;
			}
			//
			m_vecQieLine.clear();
			m_vecValQL.clear();
			if (m_bQieLine || m_bCheckVal)
			{
				bPaint = true;
				CMeDPoint mCPt = screen2xy(msPos);
				std::vector<ChartXYData>& chartData = m_vecLineData;
				int curIndex = 1;
				m_dCkLineX = -99999999;
				for (int i = 0; i < chartData.size(); ++i)
				{
					ChartXYData* pData = &(chartData[i]);
					if (pData)
					{
						//int id = pData->getID();
						SENSOR_TYPE_KEY id = pData->getID();
						if (getVisible(id))
						{
							for (int v = 0; v < pData->getData().size(); ++v)
							{
								if (v == pData->getData().size())
								{
									curIndex = max(curIndex, v + 1);
									m_dCkLineX = pData->getData()[v].x;
									break;
								}
								if (mCPt.x > pData->getData()[v].x && mCPt.x < pData->getData()[v + 1].x)
								{
									if ((mCPt.x - pData->getData()[v].x) > (pData->getData()[v + 1].x - mCPt.x))
									{
										curIndex = max(curIndex, v + 2);
										m_dCkLineX = pData->getData()[v + 1].x;
									}
									else
									{
										curIndex = max(curIndex, v + 1);
										m_dCkLineX = pData->getData()[v].x;
									}
								}
							}
						}
					}
				}
				//序号
				char strNo[32];
				sprintf(strNo, "序号:%d", curIndex);
				m_vecValQL.push_back(strNo);
				for (int i = 0; i < chartData.size(); ++i)
				{
					ChartXYData* pData = &(chartData[i]);
					if (pData)
					{
						SENSOR_TYPE_KEY id = pData->getID();
						string strRow;

						if (getVisible(id))
						{
							std::string name = CSensorManager::CreateInstance().QuerySensorNameByID(id);
							strRow = name + ":";
							char str[256];
							int size = static_cast<int>(pData->getData().size());
							int index = curIndex - 1;
							if (curIndex > size)
							{
								index = size - 1;
							}
							if (index >= 0)
							{
								if (m_bCheckVal)
								{
									sprintf(str, " %f  ", pData->getData()[index].y);
									strRow += str;
								}
								//斜率
								if (m_bQieLine&&pData->getData().size() > 1)
								{
									double rate = 0;
									CMeDPoint pDelta, p1, p2;
									CMeDPoint pCur = pData->getData()[index];
									if (index == 0)
									{
										p1 = pData->getData()[index];
										p2 = pData->getData()[1];
										pDelta = pData->getData()[1] - pData->getData()[index];
									}
									else if (index == pData->getData().size() - 1)
									{
										p1 = pData->getData()[index-1];
										p2 = pData->getData()[index];
										pDelta = pData->getData()[index] - pData->getData()[index - 1];
									}
									else
									{
										p1 = pData->getData()[index - 1];
										p2 = pData->getData()[index+1];
										pDelta = pData->getData()[index + 1] - pData->getData()[index - 1];
									}
									if (pDelta.x != 0)
									{
										rate = pDelta.y / pDelta.x*1000;
									}
									else
									{
										rate = 9999999999;
									}
									sprintf(str, "斜率 %f", rate);
									strRow += str;
									p1 = xy2screen(p1);
									p2 = xy2screen(p2);
									pCur = xy2screen(pCur);
									double len = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
									CMeDPoint sp, ep;
									sp.x = (p1.x - p2.x) / len * 50+ pCur.x;
									sp.y = (p1.y - p2.y) / len * 50 + pCur.x;
									ep.x = -(p1.x - p2.x) / len * 50 + pCur.x;
									ep.y = -(p1.y - p2.y) / len * 50 + pCur.x;
									sp = screen2xy(sp);
									ep = screen2xy(ep);
									CMeLine ln;
									ln.vecPts.push_back(sp);
									ln.vecPts.push_back(ep);
								}
							}
							m_vecValQL.push_back(strRow);
						}

					}
				}
			}
			
			if (bPaint)
			{
				paintEvent();
			}
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
	m_oldMsPt = pt;
	//repaint();
}
void ChartXY::wheelEvent(CMeDPoint msPos, int deltaWh)
{
	CMeDPoint pt = msPos;
	int delta = deltaWh/120;
	double ratio = 1.0;
	if(delta<0)
	{
		ratio = pow(1.1, abs(delta));
	}else
	{
		ratio = pow(0.9, abs(delta));
	}
	if(ratio!=1.0)
	{
		CMeDPoint ptxy = screen2xy(pt);
		m_minX = ptxy.x - (ptxy.x-m_minX)*ratio;
		m_maxX = ptxy.x - (ptxy.x-m_maxX)*ratio;
		m_minY = ptxy.y - (ptxy.y-m_minY)*ratio;
		m_maxY = ptxy.y - (ptxy.y-m_maxY)*ratio;
		//repaint();
	}
}
void ChartXY::setXRange(double min, double max)
{
	m_minX = min;
	m_maxX = max;
}
void ChartXY::setYRange(double min, double max)
{
	m_minY = min;
	m_maxY = max;
}
void ChartXY::getXRange(double& min, double& max)
{
	min = m_minX;
	max = m_maxX;
}
void ChartXY::getYRange(double& min, double& max)
{
	min = m_minY;
	max = m_maxY;
}
//void ChartXY::addChartData(ChartXYData* data)
//{
//	m_chartData.push_back(data);
//}
//const ChartXYData* ChartXY::chartData(int index)
//{
//	ChartXYData* ret = NULL;
//	if(index<m_chartData.size())
//	{
//		ret = m_chartData[index];
//	}
//	return ret;
//}
//void ChartXY::removeChartData(int index)
//{
//	vector<ChartXYData*>::iterator itr = m_chartData.begin();
//	int cnt = 0;
//	for(; itr!=m_chartData.end(); ++itr)
//	{
//		if(cnt==index)
//		{
//			delete *itr;
//			m_chartData.erase(itr);
//			break;
//		}
//	}
//}
//void ChartXY::removeAllChartData()
//{
//	for(int i=0; i<m_chartData.size(); ++i)
//	{
//		if(m_chartData[i])
//			delete m_chartData[i];
//	}
//	m_chartData.clear();
//}
void ChartXY::resize(CSize size)
{
	m_size = size;
	calcRecView();
}
void ChartXY::showAll()
{
	std::vector<ChartXYData>& chartData = m_vecLineData;//m_chartMgr->getChartData();
	for(int i=0; i<chartData.size(); ++i)
	{
		ChartXYData* pData = &chartData[i];
		if(pData)
		{
			SENSOR_TYPE_KEY id = pData->getID();
			if(getVisible(id))
			{
				const vector<CMeDPoint>& lineData = pData->getData();
				for(int v = 0; v<lineData.size(); ++v)
				{
					m_minX = min(lineData[v].x, m_minX);
					m_minY = min(lineData[v].y, m_minY);
					m_maxX = max(lineData[v].x, m_maxX);
					m_maxY = max(lineData[v].y, m_maxY);
				}
			}
		}
	}
	paintEvent();
}
void ChartXY::zoomIn(CMeDPoint cPt)
{
	CMeDPoint pt = cPt;
	double ratio = 1.1;
	if(ratio!=1.0)
	{
		CMeDPoint ptxy = screen2xy(pt);
		m_minX = ptxy.x - (ptxy.x-m_minX)*ratio;
		m_maxX = ptxy.x - (ptxy.x-m_maxX)*ratio;
		m_minY = ptxy.y - (ptxy.y-m_minY)*ratio;
		m_maxY = ptxy.y - (ptxy.y-m_maxY)*ratio;
		//repaint();
		paintEvent();
	}
}
void ChartXY::ZoomOut(CMeDPoint cPt)
{
	CMeDPoint pt = cPt;
	double ratio = 0.9;
	if(ratio!=1.0)
	{
		CMeDPoint ptxy = screen2xy(pt);
		m_minX = ptxy.x - (ptxy.x-m_minX)*ratio;
		m_maxX = ptxy.x - (ptxy.x-m_maxX)*ratio;
		m_minY = ptxy.y - (ptxy.y-m_minY)*ratio;
		m_maxY = ptxy.y - (ptxy.y-m_maxY)*ratio;
		//repaint();
		paintEvent();
	}
}
//bool ChartXY::eventFilter(QObject *obj, QEvent *e)
//{
//	if (e->type() == QEvent::Resize)
//	{
//		QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(e);
//		QSize size = resizeEvent->size();
//		calcRecView();
//		QWidget::resizeEvent(resizeEvent);
//	}
//	else if(QEvent::Move==e->type())
//	{
//		QSize sizeP = size();
//	}
//
//	return  QObject::eventFilter(obj, e);
//}
CMeDPoint ChartXY::screen2xy(CMeDPoint pt)
{
	CMeDPoint ptRet;
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
			double xratio = abs(m_maxX-m_minX)/m_recView.Width();
			double yratio = abs(m_maxY-m_minY)/m_recView.Height();
			double x = m_minX + (pt.x-m_recView.left)*xratio;
			double y = m_minY - (pt.y-m_recView.bottom)*yratio;//Y轴相反
			ptRet.x = x;
			ptRet.y = y;
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
	return ptRet;
}
CMeDPoint ChartXY::xy2screen(CMeDPoint pt)
{
	CMeDPoint ptRet;
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
			double xratio = abs(m_maxX-m_minX)/m_recView.Width();
			double yratio = abs(m_maxY-m_minY)/m_recView.Height();
			double x = m_recView.left + (pt.x-m_minX)/xratio;
			double y = m_recView.bottom - (pt.y-m_minY)/yratio;//Y轴相反
			ptRet.x = x;
			ptRet.y = y;
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
	return ptRet;
}
int ChartXY::calNumDigit(double num)
{
	int cntAll = 0;
	if(num<0)
		cntAll = 1;
	num = abs(num);
	int cntInt = 0;
	int cntDot = 0;
	if(num>=0)
	{
		double temp = num;
		while(temp>=1)
		{
			temp/=10;
			++cntInt;
		}
	}else
	{
		cntDot = 1;
		double temp = num;
		while(temp<1)
		{
			temp*=10;
			++cntDot;
		}
	}
	cntAll += (cntInt+cntDot);
	return cntAll;
}
void ChartXY::calcRecView()
{
	if(m_pDC)
	{
		m_pDC = CDC::FromHandle(m_hDC);
		m_recView = CRect(0, 0, m_size.cx, m_size.cy);
		m_recView.InflateRect(-VIEW_X_EDGE, -VIEW_Y_EDGE, -VIEW_X_EDGE, -VIEW_Y_EDGE);

		CFont font;
		BOOL bOk = font.CreatePointFont(90, _T("Arial"),m_pDC);
		CFont* oldFont = m_pDC->SelectObject(&font);
		CSize chSize = m_pDC->GetTextExtent(_T("8"));
		int lenCh = chSize.cx;			//painter.fontMetrics().width('8');
		int heiCh = chSize.cy;			//painter.fontMetrics().height();
		int left = 8*lenCh;
		int bottom = 3*heiCh;
		m_recView.InflateRect(-left, 0, -6*lenCh, -bottom);
		//释放
		m_pDC->SelectObject(oldFont);
		font.DeleteObject();
	}
}
void ChartXY::drawAxesXY()
{
	if(!m_pMemDC)
	{
		return;
	}
	CFont* oldFont = NULL;
	CPen* oldPen = NULL;
	switch(m_eChartType)
	{
		//直角座标系
	case E_CHART_LINE:
	case E_CHART_STRIP:
	case E_CHART_STRIP_ADD:
	case E_CHART_AREA:
		{
			CPoint zeroPt = xy2screen(CMeDPoint(0, 0));
			CFont font;
			font.CreatePointFont(90, _T("Arial"));
			oldFont = m_pMemDC->SelectObject(&font);
			CSize chSize = m_pMemDC->GetTextExtent(_T("8"));
			int lenCh = chSize.cx;
			int heiCh = chSize.cy;
			CPen penBlack, penGray;
			penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			penGray.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
			oldPen = m_pMemDC->SelectObject(&penGray);
			
			int x1, y1, x2, y2;
			//y axes
			y1 = m_recView.top;
			y2 = m_recView.bottom;
			if(zeroPt.x<m_recView.left||zeroPt.x>m_recView.right)
			{
				x2 = x1 = m_recView.left;
			}else
			{
				x2 = x1 = zeroPt.x;
			}
			m_recYAxes = m_recView;
			m_recYAxes.left = (x1-AXES_WIDTH);
			m_recYAxes.right = (x1+AXES_WIDTH);
			m_pMemDC->MoveTo(x1, y1);
			m_pMemDC->LineTo(x2, y2);

			//
			CRect rcTemp = m_recView;
			rcTemp.InflateRect(1, 1, 1, 1);
			//座标刻度
			//计算刻度间隔
			//Y 刻度
			double range = abs(m_maxY-m_minY);
			double minSpace = range/30;
			{
				//int cntAll = max(calNumDigit(m_maxX), calNumDigit(m_minX))+2;
				double lenScale = heiCh*2;	//刻度高度
				int maxScaleCnt = int(m_recView.Height()/lenScale);
				minSpace = range/maxScaleCnt;
				int pown = -10;
				int beishu = 1;
				while(pow(10., pown)-minSpace<-pow(10., -11))
				{
					
					if(2*pow(10., pown)-minSpace>-pow(10., -11))
					{
						beishu = 2;
						break;
					}
					if(5*pow(10., pown)-minSpace>-pow(10., -11))
					{
						beishu = 5;
						break;
					}
					beishu = 1;
					++pown;
				};
				double space = pow(10., pown)*beishu;
				double start = (int(m_minY/(space/2)))*(space/2);
				//for(double scale = start; scale<=m_maxY; scale+=(space/2))
				m_pMemDC->SelectObject(&penBlack);
				for(int i=0; start+i*space/2<=m_maxY; ++i)
				{
					double scale = start+i*space/2;
					CPoint pt = xy2screen(CMeDPoint(x1, scale));
					if(rcTemp.PtInRect(CPoint(m_recView.left+1, pt.y)))
					{
						//painter->drawLine( x1-2, pt.y, x1+2, pt.y);
						m_pMemDC->MoveTo(x1-2, pt.y);
						m_pMemDC->LineTo(x1+2, pt.y);
					}
				}

				start = (int(m_minY/space))*space;
				//网格横线
				for(int i=0; start+i*space<=m_maxY; ++i)
				{
					double scale = start+i*space;
					CPoint pt = xy2screen(CMeDPoint(x1, scale));
					if(rcTemp.PtInRect(CMeDPoint(m_recView.left+1, pt.y)))
					{
						m_pMemDC->SelectObject(&penGray);
						m_pMemDC->MoveTo(m_recView.left, pt.y);
						m_pMemDC->LineTo(m_recView.right, pt.y);
						m_pMemDC->SelectObject(&penBlack);
						int nPre = 0;
						if(pown<0)
						{
							nPre = abs(pown);
						}
						if(0==scale&&m_minX<0)
						{
						}else
						{
							//描绘刻度值
							CString str;
							str.Format(_T("%.*f"), nPre, scale);
							int w = m_pMemDC->GetTextExtent(str).cx;
							m_pMemDC->TextOut(x1-w-3, pt.y-heiCh/2, str);
						}
					}
				}
				LOGFONT stFont; //font definition
				//Set font attributes that will not change.
				memset (&stFont, 0, sizeof (LOGFONT));
				stFont.lfHeight=16;
				stFont.lfWeight=FW_NORMAL;
				stFont.lfEscapement = 900;
				//stFont.lfClipPrecision=LCIP_LH_ANGLES;
				//strcpy (stFont.lfFaceName, _T("Arial"));
				CFont fontV;
				fontV.CreateFontIndirect(&stFont);
				m_pMemDC->SelectObject(&fontV);
				CPoint pt = xy2screen(CMeDPoint(max(0.0, m_minX), (m_maxY+m_minY)/2));
				//m_strY = _T("te     st");
				int w = m_pMemDC->GetTextExtent(m_strY).cx;
				m_pMemDC->TextOut(pt.x-heiCh*2.5-3, pt.y+w/2, m_strY);
				m_pMemDC->SelectObject(&font);
			}


			//x axes
			x1 = m_recView.left;
			x2 = m_recView.right;
			if(zeroPt.y<m_recView.top||zeroPt.y>m_recView.bottom)
			{
				y2 = y1 = m_recView.bottom;
			}else
			{
				y2 = y1 = zeroPt.y;
			}
			m_recXAxes = m_recView;
			m_recXAxes.top = (y1-AXES_WIDTH);
			m_recXAxes.bottom = (y1+AXES_WIDTH);
			m_pMemDC->MoveTo(x1, y1);
			m_pMemDC->LineTo(x2, y2);
			range = abs(m_maxX-m_minX);
			//计算刻度位数
			{
				int cntAll = max(calNumDigit(m_maxX), calNumDigit(m_minX))+5;
				int lenScale = lenCh*cntAll;	//刻度长度
				int maxScaleCnt = m_recView.Width()/lenScale;
				minSpace = range/maxScaleCnt;
				int pown = -10;
				int beishu = 1;
				while(pow(10., pown)-minSpace<-pow(10., -11))
				{
					if(2*pow(10., pown)-minSpace>-pow(10., -11))
					{
						beishu = 2;
						break;
					}
					if(5*pow(10., pown)-minSpace>-pow(10., -11))
					{
						beishu = 5;
						break;
					}
					beishu = 1;
					++pown;
				};
				double space = pow(10., pown)*beishu;
				double start = (int(m_minX/(space/2)))*(space/2);
				//for(double scale = start; scale<=m_maxX; scale+=space/2)
				m_pMemDC->SelectObject(penBlack);
				for(int i=0; start+i*space/2<=m_maxX; ++i)
				{
					double scale = start+i*space/2;
					CPoint pt = xy2screen(CMeDPoint(scale, y1));
					if(rcTemp.PtInRect(CMeDPoint(pt.x, m_recView.bottom-1)))
					{
						//painter->drawLine(pt.x(), y1-2, pt.x(), y1+2);
						m_pMemDC->MoveTo(pt.x, y1-2);
						m_pMemDC->LineTo(pt.x, y1+2);
					}
				}
				start = (int(m_minX/space))*space;
				for(int i=0; start+i*space<=m_maxX; ++i)
				{
					double scale = start+i*space;
					CPoint pt = xy2screen(CMeDPoint(scale, y1));
					if(rcTemp.PtInRect(CMeDPoint(pt.x, m_recView.bottom-1)))
					{
						m_pMemDC->SelectObject(penGray);
						m_pMemDC->MoveTo(pt.x, m_recView.top);
						m_pMemDC->LineTo(pt.x, m_recView.bottom);
						m_pMemDC->SelectObject(penBlack);

						int nPre = 0;
						if(pown<0)
						{
							nPre = abs(pown);
						}
						if(0==scale&&m_minY<0)
						{
						}else
						{
							//QString str = QString::number(scale, 'f', nPre);
							//int w = painter->fontMetrics().width(str);
							//painter->drawText(pt.x()-w/2, y1+3+heiCh, str);
							CString str;
							str.Format(_T("%.*f"), nPre, scale);
							int w = m_pMemDC->GetTextExtent(str).cx;
							m_pMemDC->TextOut(pt.x-w/2, y1+3/*+heiCh*/, str);
						}
					}
				}
				CPoint pt = xy2screen(CMeDPoint((m_maxX+m_minX)/2, max(0.0, m_minY)));
				int w = m_pMemDC->GetTextExtent(m_strY).cx;
				m_pMemDC->TextOut(pt.x-w/2, y1+3+heiCh, m_strX);
			}
		}
		break;
	case E_CHART_POLAR_COORDINATE:
	case E_CHART_PIE:
	case E_CHART_RADAR:
	case E_CHART_RADAR_FILL:
	case E_CHART_YINYANG:
	case E_CHART_HILO:
	case E_CHART_HILO_OC:
	case E_CHART_BUBBLE:
		{
		}
		break;
	}
}
void ChartXY::drawLine()
{
	calcXYRange();
	drawAxesXY();
	if (!m_chartMgr)
	{
		//return;
	}
	//drawLine
	CMeDPoint zeroPt = xy2screen(CMeDPoint(0, 0));
	CPen* oldPen = m_pMemDC->GetCurrentPen();
	int lineWidth(2);
	//m_pMemDC->set
	CRgn rgn;
	rgn.CreateRectRgn(m_recView.left, m_recView.top, m_recView.right, m_recView.bottom);
	m_pMemDC->SelectClipRgn(&rgn);
	std::vector<ChartXYData>& chartData = m_vecLineData;
	for (int i = 0; i < chartData.size(); ++i)
	{
		ChartXYData* pData = &(chartData[i]);
		if (pData)
		{
			SENSOR_TYPE_KEY id = pData->getID();
			if (getVisible(id))
			{
				CMeColor color = pData->getColor();
				CPen penLine;
				COLORREF col = RGB(color.getRed() * 255, color.getGreen() * 255, color.getBlue() * 2255);
				penLine.CreatePen(PS_SOLID, lineWidth, col);
				m_pMemDC->SelectObject(&penLine);
				const vector<CMeDPoint>& lineData = pData->getData();
				for (int v = 1; v < lineData.size(); ++v)
				{
					CMeDPoint pt1 = xy2screen(lineData[v - 1]);
					CMeDPoint pt2 = xy2screen(lineData[v]);
					switch (m_eLineStyle)
					{
					case E_LINE_DOT:
						if (v == 1)
						{
							m_pMemDC->Ellipse(pt1.x - 1, pt1.y - 1, pt1.x + 1, pt1.y + 1);
						}
						m_pMemDC->Ellipse(pt2.x - 1, pt2.y - 1, pt2.x + 1, pt2.y + 1);
						break;
					case E_LINE_LINE:
						m_pMemDC->MoveTo(pt1);
						m_pMemDC->LineTo(pt2);
						break;
					case E_LINE_DOT_LINE:
						m_pMemDC->MoveTo(pt1);
						m_pMemDC->LineTo(pt2);
						if (v == 1)
						{
							m_pMemDC->Ellipse(pt1.x - 1, pt1.y - 1, pt1.x + 1, pt1.y + 1);
						}
						m_pMemDC->Ellipse(pt2.x - 1, pt2.y - 1, pt2.x + 1, pt2.y + 1);
						break;
					default:
						break;
					}

				}
			}
		}
	}
	//切线和检查
	CPen penLine;
	COLORREF col = RGB(0, 0, 0);
	penLine.CreatePen(PS_SOLID, 2, col);
	m_pMemDC->SelectObject(&penLine);
	for (int i = 0; i < m_vecQieLine.size(); ++i)
	{

		CMeLine ln = m_vecQieLine[i];
		if (ln.vecPts.size() > 1)
		{
			for (int v = 0; v < ln.vecPts.size(); ++v)
			{
				CMeDPoint pt = xy2screen(ln.vecPts[v]);
				if (0 == v)
				{
					m_pMemDC->MoveTo(pt);
				}
				else
				{
					m_pMemDC->LineTo(pt);
				}
			}
		}
	}
	CFont font;
	font.CreatePointFont(90, _T("Arial"));
	CFont* oldFont = m_pMemDC->SelectObject(&font);
	CSize chSize = m_pMemDC->GetTextExtent(_T("8"));
	int hei = static_cast<int>((chSize.cy + 2)*m_vecValQL.size());
	CRect rcQ(m_size.cx - 255 - 3, 5, m_size.cx - 5, hei + 5);
	m_pMemDC->FillSolidRect(rcQ, RGB(255, 255, 255));
	for (int i = 0; i < m_vecValQL.size(); ++i)
	{
		CString str;
		str.Format(_T("%s"), m_vecValQL[i]);
		m_pMemDC->TextOut(rcQ.left, rcQ.top + (chSize.cy + 2)*i, str);
	}
	//统计
	if (m_bStatistics)
	{
		vector<string> vecStr;
		for (int i = 0; i < chartData.size(); ++i)
		{
			ChartXYData* pData = &(chartData[i]);
			if (pData)
			{
				SENSOR_TYPE_KEY id = pData->getID();
				string strRow;
				if (getVisible(id) && pData->getData().size())
				{
					std::string name = CSensorManager::CreateInstance().QuerySensorNameByID(id);
					vecStr.push_back("");
					vecStr.push_back(name);
					char str[256];
					double minV(999999999), maxV(-9999999999), total(0);
					int indexMin(0), indexMax(0);
					int cnt = 0;
					for each(CMeDPoint pt in pData->getData())
					{
						++cnt;
						total += pt.y;
						if (minV > pt.y)
						{
							minV = pt.y;
							indexMin = cnt;
						}
						if (maxV < pt.y)
						{
							maxV = pt.y;
							indexMax = cnt;
						}
					}
					double avg = total / pData->getData().size();
					sprintf(str, "最小:%f 序号:%d", minV, indexMin);
					vecStr.push_back(str);
					sprintf(str, "最大:%f 序号:%d", maxV, indexMax);
					vecStr.push_back(str);
					sprintf(str, "共%d点，平均:%f", pData->getData().size(), avg);
					vecStr.push_back(str);
				}
			}
		}
		int hei = static_cast<int>((chSize.cy + 2)*vecStr.size());
		CRect rcS(m_size.cx - 255 - 3, rcQ.bottom, m_size.cx - 5, hei + rcQ.bottom);
		m_pMemDC->FillSolidRect(rcS, RGB(255, 255, 255));
		for (int i = 0; i < vecStr.size(); ++i)
		{
			CString str;
			str.Format(_T("%s"), vecStr[i]);
			m_pMemDC->TextOut(rcS.left, rcS.top + (chSize.cy + 2)*i, str);
		}
	}
	m_pMemDC->SelectObject(oldPen);
	rgn.DeleteObject();
}
void ChartXY::drawStrip()
{
}
void ChartXY::drawSpripAdd()
{
}
void ChartXY::drawPie()
{
}
void ChartXY::drawArea()
{
}
void ChartXY::drawPolarCoordinate()
{
}
void ChartXY::drawRadar()
{
}
void ChartXY::drawRadarFill()
{
}
void ChartXY::drawYinYang()
{
}
void ChartXY::drawHilo()
{
}
void ChartXY::drawHiloOC()
{
}
void ChartXY::drawBubble()
{
}

void ChartXY::setChartMgr(ChartManager* mgr)
{
	m_chartMgr = mgr;
}
const ChartManager* ChartXY::getChartMgr()
{
	return m_chartMgr;
}
void ChartXY::setVisible(SENSOR_TYPE_KEY id, bool bShow)
{
	m_mapVisible[id] = bShow;
}
bool ChartXY::getVisible(SENSOR_TYPE_KEY id)
{
	if(m_mapVisible.find(id) != m_mapVisible.end())
	{
		return m_mapVisible[id];
	}
	return false;
}
//更新数据
void ChartXY::refreshData()
{
	m_vecLineData.clear();
	//if(m_nXID==-1)
	//{
	//	return;
	//}
	//begin ldh 0610
	vector<float> xData;
	CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(m_nXID);
	if(pData)
	{
		pData->GetSensorData(xData);
	}
	std::string name = CSensorManager::CreateInstance().QuerySensorNameByID(m_nXID);
	m_strX = CString(name.c_str());
	//end ldh 0610
	if(xData.size()==0)
	{
		//return;
	}
	//数据匹配
	m_strY.Empty();
	//begin ldh 0610
	BOOST_FOREACH(auto it,m_mapVisible)
	{
		if(it.second)
		{
			vector<float> yData;
			CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(it.first);
			if(pData)
			{
				pData->GetSensorData(yData);
			}
			ChartXYData line;
			line.setID(it.first);
			COLORREF col = RGB(0, 0, 0);
			line.setColor(CMeColor(GetRValue(col)/255.0, GetGValue(col)/255.0, GetBValue(col)/255.0));
			std::vector<CMeDPoint> vecPt;
			for(int i=0; i<xData.size()&&i<yData.size(); ++i)
			{
				CMeDPoint pt;
				pt.x = xData[i];
				pt.y = yData[i];
				vecPt.push_back(pt);
			}
			//如果有点则插入线
			if(vecPt.size()>0)
			{
				line.setXYData(vecPt);
				m_vecLineData.push_back(line);
			}
			std::string name = CSensorManager::CreateInstance().QuerySensorNameByID(it.first);
			m_strY += _T(" ") + CString(name.c_str());
		}
	}
	//end ldh 0610

}
void ChartXY::updateData()
{
#if 0
	m_dbMgr = &(CGlobalDataManager::CreateInstance());
	if(!m_dbMgr)
	{
		return;
	}
	m_allData = m_dbMgr->getAllData();
#endif
	boost::unordered_map<SENSOR_TYPE_KEY, bool> oldVisible = m_mapVisible;
	m_mapVisible.clear();
	std::vector<SENSOR_TYPE_INFO_ELEMENT> SensorNameArray;
	CSensorManager::CreateInstance().GetSensorList(SensorNameArray);
	for(int i = 0; i < (int)SensorNameArray.size(); ++i)
	{
		
		SENSOR_TYPE_KEY nSensorID(SensorNameArray[i].nSensorID,SensorNameArray[i].nSensorSerialID);
		//int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(SensorNameArray[i]);
		if(oldVisible.find(nSensorID) != oldVisible.end())
		{
			m_mapVisible[nSensorID] = oldVisible[nSensorID];
		}
		else
		{
			m_mapVisible[nSensorID] = false;
		}
		
	}
	//end modify by xiaowei.han
	refreshData();
	paintEvent();
}
void ChartXY::calcXYRange()
{
	switch (m_eMoveStyle)
	{
	case E_X_HANDLE:
		break;
	case E_X_SCROLL:
	{
		double xRange = m_maxX - m_minX;
		std::vector<ChartXYData>& chartData = m_vecLineData;//m_chartMgr->getChartData();
		double minX(DBL_MAX), maxX(-DBL_MAX);
		for (int i = 0; i < chartData.size(); ++i)
		{
			ChartXYData* pData = &chartData[i];
			if (pData)
			{
				SENSOR_TYPE_KEY id = pData->getID();
				if (getVisible(id))
				{
					const vector<CMeDPoint>& lineData = pData->getData();
					for (int v = 0; v < lineData.size(); ++v)
					{
						minX = min(lineData[v].x, minX);
						maxX = max(lineData[v].x, maxX);
					}
				}
			}
		}
		if (maxX > m_maxX)
		{
			m_maxX = maxX;
			minX = m_maxX - xRange;
		}
	}
	break;
	case E_X_SHOWALL:
	{
		std::vector<ChartXYData>& chartData = m_vecLineData;//m_chartMgr->getChartData();
		for (int i = 0; i < chartData.size(); ++i)
		{
			ChartXYData* pData = &chartData[i];
			if (pData)
			{
				SENSOR_TYPE_KEY id = pData->getID();
				if (getVisible(id))
				{
					const vector<CMeDPoint>& lineData = pData->getData();
					for (int v = 0; v < lineData.size(); ++v)
					{
						m_minX = min(lineData[v].x, m_minX);
						m_minY = min(lineData[v].y, m_minY);
						m_maxX = max(lineData[v].x, m_minX);
						m_maxY = max(lineData[v].y, m_minY);
					}
				}
			}
		}
	}
	break;
	}
	
}
#pragma warning(pop)