#include "query_module.h"
#include "../../common/utils.h"
#include "../../error_handling/error_checking.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Processes a QUERY message, forwards it if needed, and generates a response.
 *
 * @param[in] host: Pointer to the host structure.
 * @param[in] sender: Pointer to the sender node structure.
 * @param[in] buffer: Pointer to the buffer containing the QUERY message.
 */
void process_query(host *host, node *sender, char *buffer) {
  char dest[32] = {'\0'}, orig[32] = {'\0'}, name[100] = {'\0'};

  // Parse the command and store the destination and name.
  if (sscanf(buffer, "QUERY %s %s %s", dest, orig, name) < 3) {
    printf("Less than 3 arguments\n");
    return;
  }

  // Update forwarding table with the sender node
  insert_in_forwarding_table(host, atoi(orig), atoi(sender->ID));

  // Check if the name is valid
  if (check_name(name) == -1) {
    printf("Invalid name\n");
    return;
  }

  // Initialize the protocol message buffer
  char protocol_msg[256] = {'\0'};

  // If the destination is the current host, search for the name in the names list.
  if (strcmp(host->ID, dest) == 0) {
    if (find_name(name, host)) {
      snprintf(protocol_msg, 256, "CONTENT %s %s %s\n", dest, orig, name);
    } else {
      snprintf(protocol_msg, 256, "NOCONTENT %s %s %s\n", dest, orig, name);
    }
    // Send the CONTENT or NOCONTENT message to the sender
    if (write(sender->fd, protocol_msg, 256) == -1) {
      printf("Error sending CONTENT or NOCONTENT to sender\n");
    }
    return;
  }

  // Check if the destination node is a known neighbor
  node *neighbour = check_route(host, dest);
  if (neighbour != NULL) {
    // Forward the QUERY message to the known neighbor
    if (write(neighbour->fd, buffer, 256) == -1) {
      printf("Error forwarding QUERY to known neighbor\n");
    }
    return;
  }

  // If the destination node is not in the forwarding table, broadcast the QUERY
  // message to all neighbors except the sender
  send_protocol_messages(host, sender->fd, buffer);
}

void content(void) {
  return;
  return;
}

void no_content(void) {
  return;
  return;
}
