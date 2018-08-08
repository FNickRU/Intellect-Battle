#include "server.h"

#define WNUM 5
#define RNUM 10

int main(int argc, char *argv[])
{    
    struct server_conf *cfg;
    cfg = init_server(argv[0],WNUM,RNUM);
    loop_recv(cfg->socket,cfg->msgid);
    finalize(cfg);
    return 0;
}
