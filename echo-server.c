#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


static inline void
die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    exit(EXIT_FAILURE);
}

static inline void
die_errno(int err)
{
    fprintf(stderr, "error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
}

/** @brief Main program entry point.
    @param[in] argc  Number of arguments in @c argv.
    @param[in] argv  Command-line arguments.
    @retval 0
        Success.
*/
int
main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    int server_fd = socket(PF_INET, SOCK_STREAM /* TCP */, 0);
    if (server_fd < 0)
        /* Catch error number for more detailed info */
        die_errno(errno);

    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(server_fd, (struct sockaddr *) &server, sizeof(server)))
        die_errno(errno);

    if (listen(server_fd, 0))
        die_errno(server_fd);

    for (;;) /* loop forever */
    {
        struct sockaddr_in client;
        int client_fd;
        socklen_t sl = sizeof(client); // sizeof(struct sockaddr_in)

        client_fd = accept(server_fd, (struct sockaddr *) &client, &sl);
        if (client_fd < 0)
            die_errno(errno);

        ssize_t size, sent_size;

        char *client_ip = inet_ntoa(client.sin_addr); //network to addr

        printf("Connection received from: %s\n", client_ip);

        for (;;)
        {
            char buffer[128];
            size = recv(client_fd, buffer, sizeof(buffer), 0);

            if (size < 0)
                die_errno(errno);

            // Socket has been closed
            if (size == 0)
                break;

            printf("Recv'd %ld bytes:\n<<<<<<\n", size);
            write(1, buffer, size);
            printf(">>>>>>\n");

            char *p = buffer;

            while (size)
            {
                sent_size = send(client_fd, p, size, 0);
                if (sent_size == -1)
                    die_errno(errno);

                p += sent_size;
                size -= sent_size;
            }
        }
    }

    return 0;
}

