#include "iMan.h"

void iMan(char* input)
{
    const int cmd_len = 4;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    if (arg_count == 0 || arg_count > 1)
    {
        printf("Invalid number of arguments for iMan.\n");
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    struct addrinfo info, *result;
    char *host = "man.he.net";

    char *port = "80";

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;
    info.ai_protocol = 0;

    if (getaddrinfo(host, port, &info, &result) != 0)
    {
        err("getaddrinfo() failed");
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    int socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socket_fd == -1)
    {
        err("socket() failed");
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    if (connect(socket_fd, result->ai_addr, result->ai_addrlen) == -1)
    {
        err("connect() failed");
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    char *request = SAFE_MALLOC(MAX_BUFFER_SIZE + 1);
    int request_status;

    sprintf(request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\n\r\n", args[0], host);

    if (send(socket_fd, request, strlen(request), 0) == -1)
    {
        err("send() failed");
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    char *buffer = SAFE_MALLOC(MAX_BUFFER_SIZE);
    size_t bytes;
    bool length_exceeded = false;
    if ((bytes = recv(socket_fd, buffer, MAX_BUFFER_SIZE, 0)) < 0)
    {
        err("recv() failed");
        free(buffer);
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }
    else if (bytes == 0){
        printf("No response from server\n");
        free(buffer);
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }
    else if (bytes >= MAX_BUFFER_SIZE - 1) {
        buffer[MAX_BUFFER_SIZE - 1] = '\0';
    }
    else {
        buffer[bytes] = '\0';
    }

    if (strstr(buffer, "No matches for") != NULL)
    {
        printf("Command not found.\n");
        free(buffer);
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    sscanf(buffer, "HTTP/1.0 %d", &request_status);
    if (request_status == 404)
    {
        printf("404 Error: Page does not exist.\n");
        free(buffer);
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    char* print_start = strstr(buffer, "NAME\n");
    char* print_end = strstr(buffer, "DESCRIPTION\n");

    if(print_start == NULL || print_end == NULL){
        printf("Data missing for command %s\n", args[0]);
        free(buffer);
        free(request);
        freeaddrinfo(result);
        close(socket_fd);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    while(*print_end != '\n' || isspace(*(print_end + 1))){
        print_end++;
        if ((*print_end == '\0')) {
            length_exceeded = true;
            break;
        }
    }

    char *print_buffer = SAFE_MALLOC(print_end - print_start + 1);

    strncpy(print_buffer, print_start, print_end - print_start);
    print_buffer[print_end - print_start] = '\0';
    printf("\n%s\n", print_buffer);
    if (length_exceeded) {
        printf("......\nOutput truncated due to length exceeding buffer size\n");
    }

    free(buffer);
    free(print_buffer);
    free(request);
    freeaddrinfo(result);
    close(socket_fd);
    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}