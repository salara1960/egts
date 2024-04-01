#include "egtsFunc.h"


int main(int argc, char *argv[])
{
fd_set Fds;
uint16_t tcp_port = 9876;
struct sockaddr_in srv_conn, cli_conn;
struct timeval mytv;
pthread_t tid;
pthread_attr_t threadAttr;
struct sigaction Act, OldAct;
int connsocket = -1, client = -1, resa, Vixod = 0, on = 1, ret = 0;
char dtim_str[TIME_STR_LEN] = {0};
socklen_t srvlen = sizeof(struct sockaddr_in);
socklen_t clilen = srvlen;;
char stx[256] = {0};

//-------------------   for example:  ./egts 9876 info   ---------------------

    fd_log = open(the_log, O_WRONLY | O_APPEND | O_CREAT, 0664);//open log file
    if (fd_log < 0) {
        sprintf(stx, "[Ver.%s] %s Can't open %s file\n", vers, TimeNowPrn(dtim_str), the_log);
        ToSysLogMsg(LOG_INFO, stx);
        return 1;
    }

    if (argc > 1) {
        resa = atoi(argv[1]);
        if ((resa > 0) && (resa < 0xffff)) tcp_port = resa;
    }
    if (argc > 2) {
        if (!strcmp(argv[2], "info")) service_flag = 1;//send to client (after timeout 10 sec.) test message
    }

    //--------------------  set Signals route function ------------------

    memset((uint8_t *)&Act,    0, sizeof(struct sigaction));
    memset((uint8_t *)&OldAct, 0, sizeof(struct sigaction));
    Act.sa_handler = &GetSignal_;
    Act.sa_flags   = 0;
    sigaction(SIGPIPE, &Act, &OldAct);
    sigaction(SIGHUP,  &Act, &OldAct);
    sigaction(SIGSEGV, &Act, &OldAct);
    sigaction(SIGTERM, &Act, &OldAct);
    sigaction(SIGABRT, &Act, &OldAct);
    sigaction(SIGINT,  &Act, &OldAct);
    sigaction(SIGSYS,  &Act, &OldAct);
    sigaction(SIGKILL, &Act, &OldAct);
    sigaction(SIGTRAP, &Act, &OldAct);

    //--------------------------------------------------------------------

    sprintf(stx, "[Ver.%s] %s Start egts server\n", vers, ShowTime(time(NULL)));
    print_msg(1, stx);
    ToSysLogMsg(LOG_INFO, stx);

    //--------------------------------------------------------------------

    while (!QuitAll) {

        Vixod = 0;
        connsocket = socket(AF_INET, SOCK_STREAM, 6);

        if (connsocket < 0) {
            print_msg(1, "ERROR: open socket (%d)\n", connsocket);
            ret = 1;
            goto done;
        }

        if (setsockopt(connsocket, SOL_SOCKET, SO_REUSEADDR,(const char *) &on, sizeof(on))) {
            print_msg(1, "ERROR: Setsockopt - SO_REUSEADDR (%d)\n", connsocket);
            ret = 1;
            goto done;
        }

        memset(&srv_conn, 0, srvlen);
        srv_conn.sin_family = AF_INET;
        srv_conn.sin_addr.s_addr = htonl(INADDR_ANY);
        srv_conn.sin_port = htons(tcp_port);

        if (bind(connsocket, (struct sockaddr *) &srv_conn, srvlen) == -1) {
            print_msg(1, "ERROR: Bind (port %d)\n", tcp_port);
            goto out_of_job;
        }

        if (listen(connsocket, 3) == -1) {
            print_msg(1, "ERROR: Listen (port %d)\n", tcp_port);
            goto out_of_job;
        }
        fcntl(connsocket, F_SETFL, (fcntl(connsocket, F_GETFL)) | O_NONBLOCK);
        print_msg(1, "Listen client (port %d)...\n", tcp_port);


        pthread_attr_init(&threadAttr);
        pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);


        while (!Vixod) {
            if (QuitAll) break;
            resa = 0;
            FD_ZERO(&Fds);   FD_SET(connsocket,&Fds);
            mytv.tv_sec = 0;   mytv.tv_usec = 100000;
            resa = select(connsocket + 1, &Fds, NULL, NULL, &mytv);
            if (resa > 0) {
                client = accept(connsocket, (struct sockaddr *) &cli_conn, &clilen);
                if (client > 0) {
                    fcntl(client, F_SETFL, (fcntl(client, F_GETFL)) | O_NONBLOCK);
                    print_msg(1, "New client %s:%u online (socket %d)\n",
                                (char *)inet_ntoa(cli_conn.sin_addr),
                                htons(cli_conn.sin_port),
                                client);
                    //--------------------------------------------------------------------
                    if (pthread_create(&tid, &threadAttr, egts_nitka, &client)) {
                        print_msg(1, "Error start client thread for socket %d\n", client);
                        if (client) {
                            close(client);
                            client = -1;
                        }
                    }
                    //--------------------------------------------------------------------
                }//if (client>0)
            }//if (resa>0)
        }//while (!Vixod)

out_of_job:

        pthread_attr_destroy(&threadAttr);

        if (client > 0) {
            shutdown(client, SHUT_RDWR);
            close(client);
        }
        if (connsocket > 0) {
            shutdown(connsocket, SHUT_RDWR);
            close(connsocket);
        }

    }

done:

    resa = 10;
    while (!QuitCli) {
        usleep(500000);
        resa--; if (!resa) break;
    }

    sprintf(stx, "[Ver.%s] %s Stop egts server. (timeout count=%u)\n", vers, ShowTime(time(NULL)), tout);
    print_msg(1, stx);
    ToSysLogMsg(LOG_INFO, stx);

    if (fd_log) close(fd_log);

    return ret;
}

//-----------------------------------------------------------------------------------------------------

