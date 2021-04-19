#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

int inc(int semid, struct sembuf* buf) {
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int dec(int semid, struct sembuf* buf) {
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int main()
{
    int parent[2], result;
    size_t size = 0;
    key_t key;
    char pathname[] = "1.c";
    struct sembuf buffer;

    int semid;
    char resstring[14];

    if (pipe(parent) < 0) {
        printf("Создание pipe предка невозможно\n\r");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Ключ не сгенерирован.\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Семафор не найден.\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Не возможно получить Semid\n");
            exit(-1);
        }
        printf("Создание прошло успешно!\n");
    }


    result = fork();

    if (result < 0) {
        printf("Ошибка fork\n\r");
        exit(-1);
    }

    else if (result > 0) {
        int N;
        printf("Введите N: \n");
        scanf("%d", &N);
        if (N < 2) {
            printf("N должен быть >= 2.\n");
            exit(-1);
        }

        for (size_t i = 0; i < N; i++)
        {
            if (write(parent[1], "Hello, world!", 14) != 14) {
                printf("Can\'t write all string\n\r");
                exit(-1);
            }

            printf("Пара №%d, Предок передал сообщение потомку.\n\r", i + 1);

            inc(semid, &buffer);
            dec(semid, &buffer);
            size = read(parent[0], resstring, 14);

            if (size != 14) {
                printf("Can\'t read from child\n\r");
                exit(-1);
            }

            printf("Предок прочел сообщение потомка: %s\n\r", resstring);
        }
        close(parent[0]);
    }
    else {
        int counter = 0;
        while(1){
            dec(semid, &buffer);
            size = read(parent[0], resstring, 14);

            if (size < 0) {
                close(parent[1]);
                close(parent[0]);

                printf("Успешно\n");  
                return 0;
            }

            printf("Пара №%d, Потомок прочитал %s\n\r", ++counter, resstring);

            if (write(parent[1], "Privet world!", 14) != 14) {
                printf("Невозможно записать всю строку.\n");
                exit(-1);
            }
            inc(semid, &buffer);
        }
    }
    return 0;
}
