环境：linux+yacc+lex+npm
(ubuntu上为bison+flex)
首先，安装printf包
$npm install printf
解析方法：
目录切换至../src
输入$make，即可生成level1.js,level2.js,level3.js,level4.js
使用node命令执行js文件，例如
$node level1.js，即可执行level1.js
为验证js文件是否与c文件一致，可使用
$gcc -o level1 level1.c
$./level1
获得相应c文件的输出