#ifndef CJSONP_H
#define CJSONP_H

#define CJSONP_DEBUG 1
#include "cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
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
    
    /**
     * @brief 交换json中两个对象
     * 
     * @param json_root cjson根对象
     * @param json1_path 交换对象1
     * @param json2_path 交换对象2
     * @return int 返回1成功，0失败
     */
    int cjsonp_swap(cJSON *json_root, const char *json1_path, const char *json2_path);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // CJSONP_H
