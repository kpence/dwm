/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]	    = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#22cc22";
static const char selbgcolor[]	    = "#005577";
static const char selfgcolor[]	    = "#eeeeee";
//
static const char specialbgcolor[]	= "#444444";
static const char specialfgcolor[]	= "#eeeeee";
//
static const char floatnormbordercolor[]  = "#330055";
static const char floatselbordercolor[]   = "#be11ce";
//
static const unsigned int borderpx  = 4;	/* border pixel of windows */
static const unsigned int snap	    = 32;	/* snap pixel */
static const Bool showbar	    = True;	/* False means no bar */
static const Bool topbar	    = True;	/* False means bottom bar */

/* tagging */
#define MAX_TAGLEN 14
static char tags[][MAX_TAGLEN] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static char defaulttags[MAX_TAGLEN] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance	  title       tags mask     isfloating	 monitor */
	{ "Gimp",     NULL,	  NULL,       0,	    True,	 -1 },
	{ "Firefox",  NULL,	  NULL,       0,	    False,	 -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },	 /* first entry is default */
	{ "><>",      NULL },	 /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* CUSTOM includes */
#include "focusmaster.c"
#include "push.c"
#include "tagall.c"

/* CUSTOM functions declarations */
static void shiftview(const Arg *arg);
static void shifttag(const Arg *arg);
static void warptosel(const Arg *arg);
static void shiftmousepos(const Arg *arg);
static void vieworprev(const Arg *arg);
static void focusstackf(const Arg *arg);
static void nametag(const Arg *arg);
static void moveresize(const Arg *arg);
static void untogglefloating(const Arg *arg);
static void saveandquit(const Arg *arg);
static void loadsession();
static void spawnifselwin(const Arg *arg);


/* key definitions */
#define MODKEY Mod1Mask
#define MOD4KEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,			KEY,	  vieworprev,	  {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,		KEY,	  toggleview,	  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,		KEY,	  tag,		  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,	  toggletag,	  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *emacscmd[]  = { "emacs", NULL };
static const char *reapercmd[]  = { "reaper", NULL };

