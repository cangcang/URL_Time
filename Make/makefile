EXNAME=test_server_time
vpath %.cpp ../src
objs=main.o conf.o mysql.o 
CL=g++
OPTION=-Wall -g
CCFLAGS= -I../libevent/include/
#LINK_FLAGS=-I../include/ -L/usr/lib   -L/usr/lib/mysql -lmysqlclient  ../libs/libevent.a  -lrt  
LINK_FLAGS=-L/usr/lib   -L/usr/lib64/mysql  -lmysqlclient  ../libevent/lib/libevent.a  ../libevent/lib/libevent_core.a  ../libevent/lib/libevent_extra.a  ../libevent/lib/libevent_openssl.a  ../libevent/lib/libevent_pthreads.a  -lrt  -lz   -pthread  
$(EXNAME):$(objs)
	    $(CL)  $(OPTION)  -o ../$(EXNAME) $(objs) $(LINK_FLAGS)
%.o:%.cpp
	 $(CL) $(OPTION) $(CCFLAGS) -c $<  -o $@
.PHONY:clean
clean:
	-rm ../$(EXNAME) $(objs)
