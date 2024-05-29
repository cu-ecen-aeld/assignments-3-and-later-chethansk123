#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<syslog.h>
#include<stddef.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char *argv[]){

openlog(NULL, 0, LOG_USER);
if(argc != 3)
{
 syslog(LOG_ERR, "Invalid number of arguments: %d", argc-1);
	closelog();
 return 1;
}

int fd;
ssize_t nr;
const char *file_path = argv[1];
fd = creat(file_path , 0644);

if(fd == -1)
{
 syslog(LOG_ERR, "Failed to create file");
	closelog();
  return 1;
}
else
{
 syslog(LOG_DEBUG, "File created  successfully");
 nr = write(fd, argv[2], strlen(argv[2]));
	 if(nr == -1)
	 {
	 
          syslog(LOG_ERR, "write is failed");
	closelog();
	  exit(1);
	 }
	 else{
	 
 	syslog(LOG_DEBUG, "writing %s to %s", argv[2], argv[1]);
	closelog();
	exit(0);
	 }
}

}
