#include"public.h"

#define port 80
//in order  to get  the path  of url
std::string  parse_url_get_path(std::string url)
{
    std::string path_string;
    std::string url_string=url;
    //size_t find_http=url_string.find("http://");
    //contain http://
    if(url_string.find("http://")!=std::string::npos)
    {
        size_t first_back_slash=url_string.find('/');
        std::string second_url_string=url_string.substr(first_back_slash+2,url_string.length()-first_back_slash);
        size_t second_url_string_first_back_slash=second_url_string.find('/');
        path_string=second_url_string.substr(second_url_string_first_back_slash,second_url_string.length()-second_url_string_first_back_slash);
        std::string host_string=second_url_string.substr(0,second_url_string_first_back_slash);
    }
    else
    {
        size_t first_back_slash=url_string.find('/');
        path_string=url_string.substr(first_back_slash,url_string.length()-first_back_slash);
        std::string host_string=url_string.substr(0,first_back_slash);
    }
    return  path_string;
}
//in order to get  the host of url
std::string  parse_url_get_host(std::string url)
{
    std::string host_string;
    std::string url_string=url;
    //size_t find_http=url_string.find("http://");
    //contain http://
    if(url_string.find("http://")!=std::string::npos)
    {
        size_t first_back_slash=url_string.find('/');
        std::string second_url_string=url_string.substr(first_back_slash+2,url_string.length()-first_back_slash);
        size_t second_url_string_first_back_slash=second_url_string.find('/');
        std::string path_string=second_url_string.substr(second_url_string_first_back_slash,second_url_string.length()-second_url_string_first_back_slash);
        host_string=second_url_string.substr(0,second_url_string_first_back_slash);
    }
    else
    {
        size_t first_back_slash=url_string.find('/');
        std::string path_string=url_string.substr(first_back_slash,url_string.length()-first_back_slash);
        host_string=url_string.substr(0,first_back_slash);
    }
    return  host_string;
}

