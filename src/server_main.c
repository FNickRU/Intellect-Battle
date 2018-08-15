#include "server.h"

#define WNUM 1
#define RNUM 1

int main(int argc, char *argv[])
{
    init_server(WNUM, RNUM);

    loop_recv();

    server_finalize();

    return 0;
}
