### ScriptAlias

ScriptAlias 指令指定了cgi程序的默认位置。Apache http服务器会默认将这个指令**指示的目录下的文件当成cgi程序执行**。

![](E:\Files\LearnNotes\imgs\ScriptAlias.png)

如上图的例子，当用户请求的url 从/cgi-bin/开始时，apache 会在目录/usr/local/apache2/cgi-bin/开始搜索。并且将该目录下的所有文件当成cgi程序执行。

比如:URL `http://www.example.com/cgi-bin/test.pl` 被请求了 apache 会尝试执行`/usr/local/apache2/cgi-bin/test.pl`并返回输出。当然这个文件需要存在，可执行，并且返回的输出要符合cgi 标准。否则apache 会返回一个错误消息。
