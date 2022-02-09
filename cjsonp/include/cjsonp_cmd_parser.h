#ifndef CJSON_CMD_PARSER_H
#define CJSON_CMD_PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

    #define JSON_PARSER_STR 0
    #define JSON_PARSER_NUM 1

    typedef struct
    {
        const char *json_ptr;
        const char *_next_ptr;
        unsigned long size;
        char type;
    } cjsonp_tok_t;

    char cjsonp_cmd_parser_init(cjsonp_tok_t *jsonp_tok, const char *parse_str);
    void cjsonp_cmd_parser_update(cjsonp_tok_t* jsonp_tok);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // CJSON_CMD_PARSER_H
