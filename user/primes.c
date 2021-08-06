#include "kernel/types.h"
#include "user/user.h"


int
main(void) {
   int n = 0;
   while (n <= 35) {
      write(, n, );
      if () {

      }
   }
}

void
processNum(int inChan, int lePrime) {
   int buf[1];
   int outChan[2];
   uint chanCreated = 0;
   while (read(inChan, buf,  ) > 0) {
      if (buf[0] == 0) {
         fprintf(2, "prime %d", lePrime);
         wait(0); // waitpid?
         exit(0);
      }
      if (buf[0] % lePrime == 0) {
         continue;
      }

      if (!chanCreated) {
         pipe(outChan);
         if (fork() == 0) {
            processNum(outChan[0], buf[0]);
         }
         chanCreated = 1;
      } else {
         write(outChan[1], buf[0], 1);
      }
   }
}



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