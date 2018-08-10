#include "server.h"

#define WNUM 5
#define RNUM 3

int main(int argc, char *argv[])
{
    init_server(WNUM, RNUM);

    loop_recv();

    server_finalize();

    return 0;
}
