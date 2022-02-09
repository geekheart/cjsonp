# cjsonp
这是一个基于cjson的库，cjson对于使用者来说比较繁琐。这个库调用起来比较方便、简洁。

借用了[jsonPath](https://github.com/json-path/JsonPath)的理念，但是还不能像[jsonPath](https://github.com/json-path/JsonPath)那样强大，目前只支持全路径匹配，且匹配的规则会有些许不同

## 运行例程
1. 首先，你需要clone本库
```shell
$ git clone https://github.com/geekheart/cjsonp.git
$ cd cjsonp
$ git submodule init
$ git submodule update
```
2. 然后，确保你的环境里有gcc编译器和make工具
```shell
$ gcc
gcc: fatal error: no input files
compilation terminated
$ make    
make: *** No targets specified and no makefile found.  Stop.
```
3. 进入cjsonp文件夹里就可以make编译了
```shell
$ make # 编译例程
$ make run # 运行例程
$ make clean # 清空编译生成的内容
```

## 如何移植
本库核心部分cjsonp_cmd_parser没有用到任何库（包括libc），cjsonp部分则依赖cJSON。
将本库中的cjsonp文件夹直接复制到您的工程，即可使用。

## API说明
解析的路径，路径规则：<br/>
.表示子对象<br/>
[]用于数组对象<br/>
数组对象前后不用.<br/>

```c
/**
 * @brief 查找json中指定的键对应的值，支持数组和json对象
 *
 * @param json 输入的json字符串
 * @param path 查找的json路径：键之间用.分割，序号用[]
 *  @ref resolutions[0]width "表示从resolutions的数组里找到第0个数组其中width对应的值"
 * @return char* 返回查找到的值，不用时需要手动调用cjsonp_free释放
 */
char *cjsonp_search(const char *json, const char *path);

/**
 * @brief 删除指定键值对
 *
 * @param json 输入的json字符串
 * @param path 删除的json路径：键之间用.分割，序号用[]
 * @return char* 返回删除后的json，不用时需要手动调用cjsonp_free释放
 */
char *cjsonp_delete(const char *json, const char *path);

/**
 * @brief 直接移除指定键值对
 *
 * @param json 输入的json字符串，结果直接在json里，不产生多余的内存
 * @param path 删除的json路径：键之间用.分割，序号用[]
 * @return int 返回1表示成功，0表示失败
 */
int cjsonp_remove(char *json, const char *path);

/**
 * @brief 添加指定的键值对
 *
 * @param json 输入的json字符串
 * @param path 添加的键的json路径（不能是已存在的）：键之间用.分割，序号用[]
 * @param add 需要添加的值（支持json对象，数组，数字，字符串；字符串需要添加双引号）
 * @return char* 返回添加后的json，不用时需要手动调用cjsonp_free释放
 */
char *cjsonp_add(const char *json, const char *path, const char *add);

/**
 * @brief 替换指定的键值对
 *
 * @param json 输入的json字符串
 * @param path 添加的键的json路径：键之间用.分割，序号用[]
 * @param add 需要替换的值（支持json对象，数组，数字，字符串；字符串需要添加双引号）
 * @return char* 返回添加后的json，不用时需要手动调用cjsonp_free释放
 */
char *cjsonp_replace(const char *json, const char *path, const char *add);

/**
 * @brief 释放以上函数返回的字符串内存
 *
 * @param cjsonp 需要释放空间的字符串指针
 */
void cjsonp_free(char *cjsonp);
```