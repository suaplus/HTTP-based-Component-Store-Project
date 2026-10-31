#ifndef __USER_CTL_H__
#define __USER_CTL_H__

#include <sqlite3.h>
#include <stdio.h>

extern int user_init();
extern int user_register(char *name, char *password);
extern int user_login(char *name, char *password);

#endif