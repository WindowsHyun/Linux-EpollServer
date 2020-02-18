#ifndef MYSQL_CONNECT_H
#define MYSQL_CONNECT_H

#include "../Main.h"
#include <mysql/mysql.h>

class MySQLConnect {
public:
	MySQLConnect();
	~MySQLConnect();
	void init(const char *host, const char *id, const char *pwd, const char *db);
	

private:
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

};

#endif