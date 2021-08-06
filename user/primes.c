#include "kernel/types.h"
#include "user/user.h"


void processNum(int inChan, int lePrime);

int
main(void) {
   int n = 2;
   int outChan[2];

   close(0);
   close(1);
   if (pipe(outChan) < 0) {
      fprintf(2, "pipe failed.");
   };

   if (fork() == 0) {
      close(outChan[1]);
      processNum(outChan[0], n);
   }
   close(outChan[0]);
   while (++n <= 35) {
      write(outChan[1], &n, 1);
   }
   close(outChan[1]);
   wait(0);
   exit(0);
}

void
processNum(int inChan, int lePrime) {
   int buf[1];
   int outChan[2];
   uint chanCreated = 0;

   while (read(inChan, buf, 1) > 0) {
      if (buf[0] % lePrime == 0) {
         continue;
      }
      if (!chanCreated) {
         if (pipe(outChan) < 0) {
            fprintf(2, "pipe failed in process %d, lePrime %d.", getpid(), lePrime);
            exit(1);
         };
         if (fork() == 0) {
            close(inChan);
            close(outChan[1]);
            processNum(outChan[0], buf[0]);
         }
         close(outChan[0]);
         chanCreated = 1;
      } else {
         write(outChan[1], buf, 1);
      }
   }

   fprintf(2, "prime %d\n", lePrime);
   if (chanCreated) {
      close(outChan[1]);
   }
   wait(0); // waitpid?
   exit(0);
}