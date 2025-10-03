#ifndef NTFILE_H
#define NTFILE_H

#include "ntcore.h"

unsigned char savenet( net_t * net , const char *name );
struct ntnet loadnet( char *name );

#endif // NTFILE_H