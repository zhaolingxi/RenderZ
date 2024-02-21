#pragma once
#include "zdatabase_sdk.h"
#include "sqlite3.h"
#include"zthread.h"
#include"zstring.h"
#include <memory>

ZDATABASE_NS_BEGIN
class SQLiteOperation
{
public:
	SQLiteOperation();
	virtual ~SQLiteOperation();

	SQLiteOperation(const SQLiteOperation& other) = delete;
	SQLiteOperation& operator = (const SQLiteOperation& other) = delete;

    //数据库开关
    bool open(zutils::ZString path, zutils::ZString connect);
    bool close();
    void waitClose();

    //表格操作
    bool create_table(std::vector<zutils::ZString> list);
    bool update_table_name(zutils::ZString old_name, zutils::ZString new_name);
    bool delete_table(zutils::ZString name);
    bool select_table(zutils::ZString name);

    //列操作
    bool add_column(zutils::ZString table_name, zutils::ZString column_name);
    bool delete_column(zutils::ZString table_name, zutils::ZString column_name);

    //行操作
    bool insert_row();
    bool update_row();
    bool delete_row();

    //获取数据
    bool get_row();

    int get_max_id(zutils::ZString table_name);

private:
	static constexpr int BatchCnt{ 100 }; /**< 事务批量数量 */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< 调度线程 */

    std::mutex lock_;   /**< SQLite操作句柄保护锁 */
    sqlite3* sqlite3Handler_{ nullptr }; /**< SQLite操作句柄 */
};
ZDATABASE_NS_END