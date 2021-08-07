#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find (char *dir, char *target);

int
main(int argc, char *argv[])
{
   fprintf(2, "0000\n");
   find(argv[1], argv[2]);
   exit(0);
}

void
find (char *dir, char *target) {
   struct stat st;
   struct dirent de;
   char buf[512];
   char *p;
   int fd;

   fprintf(2, "11111\n");
   if ((fd = open(dir, 0)) < 0) {
      fprintf(2, "find: cannot open %s\n", dir);
      return;
   }

   fprintf(2, "2222\n");
   if (fstat(fd, &st) < 0) {
      fprintf(2, "find: cannot fstat %s\n", dir);
      close(fd);
      return;
   }

   fprintf(2, "3333\n");
   if (st.type == T_FILE) {
      fprintf(2, "find: could not find under a file\n");
      return;
   }

   strcpy(buf, dir);
   p = buf + strlen(buf);
   *p++ = '/';
   while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      // why
      fprintf(2, "file name %s\n", de.name);
      if (de.inum == 0) {
         continue;
      }
      // get full path.
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if (strcmp(target, de.name) == 0) {
         fprintf(2, "%s\n", buf);
      }

      if (stat(buf, &st) < 0) {
         fprintf(2, "find: cannot stat %s\n", dir);
         return;
      }
      if (st.type == T_DIR) {
         find(buf, target);
      }
   }
   close(fd);
}