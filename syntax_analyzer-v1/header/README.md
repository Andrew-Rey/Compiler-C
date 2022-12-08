# header

在外面两个文件夹里面定义最基本的 **字母表** 和 **基本数据结构**

- 字母表即产生式会用到的所有终结符和非终结符, 其中终结符就是词法分析返回的 **token**, 非终结符是语法分析中会涉及的状态
- 基本数据结构为 `LR(1)` 服务, 
  - 定义了状态闭包 **closure** , 
    每个状态闭包里面包含了多个 **item** , 
    每个 **item** 定义为一个 **pair**, 
    每个 **pair** 第一个元素为 **Production** 类型, 第二个为 **nextTerminal** 类型
    每个 **Production** 是一个结构体, 存放: **char left**, **char[MAX_LEN] right**, **int current**
    每个 **nextTerminal** 是一个集合, 存放下一个终结符
  - 定义了LR分析表, 是一个二维数组, 元素为 **Action** 类型,
    **Action** 类型是函数指针, 分为移进, 规约, 报错
  - 定义了AST结点, 是一个结构体, 元素包含 **token**, 和 **孩子结点指针**
    **token** 类型是 **string** 的重命名, token 来自于字母表的终结符部分