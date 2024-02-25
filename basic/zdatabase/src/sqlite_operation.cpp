#include"sqlite_operation.h"
#include "log4z.h"

ZDATABASE_NS_BEGIN
SQLiteOperation::SQLiteOperation(zutils::ZString& baseName):
name_(baseName)
{
}
SQLiteOperation::SQLiteOperation(const char*& baseName):
name_(baseName)
{

}

SQLiteOperation::~SQLiteOperation()
{
}

SQLiteRetPtr SQLiteOperation::excuteSqlOper(SQLiteCmd& sqlCmd)
{
	SQLiteRetPtr ret= std::make_shared<SQLiteRetData>();
	excuteSqlOper(sqlCmd, ret);
	return ret;
}

bool SQLiteOperation::excuteSqlOper(SQLiteCmd& sqlCmd, SQLiteRetPtr& retPtr)
{
	int ret = false;
	if (!retPtr) {
		return ret;
	}
	auto type = sqlCmd.sql_type_;
	switch (type)
	{
	case SqlOperType::None: {
		break;
	}
	case SqlOperType::SQL_Open: {
		ret = open(sqlCmd.sqlite_sql_);
		break;
	}
	case SqlOperType::SQL_Close: {
		ret = close();
		break;
	}
	case SqlOperType::SQL_Insert: {
		ret = doSqlWithOutRetData(sqlCmd);
		break;
	}
	case SqlOperType::SQL_Delete: {
		ret = doSqlWithOutRetData(sqlCmd);
		break;
	}
	case SqlOperType::SQL_Update: {
		ret = doSqlWithOutRetData(sqlCmd);
		break;
	}
	case SqlOperType::SQL_Select: {
		ret = doSqlWithRetData(sqlCmd, retPtr);
		break;
	}
	default:
		break;
	}
	retPtr->retFlag = ret;
	return ret;
}

bool SQLiteOperation::excuteBatchSqlOper(std::vector<SQLiteCmd>& sqlCmd, SQLiteRetPtr& retPtr)
{
	std::unique_lock<std::mutex> uniqueLck(lock_);
	bool ret = false;
	// 执行批量事务操作
	sqlite3_exec(sqlite3Handler_, "begin;", 0, 0, 0);
	for (size_t i = 0; i < sqlCmd.size(); i++) {
		int errCode = SQLITE_OK;
		char* errMessage = nullptr;
		excuteSqlOper(sqlCmd[i], retPtr);
		if (errMessage != nullptr) {
			LOGFMTE("sqlite3_exec() failed message: %s", errMessage);
			sqlite3_free(errMessage);
		}
	}
	sqlite3_exec(sqlite3Handler_, "commit;", 0, 0, 0);
	return ret;
}

bool SQLiteOperation::open(zutils::ZString path)
{
	char* utf8Str=nullptr;
	//字符转换暂时有bug，待修复
//	zutils::ZStrCodingConvert::mbcsToUtf8(path.getData(), utf8Str);
//	std::shared_ptr<std::string> strPath=std::make_shared<std::string>(utf8Str);
//	int ret = sqlite3_open(strPath->c_str(), &sqlite3Handler_);
	int ret = sqlite3_open(path.getData(), &sqlite3Handler_);
	if (ret != SQLITE_OK) {
		LOGFMTE("SQLiteConnection::initConnection() sqlite3_open failed, ret = %d", ret);
		sqlite3_close(sqlite3Handler_);
		sqlite3Handler_ = nullptr;
		return false;
	}


	// 开启事务模式: "PRAGMA journal_mode=WAL;
	ret = sqlite3_exec(sqlite3Handler_, "PRAGMA synchronous = FULL; ", 0, 0, 0);
	if (ret != SQLITE_OK) {
		LOGFMTE("SQLiteConnection::initConnection() exec \"PRAGMA journal_mode = WAL;\" failed, ret = % d", ret);
		sqlite3_close_v2(sqlite3Handler_);
		sqlite3Handler_ = nullptr;
		return false;
	}

	return ret;
}

bool SQLiteOperation::close()
{
	sqlite3_close_v2(sqlite3Handler_);
	sqlite3Handler_ = nullptr;
	return true;
}

bool SQLiteOperation::doSqlWithOutRetData(SQLiteCmd& sqlCmd)
{
	bool ret = true;
	const char* sqlSentence = sqlCmd.sqlite_sql_.getData();        //SQL语句
	sqlite3_stmt* stmt = NULL;        //stmt语句句柄
	int result = sqlite3_prepare_v2(sqlite3Handler_, sqlSentence, -1, &stmt, NULL);//进行插入前的准备工作——检查语句合法性
	if (result == SQLITE_OK) {
		LOGFMTI("SQL_Insert OK sql_str:%s Time:%d \n", sqlSentence, _getCurrentTime_());
		sqlite3_step(stmt);
	}
	else {
		LOGFMTE("SQL_Insert ERROR sql_str:%s Time:%d \n", sqlSentence, _getCurrentTime_());
		ret = false;
	}
	sqlite3_finalize(stmt);//清理语句句柄，准备执行下一个语句
	return ret;
}

bool SQLiteOperation::doSqlWithRetData(SQLiteCmd& sqlCmd, SQLiteRetPtr& sqlRet)
{
	int ret = true;
	const char* sqlSentence = sqlCmd.sqlite_sql_.getData();       //SQL语句
	sqlite3_stmt * stmt = NULL;    // stmt语句句柄
	int result = sqlite3_prepare_v2(sqlite3Handler_, sqlSentence, -1, &stmt, NULL);//进行查询前的准备工作——检查语句合法性
	if (result == SQLITE_OK) {
		LOGFMTI("SQL_Insert OK sql_str:%s Time:%d \n", sqlSentence, _getCurrentTime_());
		int col_num = sqlite3_column_count(stmt);
		for (int i = 0; i < col_num; ++i) {
			const char* columnName = sqlite3_column_name(stmt, i);// 获取列名
			zutils::ZString strColName(columnName);
			sqlRet->table_name_vec_.emplace_back(strColName);
		}
		 while (sqlite3_step(stmt) == SQLITE_ROW) {// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
			 int col = 0;
			 while (col< col_num) {
				 const unsigned char* val = sqlite3_column_text(stmt, col);// 取出第i列字段的值
				 const char* charPtr = reinterpret_cast<const char*>(val);//待优化
				 sqlRet->table_value_vec_.emplace_back(zutils::ZString(charPtr));
				 col++;
			 }	
		 }		
		 ret = true;
	}
	else {
		LOGFMTE("SQL_Insert ERROR sql_str:%s Time:%d \n", sqlSentence, _getCurrentTime_());
		ret = false;
	}
	sqlite3_finalize(stmt);//清理语句句柄，准备执行下一个语句
	return ret;
}


//int SQLiteOperation::sqliteExecCallback(void* userData, int cnt, char** values, char** columnNames)
//{
//	return 0;
//}

ZDATABASE_NS_END