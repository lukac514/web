#ifndef SQLITE_H
#define SQLITE_H

#include <sqlite3.h>

extern sqlite3 *db;

int db_open(const char *db_name);
int db_create_table();
int db_query_by_brand(const char *brand, char *out_html, int buf_len);
void db_close();

#endif