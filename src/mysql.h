#ifndef _MYSQL_H_
#define _MYSQL_H_

#include "public.h"
#define CONF_NAME     "system.conf"
#define DATABASE_NAME "test_server" 
#define TABLES_NAME   "server_time"
class CDataBase
{
    public:
        //construct function
        CDataBase();
        //destruct function
        ~CDataBase();
        //connect database
        int CONNECT(const char *CDhost,const char * CDuser,const char *CDpasswd,unsigned int CDport);
        //connect appoint database
        int CONNECT_DATABASE(const char *CDhost,const char * CDuser,const char *CDpasswd,const char *CDdb,unsigned int CDport);
        //insert values
        int  INSERT(const char * url ,double insert_data[]);
    protected:
        MYSQL  conn;
};
#endif
