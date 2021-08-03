#include "kernel/types.h"
#include "user/user.h"

int
main(void) {
   int buf[1];
   int p2c[2];
   int c2p[2];
   pipe(p2c);
   pipe(c2p);

   if (fork() == 0) {
      read(p2c[0], buf, 1);
      fprintf(2, "%d: received ping\n", getpid());
      write(c2p[1], buf, 1);
      exit(0);
   }

   write(p2c[1], buf, 1);
   read(c2p[0], buf, 1);
   fprintf(2, "%d: received pong\n", getpid());
   wait(0);
   exit(0);
}