static Key keys[] = {
	/* modifier			key	   function	   argument */

    // Spawners
    { MODKEY,			        XK_o,      spawn,	       {.v = dmenucmd } },
    //{ MODKEY|ShiftMask,		    XK_Return, spawn,	       {.v = termcmd } },
    { MODKEY|ShiftMask,		    XK_t,      spawn,	       {.v = termcmd } },
    { MODKEY|ShiftMask,		    XK_f,      spawn,	       {.v = browsercmd } },
    { MODKEY|ShiftMask,		    XK_e,      spawn,	       {.v = emacscmd } },
    { MODKEY|ShiftMask,		    XK_r,      spawn,	       {.v = reapercmd } },
    { MODKEY|ShiftMask,		    XK_a,      spawn,	       SHCMD("urxvt -e alsamixer") },
    // ...
    //{ MODKEY|ShiftMask,	      XK_d,	 spawn,		 SHCMD("dwb") },
    { MODKEY|ControlMask,	    XK_t,      spawn,	       SHCMD("xsetroot -name $(date +%R)") },
    { MODKEY|ControlMask,	    XK_c,      spawn,	       SHCMD("clog") },
    // ...
    //{ 0,			        XK_Print,  spawn,		 SHCMD("scrot '%Y-%m-%d_$wx$h.png' -s -e 'mv $f ~/pic/scrot/'") },
    { MODKEY,			    XK_equal,  spawn,	       SHCMD("incvolume.sh m u") },
    { MODKEY,			    XK_minus,  spawn,	       SHCMD("incvolume.sh m d") },
    //{ MODKEY,			    XK_Up,     spawn,	       SHCMD("incvolume.sh m u") },
    //{ MODKEY,			    XK_Down,   spawn,	       SHCMD("incvolume.sh m d") },
    //{ MODKEY,			    XK_Left,  shifttag,       {.i = -1 } },
    //{ MODKEY,			    XK_Right, shifttag,       {.i = +1 } },
    // ... (hardware buttons)
    { 0,			   0x1008ff14, spawn,	       SHCMD("mpc play") },
    { ShiftMask,		   0x1008ff14, spawn,	       SHCMD("mpc pause") },
    { 0,			   0x1008ff13, spawn,	       SHCMD("incvolume.sh m u") },
    
    { 0,			   0x1008ff11, spawn,	       SHCMD("incvolume.sh m d") },
    { MODKEY,			   0x1008ff11, spawn,	       SHCMD("incvolume.sh s u") },
    { MODKEY,			   0x1008ff13, spawn,	       SHCMD("incvolume.sh s d") },


    // j/k (move/toggle selected clients)
    { MODKEY,			    XK_j,      focusstackf,    {.i = +1 } },
    { MODKEY,			    XK_j,      warptosel,      {0} },
    { MODKEY,			    XK_k,      focusstackf,    {.i = -1 } },
    { MODKEY,			    XK_k,      warptosel,      {0} },
    { MODKEY|ShiftMask,		    XK_j,      pushdown,       {0} },
    { MODKEY|ShiftMask,		    XK_j,      untogglefloating,       {0} },
    { MODKEY|ShiftMask,		    XK_k,      pushup,	       {0} },
    { MODKEY|ShiftMask,		    XK_k,      untogglefloating,	 {0} },

    //
    { MODKEY|ControlMask,	    XK_b,      togglebar,      {0} },
    { MOD4KEY,			    XK_b,      togglebar,      {0} },
    { MODKEY|ControlMask,	    XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY|ControlMask|ShiftMask, XK_d,      incnmaster,     {.i = -9999 } },
    { MODKEY|ControlMask|ShiftMask, XK_d,      incnmaster,     {.i = +1 } },
    { MODKEY|ControlMask,	    XK_i,      incnmaster,     {.i = +1 } },

    // h/l (resize verticle stack split)
    { MODKEY,			    XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,			    XK_h,      warptosel,      {0} },
    { MODKEY,			    XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,			    XK_l,      warptosel,      {0} },
    // ...
    { MODKEY|ShiftMask,		    XK_h,      setmfact,       {.f = -0.01} },
    { MODKEY|ShiftMask,		    XK_h,      warptosel,      {0} },
    { MODKEY|ShiftMask,		    XK_l,      setmfact,       {.f = +0.01} },
    { MODKEY|ShiftMask,		    XK_l,      warptosel,      {0} },

    // Window movement or resizing
    { MOD4KEY,			    XK_j,      moveresize,     {.v = (int []){ 0, 48, 0, 0 }}},
    { MOD4KEY,			    XK_k,      moveresize,     {.v = (int []){ 0, -48, 0, 0 }}},
    { MOD4KEY,			    XK_h,      moveresize,     {.v = (int []){ -48, 0, 0, 0 }}},
    { MOD4KEY,			    XK_l,      moveresize,     {.v = (int []){ 48, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_j,      moveresize,     {.v = (int []){ 0, 2, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_k,      moveresize,     {.v = (int []){ 0, -2, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_l,      moveresize,     {.v = (int []){ 4, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_h,      moveresize,     {.v = (int []){ -4, 0, 0, 0 }}},
    // ...
    { MOD4KEY|ControlMask,	    XK_d,      moveresize,     {.v = (int []){ 0, 1, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_u,      moveresize,     {.v = (int []){ 0, -1, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_a,      moveresize,     {.v = (int []){-1, 0, 0, 0 }}},
    { MOD4KEY|ControlMask,	    XK_e,      moveresize,     {.v = (int []){ 1, 0, 0, 0 }}},
    // ...
    { MOD4KEY|ShiftMask,	    XK_j,      moveresize,     {.v = (int []){ 0, 0, 0, 48 }}},
    { MOD4KEY|ShiftMask,	    XK_k,      moveresize,     {.v = (int []){ 0, 0, 0, -48 }}},
    { MOD4KEY|ShiftMask,	    XK_l,      moveresize,     {.v = (int []){ 0, 0, 48, 0 }}},
    { MOD4KEY|ShiftMask,	    XK_h,      moveresize,     {.v = (int []){ 0, 0, -48, 0 }}},

    // return (Sets client as master)
    { MODKEY,			    XK_Return, untogglefloating, {0} },
    { MODKEY,			    XK_Return, zoom,	       {0} },
    { MODKEY,			    XK_Return, warptosel,      {0} },
    // { MODKEY,			    XK_Return, spawnifselwin,  { .v = (const char*[]){ "Emacs", "/bin/sh", "-c", "emacs -e \"org-meta-return\"", NULL } } },

    // Layout stuff
    { MODKEY,			    XK_t,      untogglefloating, {0} },
    { MODKEY,			    XK_t,      setlayout,      {.v = &layouts[0]} },
    // ...
    { MODKEY|ControlMask,	    XK_f,      togglefloating, {0} },
    { MOD4KEY,			    XK_f,      togglefloating, {0} },
    // ...
    { MODKEY,			    XK_m,      untogglefloating,      {0} },
    { MODKEY,			    XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ShiftMask,		    XK_m,      untogglefloating,      {0} },
    { MODKEY|ShiftMask,		    XK_m,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,		    XK_m,      setlayout,      {.v = &layouts[2]} },

    // Multiple monitors
    { MODKEY,			    XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,			    XK_comma,  warptosel,      {0} },
    { MODKEY,			    XK_period, focusmon,       {.i = +1 } },
    { MODKEY,			    XK_period, warptosel,      {0} },
    // ...
    { MODKEY|ShiftMask,		    XK_comma,  tagmon,	       {.i = -1 } },
    { MODKEY|ShiftMask,		    XK_period, tagmon,	       {.i = +1 } },

    // Tag manipulation
    { MODKEY,			    XK_Tab,    vieworprev,     {0} },
    { MODKEY,			    XK_0,      vieworprev,     {.ui = ~0 } },
    { MODKEY|ShiftMask,		    XK_0,      tag,	       {.ui = ~0 } },
    // ... (name tag)
    { MODKEY,			  XK_apostrophe, nametag,      {0} },
    // ... (p/n)
    { MODKEY,			    XK_Prior,  shiftview,      {.i = -1 } },
    { MODKEY,			    XK_Next,   shiftview,      {.i = +1 } },
    { MODKEY|ControlMask,	    XK_p,      shiftview,      {.i = -1 } },
    { MODKEY|ControlMask,	    XK_n,      shiftview,      {.i = +1 } },
    { MODKEY|ShiftMask,		    XK_p,      shifttag,       {.i = -1 } },
    { MODKEY|ShiftMask,		    XK_p,      shiftview,      {.i = -1 } },
    { MODKEY|ShiftMask,		    XK_n,      shifttag,       {.i = +1 } },
    { MODKEY|ShiftMask,		    XK_n,      shiftview,      {.i = +1 } },
    // ... (shift sel client's tags)
    { MODKEY|ControlMask,	    XK_comma,  shifttag,       {.i = -1 } },
    { MODKEY|ControlMask,	    XK_period, shifttag,       {.i = +1 } },
    // ...
    TAGKEYS(			    XK_1,		       0)
    TAGKEYS(			    XK_2,		       1)
    TAGKEYS(			    XK_3,		       2)
    TAGKEYS(			    XK_4,		       3)
    TAGKEYS(			    XK_5,		       4)
    TAGKEYS(			    XK_6,		       5)
    TAGKEYS(			    XK_7,		       6)
    TAGKEYS(			    XK_8,		       7)
    TAGKEYS(			    XK_9,		       8)

    // Quit and close
    { MODKEY|ShiftMask,		    XK_c,      killclient,     {0} },
    { MODKEY|ShiftMask,		    XK_q,      saveandquit,	   {0} },

    /* unused (stored just in case)
    { MODKEY|ShiftMask|ControlMask, XK_q,      logoutf,        {0} },
    { MOD4KEY,			    XK_y,      spawn,	       SHCMD("sleep 1 && xdotool key --clearmodifiers Shift+Insert") },
    */

};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
/* Button{1..5} are the left, middle, and right mouse buttons then scroll up and down respectively */
static Button buttons[] = {
	/* click	       event mask	button	       function        argument */
	{ ClkLtSymbol,	       0,		Button1,       setlayout,      {0} },
	{ ClkLtSymbol,	       0,		Button3,       setlayout,      {.v = &layouts[2]} },
    //
	{ ClkWinTitle,	       0,		Button2,       untogglefloating,{0} },
	{ ClkWinTitle,	       0,		Button2,       zoom,	       {0} },
	{ ClkWinTitle,	       MODKEY,		Button4,       setmfact,       {.f = -0.05} },
	{ ClkWinTitle,	       MODKEY,		Button5,       setmfact,       {.f = +0.05} },
	{ ClkWinTitle,	       MODKEY|ShiftMask,Button4,       setmfact,       {.f = -0.01} },
	{ ClkWinTitle,	       MODKEY|ShiftMask,Button5,       setmfact,       {.f = +0.01} },
    //
	{ ClkStatusText,       0,		Button1,       spawn,	       SHCMD("xsetroot -name $(date +%R)") },
	{ ClkStatusText,       MODKEY,		Button2,       spawn,	       {.v = termcmd } },
	{ ClkStatusText,       MODKEY,		Button2,       spawn,	       {.v = termcmd } },
	{ ClkStatusText,       MODKEY|ShiftMask,Button2,       spawn,	       {.v = termcmd } },
    //
	{ ClkClientWin,        MODKEY,		Button1,       movemouse,      {0} },
	{ ClkClientWin,        MODKEY,		Button2,       togglefloating, {0} },
	{ ClkClientWin,        MODKEY,		Button3,       resizemouse,    {0} },
	{ ClkClientWin,        MOD4KEY,		Button1,       movemouse,      {0} },
	{ ClkClientWin,        MOD4KEY,		Button2,       togglefloating, {0} },
	{ ClkClientWin,        MOD4KEY,		Button3,       resizemouse,    {0} },
    //
	{ ClkTagBar,	       0,		Button1,       vieworprev,     {0} }, // left click
	{ ClkTagBar,	       MODKEY,		Button1,       toggleview,     {0} },
	{ ClkTagBar,	       0,		Button2,       toggleview,     {0} }, // middle click
	{ ClkTagBar,	       MODKEY,		Button2,       toggleview,     {0} },
	{ ClkTagBar,	       0,		Button3,       tag,	       {0} }, // right click
	{ ClkTagBar,	       MODKEY,		Button3,       toggletag,      {0} },
};

/***********************************************************************************************/


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
shifttag(const Arg *arg) {
    Arg shifted;

    if(!selmon->sel) {
	return;
    }

    if(arg->i > 0) // left circular shift
	shifted.ui = (selmon->sel->tags << arg->i)
	   | (selmon->sel->tags >> (LENGTH(tags) - arg->i));

    else // right circular shift
	shifted.ui = selmon->sel->tags >> (- arg->i)
	   | selmon->sel->tags << (LENGTH(tags) + arg->i);

    tag(&shifted);
}

void
warptosel(const Arg *arg) {
    XEvent ev;
 
    if(!selmon->sel) {
	return;
    }

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
    char *p, name_prefix[3], name[MAX_TAGLEN-3];
    FILE *f;
    int i;

    errno = 0; // popen(3p) says on failure it "may" set errno
    if(!(f = popen("dmenu < /dev/null", "r"))) {
	fprintf(stderr, "dwm: popen 'dmenu < /dev/null' failed%s%s\n", errno ? ": " : "", errno ? strerror(errno) : "");
	return;
    }
    if (!(p = fgets(name, MAX_TAGLEN-3, f)) && (i = errno) && ferror(f))
	fprintf(stderr, "dwm: fgets failed: %s\n", strerror(i));
    if (pclose(f) < 0)
	fprintf(stderr, "dwm: pclose failed: %s\n", strerror(errno));
    if(!p)
	return;
    if((p = strchr(name, '\n')))
	*p = '\0';

    for(i = 0; i < LENGTH(tags); i++) {
	if(selmon->tagset[selmon->seltags] & (1 << i)) {
	    name_prefix[0] = defaulttags[i];
	    if (name[0] == '\0') {
		name_prefix[1] = '\0';
	    }
	    else {
		name_prefix[1] = ' ';
		name_prefix[2] = '\0';
	    }
	    strcpy(tags[i], name_prefix);
	    strcat(tags[i], name);
	}
    }
    drawbars();
}

void
saveandquit(const Arg *arg)
{
    char session[(MAX_TAGLEN+1) * LENGTH(tags)];
    FILE* f;
    int i;

    // Get session information
    // TODO -- save the window locations.
    strcpy(session, tags[0]);
    strcat(session, "\n");
    for(i = 1; i < LENGTH(tags); i++) {
        strcat(session, tags[i]);
        strcat(session, "\n");
    }

    // Save in a /tmp file
    f = fopen("/tmp/.dwm.session", "w");
    fprintf(f, "%s", session);
    fclose(f);

    // Now quit
    quit(arg);
}

void
spawnifselwin(const Arg *arg) {
	const char *class, *instance;
	unsigned int i;
	Monitor *m;
  Client* c;
	XClassHint ch = { NULL, NULL };

  /* arguments */
  const Arg spawncmd = (const Arg)SHCMD(&(arg->v[1]));
  const char *expected_class = ((const char*)(arg->v))[0];

	/* rule matching */
  c = selmon->sel;
	c->isfloating = c->tags = 0;
	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name  ? ch.res_name  : broken;

  if(strstr(class, expected_class))
    {
      spawn(&spawncmd);
    }
	if(ch.res_class)
		XFree(ch.res_class);
	if(ch.res_name)
		XFree(ch.res_name);
	c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];
}

#include "loadsession.c"
#include "moveresize.c"
