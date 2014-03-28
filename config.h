/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#22cc22";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
//
static const char specialbgcolor[]      = "#444444";
static const char specialfgcolor[]      = "#eeeeee";
//
static const char floatnormbordercolor[]  = "#330055";
static const char floatselbordercolor[]   = "#be11ce";
//
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
#define MAX_TAGLEN 14
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static char defaulttags[MAX_TAGLEN] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",  NULL,       NULL,       0,            False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* CUSTOM includes */
#include "focusmaster.c"
#include "push.c"
#include "tagall.c"

/* CUSTOM functions declarations */
static void shiftview(const Arg *arg);
static void warptosel(const Arg *arg);
static void shiftmousepos(const Arg *arg);
static void vieworprev(const Arg *arg);
static void focusstackf(const Arg *arg);
static void nametag(const Arg *arg);
static void moveresize(const Arg *arg);
static void untogglefloating(const Arg *arg);
static void togglemouse(const Arg *arg);


/* CUSTOM enums */
enum {DIR_RIGHT, DIR_UP, DIR_LEFT, DIR_DOWN};

/* key definitions */
#define MODKEY Mod1Mask
#define MOD4KEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      vieworprev,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *slmenucmd[] = { "slmenu", NULL };
static const char *touchpadon[] = { "touchpad", "1", NULL };
static const char *touchpadoff[] = { "touchpad", "0", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MODKEY,                       XK_o,      spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("wpa_gui") },
    { MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("dwb") },
    { MODKEY|ShiftMask,             XK_f,      spawn,          {.v = browsercmd } },
    { MODKEY|ControlMask,           XK_p,      shiftview,      {.i = -1 } },
    { MODKEY|ControlMask,           XK_n,      shiftview,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
    { MODKEY|ShiftMask,             XK_j,      untogglefloating,       {0} },
    { MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
    { MODKEY|ShiftMask,             XK_k,      untogglefloating,         {0} },
    { MOD4KEY,                      XK_y,      spawn,          SHCMD("sleep 1 && xdotool key --clearmodifiers Shift+Insert") },
    { 0,                            XK_Print,  spawn,          SHCMD("scrot '%Y-%m-%d_$wx$h.png' -e 'mv $f ~/pic/scrot/'") },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstackf,    {.i = +1 } },
/**/{ MODKEY,                       XK_j,      warptosel,      {0} },
    { MODKEY,                       XK_k,      focusstackf,    {.i = -1 } },
/**/{ MODKEY,                       XK_k,      warptosel,      {0} },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_h,      warptosel,      {0} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_l,      warptosel,      {0} },

    { MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.01} },
    { MODKEY|ShiftMask,             XK_h,      warptosel,      {0} },
    { MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.01} },
    { MODKEY|ShiftMask,             XK_l,      warptosel,      {0} },

    { MOD4KEY,                      XK_j,      moveresize,     {.v = (int []){ 0, 48, 0, 0 }}},
    { MOD4KEY,                      XK_k,      moveresize,     {.v = (int []){ 0, -48, 0, 0 }}},
    { MOD4KEY,                      XK_l,      moveresize,     {.v = (int []){ 48, 0, 0, 0 }}},
    { MOD4KEY,                      XK_h,      moveresize,     {.v = (int []){ -48, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_k,      moveresize,     {.v = (int []){ 0, -2, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_j,      moveresize,     {.v = (int []){ 0, 2, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_l,      moveresize,     {.v = (int []){ 4, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_h,      moveresize,     {.v = (int []){ -4, 0, 0, 0 }}},

    { MOD4KEY|ControlMask,          XK_a,      moveresize,     {.v = (int []){-1, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_e,      moveresize,     {.v = (int []){ 1, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_u,      moveresize,     {.v = (int []){ 0, -1, 0, 0 }}},
    { MOD4KEY|ControlMask,          XK_d,      moveresize,     {.v = (int []){ 0, 1, 0, 0 }}},

    { MOD4KEY|ShiftMask,            XK_j,      moveresize,     {.v = (int []){ 0, 0, 0, 48 }}},
    { MOD4KEY|ShiftMask,            XK_k,      moveresize,     {.v = (int []){ 0, 0, 0, -48 }}},
    { MOD4KEY|ShiftMask,            XK_l,      moveresize,     {.v = (int []){ 0, 0, 48, 0 }}},
    { MOD4KEY|ShiftMask,            XK_h,      moveresize,     {.v = (int []){ 0, 0, -48, 0 }}},

    { MODKEY,                       XK_Return, untogglefloating, {0} },
    { MODKEY,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Return, warptosel,      {0} },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_Tab,    vieworprev,     {0} },
    { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY,                       XK_t,      untogglefloating, {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
//  { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_f,      togglefloating, {0} },
    { MOD4KEY,                      XK_f,      togglefloating, {0} },
    { MODKEY,                       XK_m,      untogglefloating,      {0} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_apostrophe,      nametag,        {0} },
//  { MODKEY,                       XK_space,  setlayout,      {0} },
//  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_0,      vieworprev,     {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
/**/{ MODKEY,                       XK_period, warptosel,      {0} },
/**/{ MODKEY,                       XK_comma,  warptosel,      {0} },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_m,      spawn,          {.v = touchpadoff} },
    { MODKEY|ShiftMask|ControlMask, XK_m,      spawn,          {.v = touchpadon} },
//  { MODKEY|ShiftMask,             XK_y,      xclipyankf,     {0} },
//  { MODKEY|ShiftMask,             XK_p,      xclipputf,      {0} },
    { MODKEY|ControlMask,           XK_c,      spawn,          SHCMD("clog") },
    { 0,                           0x1008ff13, spawn,          SHCMD("incvolume.sh u") },
    { 0,                           0x1008ff11, spawn,          SHCMD("incvolume.sh d") },
    { MODKEY,                      0x1008ff11, spawn,          SHCMD("incvolume.sh sd") },
    { MODKEY,                      0x1008ff13, spawn,          SHCMD("incvolume.sh su") },
    { 0,                           0x1008ff41, togglemouse,    {0} },
    { 0,                           0x1008ff2d, spawn,          SHCMD("xscreensaver-command -l") },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
//  { MODKEY|ShiftMask|ControlMask, XK_q,      logoutf,        {0} },

};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
/* Button{1..5} are the left, middle, and right mouse buttons then scroll up and down respectively */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },

	{ ClkWinTitle,          0,              Button2,       untogglefloating,{0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          MODKEY,         Button4,        setmfact,       {.f = +0.05} },
	{ ClkWinTitle,          MODKEY,         Button5,        setmfact,       {.f = -0.05} },
	{ ClkWinTitle,          MODKEY|ShiftMask,Button4,       setmfact,       {.f = +0.01} },
	{ ClkWinTitle,          MODKEY|ShiftMask,Button5,       setmfact,       {.f = -0.01} },

	{ ClkStatusText,        MODKEY,         Button2,        spawn,          {.v = termcmd } },

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },

	{ ClkTagBar,            0,              Button1,        vieworprev,     {0} }, // left click
	{ ClkTagBar,            MODKEY,         Button1,        toggleview,     {0} },

	{ ClkTagBar,            0,              Button2,        toggleview,     {0} }, // middle click
	{ ClkTagBar,            MODKEY,         Button2,        toggleview,     {0} },

	{ ClkTagBar,            0,              Button3,        tag,            {0} }, // right click
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/***********************************************************************************************/


void
togglemouse(const Arg *arg) {
    Arg s;
    static int on = 0;
    if (on == 1) {
        s.v = touchpadon;
        on = 0;
    }
    else {
        s.v = touchpadoff;
        on = 1;
    }
    spawn(&s);
}

void
untogglefloating(const Arg *arg) {
    if(!selmon->sel) {
        return;
    }

    if(selmon->sel->isfloating)
        togglefloating(NULL);
}

void
shiftview(const Arg *arg) {
    Arg shifted;

    if(arg->i > 0) // left circular shift
        shifted.ui = (selmon->tagset[selmon->seltags] << arg->i)
           | (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));

    else // right circular shift
        shifted.ui = selmon->tagset[selmon->seltags] >> (- arg->i)
           | selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);

    view(&shifted);
}

void
warptosel(const Arg *arg) {
    XEvent ev;
 
    if(selmon->sel)
        XWarpPointer(dpy, None, selmon->sel->win, 0, 0, 0, 0, 0, 0);
    XSync(dpy, False);
    while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
shiftmousepos(const Arg *arg) {
    XEvent ev;
    Display *dpy;
    Window root_window;
    dpy = XOpenDisplay(0);
    root_window = XRootWindow(dpy, 0);
    int x, y;
    x = ((int *)arg->v)[0];
    y = ((int *)arg->v)[1];

    XWarpPointer(dpy, None, selmon->sel->win, x, y, 0, 0, 100, 100);
    XSelectInput(dpy, root_window, KeyReleaseMask);
    XFlush(dpy);
    XSync(dpy, False);
    while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
vieworprev(const Arg *arg) {
    Arg a = {0};

    view(((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags]) ? &a : arg);
}

void
focusstackf(const Arg *arg) {
    Client *c = NULL, *i;
 
    if(!selmon->sel) {
        return;
    }
    if(selmon->lt[selmon->sellt]->arrange) {
        if (arg->i > 0) {
            for(c = selmon->sel->next; c && (!ISVISIBLE(c, selmon) || c->isfloating != selmon->sel->isfloating); c = c->next);
            if(!c) {
                for(c = selmon->cl->clients; c && (!ISVISIBLE(c, selmon) || c->isfloating == selmon->sel->isfloating); c = c->next);
            }
        }
        else {
            for(i = selmon->cl->clients; i != selmon->sel; i = i->next) {
                if(ISVISIBLE(i, selmon) && i->isfloating == selmon->sel->isfloating) {
                    c = i;
                }
            }
            if(!c) {
                for(i =  selmon->sel; i; i = i->next) {
                    if(ISVISIBLE(i, selmon) && i->isfloating != selmon->sel->isfloating) {
                        c = i;
                    }
                }
            }
        }
    }
    if(c) {
        focus(c);
        restack(selmon);
    }
    else {
        focusstack(arg);
    }
}

void
nametag(const Arg *arg) {
    char *p, name[MAX_TAGLEN];
    FILE *f;
    int i;

    errno = 0; // popen(3p) says on failure it "may" set errno
    if(!(f = popen("dmenu < /dev/null", "r"))) {
        fprintf(stderr, "dwm: popen 'dmenu < /dev/null' failed%s%s\n", errno ? ": " : "", errno ? strerror(errno) : "");
        return;
    }
    if (!(p = fgets(name, MAX_TAGLEN, f)) && (i = errno) && ferror(f))
        fprintf(stderr, "dwm: fgets failed: %s\n", strerror(i));
    if (pclose(f) < 0)
        fprintf(stderr, "dwm: pclose failed: %s\n", strerror(errno));
    if(!p)
        return;
    if((p = strchr(name, '\n')))
        *p = '\0';

    for(i = 0; i < LENGTH(tags); i++) {
        if(selmon->tagset[selmon->seltags] & (1 << i)) {
            if (name[0] == '\0') {
                name[0] = defaulttags[i];
                name[1] = '\0';
            }
            strcpy(tags[i], name);
        }
    }
    drawbars();
}

#include "moveresize.c"
