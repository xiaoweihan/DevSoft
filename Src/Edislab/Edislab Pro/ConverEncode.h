#ifndef CONVER_ENCODE_H
#define CONVER_ENCODE_H
#include <string>
class ConverEncode
{
public:
	ConverEncode(void);
	~ConverEncode(void);
	//utf_8转为gb2312
	static void UTF_8ToGB2312(std::string &strGB2312Content, char* pSrc, int nSrcLength);
	//gb2312 转utf_8
	static void GB2312ToUTF_8(std::string& pOut, char* szSrc, int pLen);
	//urlgb2312编码
	static std::string UrlGB2312(char* str);   
	//urlutf8 编码
	static std::string UrlUTF8(char* str);  
	//urlutf8解码
	static std::string UrlUTF8Decode(std::string str);   
	//urlgb2312解码
	static std::string UrlGB2312Decode(std::string str);  
	//UTF-8->ASCII编码
	static std::string UTF_8ToASCII(std::string& strSrc);

private:
	static void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	static void UTF_8ToUnicode(WCHAR* pOut,char *pText);
	static void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	static void UnicodeToGB2312(char* pOut,WCHAR uData);
	static char CharToInt(char ch);
	static char StrToBin(char* str);
};

#endif