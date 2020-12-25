/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel 32 */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack:style=Bold:antialias=true:autohint=true:size=10", "noto color emoji:pixelsize=12:antialias=true:autohint=true"  };
static const unsigned int baralpha  = 0x77; //default 0xd0
static const unsigned int borderalpha = 0x00; //default OPAQUE
static char dmenufont[]             = "Hack:size=10";
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#3b3b3b";
static char normfgcolor[]           = "#00ff99"; //#00ff99
static char selfgcolor[]            = "#00ff99"; //#00ff99
static char selbordercolor[]        = "#00ff99"; //#00ff99
static char selbgcolor[]            = "#333333";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};


typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-g", "65x20", "-e", "python", "-q", "-i", "/home/jimkat/.config/scratchpy/scratch.py", NULL };
const char *spcmd3[] = {TERMINAL, "-n", "spsurf", "-g", "120x34", "-e", "surf", "duckduckgo.com", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",	spcmd1},
	{"spcalc",	spcmd2},
	{"spsurf",	spcmd3},
};

static const char *const autostart[] = {
	"/usr/lib/polkit-kde-authentication-agent-1", 	NULL,
	"dwmblocks", 					NULL,
	"dunst", 					NULL,
	"xcompmgr",			 		NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,       "spterm",   NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,       "spcalc",   NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
	{ NULL,       "spsurf",   NULL,             SPTAG(2),     1,           1,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "H[]",	deck },			/* Master on left, slaves in monocle-like mode on right */
 	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */

	{ "HHH",     	grid },

	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_recent", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function  		argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,			XK_grave,	spawn,		{.v = termcmd} },
	{ MODKEY|ShiftMask,		XK_grave,	togglescratch,	{.ui = 0} },
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_minus,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("") },
	{ MODKEY,			XK_BackSpace,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_BackSpace,	spawn,		SHCMD("") },

	{ MODKEY,			XK_Tab,		view,		{0} },
	/* { MODKEY|ShiftMask,		XK_Tab,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY|ShiftMask,		XK_q,		spawn,		SHCMD("sysact") },
	{ MODKEY,			XK_w,		spawn,		SHCMD("firefox") },
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD(TERMINAL " -e sudo nmtui") },
	{ MODKEY,			XK_e,		spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_e,		spawn,		SHCMD("") },
	{ MODKEY,			XK_r,		spawn,		SHCMD(TERMINAL " -e lf") },
	{ MODKEY|ShiftMask,		XK_r,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			XK_i,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			XK_o,		setlayout,      {.v = &layouts[9]} },
	{ MODKEY|ShiftMask,		XK_o,		setlayout,     	{.v = &layouts[8]} },
//	{ MODKEY,			XK_p,		incnmaster,	{.i = +1 } },
//	{ MODKEY|ShiftMask,		XK_p,		incnmaster,	{.i = -1 } },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("") },
	{ MODKEY,			XK_backslash,		view,		{0} },
	/* { MODKEY|ShiftMask,		XK_backslash,		spawn,		SHCMD("") }, */

	{ MODKEY,			XK_a,		togglegaps,	{0} },
	{ MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },
	{ MODKEY,			XK_s,		togglesticky,	{0} },
	/* { MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_d,		spawn,          {.v = dmenucmd } },
	/* { MODKEY,			XK_d,		spawn,		SHCMD("") } }, */
	{ MODKEY,			XK_f,		togglefullscr,	{0} },
	{ MODKEY|ShiftMask,		XK_f,		togglebackfull,	{0} },
