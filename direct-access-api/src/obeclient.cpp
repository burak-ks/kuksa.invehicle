#include "obeclient.hpp"
#include "common.hpp"

int sockfd;

e_result obe_client_init() {

  pthread_t obe_thread;

  const char *server_ip = "127.0.0.1";  //"192.168.3.40"; //
  int port = 6789;

  struct sockaddr_in serv_addr;

  // create socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Failed to create socket\n");
    return FAILURE;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));
  inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // connect
  int conn = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (conn < 0) {
    printf("Failed to connect to server\n");
    return FAILURE;
  }
  printf("Connected to server\n");

  if (pthread_create(&obe_thread, NULL, obe_listen, (void *)1) != 0) {
    perror("vcan_read thread creation failure !");
    return FAILURE;
  }



  return SUCCESS;
}

void *obe_listen(void *notused) {
  char buffer[BUFFER_SIZE] = {};
  while (1) {
    memset(&buffer, '\0', sizeof(buffer));

    // read
    int r = read(sockfd, buffer, BUFFER_SIZE);
    if (r < 0) {
      printf("Failed to read from daa_obe\n");
      continue;
    } else if (r == 0) {
      printf("daa_obe disconnected\n");
      continue;
    } else {
      printf("daa_obe: %s\n", buffer);

      // handle msg
      parse_buffer(buffer);

      memset(&buffer, '\0', sizeof(buffer));
    }
  }
}

char *prepare_outgoing_msg(char *daa_msg, struct can_frame *frame) {
  //"{can_id|can_dlc|data[0]|data[1]}"  - can frame data
  char hex_str[5];

  strcat(daa_msg, "{");
  sprintf(hex_str, "%x", frame->can_id);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->can_dlc);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[0]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[1]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[2]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[3]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[4]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[5]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[6]);
  strcat(daa_msg, hex_str);
  strcat(daa_msg, "|");
  sprintf(hex_str, "%x", frame->data[7]);
  strcat(daa_msg, hex_str);

  strcat(daa_msg, "}");

  printf("%s\n", daa_msg);

  return daa_msg;
}

void handle_incoming_msg(char *daa_msg) {
  const char *delimiter = "|";
  struct can_frame frame;

  char *tempstr = strdup(daa_msg);

  frame.can_id = strtol(strsep(&tempstr, delimiter), NULL, 16);
  frame.can_dlc = strtol(strsep(&tempstr, delimiter), NULL, 16);
  frame.data[0] = strtol(strsep(&tempstr, delimiter), NULL, 16);
  frame.data[1] = strtol(strsep(&tempstr, delimiter), NULL, 16);

  printf("write to vcan:\n");

  printf("frame.can_id = %X\n", frame.can_id);
  printf("frame.can_dlc = %X\n", frame.can_dlc);
  printf("frame.data[0] = %X\n", frame.data[0]);
  printf("frame.data[1] = %X\n", frame.data[1]);

  // check which vcans have read permissions
  dbmanager db;
  string vcan_name;
  vector<int> vcan_list = db.select_vcanids_from_read_table(frame.can_id);
  for (int id : vcan_list) {
    vcan_name = db.get_vcan_name(id);

    // write to vcan;
    cout << "write to vcan" << endl;
    vcanlistener listener(vcan_name);
    listener.vcan_write_frame(&frame);
  }
}

void parse_buffer(char *msg) {
  int i = 0;
  size_t sindex = -1;
  size_t eindex = -1;
  char daa_msg[BUFFER_SIZE];

  // locate msg start-endpoints
  while (msg[i] != '\0') {
    if (msg[i] == '{') sindex = i + 1;

    if (msg[i] == '}') {
      eindex = i;
      break;
    }
    i++;
  }

  // check endpoints
  if (sindex > 0 && eindex > 2) {
    memcpy(daa_msg, &msg[sindex], eindex - sindex);
    daa_msg[eindex - 1] = '\0';

    // handle msg
    // printf("Handle: %s\n", daa_msg);
    handle_incoming_msg(daa_msg);

    daa_msg[0] = '\0';

    if (eindex + 1 != strlen(msg))
      parse_buffer(&msg[eindex + 1]);
    else {
      printf("...\n");
      return;
    }
  } else {
    printf("...\n");
    return;
  }
}
