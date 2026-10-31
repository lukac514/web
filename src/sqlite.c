#include "sqlite.h"
#include <stdio.h>
#include <string.h>

sqlite3 *db = NULL;

int db_open(const char *db_name)
{
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK)
    {
        printf(" 打开数据库失败: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return 0;
}
static int query_html_callback(void *arg, int col_cnt, char **val, char **col_name)
{
    char *html_buf = (char *)arg;
    char tmp[256] = {0};
    sprintf(tmp, "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
            val[1], val[2], val[3], val[4], val[5]);
    strcat(html_buf, tmp);
    return 0;
}

int db_query_by_brand(const char *brand, char *out_html, int buf_len)
{
    memset(out_html, 0, buf_len);
    char sql[512] = {0};
    char *err_msg = NULL;

    // 拼接查询SQL（简易写法，仅课程使用，生产用预处理防注入）
    snprintf(sql, sizeof(sql), "SELECT * FROM  phone.db  WHERE 品牌='%s';", brand);

    int rc = sqlite3_exec(db, sql, query_html_callback, out_html, &err_msg);
    if (rc != SQLITE_OK)
    {
        printf("[DB ERROR] 查询失败: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

void db_close()
{
    if (db != NULL)
    {
        sqlite3_close(db);
        db = NULL;
    }
}