//	{ MODKEY,			XK_g,		shiftview,	{ .i = -1 } },
//	{ MODKEY|ShiftMask,		XK_g,		shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_h,		setmfact,	{.f = -0.005} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			XK_l,		setmfact,      	{.f = +0.005} },
//	{ MODKEY,			XK_semicolon,	shiftview,	{ .i = 1 } },
//	{ MODKEY|ShiftMask,		XK_semicolon,	shifttag,	{ .i = 1 } },
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 1} },
	{ MODKEY|ShiftMask,		XK_apostrophe,	togglescratch,	{.ui = 2} },
	// { MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	// { MODKEY|ShiftMask,		XK_Return,	togglescratch,	{.ui = 0} },

	{ MODKEY,			XK_z,		incrgaps,	{.i = +3 } },
	/* { MODKEY|ShiftMask,		XK_z,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_x,		incrgaps,	{.i = -3 } },
	/* { MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_c,		movecenter,	{0} },
	/* { MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("") }, */
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			XK_b,		togglebar,	{0} },
	{ MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("st -e bluetoothctl") },
	{ MODKEY,			XK_n,		spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD("") },
	{ MODKEY,			XK_m,		incnmaster,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_m,		incnmaster,	{.i = -1 } },
	{ MODKEY,			XK_comma,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_comma,	spawn,		SHCMD("") },
	{ MODKEY,			XK_period,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_period,	spawn,		SHCMD("") },

	{ MODKEY|ControlMask,          	XK_Down, 	moveresize,     {.v = "0x 15y 0w 0h" } },
	{ MODKEY|ControlMask,          	XK_Up,	 	moveresize,     {.v = "0x -15y 0w 0h" } },
	{ MODKEY|ControlMask,          	XK_Right,	moveresize,     {.v = "15x 0y 0w 0h" } },
	{ MODKEY|ControlMask,          	XK_Left,	moveresize,     {.v = "-15x 0y 0w 0h" } },
	{ MODKEY|ControlMask|ShiftMask,	XK_Down,	moveresize,     {.v = "0x 0y 0w 15h" } },
	{ MODKEY|ControlMask|ShiftMask,	XK_Up,		moveresize,     {.v = "0x 0y 0w -15h" } },
	{ MODKEY|ControlMask|ShiftMask,	XK_Right,	moveresize,     {.v = "0x 0y 15w 0h" } },
	{ MODKEY|ControlMask|ShiftMask,	XK_Left,	moveresize,     {.v = "0x 0y -15w 0h" } },

	{ MODKEY,			XK_Left,	shiftview,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,	shifttag,	{.i = -1 } },
	{ MODKEY,			XK_Right,	shiftview,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,	shifttag,	{.i = +1 } },
	{ MODKEY,			XK_Down,	focusstack,	{.i = INC(+1)} },
	{ MODKEY|ShiftMask,		XK_Down,	pushstack,	{.i = INC(+1)} },
	{ MODKEY,			XK_Up,		focusstack,	{.i = INC(-1)} },
	{ MODKEY|ShiftMask,		XK_Up,		pushstack,	{.i = INC(-1)} },

	{ MODKEY,			XK_Page_Up,	focusmon,	{ .i = -1 } }, //change monitor
	{ MODKEY|ShiftMask,		XK_Page_Up,	shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,		XK_Page_Down,	shifttag,	{ .i = +1 } },
	{ MODKEY,			XK_Insert,	spawn,		SHCMD("xdotool type $(cat ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },

	{ MODKEY,			XK_F1,		spawn,		SHCMD("scriptor -cc") }, // groff -mom /home/jimkat/src/dwm/larbs.mom -Tpdf | zathura -
	{ MODKEY,			XK_F2,		spawn,		SHCMD("cliper -g") },
	{ MODKEY|ShiftMask,		XK_F2,		spawn,		SHCMD("cliper -n") },
	{ MODKEY,			XK_F3,		spawn,		SHCMD("memo") },
	{ MODKEY,			XK_F4,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F5,		xrdb,		{.v = NULL } },
	{ MODKEY,			XK_F6,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F7,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F8,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F9,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F10,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F11,		spawn,		SHCMD("") },
	{ MODKEY,			XK_F12,		xrdb,		{.v = NULL } },
	{ MODKEY,			XK_space,	zoom,		{0} },
	{ MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} },

	{ 0,				XK_Print,	spawn,		SHCMD("maim ~/Pictures/screenshots/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			XK_Print,	spawn,		SHCMD("maim -i root | xclip -selection clipboard -t image/png") },
	{ ControlMask,			XK_Print,	spawn,		SHCMD("import jpg:- |  xclip -selection clipboard -t image/png") },
	{ Mod1Mask,			XK_Print,	spawn,		SHCMD("maim -i $(xdotool getactivewindow) | xclip -selection clipboard -t image/png")},
	{ MODKEY,			XK_Print,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("") },
	{ MODKEY,			XK_Delete,	spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_Delete,	spawn,		SHCMD("xkill") },
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("") },

	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t && pkill -RTMIN+2 dwmblocks") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 5 && pkill -RTMIN+2 dwmblocks") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 5 && pkill -RTMIN+2 dwmblocks") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc toggle") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	{ 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) },
	{ 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL/* " -e lf /"*/) },
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("n=/sys/class/backlight/intel_backlight/brightness; echo $((`cat $n` + 300)) > $n") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("n=/sys/class/backlight/intel_backlight/brightness; echo $((`cat $n` - 300)) > $n") },

	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/src/dwmblocks/config.h") },
	{ ClkStatusText,        ControlMask,    Button1,        sigdwmblocks,   {.i = 7} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button1,	spawn,		SHCMD("/home/jimkat/src/dwmblocks/sigall") },
	{ ClkRootWin,		0,		Button2,	spawn,		SHCMD(TERMINAL) },
};
