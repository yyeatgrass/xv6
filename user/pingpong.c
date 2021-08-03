#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
   int buf[1];
   int p[2];
   pipe(p);
   if (fork() == 0) {
      read(p[0], buf, 1);
      fprintf(2, "received ping\n");
      write(p[1], buf, 1);
      exit(0);
   }

   write(p[1], buf, 1);
   read(p[0], buf, 1);
   fprintf(2, "received pong\n");
   wait(0);
   exit(0);
}