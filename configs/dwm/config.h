/* See LICENSE file for copyright and license details. */

/* constants */
static const char TERMINAL[] = "st";

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */

static const int showbar = 1;
static const int topbar = 1;

static const char *fonts[]          = { "monospace:size=9:antialias=true:autohint=true" };
static const char dmenufont[]       =   "monospace:size=9:antialias=true:autohint=true";

static const char normbgcolor[]	    = "#222222";
static const char normbordercolor[] = "#444444";
static const char normfgcolor[]	    = "#bbbbbb";
static const char slctbgcolor[]	    = "#005577";
static const char slctbordercolor[] = "#770000";
static const char slctfgcolor[]	    = "#eeeeee";
static const char *colors[][3]	    = {
	/*		 fg		bg		border		*/
	[SchemeNorm] = { normfgcolor,	normbgcolor,	normbordercolor },
	[SchemeSel]  = { slctfgcolor,	slctbgcolor,	slctbordercolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance               title       tags mask     isfloating   monitor */
	{ NULL,               NULL,                  NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } },       \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } },       \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } },       \
	{ MOD, XK_x,     ACTION##stack, {.i = -1  } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { TERMINAL, NULL };

static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbordercolor, "-nf", normfgcolor, "-sb", slctbordercolor, "-sf", slctfgcolor, NULL };

static const char *browsercmd[] = { "surf", NULL };
static const char *emailcmd[] = { "thunderbird", NULL };
static const char *irccmd[] = { TERMINAL, "irc", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { TERMINAL, "-t", scratchpadname, "-g" "120x34", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key            function        argument */
	{ MODKEY|ShiftMask,             XK_Return,     spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_p,          spawn,          {.v = dmenucmd } },

	{ MODKEY,                       XK_b,          spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_m,          spawn,          {.v = emailcmd } },
	{ MODKEY,                       XK_i,          spawn,          {.v = irccmd } },

	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)

	{ MODKEY,                       XK_h,          setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_equal,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Return,     zoom,           {0} },
	{ MODKEY,                       XK_Tab,        view,           {0} },

	{ MODKEY|ShiftMask,             XK_c,          killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,          quit,           {0} },
	{ MODKEY|ShiftMask,             XK_l,          spawn,          SHCMD("slock") },
	{ MODKEY|ShiftMask,             XK_Escape,     spawn,          SHCMD("sudo poweroff") },

	{ MODKEY|ShiftMask,             XK_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,          fullscreen,     {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating, {0} },

	{ MODKEY,                       XK_apostrophe, togglescratch, {.v = scratchpadcmd } },

	{ MODKEY,                       XK_0,          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,            {.ui = ~0 } },

	{ MODKEY,                       XK_comma,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,         {.i = +1 } },

	TAGKEYS(                        XK_1,                          0)
	TAGKEYS(                        XK_2,                          1)
	TAGKEYS(                        XK_3,                          2)
	TAGKEYS(                        XK_4,                          3)
	TAGKEYS(                        XK_5,                          4)
	TAGKEYS(                        XK_6,                          5)
	TAGKEYS(                        XK_7,                          6)
	TAGKEYS(                        XK_8,                          7)
	TAGKEYS(                        XK_9,                          8)

	{ 0, XF86XK_AudioMute,                         spawn,          SHCMD("pulsemixer --toggle-mute") },
	{ 0, XF86XK_AudioRaiseVolume,                  spawn,          SHCMD("pulsemixer --change-volume +5") },
	{ 0, XF86XK_AudioLowerVolume,                  spawn,          SHCMD("pulsemixer --change-volume -5") },
	{ 0, XF86XK_AudioMicMute,                      spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_AudioStop,                         spawn,          SHCMD("mpc stop") },
	{ 0, XF86XK_AudioPlay,                         spawn,          SHCMD("mpc toggle") },
/* 	{ 0, XF86XK_AudioPlay,                         spawn,          SHCMD("mpc play") }, */
/*	{ 0, XF86XK_AudioPause,                        spawn,          SHCMD("mpc pause") }, */
	{ 0, XF86XK_AudioNext,                         spawn,          SHCMD("mpc next") },
	{ 0, XF86XK_AudioPrev,                         spawn,          SHCMD("mpc prev") },
	{ 0, XF86XK_AudioForward,                      spawn,          SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioRewind,                       spawn,          SHCMD("mpc seek -10") },

	{ 0, XF86XK_MonBrightnessUp,                   spawn,          SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,                 spawn,          SHCMD("xbacklight -dec 15") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};

