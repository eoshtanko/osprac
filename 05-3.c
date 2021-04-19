#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void try_semop(int sem_id, int sem_op) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_op  = sem_op;
  mybuf.sem_flg = 0;

  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

void first(int sem_id, int value) {
  try_semop(sem_id, value);
}

void second(int sem_id, int value) {
  try_semop(sem_id, -value);
}

void third(int sem_id) {
  try_semop(sem_id, 0);
}

int getSem() {
  int    sem_id;
  char   pathname[]="05-3.c";
  key_t  key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((sem_id = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create semaphore set\n");
      exit(-1);
    } else if ((sem_id = semget(key, 1, 0)) < 0) {
      printf("Can\'t find semaphore\n");
      exit(-1);
    }
  } else {
    first(sem_id, 2);
  }
  return sem_id;
} 

int main(int argc, char** argv)
{
  int     fd[2], result;
  size_t  size;
  
  char    message[15];
  int     n = 0;

  if (argc < 2) {
    printf("Должно быть введено N.\n");
    exit(-1);
  }

  int parse_status = sscanf(argv[1], "%d", &n);
  if (parse_status < 0) {
    printf("Некорректное N.\n");
    exit(-1);
  }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  int sem_id = getSem();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {
    for (int i = 1; i <= n; ++i) {
      second(sem_id, 1);

      if (i != 1) {
        size = read(fd[0], message, 15);
        if (size < 0) {
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }
        printf("%d. Предок прочел:%s\n", i, message);
      }

      size = write(fd[1], "Hello, child!", 14);
      if (size != 14) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      second(sem_id, 1);
    }
    printf("Предок закончил\n");
  } else {
    for (int i = 1; i <= n; ++i) {
      third(sem_id);

      size = read(fd[0], message, 14);
      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }
      printf("%d. Ребенок прочел:%s\n", i, message);

      size = write(fd[1], "Hello, parent!", 15);
      if (size != 15) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      first(sem_id, 2);
    }

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }
    printf("Потомок закончил\n");
  }

  return 0;
}
