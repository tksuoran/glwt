#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata),
    void *userdata)
{
    GLWTWindow *win= calloc(1, sizeof(struct GLWTWindow));
    if(!win)
        return 0;

    win->win_callback = win_callback;
    win->userdata = userdata;

    RECT rect;
    rect.left = rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    int style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
    int exstyle = WS_EX_OVERLAPPEDWINDOW;

    if(!AdjustWindowRectEx(&rect, style, 0, exstyle))
    {
        glwtWin32Error("AdjustWindowRect failed");
        goto error;
    }

    WCHAR nullterm = 0;
    intptr_t classptr = glwt.win32.classatom;
    win->win32.hwnd = CreateWindowExW(
        exstyle,
        (LPCWSTR)classptr,
        &nullterm,  // window title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        glwt.win32.hinstance,
        0);
    if(!win->win32.hwnd)
    {
        glwtWin32Error("CreateWindowExW failed");
        goto error;
    }

    SetWindowLongPtr(win->win32.hwnd, 0, (LONG_PTR)win);

    win->win32.hdc = GetDC(win->win32.hwnd);
    if(!win->win32.hdc)
    {
        glwtWin32Error("GetDC failed");
        goto error;
    }

    PIXELFORMATDESCRIPTOR pfd;
    if(!SetPixelFormat(win->win32.hdc, glwt.win32.pixel_format, &pfd))
    {
        glwtWin32Error("SetPixelFormat failed");
        goto error;
    }

#ifdef GLWT_USE_EGL
    if(glwtWindowCreateEGL(win, share))
#else
    if(glwtWindowCreateWGL(win, share))
#endif
        goto error;

    (void)title;

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;

    glwtWindowDestroyWGL(win);

    if(win->win32.hdc)
        ReleaseDC(win->win32.hwnd, win->win32.hdc);

    if(win->win32.hwnd)
        DestroyWindow(win->win32.hwnd);

    free(win);
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    ShowWindow(win->win32.hwnd, show ? SW_SHOWNORMAL : SW_HIDE);
}

void glwtWindowSetTitle(GLWTWindow *win, const char *title)
{
    (void)win; (void)title;
}