int single_url(const char * query_string,std::string set_packet_time,int insert_sql_flag,std::string flag_name,std::string set_user_agent,std::string set_referer)
{
    const char *query=query_string;
    //printf("single url:;%s\n",query);

    //std::string host_string=parse_url_get_host(query);
    //std::string path_string=parse_url_get_path(query);
    //char * host=(char *)host_string.c_str();
    //char * path=(char *)path_string.c_str();
    struct evhttp_uri *ev_uri=NULL;
    ev_uri=evhttp_uri_parse(query);
    if(ev_uri==NULL)
    {
        printf("parse uri  error::%d,%s\n",errno,strerror(errno));
    }
    const char * host=evhttp_uri_get_host(ev_uri);
    const char * path=evhttp_uri_get_path(ev_uri);
    printf("query host::%s,path::%s\n",host,path);
    
    //the whole process time 
    struct timeval whole_process_time_begin;
    struct timeval whole_process_time_end;
    double  whole_process_time;
    //resolve domain name into ip
    double  resolve_domain_time;
    struct timeval resolve_domain_begin;
    struct timeval resolve_domain_end;
    struct hostent * h_name;
    gettimeofday(&whole_process_time_begin,NULL);
    gettimeofday(&resolve_domain_begin,NULL);
    h_name=gethostbyname(host);
    gettimeofday(&resolve_domain_end,NULL);
    resolve_domain_time=resolve_domain_end.tv_sec+resolve_domain_end.tv_usec/1000000.0-resolve_domain_begin.tv_sec-resolve_domain_begin.tv_usec/1000000.0; 
    //resolve fail
    if(!h_name)
    {
        fprintf(stderr,"could't resolve domain name %s:%s\n",host,hstrerror(h_errno));
        return 1;
    }
    //printf("resolve domain time::%f\n",resolve_domain_time);
    //
    int sock;
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        printf("socket error\n");
        return 1;
    }
    //connect server  
    double connect_server_time;
    struct sockaddr_in sin; 
    sin.sin_family=AF_INET;
    sin.sin_port  =htons(port);
    sin.sin_addr  =*(struct in_addr*)h_name->h_addr;

    struct timeval connect_server_begin;
    struct timeval connect_server_end;
    gettimeofday(&connect_server_begin,NULL); 
    if(connect(sock,(struct sockaddr *)&sin,sizeof(sin))<0)
    {
        printf("connect error::%d,%s\n",errno,strerror(errno));
        close(sock);
        return 1;
    }
    gettimeofday(&connect_server_end,NULL);
    connect_server_time=connect_server_end.tv_sec+connect_server_end.tv_usec/1000000.0-connect_server_begin.tv_sec-connect_server_begin.tv_usec/1000000.0;
    //printf("connect server time::%f\n",connect_server_time);

    struct timeval read_time_set;
    //trans file time  
    //std::cout<<"file time::"<<set_packet_time<<std::endl;
    if(set_packet_time.size()>1)
    {
        //printf("size*******\n");
        //struct timeval read_time_set;
        std::string set_packet_time_last_two=set_packet_time.substr(set_packet_time.find_last_of('s')-1,2);
        if((set_packet_time_last_two.compare("ms"))==0)
        {
            std::string set_packet_time_value=set_packet_time.substr(0,set_packet_time.length()-2); 
            //std::cout<<"ms--set packet time::"<<set_packet_time_value<<std::endl;
            //printf("ms to int::%d\n",atoi(set_packet_time_value.c_str())); 
            read_time_set.tv_sec=0;
            read_time_set.tv_usec=atoi(set_packet_time_value.c_str());
        }
        else
        {
            std::string set_packet_time_value=set_packet_time.substr(0,set_packet_time.length()-1); 
            //std::cout<<"s--set packet time::"<<set_packet_time_value<<std::endl;
            //printf("s to int::%d\n",atoi(set_packet_time_value.c_str())); 
            read_time_set.tv_sec=atoi(set_packet_time_value.c_str());
            read_time_set.tv_usec=0;
        }
        //setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&read_time_set,sizeof(struct timeval));
    }
    //set default trans file time "10s"
    else if(set_packet_time.size()==0)
    {
        //printf("no set_packet_time\n");
        read_time_set.tv_sec=10;
        read_time_set.tv_usec=0;

    }
    setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&read_time_set,sizeof(struct timeval));
    char * request=(char *)malloc(4*1024*sizeof(char));
    if((set_user_agent.size()>0)&&(set_referer.size()==0))
    {
        sprintf(request,"GET  %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nUser-Agent: %s\r\n\r\n",path,host,set_user_agent.c_str());
    }
    else if((set_referer.size()>0)&&(set_user_agent.size()==0))
    {
        sprintf(request,"GET  %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nReferer: %s\r\n\r\n",path,host,set_referer.c_str());
    }
    else if((set_referer.size()>0)&&(set_user_agent.size()>0))
    {
        sprintf(request,"GET  %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\nReferer: %s\r\nUser-Agent: %s\r\n\r\n",path,host,set_referer.c_str(),set_user_agent.c_str());
    }
    else
    {
        sprintf(request,"GET  %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path,host);
    }
    //sprintf(request,"GET  %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path,host);
    //send request
    int write_number=write(sock,request,strlen(request));
    if(write_number<0)
    {
        printf("request  data error::%d,%s\n",errno,strerror(errno));
        close(sock); 
        free(request);
        return 1;
    }
    //recv first pack 
    struct timeval recv_first_begin;
    struct timeval recv_first_end;
    double recv_first_time;
    char * response_first=(char *)malloc(1*sizeof(char));
    gettimeofday(&recv_first_begin,NULL);
    int recv_first_number=recv(sock,response_first,1,0);
    if(recv_first_number<0)
    {
        printf("response_first  data error::%d,%s\n",errno,strerror(errno));
        free(response_first);
        close(sock);
        return 1;
    }
    gettimeofday(&recv_first_end,NULL);
    //recv large  file time 
    struct timeval recv_large_file_begin;
    struct timeval recv_large_file_end;
    double recv_large_file_time;
    double large_file_size=0;
    char * response_large_file=(char *)malloc(1024*sizeof(char));
    gettimeofday(&recv_large_file_begin,NULL);
    while(1)
    {
        int recv_large_number=recv(sock,response_large_file,1024,0);
        if(recv_large_number==0)
            break;
        else if(recv_large_number<0)
        {
            printf("response_large  data error::%d,%s\n",errno,strerror(errno));
            free(response_large_file);
            close(sock);
            return 1;
        }
        large_file_size=large_file_size+recv_large_number;
    }
    gettimeofday(&recv_large_file_end,NULL);
    gettimeofday(&whole_process_time_end,NULL);
    recv_large_file_time=recv_large_file_end.tv_sec+recv_large_file_end.tv_usec/1000000.0-recv_large_file_begin.tv_sec-recv_large_file_begin.tv_usec/1000000.0;
    recv_first_time=recv_first_end.tv_sec+recv_first_end.tv_usec/1000000.0-recv_first_begin.tv_sec-recv_first_begin.tv_usec/1000000.0;
    //printf("recv time::%f\n",recv_first_time);
    whole_process_time=whole_process_time_end.tv_sec+whole_process_time_end.tv_usec/1000000.0-whole_process_time_begin.tv_sec-whole_process_time_begin.tv_usec/1000000.0;
    if(flag_name.size()>0)
    {
        printf("%s::%f\t%s::%f\t%s::%f\t%s::%f\t%s::%f\t%s::%.2fKB\n",flag_name.c_str(),resolve_domain_time,flag_name.c_str(),connect_server_time,flag_name.c_str(),recv_first_time,flag_name.c_str(),recv_large_file_time,flag_name.c_str(),whole_process_time,flag_name.c_str(),large_file_size/1024/recv_large_file_time);
    }
    else
    {
        printf("DNS::%f\tCONNECT::%f\tFIRST_PACK::%f\tDATA_TRANS::%f\tTOTAL::%f\tSPEED::%.2fKB\n",resolve_domain_time,connect_server_time,recv_first_time,recv_large_file_time,whole_process_time,large_file_size/1024/recv_large_file_time);
    }
    //printf("DNS::%f\tCONNECT::%f\tFIRST_PACK::%f\tDATA_TRANS::%f\tTOTAL::%f\tSPEED::%.2fKB\n",resolve_domain_time,connect_server_time,recv_first_time,recv_large_file_time,whole_process_time,large_file_size/1024/recv_large_file_time);
    //stand for  data inserting mysql
    double insert_data[6];
    insert_data[0]=resolve_domain_time;
    insert_data[1]=connect_server_time;
    insert_data[2]=recv_first_time;
    insert_data[3]=recv_large_file_time;
    insert_data[4]=whole_process_time;
    insert_data[5]=large_file_size/1024/recv_large_file_time;
    
    if(insert_sql_flag==1)
    {
        CDataBase mysql_instance;
        mysql_instance.INSERT(query,insert_data);
    }
    free(request);
    free(response_first);
    free(response_large_file);
    evhttp_uri_free(ev_uri);
    close(sock);
    return 0;
}

