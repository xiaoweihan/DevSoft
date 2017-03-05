/****************************************************************************************
*版权所有:
*文件名:Observer.h
*描述:观察者模式--观察者
*日期:2016.9
******************************************************************************************/
#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdio.h>

class CObservable;

//观察者，纯虚基类
//作为观察者需继承此纯虚基类，do your something
class CObserver
{
public:
	
    CObserver();
    virtual ~CObserver();
 
public:
    //当被观察的目标发生变化时，通知调用该方法
    //来自被观察者pObs, 扩展参数为pArg
    virtual void Update(CObservable* pObs, void* pArg = NULL) = 0;
};

#endif