#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char* path, char* fileToFind)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if( strcmp(fmtname(path),fileToFind) == 0 )
        printf("%s\n", path);//, st.type, st.ino, (int) st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      //printf("fmtname(buf): %s",fmtname(buf));
      if( strcmp(fmtname(buf),fileToFind) == 0 )
          printf("%s\n", buf);//, st.type, st.ino, (int) st.size);
    }
    chdir(buf);
    find(fmtname(buf),fileToFind);
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  //int i;

  if(argc < 2){
    printf("Usage: find <dir> <file>");
    exit(-1);
  }
  printf("fileToFind: %s\n",argv[2]);
  find(argv[1],argv[2]);
  exit(0);
}
