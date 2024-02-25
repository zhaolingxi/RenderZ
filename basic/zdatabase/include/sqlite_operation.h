#pragma once
#include "sqlite3/sqlite3.h"
#include"zthread.h"
#include"zstr_coding_convert.h"
#include"zdatabase_operation_define.h"

/**********************************************************************
 *@brief  sqlite�����ӿ�
 * sql���ʾ����
 * �½���"CREATE TABLE IF NOT EXISTS xxx (id integer PRIMARY KEY AUTOINCREMENT, name text NOT NULL, age integer NOT NULL);" 
 * ɾ����"DROP TABLE IF EXISTS xxx; "
 * ������Ŀ��"INSERT INTO xxx (name, age) VALUES ('����', 22);"
 * �޸���Ŀ��"UPDATE xxx SET age = 12 WHERE name = 'С��';"
 * ɾ����Ŀ��"DELETE FROM xxx WHERE age > 12 AND age < 15;"
 * ��ѯ��Ŀ��"SELECT name, age FROM xxx WHERE age < 80; "
 * ��ѯ��Ŀ��"SELECT * FROM xxx WHERE age < 100 ORDER BY age DESC, name ASC; "
 * ��ѯ��Ŀ��"SELECT * FROM xxx WHERE age < 100 ORDER BY age DESC, name ASC LIMIT 3, 5; "
 * ��ѯ��Ŀ��"SELECT * FROM xxx WHERE name like '%��%';  "
 * ɾ����Ŀ��""
 * ɾ����Ŀ��""
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
    //���ݿ⿪��
    bool open(zutils::ZString path);
    bool close();
    //�ȴ���ǰ���ݿ����������
    void waitClose();

    bool doSqlWithOutRetData(SQLiteCmd& sqlCmd);
    bool doSqlWithRetData(SQLiteCmd& sqlCmd, SQLiteRetPtr&sqlRet);

    //static int sqliteExecCallback(void* userData, int cnt, char** values, char** columnNames);

    //��ȡ����
    bool get_row();

private:
	static constexpr int BatchCnt{ 100 }; /**< ������������ */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< �����߳� */

    std::mutex lock_;   /**< SQLite������������� */
    sqlite3* sqlite3Handler_{ nullptr }; /**< SQLite������� */
    zutils::ZString name_{ "" };//�û��Զ����ʶ

};
ZDATABASE_NS_END