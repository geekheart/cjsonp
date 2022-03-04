#include "cjsonp_cmd_parser.h"
#include "cjsonp.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>

#if CJSONP_DEBUG
#include <stdio.h>
#define CJSONP_LOG() printf("\033[31mcjsonp: %d has been return\033[0m\n", __LINE__)
#else
#define CJSONP_LOG()
#endif

#define json_key_LEN 128
#define CJSONP_ASSERT(x) \
    if (x)               \
    {                    \
        CJSONP_LOG();    \
        goto end;        \
    }

static cJSON *_cjsonp_find(cJSON **last_obj, cjsonp_tok_t *last_jsonp_tok, unsigned char last_err)
{
    cjsonp_tok_t jsonp_tok = *last_jsonp_tok;
    cJSON *json_obj_before = *last_obj;
    char json_key[128] = {0};
    int index;

    // 处理parse
    cjsonp_cmd_parser_update(&jsonp_tok);
    while (jsonp_tok.size != 0)
    {
        json_obj_before = *last_obj;
        *last_jsonp_tok = jsonp_tok;
        memset(json_key, 0, sizeof(json_key));
        strncpy(json_key, jsonp_tok.json_ptr, jsonp_tok.size);
        switch (jsonp_tok.type)
        {
        case JSON_PARSER_STR:
            *last_obj = cJSON_GetObjectItem(*last_obj, json_key);
            break;
        case JSON_PARSER_NUM:
            index = atoi(json_key);
            *last_obj = cJSON_GetArrayItem(*last_obj, index);
            break;
        default:
            break;
        }
        cjsonp_cmd_parser_update(&jsonp_tok);
        if (last_err)
        {
            CJSONP_ASSERT(*last_obj == NULL && jsonp_tok.size != 0);
        }
        else
        {
            CJSONP_ASSERT(*last_obj == NULL);
        }
    }
    return json_obj_before;
end:
    return NULL;
}

char *cjsonp_search(const char *json, const char *path)
{
    cJSON *json_root;
    cJSON *json_obj;
    cjsonp_tok_t jsonp_tok;
    char res;
    char *ret;

    // 输入断言
    CJSONP_ASSERT(json == NULL || path == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 初始化json解析器
    json_root = cJSON_Parse(json);
    CJSONP_ASSERT(json_root == NULL);
    json_obj = json_root;

    // 找到json子对象
    _cjsonp_find(&json_obj, &jsonp_tok, 0);
    CJSONP_ASSERT(json_obj == NULL);

    // 无格式化输出与内存回收
    ret = cJSON_PrintUnformatted(json_obj);
    cJSON_Delete(json_root);
    return ret;
end:
    cJSON_Delete(json_root);
    return NULL;
}

char *cjsonp_delete(const char *json, const char *path)
{
    cJSON *json_root;
    cJSON *json_parent;
    cJSON *json_obj;
    cjsonp_tok_t jsonp_tok;
    char res;
    char *ret;

    // 输入断言
    CJSONP_ASSERT(json == NULL || path == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 初始化json解析器
    json_root = cJSON_Parse(json);
    CJSONP_ASSERT(json_root == NULL);
    json_obj = json_root;

    // 找到json子对象
    json_parent = _cjsonp_find(&json_obj, &jsonp_tok, 0);
    CJSONP_ASSERT(json_parent == NULL || json_obj == NULL);

    // 分离和删除子节点

    json_obj = cJSON_DetachItemViaPointer(json_parent, json_obj);
    cJSON_Delete(json_obj);

    // 无格式化输出
    ret = cJSON_PrintUnformatted(json_root);
    cJSON_Delete(json_root);
    return ret;

end:
    cJSON_Delete(json_root);
    return NULL;
}

int cjsonp_remove(char *json, const char *path)
{
    char *str = cjsonp_delete(json, path);
    if (str == NULL)
    {
        return 0;
    }
    strcpy(json, str);
    cjsonp_free(str);
    return 1;
}

char *cjsonp_add(const char *json, const char *path, const char *add)
{
    cJSON *json_root = NULL;
    cJSON *json_obj = NULL;
    cJSON *add_json = NULL;
    cjsonp_tok_t jsonp_tok;
    char res;
    char *ret;
    int index;
    char json_key[128] = {0};

    // 输入断言
    CJSONP_ASSERT(json == NULL || path == NULL || add == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 初始化json解析器
    json_root = cJSON_Parse(json);
    CJSONP_ASSERT(json_root == NULL);
    json_obj = json_root;

    // 初始化准备好的json字符串
    add_json = cJSON_Parse(add);
    CJSONP_ASSERT(add_json == NULL);

    // 解析
    json_obj = _cjsonp_find(&json_obj, &jsonp_tok, 1);
    CJSONP_ASSERT(json_obj == NULL);

    // 添加json
    strncpy(json_key, jsonp_tok.json_ptr, jsonp_tok.size);
    switch (jsonp_tok.type)
    {
    case JSON_PARSER_STR:
        CJSONP_ASSERT(cJSON_GetObjectItem(json_obj, json_key) != NULL);
        cJSON_AddItemToObject(json_obj, json_key, add_json);
        break;
    case JSON_PARSER_NUM:
        index = atoi(json_key);
        cJSON_InsertItemInArray(json_obj, index, add_json);
        break;
    default:
        cJSON_Delete(add_json);
        cJSON_Delete(json_root);
        return NULL;
        break;
    }

    // 无格式化输出
    ret = cJSON_PrintUnformatted(json_root);
    cJSON_Delete(json_root);
    return ret;

end:
    cJSON_Delete(add_json);
    cJSON_Delete(json_root);
    return NULL;
}

char *cjsonp_replace(const char *json, const char *path, const char *add)
{
    cJSON *json_root = NULL;
    cJSON *json_obj = NULL;
    cJSON *json_parent = NULL;
    cJSON *add_json = NULL;
    cjsonp_tok_t jsonp_tok;
    char res;
    char *ret;

    // 输入断言
    CJSONP_ASSERT(json == NULL || path == NULL || add == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 初始化json解析器
    json_root = cJSON_Parse(json);
    CJSONP_ASSERT(json_root == NULL);
    json_obj = json_root;
    json_parent = json_root;

    // 初始化准备好的json字符串
    add_json = cJSON_Parse(add);
    CJSONP_ASSERT(add_json == NULL);

    // 解析
    json_parent = _cjsonp_find(&json_obj, &jsonp_tok, 0);
    CJSONP_ASSERT(json_parent == NULL);

    // 替换json
    add_json->string = strdup(json_obj->string);
    res = cJSON_ReplaceItemViaPointer(json_parent, json_obj, add_json);
    CJSONP_ASSERT(res == 0);

    // 无格式化输出
    ret = cJSON_PrintUnformatted(json_root);
    cJSON_Delete(json_root);
    return ret;
end:
    cJSON_Delete(add_json);
    cJSON_Delete(json_root);
    return NULL;
}

void cjsonp_free(char *cjsonp)
{
    cJSON_free(cjsonp);
}