ds1:
perloc数据是很简单的, 可以用来简单的测试

ds2:
关于revenue的数据，在aql中创建的其中两个view：一个RevenueAndDivision，另一个RevenueAndDivision2。
实际上应该合成一个view的，但是因为本子集的功能有限，针对不同的pattern但同一种提取信息，只好分开创建view并提取。



自己写的测试样例总共有3个，数据输入文件分别在ds3(education), ds4(economy)和ds5(event)三个文件夹内，其中event文件夹中有3个输入文件，用于应用于同一个aql。相应的就有三个.aql文件，分别是Education.aql, Economy.aql和Event.aql。

ds3:
education数据是一篇关于讨论留学热的文章，在aql中用正则表达式找出了格式化表示的年龄和大写字母开头的单词，生成两个view。然后通过extract pattern找出每个年龄对应的人。测试了aql的简单功能。
create view Age————测试在文章中用正则表达式提取特定内容的功能
create view Name————测试在文章中用正则表达式提取特定内容的功能
create view AgeOfPerson————测试了简单的两个表的拼接，以及非嵌套括号下的group捕获
output特定view测试了格式化输出

ds4:
Economy数据是关于一篇总结2014年中国宏观经济概况的文章。用aql的正则表达式找出了表示GDP的百分比，四个数字组成的年份，对各种GDP描述的关键字（如annual, expected），生成了三个view，然后通过拼接找出关键字和对应的GDP。
create view Year————测试正则表达式中为每个捕获块生成一个group，并作为view中的列
create view GDP1————测试了两个view中间隔了不定个数（<=5）的token的情况下的拼表
output view Percent as per;————测试了输出重命名的view

ds5:
Event数据是对特定年份所发生大事件进行总结的一篇文章，在这个数据集中有三个input文件，分别是2012到2014的大事件总结。在aql中，通过用正则表达式提取关键字，生成表示月份的view，然后提取数字代表时间，提取事件，最后把这三个view进行各种拼接，测试功能。
create view Month————测试在文章中用正则表达式提取关键字的功能
create view Date as————测试了只有两个列相连接，中间没有隔着token的pattern表达式
create view Sentence————测试了中间有多个token相隔（10~50）的pattern表达式
create view Event————测试了有3层括号嵌套的pattern中对对应group的准确捕获
create view DateOnly————测试了用select关键字选取一个表中的特定列作为新的表的列