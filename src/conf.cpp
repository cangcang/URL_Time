#include "conf.h"
//option = value  //中间必须有 '='
//value 全当做字符串处理
int getConfValue(const char *confFile, const char * section, const char * option,char *value)
{
    FILE * fp=NULL;
    fp=fopen(confFile,"r");
    if(NULL==fp)
    {
        printf("read system.conf  error::%d,%s\n",errno,strerror(errno));
        return -1;
    }
    char d_section[1024];
    char d_option[1024];
    //read section
    while(1)
    {
        memset(d_section,'\0',sizeof(d_section));
        fgets(d_section,sizeof(d_section),fp);
        if(feof(fp))
        {
            printf("read end of file section\n");
            fclose(fp);
            return -1;
        }
        if('['==d_section[0])
        {
            if(!strncmp(d_section+1,section,strlen(section)) )
            {
                break;
            } 
        }
    }
    //read value
    while(1)
    {
        memset(d_option,'\0',sizeof(d_option));
        fgets(d_option,sizeof(d_option),fp);
        if(feof(fp))
        {
            printf("read end of file value\n");
            fclose(fp);
            return -1;
        }
        if(!strncmp(d_option,option,strlen(option)))
        {
            char  e_value[1024];
            memset(e_value,'\0',sizeof(e_value));
            char *sp=strchr(d_option,'=');
            int j=0;
            //according to 1 ,not 0 ,except for '='
            for(unsigned int i=1;i<strlen(sp);i++) 
            {
                //printf("%c,%d\n",sp[i],isspace(sp[i]));
                if(!isspace(sp[i]))
                {
                   e_value[j++]=sp[i];
                }
            }
            //printf("e_value::%s\n",e_value);
            strcpy(value,e_value);
            //printf("value ::%s::\n",value);
            break;
        }
    }
    fclose(fp);
    return 0;
}
