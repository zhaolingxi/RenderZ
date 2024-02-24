#pragma once
#include <memory>
#include"zstring.h"
#include "zdatabase_sdk.h"

ZDATABASE_NS_BEGIN
enum class SqlOperType :int {
    None = 0,   /**< �� */
    SQL_Open,   /**< ���ݿ�� */
    SQL_Close,  /**< ���ݿ�ر� */
    SQL_Insert, /**< ���� */
    SQL_Delete, /**< ɾ�� */
    SQL_Update, /**< �޸� */
    SQL_Select,   /**< ���� */
    SQL_Other   /**< ����SQL��� */
};


struct SQLiteRetData {//�������ݿ�֮��ķ���ֵ�������ǲ������ݿ�
public:
    std::vector<zutils::ZString> table_name_vec_;
    std::vector<zutils::ZString> table_value_vec_;
    bool retFlag{false};
};
using SQLiteRetPtr = std::shared_ptr<SQLiteRetData>;


struct SQLiteCmd {
public:
    zutils::ZString sqlite_sql_;//�������ݿ��sql���
    SqlOperType sql_type_;//��������
};
using SQLiteCmdPtr = std::shared_ptr<SQLiteCmd>;

ZDATABASE_NS_END