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

    //���ݿ⿪��
    bool open(zutils::ZString path, zutils::ZString connect);
    bool close();
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

    int get_max_id(zutils::ZString table_name);

private:
	static constexpr int BatchCnt{ 100 }; /**< ������������ */
	std::shared_ptr<zutils::ZThread> schedThread_{ nullptr }; /**< �����߳� */

    std::mutex lock_;   /**< SQLite������������� */
    sqlite3* sqlite3Handler_{ nullptr }; /**< SQLite������� */
};
ZDATABASE_NS_END