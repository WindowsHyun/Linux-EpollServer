#include "MySQLConnect.h"

void MySQLConnect::init(const char * host, const char * id, const char * pwd, const char * db)
{
	// mysql init
	if (!(conn = mysql_init((MYSQL*)NULL))) {
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
	//// 쿼리
	//if (mysql_query(conn, "select * from 테이블명")) {  // 테이블명을 테이블에 맞게 변경
	//	printf("query fail\n");
	//	exit(1);
	//}
	//res = mysql_store_result(conn);                 //쿼리에 대한 결과를 row에 저장
	//// 쿼리 결과
	//while ((row = mysql_fetch_row(res)) != NULL) {
	//	printf("%s %s %s %s %s %s\n", row[0], row[1], row[2], row[3], row[4], row[5]);       //이전과 같이 디비테이블을 만들었다면 id와 패스워드값이 나온다.
	//}
}

MySQLConnect::MySQLConnect()
{
}

MySQLConnect::~MySQLConnect()
{
	mysql_close(conn);
}
