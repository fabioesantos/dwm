/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define TERMINAL "st"
#define STATUSBAR "dwmblocks"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code Nerd Font:pixelsize=11" };
static const char dmenufont[]       = "Fira Code Nerd Font:pixelsize=11";
static const char norm_fg[]         = "#0abdc6";
static const char norm_bg[]         = "#091833";
static const char norm_border[]     = "#091833";
static const char sel_fg[]          = "#ea00d9";
static const char sel_bg[]          = "#321959";
static const char sel_border[]      = "#0abdc6";

/* bar opacity
 * 0xff is no transparency.
 * 0xee adds wee bit of transparency.
 * 0xdd adds adds a bit more transparency.
 * Play with the value to get desired transparency.
 */
static const unsigned int baralpha  = 0xee;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
    /*               fg       bg       border   */
    [SchemeNorm] = { norm_fg, norm_bg, norm_border },
    [SchemeSel]  = { sel_fg,  sel_bg,  sel_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static const char drun_prompt[] = "run:"; /* dmenu prompt */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", drun_prompt, "-m", dmenumon, "-fn",dmenufont, "-nb", norm_bg, "-nf", norm_fg, "-sb", sel_bg, "-sf", sel_fg, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *xob[] = { "alsa-xob", NULL };
static const char *upvol[] = { "amixer", "-q", "set", "Master", "5%+", NULL };
static const char *downvol[] = { "amixer", "-q", "set", "Master", "5%-", NULL };
static const char *mute[] = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *screenshot[] = { "/bin/sh", "-c", "import -window root $HOME/Pictures/Screenshots/$(date +'%s_print.png') && notify-send '  Screenshot saved!'", NULL };
static const char *pausempd[] = { "mpc", "toggle", NULL };
static const char *music[] = { TERMINAL, "-e", "ncmpcpp-ueberzug", NULL };
static const char *browser[] = { "luakit", NULL };
static const char *email[] = { "/bin/sh", "-c", "st -e neomutt && pkill -RTMIN+11 dwmblocks", NULL };
static const char *mixertui[] = { TERMINAL, "-e", "alsamixer", NULL };
static const char *sysact[] = { "sysact", NULL };
static const char *bookmarks[] = { "bookmarks", NULL };
static const char *brightup[] = { "/bin/sh", "-c", "xbacklight -inc 5 -steps 30 && pkill -RTMIN+12 dwmblocks", NULL };
static const char *brightdown[] = { "/bin/sh", "-c", "xbacklight -dec 5 -steps 30 && pkill -RTMIN+12 dwmblocks", NULL };
static const char *comp[] = { "togglecomp", NULL };
static const char *rss[] = { "rss_feed", NULL };
static const char *youtube[] = { "youtube", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
        { MODKEY|ShiftMask,             XK_c,      quit,           {1} },
        { MODKEY|ShiftMask,             XK_b,      spawn,          {.v = bookmarks } },
        { MODKEY|ShiftMask,             XK_r,      spawn,          {.v = sysact } },
        { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = browser } },
        { MODKEY|ShiftMask,             XK_m,      spawn,          {.v = music } },
        { MODKEY,                       XK_a,      spawn,          {.v = mixertui } },
        { MODKEY,                       XK_e,      spawn,          {.v = email } },
        { MODKEY,                       XK_r,      spawn,          {.v = rss } },
        { MODKEY,                       XK_y,      spawn,          {.v = youtube } },
        { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = upvol } },
        { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = downvol } },
        { 0,         XF86XK_AudioMute,             spawn,          {.v = mute } },
        { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = xob } },
        { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = xob } },
        { 0,         XF86XK_AudioMute,             spawn,          {.v = xob } },
        { 0,         XF86XK_MonBrightnessUp,       spawn,          {.v = brightup } },
        { 0,         XF86XK_MonBrightnessDown,     spawn,          {.v = brightdown } },
        { 0,         XK_Pause,                     spawn,          {.v = pausempd } },
        { 0,         XK_Print,                     spawn,          {.v = screenshot } },
        { 0,         XK_F8,                        spawn,          {.v = comp } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
        { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
