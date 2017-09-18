/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:Msg.h
Description:消息头定义
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef MSG_H
#define MSG_H

enum MESSAGE
{
	WM_BASE_INDEX = WM_USER + 100,

	//通知主框架点击了什么
	WM_NOTIFY_ACTIVE_WND_TYPE,

	//通知Grid的分组信息改变
	WM_NOTIFY_GRID_GROUP_INFO_CHANGE,

	//TreeCtrl上双击
	WM_NOTIFY_TREE_CTRL_DBCLICK,

	//监测到了新设备
	WM_NOTIFY_DETECT_DEVICE,

	//通知GridContainer点击了右键
	WM_NOTIFY_RBUTTON_DOWN,

	//设置对话框是否激活
	WM_SET_DLG_ACTIVE,

	//响应Grid的选项消息
	WM_NOTIFY_GRID_OPTION,

	//响应Grid的复制消息
	WM_NOTIFY_GRID_COPY,

	//响应Grid的粘贴消息
	WM_NOTIFY_GRID_PASTE,

	//响应Grid第一行的菜单消息
	WM_NOTIFY_GRID_FIRST_ROW,

	//响应Grid最后一行的菜单消息
	WM_NOTIFY_GRID_LAST_ROW,

	//响应Grid的插入行消息
	WM_NOTIFY_GRID_INSERT_ROW,

	//响应Grid的删除格子消息
	WM_NOTIFY_GRID_DEL_CELL,

	//响应Grid的增加数据列消息
	WM_NOTIFY_GRID_ADD_DATA_COLUMN,

	//响应Grid的生成数据消息
	WM_NOTIFY_GRID_YIELD_DATA,

	//响应Grid的运算消息
	WM_NOTIFY_GRID_CALCULATE,

	//响应Grid清除格子数据消息
	WM_NOTIFY_GRID_CLEAR_CELL_DATA,

	//响应Grid存储为Excel数据消息
	WM_NOTIFY_GRID_SAVE_AS_EXCEL,

	//响应Grid的打印消息
	WM_NOTIFY_GRID_PRINT,

	//响应Grid的打印预览消息
	WM_NOTIFY_GRID_PRINT_PREVIEW,

	//处理点击开始采集的开始变为停止
	WM_NOTIFY_RIBBON_CHANGE

	
};

enum COMMAND
{
	ID_BASE_COMMAND = WM_USER + 2000,

	//典型实验
	ID_TYPICAL_TEST,

	//输出实验报告
	ID_OUTPUT_TEST_REPORT,

	//选择传感器
	ID_SELECT_SENSOR,

	//自动识别
	ID_AUTO_SELECT,

	//手动选择
	ID_MANUAL_SELECT,

	//采集参数
	ID_COLLECT_PARAM,

	//数据配置
	ID_CONFIG_DATA,

	//添加表格
	ID_ADD_TABLE,

	//添加图
	ID_ADD_IMAGE,

	//添加仪表
	ID_ADD_DEVICE,

	//删除元素
	ID_DEL_ELEMENT,

	//开始
	ID_START,

	//记点
	ID_RECORD_POINT,

	//调零
	ID_ZERO,

	//校准
	ID_CALIBRATION,

	//取消调零与校准
	ID_CANCEL_ZERO_AND_CALIBRATION,

	//开始回放
	ID_START_REPLAY,

	//暂停回放
	ID_PAUSE_REPLAY,

	//数组
	ID_SELECT_ARRAY,

	//速度
	ID_SELECT_SPEED,

	//示波器
	ID_OSCILLOSCOPE,

	//二维运动合成与分解
	ID_2D_COMPOSE_AND_DECOMPOSE,

	//添加页面
	ID_ADD_PAGE,

	//删除页面
	ID_DEL_PAGE,

	//页面名称
	ID_SET_PAGE_NAME,

	//跳转页面
	ID_GOTO_PAGE,

	//选项
	ID_OPTION,

	ID_DEVICE_OPTION,
	//数值风格
	ID_NUM_STYLE,

	//表盘风格
	ID_WATCH_STYLE,

	//下一组数据列
	ID_NEXT_DATA_COLUMN,

	//复制
	ID_COPY,

	//粘贴
	ID_PASTE,

	//第一行
	ID_FIRST_ROW,

	//最后一行
	ID_LAST_ROW,

	//插入行
	ID_INSERT_ROW,

	//删除格子
	ID_DEL_CELL,

	//增加数据列
	ID_ADD_DATA_COLUMN,

	//生成数据
	ID_YIELD_DATA,

	//运算
	ID_CALCULATE,

	//清除子数据
	ID_CLEAR_CHILD_DATA,

	//存储为Excel
	ID_SAVE_EXCEL,

	//打印
	ID_PRINT,

	//打印预览
	ID_PRINT_PREVIEW,

	ID_CHART_OPTION,
	//拖动模式
	ID_DRAG_MODE,

	//选择模式
	ID_SELECT_MODE,

	//自动滚屏
	ID_AUTO_SCROLL,

	//自动缩屏
	ID_AUTO_ZOOM,

	//不滚屏
	ID_NO_SCROLL,

	//连线
	ID_LINE,

	//点
	ID_POINT,

	//点连线
	ID_POINT_TO_LINE,

	//100%
	ID_100_PERCENT,

	//放大
	ID_ZOOM_IN,

	//缩小
	ID_ZOOM_OUT,

	//查看
	ID_VIEW,

	//切线
	ID_TANGENT,

	//积分
	ID_INTEGRAL,

	//统计
	ID_STATISTICS,

	//直线拟合
	ID_LINEAR_FITTING,

	//拟合
	ID_FITTING,

	//保存突变
	ID_SAVE_IMAGE,

	//导出到实验报告
	ID_EXPORT_EXPERIMENT_REPORT,

	//实验指导
	ID_EXPERIMENT_GUIDE,

	ID_COMMAND_TOTAL,

	ID_MIN_COMMAND = ID_BASE_COMMAND + 1,
	ID_MAX_COMMAND = ID_COMMAND_TOTAL - 1
};

#endif