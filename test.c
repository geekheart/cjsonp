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

int main(void)
{
    char *res = cjsonp_search(json_text, "resolutions[0]width");
    if (res != NULL)
    {
        printf("path: resolutions[0]width \nsearch: %s\n\n", res);
    }
    cjsonp_free(res);

    res = cjsonp_delete(json_text, "resolutions[0]width");
    if (res != NULL)
    {
        printf("path: resolutions[0]width \ndelete: %s\n\n", res);
    }
    cjsonp_free(res);

    res = cjsonp_add(json_text, "resolutions[2]", "{\"width\":0, \"height\":0}");
    if (res != NULL)
    {
        printf("path: resolutions[2] \nadd: %s\n\n", res);
    }
    cjsonp_free(res);

    res = cjsonp_replace(json_text, "resolutions[0]width", "1200");
    if (res != NULL)
    {
        printf("path: resolutions[0]width \nreplace: %s\n\n", res);
    }
    cjsonp_free(res);

    int ret = cjsonp_remove(json_text, "resolutions[0]width");
    if (ret)
    {
        printf("path: resolutions[0]width \nremove: %s\n\n", json_text);
    }
    return 0;
}