#include "cjsonp_cmd_parser.h"

#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif

static unsigned long _strlen(const char *str)
{
    unsigned long count = 0;
    if (str == NULL)
    {
        return 0;
    }
    while (*str != '\0' && *str != 0)
    {
        count++;
        str++;
    }
    return count;
}
char cjsonp_cmd_parser_init(cjsonp_tok_t *jsonp_tok, const char *parse_str)
{
    if (parse_str == NULL)
    {
        return 0;
    }
    jsonp_tok->json_ptr = NULL;
    jsonp_tok->_next_ptr = parse_str;
    jsonp_tok->size = 0;
    jsonp_tok->type = JSON_PARSER_STR;
    return 1;
}

void cjsonp_cmd_parser_update(cjsonp_tok_t *jsonp_tok)
{
    unsigned long size = 0;
    unsigned long i = 0;
    if (jsonp_tok->_next_ptr[0] == '[')
    {
        jsonp_tok->_next_ptr++;
    }
    jsonp_tok->size = 0;
    jsonp_tok->type = JSON_PARSER_STR;
    jsonp_tok->json_ptr = NULL;
    size = _strlen(jsonp_tok->_next_ptr);
    for (i = 0; i < size; i++)
    {
        if (jsonp_tok->_next_ptr[i] == '.' || jsonp_tok->_next_ptr[i] == '[' || jsonp_tok->_next_ptr[i] == ']')
        {
            jsonp_tok->size = i;
            jsonp_tok->json_ptr = jsonp_tok->_next_ptr;
            if (jsonp_tok->_next_ptr[i] == ']')
            {
                jsonp_tok->type = JSON_PARSER_NUM;
                if (jsonp_tok->_next_ptr[i + 1] == '[')
                {
                    jsonp_tok->_next_ptr++;
                }
            }
            else
            {
                jsonp_tok->type = JSON_PARSER_STR;
            }
            jsonp_tok->_next_ptr = jsonp_tok->_next_ptr + i + 1;
            return;
        }
    }
    if (_strlen(jsonp_tok->_next_ptr) != 0)
    {
        jsonp_tok->json_ptr = jsonp_tok->_next_ptr;
        jsonp_tok->size = _strlen(jsonp_tok->_next_ptr);
        jsonp_tok->_next_ptr = "";
    }
}