#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char **argv)
{
    // mysql -h106.75.6.153  -uimsvr -pimsvf098 -A ym_resmgr
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "106.75.6.153", "imsvr", "imsvf098",
                           "ym_resmgr", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    if (mysql_query(con, "SELECT * FROM t_cpures limit 5;"))
    {
        finish_with_error(con);
    }
    /*所有结果都保存在result里面了。*/
    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;
    MYSQL_FIELD *field;
    printf("num_fields = %d\n", num_fields);
    printf("---------------------------------------------------------------\n");
    while ((row = mysql_fetch_row(result)))
    {
        int i;
        for (i = 0; i < num_fields; i++)
        {
            if (i == 0)
            {
                while (field = mysql_fetch_field(result))
                {
                    printf("%s ", field->name);
                }

                printf("\n");
            }
            printf("%s ", row[i] ? row[i] : "NULL");
        } // 打印一行。

        printf("\n");
    }

    mysql_free_result(result);
    printf("------------------------after free result ------------------------\n");
    while ((row = mysql_fetch_row(result)))
    {
        int i;
        for (i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        } // 打印一行。

        printf("\n");
    }
    printf("close\n");
    mysql_close(con);

    exit(0);
}