#ifndef  DRAGON_H_INC
#define  DRAGON_H_INC

#ifdef __cplusplus
extern "C"
{
#endif

#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>

#ifdef __cplusplus
} // extern "C" {
#endif

#ifdef WIN32
#include <winsock2.h> 
#include <ctype.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/time.h>
#endif

#if defined(__BSD__)||defined(__FreeBSD__)||defined(__OpenBSD__)||defined(__NetBSD__) 
#include <netinet/in.h> 
#endif


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <math.h>

using namespace std;

#define G_THREAD_FUNC(f) ((GThreadFunc)(f))

#define MAX_STRING		1024
#define MAX_FILE_NAME 	256
#define VERSION			"V0.2"

#endif   /* ----- #ifndef DRAGON_H_INC  ----- */

