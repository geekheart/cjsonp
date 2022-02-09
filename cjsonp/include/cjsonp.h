#ifndef CJSONP_H
#define CJSONP_H

#define CJSONP_DEBUG 0

#ifdef __cplusplus
extern "C"
{
#endif

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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // CJSONP_H
