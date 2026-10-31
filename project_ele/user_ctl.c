#include "user_ctl.h"

int show_mean(void* arg, int col, char** val, char** title)
{
    *(int*)arg = 1;
    return 0;
}

int user_init()
{
    sqlite3 *pdb;

    int ret = sqlite3_open("user.db", &pdb);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "sqlite open error:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    char *psql = "drop table user;";
    sqlite3_exec(pdb, psql, NULL, NULL, NULL);
    
    psql = "create table user(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT);";
    ret = sqlite3_exec(pdb, psql, NULL, NULL, NULL);
	if (ret != SQLITE_OK)
	{	
		fprintf(stderr, "sqlite3_exec error : %s\n", sqlite3_errmsg(pdb));
		return -1;
	}

    sqlite3_close(pdb);

    return 0;
}
int user_register(char *name, char *password)
{
    sqlite3 *pdb;

    int ret = sqlite3_open("user.db", &pdb);
    if(ret != SQLITE_OK)
    {
        fprintf(stderr, "sqlite open error:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }
    char sql[1024] = {0};
    
    sprintf(sql, "insert into user values(NULL, \"%s\", \"%s\");", name, password);
    ret = sqlite3_exec(pdb, sql, NULL, NULL, NULL);
	if (ret != SQLITE_OK)
	{	
		fprintf(stderr, "sqlite3_exec error : %s\n", sqlite3_errmsg(pdb));
		return -1;
	} 

    sqlite3_close(pdb);

    return 0;
}
int user_login(char *name, char *password)
{
    sqlite3 *pdb;
    int ret = sqlite3_open("user.db", &pdb);
    if (ret != SQLITE_OK)
    {
        fprintf(stderr, "sqlite open error:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    char sql[1024] = {0};
    char *errmsg = NULL;

    sprintf(sql, "select * from user where name = '%s' and password = '%s';", name, password);
    int flag = 0;
    ret = sqlite3_exec(pdb, sql, show_mean, &flag, &errmsg);
    if (SQLITE_OK != ret)
    {
        fprintf(stderr, "create table  error %s\n", errmsg);
        sqlite3_close(pdb);
        return -1;
    }

    if (0 == flag)
    {
        printf("user %s not exist\n", name);
        return -2;
    }

    return 0;
}