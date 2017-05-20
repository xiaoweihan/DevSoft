/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:DumpFileSwitch.h
Description:Dump文件开关器
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef DUMP_FILE_SWITCH_H
#define DUMP_FILE_SWITCH_H
class CDumpFileSwitch
{
public:
    static CDumpFileSwitch& CreateInstance(void);

    void SetDumpFilePath(const char* szDumpFilePath);
    void OpenSwitch(void);
    //通过修改注册表生成dmp文件 （1:minidump 2:fulldump）
    void OpenSwitchWithReg(int nType,const char* szExeName);
private:
    CDumpFileSwitch(void);
    ~CDumpFileSwitch(void);

private:
	static CDumpFileSwitch s_obj;
};



#endif