#include <stdio.h>

#include <GLWT/glwt.h>

#ifdef GLWT_TESTS_GLES
#include <GLXW/glxw_es2.h>
#else
#include <GLXW/glxw.h>
#endif

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    fprintf(stderr, "%s\n", msg);
}

static void window_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)window;
    (void)userdata;
    double time = glwtGetTime();

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            printf("[%lf] Window closed\n", time);
            break;
        case GLWT_WINDOW_EXPOSE:
            printf("[%lf] Window exposed\n", time);
            {
                glwtMakeCurrent(window);
                glwtSwapInterval(window, 1);

                glxwInit();

                glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glwtSwapBuffers(window);

                glwtMakeCurrent(NULL);
            }
            break;
        case GLWT_WINDOW_RESIZE:
            printf("[%lf] Window resized  width: %d  height: %d\n", time, event->resize.width, event->resize.height);
            break;
        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            printf("[%lf] Window %s\n", time, (event->type == GLWT_WINDOW_SHOW) ? "show" : "hide");
            break;
        case GLWT_WINDOW_FOCUS_IN:
        case GLWT_WINDOW_FOCUS_OUT:
            printf("[%lf] Window focus %s\n", time, (event->type == GLWT_WINDOW_FOCUS_IN) ? "in" : "out");
            break;
        case GLWT_WINDOW_KEY_UP:
        case GLWT_WINDOW_KEY_DOWN:
            printf("[%lf] Key %s  keysym: 0x%x  scancode: %d  mod: %X\n", time,
                (event->type == GLWT_WINDOW_KEY_DOWN) ? "down" : "up",
                event->key.keysym, event->key.scancode, event->key.mod);
            break;
        case GLWT_WINDOW_BUTTON_UP:
        case GLWT_WINDOW_BUTTON_DOWN:
            printf("[%lf] Button %s  x: %d  y: %d  button: %d  mod: %X\n", time,
                (event->type == GLWT_WINDOW_BUTTON_DOWN) ? "down" : "up",
                event->button.x, event->button.y, event->button.button, event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            printf("[%lf] Motion  x: %d  y: %d  buttons: %X\n", time,
                event->motion.x, event->motion.y, event->motion.buttons);
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            printf("[%lf] Mouse %s\n", time, (event->type == GLWT_WINDOW_MOUSE_ENTER) ? "enter" : "leave");
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    int err = -1;
    GLWTWindow *window = 0;
    GLWTConfig glwt_config = {
        0, 0, 0, 0,
        0, 0,
        0, 0,
#ifdef GLWT_TESTS_GLES
        GLWT_API_OPENGL_ES | GLWT_PROFILE_DEBUG,
        2, 0,
#else
        GLWT_API_OPENGL | GLWT_PROFILE_DEBUG,
        0, 0,
#endif
    };

    (void)argc; (void)argv;

    if(glwtInit(&glwt_config, error_callback, NULL) != 0)
        goto error;

    if(!(window = glwtWindowCreate("", 400, 300, NULL, window_callback, NULL)))
        goto error;

    glwtWindowSetTitle(window, "GLWT Events test");
    glwtWindowShow(window, 1);

    while(!glwtWindowClosed(window))
    {
        if(glwtEventHandle(1) != 0)
            goto error;
    }

    err = 0;
error:
    glwtWindowDestroy(window);
    glwtQuit();
    return err;
}
