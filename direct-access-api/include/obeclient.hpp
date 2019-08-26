#ifndef OBECLIENT_HPP
#define OBECLIENT_HPP

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "common.hpp"
#include "dbmanager.hpp"
#include "vcanhandler.hpp"

#define BUFFER_SIZE 99999


e_result obe_client_init();
void handle_incoming_msg(char* framestr);
void handle_incoming_msgs(string daa_msgs);
char* prepare_outgoing_msg(char* can_msg, struct can_frame* frame);
void parse_buffer(string buffer);
void* obe_listen(void* notused);
void* parse_obe_queue(void* notused);

#endif  // OBECLIENT_HPP
