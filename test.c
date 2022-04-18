#include "cjsonp.h"
#include <stdio.h>

/*
{
    "name":"Awesome 4K",
    "resolutions":[
        {
            "width":1280,
            "height":720
        },
        {
            "width":1920,
            "height":1080
        },
        {
            "width":3840,
            "height":2160
        }
    ]
}
*/
char json_text[150] = "{\"name\":\"Awesome 4K\",\"resolutions\":[{\"width\":1280,\"height\":720},{\"width\":1920,\"height\":1080},{\"width\":3840,\"height\":2160}]}";
char* res;

int main(void)
{
    cJSON *json_root = cJSON_Parse(json_text);

    cJSON *json_res = cjsonp_search(json_root, "resolutions[0]width");
    if (json_res != NULL)
    {
        printf("path: resolutions[0]width \nsearch: %d\n\n", json_res->valueint);
    }

    int ret = cjsonp_delete(json_root, "resolutions[0]width");
    if (ret == 1)
    {
        res = cJSON_Print(json_root);
        printf("path: resolutions[0]width \ndelete: %s\n\n", res);
        cJSON_free(res);
    }

    cJSON *json_add =  cJSON_Parse("{\"width\":0, \"height\":0}");
    ret = cjsonp_add(json_root, "resolutions[2]", json_add);
    if (ret == 1)
    {
        res = cJSON_Print(json_root);
        printf("path: resolutions[2] \nadd: %s\n\n", res);
        cJSON_free(res);
    }

    cJSON *json_rep =  cJSON_Parse("1200");
    ret = cjsonp_replace(json_root, "resolutions[1]width", json_rep);
    if (ret == 1)
    {
        res = cJSON_Print(json_root);
        printf("path: resolutions[1]width \nreplace: %s\n\n", res);
        cJSON_free(res);
    }
    
    ret = cjsonp_swap(json_root, "resolutions[1]", "resolutions[2]");
    if (ret == 1)
    {
        res = cJSON_Print(json_root);
        printf("path: resolutions[0]width \nswap: %s\n\n", res);
        cJSON_free(res);
    }

    cJSON_Delete(json_root);

    return 0;
}