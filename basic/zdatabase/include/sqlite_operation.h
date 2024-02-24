#pragma once
#include "sqlite3.h"
#include"zthread.h"
#include"zstr_coding_convert.h"
#include"zdatabase_operation_define.h"
ZDATABASE_NS_BEGIN
class SQLiteOperation
{
public:
	SQLiteOperation();
	virtual ~SQLiteOperation();

	SQLiteOperation(const SQLiteOperation& other) = delete;
	SQLiteOperation& operator = (const SQLiteOperation& other) = delete;

    zutils::ZString name_{""};//�û��Զ����ʶ

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

    //������
    bool create_table(std::vector<zutils::ZString> list);
    bool update_table_name(zutils::ZString old_name, zutils::ZString new_name);
    bool delete_table(zutils::ZString name);
    bool select_table(zutils::ZString name);

    //�в���
    bool add_column(zutils::ZString table_name, zutils::ZString column_name);
    bool delete_column(zutils::ZString table_name, zutils::ZString column_name);

    //�в���
    bool insert_row();
    bool update_row();
    bool delete_row();

    //��ȡ����
    bool get_row();

private:
	static constexpr int BatchCnt{ 100 }; /**< ������������ */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< �����߳� */

    std::mutex lock_;   /**< SQLite������������� */
    sqlite3* sqlite3Handler_{ nullptr }; /**< SQLite������� */
};
ZDATABASE_NS_END