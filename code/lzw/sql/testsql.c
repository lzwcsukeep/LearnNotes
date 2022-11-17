#include <mysql.h>
#include <stdio.h>
int main()
{
    // mysql -h106.75.6.153  -uimsvr -pimsvf098 -A ym_resmgr
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "106.75.6.153";
    char *user = "imsvr";
    char *password = "imsvf098"; /* set me first */
    char *database = "ym_resmgr";
    conn = mysql_init(NULL);
    /* Connect to database */
    if (!mysql_real_connect(conn, server,
                            user, password, database, 0, NULL, 0))
    {
        printf("something wrong.\n");
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    /* send SQL query */
    if (mysql_query(conn, "show tables"))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    int num_fields = mysql_num_fields(res);
    int num_rows = mysql_num_rows(res);
    printf("num_rows = %d\n", num_rows);
    printf("-----------------------------------\n");

    printf("num_fields = %d\n", num_fields);
    printf("-----------------------------------\n");
    /* output table name */
    printf("MySQL Tables in mysql database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);

    /* close connection */
    mysql_free_result(res);

    mysql_close(conn);
}