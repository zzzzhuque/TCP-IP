1、下列关于DNS的说法错误的是？

a.因为DNS的存在，故可以用域名替代IP。（正确）
b.DNS服务器实际上是路由器，因为路由器根据域名决定数据路径。（错误）
c.所有域名信息并非集中于1台DNS服务器，但可以获取某一DNS服务器中未注册的IP地址。（正确）
d.DNS服务器根据操作系统进行区分，Windows下的DNS服务器和Linux下的DNS服务器是不同的。（错误）

2、阅读如下对话，并说明东秀的解决方案是否可行。这些都是大家可以在大学计算机实验室验证的内容。
字太多不打了...

东秀的解决方案可行，因为DNS是一个分布式系统，所以通过东秀学校的DNS服务器也可以访问别的DNS服务器。

3、在浏览器地址栏输入www.orentec.co.kr，并整理出主页显示过程。假设浏览器访问的默认DNS服务器
中并没有关于www.orentec.co.kr的IP地址信息。

浏览器通过默认DNS服务器向上级DNS服务器询问，直到解析出域名对应的IP，之后接入该网站并获取主页信
息。