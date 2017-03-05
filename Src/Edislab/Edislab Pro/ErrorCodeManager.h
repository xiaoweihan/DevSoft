/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:ErrorCodeManager.h
Description:错误码管理类
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef ERROR_CODE_MANAGER_H
#define ERROR_CODE_MANAGER_H
#include <map>
#include <string>
class CErrorCodeManager
{

public:
    //单实例
    static CErrorCodeManager& CreateInstance(void);  
public:
    //根据错误码查询错误描述
    std::string QueryErrorInfoByErrorCode(int nErrorCode);
private:
    CErrorCodeManager(void);
    ~CErrorCodeManager(void);
private:
    //初始化
    void Init(void);
    //反初始化
    void UnInit(void);
private:
    std::map<int,std::string> m_ErrorCodeMap;

};

#endif