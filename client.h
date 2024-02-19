#pragma once

int client(char *hostname, char *port);
int get_server_conn(char *server, char *port);
void *get_server_msgs(void *sfd);
