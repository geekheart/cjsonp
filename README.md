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
include本库后自动添加cJSON库

## API说明
解析的路径，路径规则：<br/>
.表示子对象<br/>
[]用于数组对象<br/>
数组对象前后不用.<br/>

```c
    /**
     * @brief 从cjson对象中找到你需要的元素
     * 
     * @param json_root cjson根对象
     * @param path 查找cjson的路径
     * @return cJSON* 返回的额元素对象
     */
    cJSON* cjsonp_search(cJSON* json_root, const char *path);

    /**
     * @brief 删除cjson对象的元素
     * 
     * @param json_root cjson根对象
     * @param path 删除cjson的路径
     * @return int 返回1成功，0失败
     */
    int cjsonp_delete(cJSON *json_root, const char *path);

    /**
     * @brief 添加cjson元素到对象中
     * 
     * @param json_root cjson根对象
     * @param path 添加cjson的路径
     * @param add_json 添加的cjson对象
     * @return int 返回1成功，0失败
     */
    int cjsonp_add(cJSON *json_root, const char *path, cJSON *add_json);

    /**
     * @brief 替换cjson对象中的元素
     * 
     * @param json_root cjson根对象
     * @param path 替换cjson的路径
     * @param rep_json 替换的cjson对象
     * @return int 返回1成功，0失败
     */
    int cjsonp_replace(cJSON *json_root, const char *path, cJSON *rep_json);

```