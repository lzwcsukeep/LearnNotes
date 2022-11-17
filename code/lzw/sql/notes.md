mysql connect using c language, key api ;
- conn = mysql_init(NULL); // The mysql_init() function allocates or initialises a MYSQL object suitable for mysql_real_connect() function. Remember this is C99
- mysql_real_connect(conn, server,user, password, database, 0, NULL, 0) ;  // 连接数据库，参数有主机，用户，密码，具体数据库
- mysql_query(conn, "show tables") ; // 连接之后对数据库进行操作
- mysql_close(conn);   // 关闭数据库连接

fetch data from mysql database using c language ,key api;
- res = `mysql_use_result(conn)` ;
- while ((`row = mysql_fetch_row(res)`) != NULL)
        printf("%s \n", row[0]);
- mysql_free_result(res);

// 另一种取数据方式
- mysql_query(con, "SELECT * FROM t_agent");  // 查询语句
- MYSQL_RES *result = `mysql_store_result(con)`; // 保存mysql_query(con, "SELECT * FROM t_agent")的结果
- int num_fields = `mysql_num_fields(result)`; // 获取字段数(列数)
- row = mysql_fetch_row(result); // 获取行
- mysql_free_result(result); //清空结果result

// 上述api 返回结果

- mysql_real_connect(conn, server,user, password, database, 0, NULL, 0) ;
> return value:
A MYSQL* connection handler if the connection was successful, NULL if the connection was unsuccessful. For a successful connection, the return value is the same as the value of the first argument.

- mysql_query(conn, "show tables") ;
> Return Values: Zero for success. Nonzero if an error occurred.

- MYSQL_RES *result = `mysql_store_result(con)`;
> A pointer to a MYSQL_RES result structure with the results.指向结果集的指针。后面可以使用mysql_fetch_row
(result);函数从结果集中按行取出数据。

-  int num_fields = `mysql_num_fields(result)`;
> 返回结果集中字段(即列数)的个数，后面for循环可以用来设置row[i]中i的索引范围

- row = `mysql_fetch_row(result) `;// 从结果集中取下一行。
> Return Values:A MYSQL_ROW structure for the next row, or NULL. 
可以用row[i] 来取出本行第i个字段。i从0开始。、

- field = `mysql_fetch_field(result)`  // 这个函数可以给查询结果添加字段名
> Description:
 Returns the definition of one column of a result set as a MYSQL_FIELD structure. Call this function repeatedly to retrieve information about all columns in the result set. `mysql_fetch_field()` returns NULL when no more fields are left.
Return Values:
  The MYSQL_FIELD structure for the current column. NULL if no columns are left or the result set has no metadata.
```c
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
                }// 打印字段名

                printf("\n");
            }
            printf("%s ", row[i] ? row[i] : "NULL");
        } // 打印一行。

        printf("\n");
    }
```
