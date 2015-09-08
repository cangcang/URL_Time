#ifndef  _PUBLIC_H_
#define  _PUBLIC_H_

    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <sys/time.h>
    #include <errno.h>
    #include <ctype.h>

    #include "event2/event.h"
    #include "event2/buffer.h"
    #include "event2/bufferevent.h"
    #include "event2/http.h"
    #include "event2/http_struct.h"
    #include "event2/http_compat.h"
    #include "event2/event_compat.h"
    //#include <evhttp.h>
    //#include <event2/buffer.h>
    //#include <event2/event_compat.h>
    //#include <event2/event.h>
    //#include <event2/bufferevent.h>

    #include <mysql/mysql.h>    

    #include <string>
    #include <iostream>

    #include "conf.h"
    #include "mysql.h"

#endif
