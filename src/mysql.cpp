#include "mysql.h"
CDataBase::CDataBase()
{
    if(mysql_init(&conn)==NULL)
    {
        printf("[\"%s\",\"%d\"]init mysql conn is error::%u::%s\n",__FILE__,__LINE__,mysql_errno(&conn),mysql_error(&conn));
    }
}
CDataBase::~CDataBase()
{
    mysql_close(&conn);
    mysql_library_end();
}
int CDataBase::CONNECT(const char *CDhost,const char * CDuser,const char *CDpasswd,unsigned int CDport)
{
    //printf("CONNECT value::%s::%s::%s::%d\n",CDhost,CDuser,CDpasswd,CDport);
    if(NULL==mysql_real_connect(&conn,CDhost,CDuser,CDpasswd,NULL,CDport,NULL,0))
    {
        printf("mysql connect conn is error::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
        return 1;
    }
    return 0;
}
int CDataBase::CONNECT_DATABASE(const char * CDhost,const char * CDuser,const char *CDpasswd,const char *CDdb,unsigned int CDport)
{
    if(NULL==mysql_real_connect(&conn,CDhost,CDuser,CDpasswd,CDdb,CDport,NULL,0))
    {
        printf("[]mysql connect_database conn is error::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
        return 1;
    }
    return 0;
}
int CDataBase::INSERT(const char * url,double insert_data[])
{
    //printf("insert url::%s\n",url);
    //printf("INSERT %f\n",insert_data[0]);
    //read  mysql conf 
    const char * confName=CONF_NAME;
    char server_value[128];
    memset(server_value,'\0',sizeof(server_value));
    getConfValue(confName,"MYSQL","server",server_value);

    char port_value[128];
    memset(port_value,'\0',sizeof(port_value));
    getConfValue(confName,"MYSQL","port",port_value);
    int port_value_int=atoi(port_value);

    char user_value[128];
    memset(user_value,'\0',sizeof(user_value));
    getConfValue(confName,"MYSQL","user",user_value);

    char passwd_value[128];
    memset(passwd_value,'\0',sizeof(passwd_value));
    getConfValue(confName,"MYSQL","passwd",passwd_value);
    
    //first judge whether database  is  existen or not
    if(CONNECT_DATABASE(server_value,user_value,passwd_value,DATABASE_NAME,port_value_int)==1)
    {
        printf("connect  appoint  database error,meaning to database does't exist\n");
        char query_create_database[128];
        memset(query_create_database,'\0',sizeof(query_create_database));
        sprintf(query_create_database,"create database %s",DATABASE_NAME);
        //printf("%s\n",query_create_database);

        if(CONNECT(server_value,user_value,passwd_value,port_value_int)==1)
        {
            printf("connect init mysql is error\n");
            return 1;
        }
        if(mysql_query(&conn,query_create_database))
        {
            printf("create database mysql  error ::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
            return 1;
        }
        //order to create tables ,need connect appoint database; 
        MYSQL conn_init;   
        mysql_init(&conn_init);
        if(NULL==mysql_real_connect(&conn_init,server_value,user_value,passwd_value,DATABASE_NAME,port_value_int,NULL,0))
        {
            printf("seond connect appoint database is error\n");
            mysql_close(&conn_init);
            mysql_library_end();
            return 1;
        }
        char query_create_tables[256];
        memset(query_create_tables,'\0',sizeof(query_create_tables));
        sprintf(query_create_tables,"create table %s(id int unsigned not null auto_increment primary key,URL varchar(2048) not null,DNS double not null,CONNECT double not null,FIRST_PACK double not null,DATA_TRANS double not null,TOTAL  double not null,SPEED double not null)",TABLES_NAME);
        printf("%s\n",query_create_tables);

        if(mysql_query(&conn_init,query_create_tables))
        {
            printf("create tables  error ::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
            mysql_close(&conn_init);
            mysql_library_end();
            return 1;
        }
        char insert_values[2048+200];
        memset(insert_values,'\0',sizeof(insert_values));
        sprintf(insert_values,"insert into server_time values(NULL,\"%s\",%f,%f,%f,%f,%f,%.2f)",url,insert_data[0],insert_data[1],insert_data[2],insert_data[3],insert_data[4],insert_data[5]);
        //printf("init::%s\n",insert_values);

        if(mysql_query(&conn_init,insert_values))
        {
            printf("init ::insert values error ::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
            mysql_close(&conn_init);
            mysql_library_end();
            return 1;
        }

        mysql_close(&conn_init);
        mysql_library_end();
        return 0;
    }
    //insert  data 
    char insert_values[2048+200];
    memset(insert_values,'\0',sizeof(insert_values));
    sprintf(insert_values,"insert into server_time values(NULL,\"%s\",%f,%f,%f,%f,%f,%.2f)",url,insert_data[0],insert_data[1],insert_data[2],insert_data[3],insert_data[4],insert_data[5]);
    //printf("%s\n",insert_values);
    if(mysql_query(&conn,insert_values))
    {
        printf("insert values error ::%u::%s\n",mysql_errno(&conn),mysql_error(&conn));
    }
    return 0;
}
