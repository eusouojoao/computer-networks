#include "../hdr/UDP_utils.h"
#include "../hdr/error_handling.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFFER_SIZE 128

char *send_user_message_UDP(user_args *uip, char *msg) {
  int fd = -1;
  ssize_t n = 0;
  char buffer[BUFFER_SIZE] = "";
  char *received_msg = NULL;
  struct sockaddr_in addr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    system_error("In send_user_message_UDP() -> socket() failed: ");
    return NULL;
  }

  memset(&addr, 0, sizeof(addr));
  if (inet_pton(AF_INET, uip->regIP, &(addr.sin_addr)) != 1) {
    system_error("In send_user_message_UDP() -> inet_pton() failed: ");
    return NULL;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons((in_port_t)uip->regUDP);

  n = sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
  if (n == -1) {
    system_error("In send_user_message_UDP() -> sendto() failed: ");
    return NULL;
  }

  /* Inicializar a estrutura timeval para o timeout */
  struct timeval timeout;
  timeout.tv_sec = 30;
  timeout.tv_usec = 0;

  if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    system_error("In send_user_message_UDP() -> setsockopt() failed: ");
    return NULL;
  }

  socklen_t addrlen = sizeof(addr);
  n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr,
               &addrlen);
  if (n == -1) {
    system_error("In send_user_message_UDP() -> recvfrom() failed: ");
    return NULL;
  }

  received_msg = calloc((size_t)n + 1, sizeof(char));
  strncpy(received_msg, buffer, (size_t)n + 1);

  close(fd);
  return received_msg;
}
