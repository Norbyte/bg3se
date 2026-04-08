#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WIN10


#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSCROLL
#define NOSHOWWINDOW
#define NOWH
#define NODESKTOP
#define NOWINDOWSTATION
#define NOSECURITY
#define NOWINOFFSETS
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOCLIPBOARD
#define NODEFERWINDOWPOS
#define NOCTLMGR
#define NOSYSMETRICS
#define NOMENUS
#define NODRAWTEXT
#define NOCOLOR
#define NOSYSCOMMANDS
#define NOICONS
#define NOMDI
#define NOWINABLE
#define NOGDI
#define NOHELP
#define NOSYSPARAMSINFO
#define NOWINABLE
#define NONLS

#include <windows.h>
//#include <detours.h>
