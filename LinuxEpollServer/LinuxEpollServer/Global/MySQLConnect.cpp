#include "MySQLConnect.h"

void MySQLConnect::init(const char * host, const char * id, const char * pwd, const char * db)
{
	// mysql init
	if (!(conn = mysql_init(conn))) {
		spdlog::error("MySQL init Fail..!");
		exit(1);
	}

	// connect
	if (!mysql_real_connect(conn, host, id, pwd, NULL, 3306, NULL, 0)) {
		spdlog::error("MySQL({}) Connect Fail..!", host);
		exit(1);
	}

	// select DB
	if (mysql_select_db(conn, db) != 0) {
		mysql_close(conn);
		spdlog::error("MySQL({}) Select DB({}) Fail..!", host, db);
		exit(1);
	}
	spdlog::info("MySQL init Success..!");

}

bool MySQLConnect::query(const char * query, vector<std::string>& result)
{
	std::lock_guard<std::mutex> guard(mLock);
	// ����
	if (mysql_query(conn, query)) {  // ���̺���� ���̺� �°� ����
		spdlog::error("MySQL Query({}) Fail..!", query);
		return false;
	}
	MYSQL_RES *res = mysql_store_result(conn);                 // ������ ���� ����� row�� ����
	int fields = mysql_num_fields(res);				// �ʵ� ���� ����
	MYSQL_ROW row;
	// ���� ���
	while (row = mysql_fetch_row(res)) {
		for (int i = 0; i < fields; ++i) {
			// DB�� NULL ���� ���� ��� ó��
			if (row[i] != NULL) {
				result.push_back(row[i]);
			}
			else {
				result.push_back("");
			}
		}
	}
	mysql_free_result(res);
	return true;
}

MySQLConnect::MySQLConnect()
{
	conn = new MYSQL;
}

MySQLConnect::~MySQLConnect()
{
	mysql_close(conn);
}