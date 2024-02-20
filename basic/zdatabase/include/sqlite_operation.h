#pragma once
#include "zdatabase_sdk.h"
#include "sqlite3.h"
#include"zthread.h"
#include <memory>

ZDATABASE_NS_BEGIN
class SQLiteOperation
{
public:
	SQLiteOperation();
	virtual ~SQLiteOperation();

	SQLiteOperation(const SQLiteOperation& other) = delete;
	SQLiteOperation& operator = (const SQLiteOperation& other) = delete;


private:
	static constexpr int BatchCnt{ 100 }; /**< 事务批量数量 */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< 调度线程 */
};
ZDATABASE_NS_END