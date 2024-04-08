#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
/* 학번을 입력받아 해당 학생 레코드를 수정한다. */
int main(int argc, char *argv[])
{
   int fd, id, withdraw;
   char c;
   struct account record;
   struct flock lock;
   if (argc < 2) {
      fprintf(stderr, "사용법 : %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_RDWR)) == -1) {
      perror(argv[1]);
      exit(2);
   }
   do {
      printf("Account ID: ");
      if (scanf("%d", &id) == 1) {
	 lock.l_type = F_WRLCK;
	 lock.l_whence = SEEK_SET;
	 lock.l_start = (id-START_ID)*sizeof(record);
	 lock.l_len = sizeof(record);
	 if(fcntl(fd, F_SETLKW, &lock) == -1) {
		 perror(argv[1]);
		 exit(3);
	 }
         lseek(fd, (long) (id-START_ID)*sizeof(record), SEEK_SET);
         if ((read(fd, &record, sizeof(record)) > 0) && (record.id != 0)) {
            printf("ID:%8d\t Name:%4s\t Balance:%4d\n",
                    record.id, record.name, record.balance);
            printf("Amount of money to withdraw: ");
            scanf("%d", &withdraw);
	    if(withdraw > record.balance)
	    {
		    printf("should be smaller than %d\n",record.balance);
	    }
	    else
	    {
		    record.balance -= withdraw;
	    }
	    printf("Balance: %d\n",record.balance);
            lseek(fd, (long) -sizeof(record), SEEK_CUR);
            write(fd, &record, sizeof(record));
         } else printf("레코드 %d 없음\n", id);
      } else printf("입력오류\n");
      lock.l_type = F_UNLCK;
      fcntl(fd, F_SETLK, &lock);
      printf("계속하겠습니까?(Y/N)");
      scanf(" %c",&c);
   } while (c == 'Y');
   close(fd);
   exit(0);
}

