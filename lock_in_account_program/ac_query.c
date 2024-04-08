#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
/* 학번을 입력받아 해당 학생의 레코드를 파일에서 읽어 출력한다. */
int main(int argc, char *argv[])
{
   int fd, id;
   struct account record;
   char c;
   struct flock lock;
   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_RDONLY)) == -1) {
      perror(argv[1]);
      exit(2);
   }
   do {
      printf("\nSearch for account ID:");
      if (scanf("%d", &id) == 1) {
	 lock.l_type = F_RDLCK;
	 lock.l_whence = SEEK_SET;
	 lock.l_start = (id-START_ID)*sizeof(record);
	 lock.l_len = sizeof(record);
	 if (fcntl(fd, F_SETLKW, &lock) == -1) {
		 perror(argv[1]);
		 exit(3);
	 }
	 lseek(fd, (id-START_ID)*sizeof(record), SEEK_SET);
         if ((read(fd, &record, sizeof(record)) > 0) && (record.id != 0))
            printf("ID:%d\t Name:%s\t Balance:%d\n", record.id, record.name, 
                    record.balance);
         else printf("레코드 %d 없음\n", id);
	 
	 lock.l_type = F_UNLCK;
	 fcntl(fd, F_SETLK, &lock);
      } else printf("입력 오류");
      printf("계속하겠습니까?(Y/N)");
      scanf(" %c", &c);
   } while (c=='Y');
   close(fd);
   exit(0);
}

