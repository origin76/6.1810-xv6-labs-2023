#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int my_strncmp(const char *s1, const char *s2, int n)
{
  for (int i = 0; i < n; i++)
  {

    if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
    {
      return (unsigned char)s1[i] - (unsigned char)s2[i];
    }
  }
  return 0;
}

void find_name(char *path, char *name, int mode)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    if (mode == 1)
    {
      uint l1 = strlen(name);
      uint l2 = strlen(path);
      if (l2 >= l1)
      {
        if (my_strncmp(path + l2 - l1, name, l1) == 0)
        {
          printf("%s\n", path);
        }
      }
    } else {
      printf("%s\n", path);
    }
    break;
  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;
      if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      find_name(buf, name, mode);
      *p = 0;
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    find_name(".", "", 0);
  }
  if (argc == 2) // only path no name
  {
    find_name(argv[1], "", 0);
  }
  if (argc == 3) // path and name
  {
    find_name(argv[1], argv[2], 1);
  }
  exit(0);
}
