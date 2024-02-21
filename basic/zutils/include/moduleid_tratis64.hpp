#pragma once
#include <cstdint>
#include <string>
#include <stdexcept>
/****************************************************************************************************
 *@file  moduleid_tratis64.hpp
 *@brief 
	模块ID和模块枚举标识值定义规则如下:
	1、每个模块枚举标识值, 占4=8字节, 共64位
	2、8个字节按如下规则划分:
		 +----------+-------------------------+--------------+---------------+
		 | 63[1bit] |      62 - 13 [50bit]    | 12 - 11[2bit]| 10 - 0[11bit] |
		 +----------+-------------------------+--------------+---------------+
		 |    s     | aaaaa bbbbb ... jjjjj   |     xx       |  yyyyyyyyyyy  |
		 +----------+-------------------------+--------------+---------------+
	3、每个二进制位的含义如下:
		 s     - 符号位
		 aaaaa - 第1个标识字符的索引值,因为一共26个字母，可以将A~Z映射为数字1~26，存储每个字母需要5bit
		 bbbbb - 第2个标识字符的索引值
		 ccccc - 第3个标识字符的索引值
				 ..........
		 jjjjj - 第10个标识字符的索引值
		 xx          - 类型ID值(取值范围为: 0 - 3)
					   00: UI模块中的枚举值定义
					   01: Logic模块中的枚举值定义
					   10: Service模块中的枚举值定义
		 yyyyyyyyyyy - 模块枚举值
*************************************************************************************************/
constexpr int tag_char_cnt64 = 10;
constexpr int tag_char_bits64 = 5;
constexpr int append_id_bits64 = 2;
constexpr int sn_val_bits64 = 11;
constexpr int64_t int_max64 = 0x7FFFFFFFFFFFFFFF;
// 模块类型ID定义
constexpr uint8_t uimod_typeid = 0x00;      // 界面模块
constexpr uint8_t logicmod_typeid = 0x01;   // 逻辑模块
constexpr uint8_t servicemod_typeid = 0x02; // 服务模块

/************************************************************
  @brief 将字符转换成对应的索引值(非对外接口)
  @note 在C++11中constexpr函数要实现编译运行,只能有一行代码
		 将字母转换成对应的索引值,不区分大小写,默认按大写处理
		 有效字符为: 'a'-'z'/'A'-'Z'/'-'/'_'
		 +-------------+-----+-----+--------+
		 | 'A' - 'Z'   | '-' | '_' | other  |
		 +-------------+-----+-----+--------+
		 |  1 - 26     |  27 |  28 |   0    |
		 +-------------+-----+-----+--------+
  @param[in] ch 字符
  @return 字符对应的索引值
 */
inline constexpr int _char_to_chidx(const char ch)
{
	return (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 1)
		: ((ch >= 'a' && ch <= 'z') ? (ch - 'a' + 1)
			: (ch == '-' ? 27 : (ch == '_' ? 28 : 0)));
}

/**
  @brief 将索引值转换成大写字母,若索引值无效,则返回 '*'(非对外接口)
  @param[in] ch_idx 字符的索引值
  @return 索引值对应的大写字母
 */
inline constexpr char _chidx_to_upperchar(const int ch_idx)
{
	return (ch_idx >= 1 && ch_idx <= 26) ? (ch_idx - 1 + 'A')
		: (ch_idx == 27 ? '-' : (ch_idx == 28 ? '_' : '*'));
}

// 按格式定义规则, 左移索引值, pos 从0开始编号,取值为 0 - 9
inline constexpr int64_t _left_move_idxval64(const int idxval, const int pos)
{
	return ((int64_t)idxval << ((tag_char_cnt64 - pos - 1) * tag_char_bits64 + append_id_bits64 + sn_val_bits64));
}

// 按格式定义规则, 左移mod_typeid
inline constexpr int64_t _left_move_mod_typeid64(const int mod_typeid)
{
	return ((int64_t)(mod_typeid & 0x03) << sn_val_bits64);
}

// 求枚举值的绝对值
inline constexpr int64_t _abs_enumval64(const int64_t enum_val)
{
	return (enum_val > 0 ? enum_val : -enum_val);
}

/*******************************************************************************
  @brief 外部调用接口 
*******************************************************************************/
/**
  @brief 求字符串标识的长度
  @note  因为需要去除结束符'\0',所以需要减 1
  @param[in] strTag 字符串标识
  @return 字符串标识的长度
*/
#define module_tag_len(strTag) (sizeof(strTag) - 1)

