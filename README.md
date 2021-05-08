# MetamorphicTestFramework

1.支持多个MR，多组MTG

2.示例如src/sin文件夹内

>只需override相应的函数即可

>在main内对MetamorphicTest进行初始化，并调用metamorphicTest()计算测试结果

3.支持自定义分析函数

4.支持多线程

5.添加生成模板脚手架

>执行scripts下的build_sample.py

>参数:
>--name 被测程序名称
>--T 程序输入类型
>--U 程序输出类型
>--n mr个数

6.自动构建

>执行scripts下的build.sh,生成可执行文件到output目录下

### TODO

1. 添加结果保存文件功能