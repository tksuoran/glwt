#include <windows.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

int glwtInit(const GLWTConfig *config, const GLWTAppCallbacks *app_callbacks)
{
    if(app_callbacks)
        glwt.app_callbacks = *app_callbacks;

    glwtErrorPrintf("Win32 sucks!\n");
    return -1;
}

void glwtQuit()
{
}
