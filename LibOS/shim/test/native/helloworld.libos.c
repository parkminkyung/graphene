/* -*- mode:c; c-file-style:"k&r"; c-basic-offset: 4; tab-width:4; indent-tabs-mode:nil; mode:auto-fill; fill-column:78; -*- */
/* vim: set ts=4 sw=4 et tw=78 fo=cqt wm=0: */

/* a simple helloworld test */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <shim_unistd.h>

int main(int argc, char ** argv)
{
    printf("Hello world 2 (%s)!\n", argv[0]);

    send_request();
    return 0;
}
