#ifndef COMMON_HPP
#define COMMON_HPP

typedef enum t_return_value { SUCCESS, FAILURE, UNKNOWN } e_result;

extern int sockfd;
extern const char *obe_server_ip;
extern int obe_port;
extern int connection_status;

#endif  // COMMON_HPP
