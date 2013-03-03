#include <string.h>

#include <glwt_internal.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, __FILE__, __VA_ARGS__))

int glwtInit(
    const GLWTConfig *config,
    void (*error_callback)(const char *msg, void *userdata),
    void *userdata
    )
{
    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    if(glwtInitEGL(config) != 0)
        goto error;

    return 0;
error:
    glwtQuit();
    return -1;
}

void glwtQuit()
{
    glwtQuitEGL();

    memset(&glwt, 0, sizeof(struct glwt));
}

int glwtEventHandle(int wait)
{
    (void)wait;
    return 0;
}

static void onStart(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onStart");
}

static void onResume(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onResume");
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
    (void)activity;
    LOGI("ANativeActivity onSaveInstanceState");

    *outSize = 0;
    return 0;
}

static void onPause(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onPause");
}

static void onStop(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onStop");
}

static void onDestroy(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onDestroy");

    //egl_quit();
}

static void onWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
    (void)activity;
    (void)hasFocus;
    LOGI("ANativeActivity onWindowFocusChanged");
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    (void)activity;
    (void)window;
    LOGI("ANativeActivity onNativeWindowCreated");

    //gles_init(window);

    LOGI("**** NATIVE WINDOW: %p\n", window);

}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
    (void)activity;
    (void)window;
    LOGI("ANativeActivity onNativeWindowResized");
}

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
    (void)activity;
    (void)window;
    LOGI("ANativeActivity onNativeWindowRedrawNeeded");

    //gles_paint();
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    (void)activity;
    (void)window;
    LOGI("ANativeActivity onNativeWindowDestroyed");

    LOGI("**** NATIVE WINDOW: %p\n", window);

    //gles_quit();
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    (void)activity;
    LOGI("ANativeActivity onInputQueueCreated");

    (void)queue;
    /*
    ALooper* looper = ALooper_forThread();
    int ident = ALOOPER_POLL_CALLBACK;
    int (*callback)(int fd, int events, void* data) = input_callback;
    void *data = (void*)queue;
    AInputQueue_attachLooper(queue, looper, ident, callback, data);
    */
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    (void)activity;
    LOGI("ANativeActivity onInputQueueDestroyed");

    (void)queue;
    //AInputQueue_detachLooper(queue);
}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    (void)activity;
    (void)rect;
    LOGI("ANativeActivity onContentRectChanged");
}

static void onConfigurationChanged(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onConfigurationChanged");
}

static void onLowMemory(ANativeActivity* activity)
{
    (void)activity;
    LOGI("ANativeActivity onLowMemory");
}

void ANativeActivity_onCreate(
    ANativeActivity* activity,
    void* saved_state,
    size_t saved_state_size)
{
    (void)saved_state;
    (void)saved_state_size;

    glwt.android.activity = activity;

    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;

    activity->instance = (void*)0xdeadbeef;

    //egl_init();
}
