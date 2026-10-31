#include "search.h"

int show_mean(void* arg, int col, char** val, char** title)
{
    *(int*)arg = 1;
    return 0;
}
int search_find(char *name, char *password)
{
    sqlite3 *pdb;
    int ret = sqlite3_open("component.db", &pdb);
    if (ret != SQLITE_OK)
    {
        fprintf(stderr, "sqlite open error:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    char sql[1024] = {0};
    char *errmsg = NULL;

    sprintf(sql, "select * from goods where name = '%s';", name);
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
        printf("goods %s not exist\n", name);
        return -2;
    }

    return 0;
}