/**
 @brief 根据模块标识生成模块的唯一ID(常量函数)
 @note  对外提供的接口,利用字符的映射解决整型ID的唯一性
 @param[in]  mod_tag 模块标识
 @param[in] mod_typeid 模块类型ID
 @return 模块的唯一标识
*/
template<int TagLen>
inline constexpr int64_t module_uid64(const char* mod_tag, const int mod_typeid)
{
	static_assert(TagLen <= 10, "tag len invalid, mod_tag maxinum len 10 chars");
	return _left_move_idxval64(_char_to_chidx(mod_tag[TagLen - 1]), TagLen - 1) | module_uid64<TagLen - 1>(mod_tag, mod_typeid);
}

template<>
inline constexpr int64_t module_uid64<1>(const char* mod_tag, const int mod_typeid)
{
	return _left_move_idxval64(_char_to_chidx(mod_tag[0]), 0) | _left_move_mod_typeid64(mod_typeid);
}

/**
 @brief 根据模块标识生成模块的唯一ID(普通函数)
 @note  对外提供的接口,利用字符的映射解决整型ID的唯一性
 @param[in]  mod_tag 模块标识
 @param[in]  tag_len 模块标识的长度
 @param[in] mod_typeid 模块类型ID
 @return 模块的唯一标识
*/
inline int64_t module_uid64(const char* mod_tag, const int tag_len, const int mod_typeid)
{
	if (tag_len == 1) {
		return _left_move_idxval64(_char_to_chidx(mod_tag[0]), 0) | _left_move_mod_typeid64(mod_typeid);
	}
	if (tag_len > 10) {
		throw std::runtime_error("tag len invalid, mod_tag maxinum len 10 chars");
	}
	return _left_move_idxval64(_char_to_chidx(mod_tag[tag_len - 1]), tag_len - 1) | module_uid64(mod_tag, tag_len - 1, mod_typeid);
}

/**
  @brief 生成模块中唯一的标识值(用于标识命令和事件通知)
  @param[in] mod_uid 模块标识
  @param[in] val 枚举值(最大取值为2048)
  @return 模块中唯一的标识值
 */
inline constexpr int64_t module_enumval64(const int64_t mod_uid, const int val)
{
	return val >= 0 ? (mod_uid | val) : -(mod_uid | -val);
}

/**
 \brief 提取枚举值中的val
 \param[in] enum_val 枚举值
 \return 枚举值中的val
 */
inline constexpr int pure_enumval64(const int64_t enum_val)
{
	// 去除标识值的符号,附加类型值
	return (int)(_abs_enumval64(enum_val) & 0x7FF) * (enum_val > 0 ? 1 : -1);
}

/**
 @brief 提取枚举值中的模块ID
 @param[in] enum_val 模块枚举值
 @return 模块ID
 */
inline constexpr int64_t pure_module_uid64(const int64_t enum_val)
{
	return (enum_val > 0 ? enum_val : -enum_val) & (int_max64 & (int_max64 << (sn_val_bits64)));
}

/**
  @brief 提取模块标识,全大写字母表示
  @param[in] enum_val 枚举值
  @return 模块标识,全大写字母表示
 */
inline std::string module_uppertag64(const int64_t enum_val)
{
	// 去除mod_typeid和自定义value的模块ID
	int64_t pure_uid = pure_module_uid64(enum_val);
	char type_id = (pure_uid >> sn_val_bits64) & 0x03;
	int64_t pure_uidval = pure_uid >> (append_id_bits64 + sn_val_bits64); // 纯模块ID值
	// 模块Tag
	std::string str_tag = "";
	for (int i = tag_char_cnt64 - 1; i >= 0; i--) {
		int idx = (pure_uidval >> (i * 5)) & 0x1F;
		if (idx > 0) {
			str_tag += _chidx_to_upperchar(idx);
		}
	}
	if (pure_uid > 0) {
		if (type_id == uimod_typeid) {
			str_tag += "#UI";
		}
		else if (type_id == logicmod_typeid) {
			str_tag += "#Logic";
		}
		else if (type_id == servicemod_typeid) {
			str_tag += "#Service";
		}
	}
	return str_tag;
}

// 视图层模块的唯一ID
template<int TagLen>
inline constexpr int64_t uimodule_uid64(const char* mod_tag)
{
	return module_uid64<TagLen>(mod_tag, uimod_typeid);
}

// 服务模块的唯一ID
template<int TagLen>
inline constexpr int64_t servicemodule_uid64(const char* mod_tag)
{
	return module_uid64<TagLen>(mod_tag, servicemod_typeid);
}

// 非constexpr常量函数
inline int64_t servicemodule_uid64(const char* mod_tag, const int tag_len)
{
	return module_uid64(mod_tag, tag_len, servicemod_typeid);
}

// 逻辑模块的唯一ID
template<int TagLen>
inline constexpr int64_t logicmodule_uid64(const char* mod_tag)
{
	return module_uid64<TagLen>(mod_tag, logicmod_typeid);
}

// 非constexpr常量函数
inline int64_t logicmodule_uid64(const char* mod_tag, const int tag_len)
{
	return module_uid64(mod_tag, tag_len, logicmod_typeid);
}
