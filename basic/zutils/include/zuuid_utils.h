#pragma once
#include "zutils_sdk.h"
#include"zstring.h"
#include"encode/zhash.h"
#include<unordered_map>
/**********************************************************************
 *@brief  生成UUID相关信息
 ***********************************************************************/
ZUTILS_NS_BEGIN
enum MODTYPE:int {
	LOGIC_MOD=0,
	PAGE_MOD,
	SHARE_MOD,
};

static std::unordered_map<MODTYPE, ZString> mod_str_{
{LOGIC_MOD,"logic"},{PAGE_MOD,"page"} ,{SHARE_MOD,"shared"}
};

struct UUIDINFO
{
	MODTYPE mod_type;
	ZString main_content;
	ZString extend;
};


class ZUTILS_API ZUUIDUtils
{
public:
	//static void generateUUIDString(const char* str,ZString &uuid);
	static void generateUUIDINT(UUIDINFO& info, int64_t& uuid);
private:
	static void generateUUIDINT(const char* str, int64_t & uuid);


};
ZUTILS_NS_END