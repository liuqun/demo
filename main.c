#include "tool.h"
int main()
{
    int err;
    err = tool_init();
    if (err)
    {
        return (255);
    }
    return (0);
}
