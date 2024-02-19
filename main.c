#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
	if (argc > 1) {
		client(argv[1], argv[2]);
		return 0;
	}

	// client(argv[1]);

	return 0;
}
