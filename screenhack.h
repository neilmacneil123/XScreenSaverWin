#ifndef __SCREENHACK_H__
#define __SCREENHACK_H__

#ifndef __XWS2WIN_H__
    #include "xws2win.h"
#endif

#include <scrnsave.h>
#include "yarandom.h"
#include "colors.h"
#include "hsv.h"

extern const char *progname;
extern Bool mono_p;

#ifndef NOMSGBOXOUTPUT
    int __cdecl win32_fprintf(FILE *fp, const char *fmt, ...);
    void __cdecl win32_abort(void);
    int __cdecl win32_exit(int n);

    #define fprintf win32_fprintf
    #define abort win32_abort
    #define exit win32_exit
#endif

#define inline  /*empty*/
#define GETTIMEOFDAY_TWO_ARGS 1

//////////////////////////////////////////////////////////////////////////////

typedef struct screenhack_timeval
{
    DWORD tv_sec;
    DWORD tv_usec;
} screenhack_timeval;

typedef struct screenhack_timezone
{
	char dummy1, dummy2;
} screenhack_timezone;

#define timeval screenhack_timeval
#define timezone screenhack_timezone

#define HAVE_GETTIMEOFDAY 1
void gettimeofday(timeval *t, timezone *tz);

//////////////////////////////////////////////////////////////////////////////

typedef enum
{
    t_Bool, t_Int, t_Float, t_String
} argdatatype;

typedef struct
{
    void *data;
    char *name;
    char *text;
    char *def;
    argdatatype type;
} argtype;

//////////////////////////////////////////////////////////////////////////////

#define XSCREENSAVER_LINK(NAME)

typedef void *(*HACK_INIT)(Display *, Window);
typedef unsigned long (*HACK_DRAW)(Display *, Window, void *);
typedef void (*HACK_RESHAPE)(Display *, Window, void *, unsigned int, unsigned int);
typedef void (*HACK_FREE)(Display *, Window, void *);

extern HACK_INIT hack_init;
extern HACK_DRAW hack_draw;
extern HACK_RESHAPE hack_reshape;
extern HACK_FREE hack_free;
extern int hack_argcount;
extern argtype *hack_arginfo;

#ifdef NOARGS
    #define XSCREENSAVER_MODULE_2(CLASS,NAME,PREFIX) \
	    const char *progname = #NAME; \
        HACK_INIT hack_init = PREFIX ## _init; \
        HACK_DRAW hack_draw = PREFIX ## _draw; \
        HACK_RESHAPE hack_reshape = PREFIX ## _reshape; \
        HACK_FREE hack_free = PREFIX ## _free; \
        int hack_argcount = 0; \
        argtype *hack_arginfo = NULL;
#else
    #define XSCREENSAVER_MODULE_2(CLASS,NAME,PREFIX) \
	    const char *progname = #NAME; \
        HACK_INIT hack_init = PREFIX ## _init; \
        HACK_DRAW hack_draw = PREFIX ## _draw; \
        HACK_RESHAPE hack_reshape = PREFIX ## _reshape; \
        HACK_FREE hack_free = PREFIX ## _free; \
        int hack_argcount = sizeof(vars) / sizeof(vars[0]); \
        argtype *hack_arginfo = vars;
#endif

#define XSCREENSAVER_MODULE(CLASS,PREFIX) \
      XSCREENSAVER_MODULE_2(CLASS,PREFIX,PREFIX)

//////////////////////////////////////////////////////////////////////////////

typedef struct async_load_state
{
    Bool done_p;
    char *filename;
    XRectangle geom;
} async_load_state;

async_load_state *load_image_async_simple(
    async_load_state *state, Screen *screen, Window window, Drawable target, 
    char **filename_ret, XRectangle *geometry_ret);

//////////////////////////////////////////////////////////////////////////////
// screen saver

typedef struct SCREENSAVER
{
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    UINT x0, y0;
    UINT width, height;
    HBITMAP hbmScreenShot;
    Display *dpy;
    Window window;
    XWindowAttributes xgwa;
    void *closure;
} SCREENSAVER;

extern SCREENSAVER ss;

BOOL InitPixelFormat(SCREENSAVER *ss);
VOID MakeCurrent(SCREENSAVER *ss);
XImage *GetScreenShotXImage(void);
BOOL ss_init(HWND hwnd);
void ss_term(void);
void ss_clear(Display *d);
Bool has_writable_cells(Screen *s, Visual *v);
int visual_depth(Screen *screen, Visual *visual);

//////////////////////////////////////////////////////////////////////////////

#endif /* __SCREENHACK_H__ */