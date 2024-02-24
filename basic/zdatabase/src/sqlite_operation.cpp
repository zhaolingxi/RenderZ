#include"sqlite_operation.h"
ZDATABASE_NS_BEGIN
SQLiteOperation::SQLiteOperation()
{
}

SQLiteOperation::~SQLiteOperation()
{
}

SQLiteRetPtr SQLiteOperation::excuteSqlOper(SQLiteCmd& sqlCmd)
{
	SQLiteRetPtr ret= std::make_shared<SQLiteRetData>();
	auto type = sqlCmd.sql_type_;
	switch (type)
	{
	case SqlOperType::None: {
		return ret;
	}
	case SqlOperType::SQL_Open: {
		ret->retFlag=open(sqlCmd.sqlite_sql_);
		return ret;
	}
	case SqlOperType::SQL_Close: {
		ret->retFlag = close();
		return ret;
	}
	case SqlOperType::SQL_Insert: {

		return ret;
	}
	case SqlOperType::SQL_Delete: {
		return ret;
	}
	default:
		break;
	}
}



bool SQLiteOperation::open(zutils::ZString path)
{
	char* utf8Str=nullptr;
	zutils::ZStrCodingConvert::mbcsToUtf8(path.getData(), utf8Str);
	std::shared_ptr<std::string> strPath=std::make_shared<std::string>(utf8Str);
	int ret = sqlite3_open(strPath->c_str(), &sqlite3Handler_);
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
		sqlite3_close(sqlite3Handler_);
		sqlite3Handler_ = nullptr;
		return false;
	}

	return ret;
}

bool SQLiteOperation::close()
{
	sqlite3_close(sqlite3Handler_);
	sqlite3Handler_ = nullptr;
	return true;
}


ZDATABASE_NS_END