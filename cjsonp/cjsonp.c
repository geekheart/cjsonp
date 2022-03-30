#include "cjsonp_cmd_parser.h"
#include "cjsonp.h"
#include <string.h>
#include <stdlib.h>

#if CJSONP_DEBUG
#include <stdio.h>
#define CJSONP_LOG() printf("\033[31mcjsonp: %d has been return\033[0m\n", __LINE__)
#else
#define CJSONP_LOG()
#endif

#define JSON_KEY_LEN 128

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
    char json_key[JSON_KEY_LEN] = {0};
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

cJSON *cjsonp_search(cJSON *json_root, const char *path)
{
    char res;
    cjsonp_tok_t jsonp_tok;
    cJSON *json_obj = NULL;

    // 输入断言
    CJSONP_ASSERT(json_root == NULL || path == NULL);

    // 初始化命令解析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    json_obj = json_root;
    _cjsonp_find(&json_obj, &jsonp_tok, 0);

    return json_obj;
end:
    return NULL;
}

int cjsonp_delete(cJSON *json_root, const char *path)
{
    cJSON *json_parent = NULL;
    cJSON *json_obj = NULL;
    cjsonp_tok_t jsonp_tok;
    char res;

    // 输入断言
    CJSONP_ASSERT(json_root == NULL || path == NULL);

    // 初始化命令解析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 找到json子对象
    json_obj = json_root;
    json_parent = _cjsonp_find(&json_obj, &jsonp_tok, 0);
    CJSONP_ASSERT(json_parent == NULL || json_obj == NULL);

    // 分离和删除子节点
    json_obj = cJSON_DetachItemViaPointer(json_parent, json_obj);
    cJSON_Delete(json_obj);

    return 1;
end:
    return 0;
}

int cjsonp_add(cJSON *json_root, const char *path, cJSON *add_json)
{
    cJSON *json_obj = NULL;
    cjsonp_tok_t jsonp_tok;
    char res;
    char json_key[JSON_KEY_LEN] = {0};
    int index;

    // 输入断言
    CJSONP_ASSERT(json_root == NULL || path == NULL || add_json == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 解析
    json_obj = json_root;
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
        break;
    }

    return 1;
end:
    return 0;
}

int cjsonp_replace(cJSON *json_root, const char *path, cJSON *rep_json)
{
    cJSON *json_obj = NULL;
    cJSON *json_parent = NULL;
    cjsonp_tok_t jsonp_tok;
    char res;

    // 输入断言
    CJSONP_ASSERT(json_root == NULL || path == NULL || rep_json == NULL);

    // 初始化命令析器
    res = cjsonp_cmd_parser_init(&jsonp_tok, path);
    CJSONP_ASSERT(!res);

    // 解析
    json_obj = json_root;
    json_parent = _cjsonp_find(&json_obj, &jsonp_tok, 0);
    CJSONP_ASSERT(json_parent == NULL);

    // 替换json
    rep_json->string = strdup(json_obj->string);
    res = cJSON_ReplaceItemViaPointer(json_parent, json_obj, rep_json);

    return res;
end:
    return 0;
}
