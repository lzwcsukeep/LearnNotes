## mysql 笔记

#### mysql 可以将查询的结果输出到一个文件中。

启动mysql 的时候带上--tee=filename 选项即可。

### mysql 的命令提示行也可以自定义

man mysql文档查看

交互式修改： prompt  \u@\h [\d]>   // prompt 命令直接跟提示符串

> test02 最初的提示符设置：\N [\d]> 

启动时mysql 时带上 --prompt 选项指定命令提示行格式。
