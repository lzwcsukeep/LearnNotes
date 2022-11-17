#### 参数是如何传递给cgi脚本的？

如果参数是通过http GET 方法发送给cgi 脚本的话。这些参数被保存在`QUERY_STRING` 环境变量中。

如果参数是通过HTTP POST 方法发送给cgi脚本的话，这些参数是被发送给脚本的标准输入。脚本可以从自己的标准输入中读取出这些环境变量，并适应web 浏览器的请求。

>  If parameters are sent to the script via an [HTTP GET](https://en.wikipedia.org/wiki/HTTP_GET "HTTP GET") request (a question mark appended to the URL, followed by param=value pairs;), then those parameters are stored in the `QUERY_STRING` environment variable before the script is called. If parameters are sent to the script via an [HTTP POST](https://en.wikipedia.org/wiki/HTTP_POST "HTTP POST") request, they are passed to the script's [standard input](https://en.wikipedia.org/wiki/Standard_input "Standard input"). The script can then read these environment variables or data from standard input and adapt to the Web browser's request.[[9]](https://en.wikipedia.org/wiki/Common_Gateway_Interface#cite_note-9)
