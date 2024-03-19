#pragma once
#include <memory>
#include"zstring.h"
#include "zdatabase_sdk.h"

ZDATABASE_NS_BEGIN
enum class SqlOperType :int {
    None = 0,   /**< 无 */
    SQL_Open,   /**< 数据库打开 */
    SQL_Close,  /**< 数据库关闭 */
    SQL_Insert, /**< 增加 */
    SQL_Delete, /**< 删除 */
    SQL_Update, /**< 修改 */
    SQL_Select,   /**< 查找 */
    SQL_Other   /**< 其他SQL语句 */
};


struct SQLiteRetData {//操作数据库之后的返回值，尤其是查找数据库
public:
    std::vector<ZString> table_name_vec_;
    std::vector<ZString> table_value_vec_;
    bool retFlag{false};
};
using SQLiteRetPtr = std::shared_ptr<SQLiteRetData>;


struct SQLiteCmd {
public:
    ZString sqlite_sql_;//操作数据库的sql语句
    SqlOperType sql_type_;//操作类型
};
using SQLiteCmdPtr = std::shared_ptr<SQLiteCmd>;

ZDATABASE_NS_END