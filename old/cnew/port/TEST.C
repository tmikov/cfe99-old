#include <dirent.h>
#include <stdio.h>

int main ( int argc, char ** argv )
{
  DIR * dir;
  struct dirent * de;
  char * arg;

  arg = argc > 1 ? argv[1] : ".";

  if (dir = opendir( arg ))
  {
    while (de = readdir( dir ))
    {
      puts( de->d_name );
    }
    closedir( dir );
  }
  return 0;
};

