#include "kernel/types.h"
#include "user/user.h"


void processNum(int inChan, int lePrime);

int
main(void) {
   int n = 2;
   int outChan[2];
   if (pipe(outChan) < 0) {
      fprintf(2, "pipe failed.");
   };
//   fprintf(2, "outChan %d, %d\n", outChan[0], outChan[1]);
   if (fork() == 0) {
      processNum(outChan[0], n);
   }
//   fprintf(2, "cccc\n");
   while (++n <= 35) {
//      fprintf(2, "bbbb\n");
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
//   fprintf(2, "aaaaa %d\n", lePrime);
   while (read(inChan, buf, 1) > 0) {
//      fprintf(2, "read num %d in process %d\n", buf[0], getpid());
      if (buf[0] % lePrime == 0) {
         continue;
      }
      if (!chanCreated) {
         if (pipe(outChan) < 0) {
            fprintf(2, "pipe failed.");
         };
//         fprintf(2, "outChan %d, %d\n", outChan[0], outChan[1]);
         if (fork() == 0) {
            processNum(outChan[0], buf[0]);
         }
         chanCreated = 1;
      } else {
         write(outChan[1], buf, 1);
      }
   }

//   fprintf(2, "prime %d", lePrime);
   if (chanCreated) {
      close(outChan[0]);
   }
   wait(0); // waitpid?
   exit(0);
}