int main(int argc,char **argv)
{
    if(argc < 2) {
        fprintf(stderr, "%s,%d:error input,need parameters\n", __FILE__, __LINE__);
        return -1;
    }
    if(strcmp(argv[1],"-f")==0)
    {
        //stand for no insert
        int insert_sql_flag=0;
        //printf("@@@@@@@@@@@@@open file@@@@@@@@@@@@@@\n");
        //std::string insert_sql;
        if((argc==4)&&(strcmp(argv[3],"-q")==0))
        {
            //printf("@@@@@open file insert mysql\n");
            insert_sql_flag=1;
        }
        FILE *fp=fopen(argv[2],"rb");
        if(NULL==fp)
        {
            printf("open url_file is error %d::%s\n",errno,strerror(errno));
            return 0;
        }
        char url_buf[2048];
        memset(url_buf,'\0',sizeof(url_buf));
        //fgets(url_buf,sizeof(url_buf)+1,fp);
        while(!feof(fp))
        {
            if(!fgets(url_buf,sizeof(url_buf),fp))
                break;
            int r=strlen(url_buf);
            //in order to prevent blank 
            if(r&&url_buf[r-1]=='\n')
            {
                url_buf[r-1]='\0';
                //printf("url_buf::%s",url_buf);
                char query[2048];
                memset(query,'\0',sizeof(query));
                //no http
                if(strncmp("http", url_buf, 4))
                {
                    char * pc=strchr(url_buf,'/');
                    //"www.dnion.com"
                    if(pc==NULL)
                    {
                        sprintf(query,"http://%s/",url_buf);
                    }
                    //"www.dnion.com/"
                    else
                        sprintf(query, "http://%s", url_buf);
                }
                //has http
                else
                {
                    char tem_query[2048];
                    memset(tem_query,'\0',sizeof(tem_query));
                    strcpy(tem_query,url_buf+7);
                    char *pq =strchr(tem_query,'/');
                    //"http://www.dnion.com"
                    if(pq==NULL)
                    {
                        sprintf(query,"%s/",url_buf);
                    }
                    else
                    {
                        strcpy(query, url_buf);
                    }
                    //printf("tem_query:;%s\n",tem_query);
                }
        //printf("main query %s\n",query);

                single_url(query,"",insert_sql_flag,"","","");
            }
            //memset(url_buf,'\0',sizeof(url_buf));
            //fgets(url_buf,sizeof(url_buf)+1,fp);
        }
        fclose(fp);
    }
    else
    {
        //printf("################single url############\n");
        
        char query[2048];
        memset(query,'\0',sizeof(query));
        //no http
        if(strncmp("http", argv[1], 4))
        {
            char * pc=strchr(argv[1],'/');
            //"www.dnion.com"
            if(pc==NULL)
            {
                sprintf(query,"http://%s/",argv[1]);
            }
            //"www.dnion.com/"
            else
                sprintf(query, "http://%s", argv[1]);
        }
        //has http
        else
        {
            char tem_query[2048];
            memset(tem_query,'\0',sizeof(tem_query));
            strcpy(tem_query,argv[1]+7);
            char *pq =strchr(tem_query,'/');
            //"http://www.dnion.com"
            if(pq==NULL)
            {
                sprintf(query,"%s/",argv[1]);
            }
            else
            {
                strcpy(query, argv[1]);
            }
            //printf("tem_query:;%s\n",tem_query);
        }
        //printf("main query %s\n",query);

        int ch;
        int url_execute_numbers=1;
        std::string flag_name;
        std::string set_packet_time;
        int insert_sql_flag=0;
        std::string set_user_agent; 
        std::string set_referer;
        //std::string 
        while((ch=getopt(argc,argv,"c:n:t:qu:r:"))!=-1)
        {
            switch(ch)
            {
                case 'c':
                    url_execute_numbers=atoi(optarg);
                    //printf("c :: %d\n",url_execute_numbers);
                    break;
                case 'n':
                    flag_name=optarg;
                    break;
                    //std::cout<<"flag_name::"<<flag_name<<std::endl;
                case 't':
                    set_packet_time=optarg;
                    //std::cout<<"set_packet_time::"<<set_packet_time<<std::endl;
                    break;
                case 'q':
                    insert_sql_flag=1;
                    break;
                case 'u':
                    set_user_agent=optarg;
                    break;
                case 'r':
                    set_referer=optarg;
                    break;

            }
        }
        //printf("getopt later::set_packet_time%s,flag_name::%s,\n",set_packet_time.c_str(),flag_name.c_str());
        //printf("url_execute_numbers::%d,insert_sql_flag::%d\n",url_execute_numbers,insert_sql_flag);
        /*
        if(flag_name.size()>0)
        {
            printf("DNS::%s\tCONNECT::%s\tFIRST_PACK::%s\tDATA_TRANS::%s\tTOTAL::%s\tSPEED::%s\n",flag_name.c_str(),flag_name.c_str(),flag_name.c_str(),flag_name.c_str(),flag_name.c_str(),flag_name.c_str());
            return 0;
        }
        else
        {
        */
        for(int i=0;i<url_execute_numbers;i++)
        {
            if(flag_name.size()>0)
            {
                single_url(query,set_packet_time,insert_sql_flag,flag_name,set_user_agent,set_referer);
            }
            else
            {
                single_url(query,set_packet_time,insert_sql_flag,"",set_user_agent,set_referer);
            }
            //single_url(query,set_packet_time,insert_sql_flag,set_user_agent,set_referer);
        }
        
    }
    return 0;
}
