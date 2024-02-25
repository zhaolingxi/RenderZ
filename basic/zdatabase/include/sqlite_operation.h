#pragma once
#include "sqlite3/sqlite3.h"
#include"zthread.h"
#include"zstr_coding_convert.h"
#include"zdatabase_operation_define.h"

/**********************************************************************
 *@brief  sqlite操作接口
 * sql语句示例：
 * 新建表："CREATE TABLE IF NOT EXISTS xxx (id integer PRIMARY KEY AUTOINCREMENT, name text NOT NULL, age integer NOT NULL);" 
 * 删除表："DROP TABLE IF EXISTS xxx; "
 * 插入条目："INSERT INTO xxx (name, age) VALUES ('大明', 22);"
 * 修改条目："UPDATE xxx SET age = 12 WHERE name = '小明';"
 * 删除条目："DELETE FROM xxx WHERE age > 12 AND age < 15;"
 * 查询条目："SELECT name, age FROM xxx WHERE age < 80; "
 * 查询条目："SELECT * FROM xxx WHERE age < 100 ORDER BY age DESC, name ASC; "
 * 查询条目："SELECT * FROM xxx WHERE age < 100 ORDER BY age DESC, name ASC LIMIT 3, 5; "
 * 查询条目："SELECT * FROM xxx WHERE name like '%明%';  "
 * 删除条目：""
 * 删除条目：""
 ***********************************************************************/

ZDATABASE_NS_BEGIN
class ZDATABASE_API SQLiteOperation
{
public:
    SQLiteOperation(zutils::ZString& baseName);
    SQLiteOperation(const char* & baseName);
	virtual ~SQLiteOperation();

	SQLiteOperation(const SQLiteOperation& other) = delete;
	SQLiteOperation& operator = (const SQLiteOperation& other) = delete;


    SQLiteRetPtr excuteSqlOper(SQLiteCmd& sqlCmd);
    bool excuteSqlOper(SQLiteCmd &sqlCmd, SQLiteRetPtr& retPtr);

    SQLiteRetPtr excuteBatchSqlOper(std::vector<SQLiteCmd>& sqlCmd);
    bool excuteBatchSqlOper(std::vector<SQLiteCmd>& sqlCmd, SQLiteRetPtr& retPtr);

    int get_max_id(zutils::ZString table_name);

protected:
    //数据库开关
    bool open(zutils::ZString path);
    bool close();
    //等待当前数据库任务处理完毕
    void waitClose();

    bool doSqlWithOutRetData(SQLiteCmd& sqlCmd);
    bool doSqlWithRetData(SQLiteCmd& sqlCmd, SQLiteRetPtr&sqlRet);

    //static int sqliteExecCallback(void* userData, int cnt, char** values, char** columnNames);

    //获取数据
    bool get_row();

private:
	static constexpr int BatchCnt{ 100 }; /**< 事务批量数量 */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< 调度线程 */

    std::mutex lock_;   /**< SQLite操作句柄保护锁 */
    sqlite3* sqlite3Handler_{ nullptr }; /**< SQLite操作句柄 */
    zutils::ZString name_{ "" };//用户自定义标识

};
ZDATABASE_NS_END