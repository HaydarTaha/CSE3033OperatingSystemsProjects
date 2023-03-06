#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

/*
 * Emir Ege Eren 150119739
 * Burak Dursun 150119743
 * Haydar Taha Tunç 150119745
 */

struct lineContent{
    char *line;
    int isRead;
    int isUpper;
    int isReplace;
    int isWrite;
    int lineSize;
};

struct lineContent *lineContent;

sem_t semRead;
sem_t semUpper;
sem_t semReplace;
sem_t semWrite;

pthread_mutex_t mutex;

int numberOfLines;
int readSize;

int fd;

//Creating prototype methods

void *read_thread(void *arg);

void *upper_thread(void *arg);

void *replace_thread(void *arg);

void *write_thread(void *arg);

char *read_line(int lineNum);

int getNextReadLine();

int getNextReadLine();

int getNextUpperLine();

int getNextReplaceLine();

int getNextWriteLine();

int getNextWriteLine();

int main(int argc, char *argv[]) {

    // Creating variables
    char *fileName;
    int numberOfReadThread, numberOfUpperThread, numberOfReplaceThread, numberOfWriteThread;

    //Error handling for usage of program
    if ((argc != 8) || (strncmp(argv[1], "-d", strlen("-d")) != 0) || (strncmp(argv[3], "-n", strlen("-n")) != 0)) {
        fprintf(stderr, "ERROR: Invalid argument type.\n");
        fprintf(stderr, "USAGE: %s -d deneme.txt -n 15 5 8 6.\n", argv[0]);
        return 1;
    }

    //Getting fileName
    fileName = argv[2];

    //Get number of lines from file
    if ((fd = open(fileName, O_RDONLY)) < 0){
        fprintf(stderr, "ERROR: Opening file.\n");
    }
    numberOfLines = 0;
    char buffer[MAX_LINE_LENGTH];
    ssize_t byteRead;
    lseek(fd, 0, SEEK_SET);
    while ((byteRead = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
        for (int i = 0; i < byteRead; i++) {
            if (buffer[i] == '\n') {
                numberOfLines++;
            }
        }
    }
    numberOfLines++;

    //Reset file
    lseek(fd, 0, SEEK_SET);

    //Calculate every line size
    int lineSizeArray[numberOfLines];
    memset(lineSizeArray, 0, sizeof(lineSizeArray));
    char c;
    int i = 0;
    while ((byteRead = read(fd, &c, sizeof(char ))) > 0){
        if (c != '\n'){
            lineSizeArray[i]++;
        } else{
            i++;
        }
    }
    lseek(fd, 0, SEEK_SET);

    //Setting lineContent struct
    lineContent = (struct lineContent *) malloc(numberOfLines * sizeof(struct lineContent));
    i = 0;
    while (i < numberOfLines){
        lineContent[i].isRead = 0;
        lineContent[i].isReplace = 0;
        lineContent[i].isUpper = 0;
        lineContent[i].lineSize = lineSizeArray[i];
        lineContent[i].isWrite = 0;
        lineContent[i].line = (char *) malloc(lineContent[i].lineSize * sizeof(char));
        i++;
    }

    //Separating from arguments
    if ((numberOfReadThread = atoi(argv[4])) != 0) {
        if (numberOfReadThread < 0) {
            fprintf(stderr, "ERROR: Read thread number must be positive Integer.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR: Upper thread number must be Integer.\n");
        return 1;
    }

    if ((numberOfUpperThread = atoi(argv[5])) != 0) {
        if (numberOfUpperThread < 0) {
            fprintf(stderr, "ERROR: Upper thread number must be positive Integer.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR: Read thread number must be Integer.\n");
        return 1;
    }

    if ((numberOfReplaceThread = atoi(argv[6])) != 0) {
        if (numberOfReplaceThread < 0) {
            fprintf(stderr, "ERROR: Replace thread number must be positive Integer.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR: Upper thread number must be Integer.\n");
        return 1;
    }

    if ((numberOfWriteThread = atoi(argv[7])) != 0) {
        if (numberOfWriteThread < 0) {
            fprintf(stderr, "ERROR: Replace thread number must be positive Integer.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "ERROR: Upper thread number must be Integer.\n");
        return 1;
    }

    //Initialize semaphore and mutex
    sem_init(&semRead, 0, numberOfReadThread);
    sem_init(&semUpper, 0, numberOfUpperThread);
    sem_init(&semReplace, 0, numberOfReplaceThread);
    sem_init(&semWrite, 0, numberOfWriteThread);
    pthread_mutex_init(&mutex, NULL);

    printf("<Thread-type and ID>                <Output>\n");

    //Create all threads
    pthread_t readPthread_t[numberOfReadThread];
    pthread_t upperPthread_t[numberOfUpperThread];
    pthread_t replacePthread_t[numberOfReplaceThread];
    pthread_t writePthread_t[numberOfWriteThread];

    readSize = 0;
    for (int i = 0; i < numberOfReadThread; i++) {
        pthread_create(&readPthread_t[i], NULL, read_thread, &i);
    }

    for (int i = 0; i < numberOfUpperThread; i++) {
        pthread_create(&upperPthread_t[i], NULL, upper_thread, &i);
    }

    for (int i = 0; i < numberOfReplaceThread; i++) {
        pthread_create(&replacePthread_t[i], NULL, replace_thread, &i);
    }

    for (int i = 0; i < numberOfWriteThread; i++) {
        pthread_create(&writePthread_t[i], NULL, write_thread, &i);
    }

    // Wait all threads
    for (int i = 0; i < numberOfReadThread; i++) {
        pthread_join(readPthread_t[i], NULL);
    }

    for (int i = 0; i < numberOfUpperThread; i++) {
        pthread_join(upperPthread_t[i], NULL);
    }

    for (int i = 0; i < numberOfReplaceThread; i++) {
        pthread_join(replacePthread_t[i], NULL);
    }

    for (int i = 0; i < numberOfWriteThread; i++) {
        pthread_join(writePthread_t[i], NULL);
    }

    //free(lineContent);
    return 0;
}

int getNextReadLine() {
    for (int i = 0; i < numberOfLines; i++) {
        if (!lineContent[i].isRead) {
            return i;
        }
    }
    return -1;
}

char *read_line(int line_num) {

    //Calculating line_num from beginning to the given line
    int current_offset = 0;
    for (int i = 0; i < line_num; i++) {
        current_offset += (lineContent[i].lineSize + 1) * sizeof(char );
    }

    //Move to the beginning of the line
    if ((lseek(fd, current_offset, SEEK_SET)) == -1) {
        fprintf(stderr, "ERROR: lseek.\n");
    }

    //Read line from file
    char *line = (char *)malloc((lineContent[line_num].lineSize) * sizeof(char));
    int bytes_read = read(fd, line, ((lineContent[line_num].lineSize) * sizeof(char)));
    if (bytes_read == -1) {
        fprintf(stderr, "ERROR: read.\n");
    }

    //Add null-terminating character
    line[bytes_read] = '\0';
    return line;
}

void *read_thread(void *arg){
    int number = *((int*) arg);
    while (1) {
        int line_num;

        sem_wait(&semRead);

        // Lock mutex
        pthread_mutex_lock(&mutex);

        //Getting next available line
        line_num = getNextReadLine();

        if (line_num == -1) {
            //Release lock on mutex
            pthread_mutex_unlock(&mutex);
            break;
        }

        //Read line from file
        char *line = read_line(line_num);
        int line_size = strlen(line);
        lineContent[line_num].line = line;
        lineContent[line_num].lineSize = line_size;
        lineContent[line_num].isRead = 1;
        printf("Read_%d                         Read_%d read the line %d which is \"%s\"\n", number, number, line_num + 1, lineContent[line_num].line);

        //Release lock on mutex
        pthread_mutex_unlock(&mutex);

        //Signal semaphore
        sem_post(&semUpper);
        sem_post(&semRead);
    }
    return NULL;
}

int getNextUpperLine(){
    for (int i = 0; i < numberOfLines; i++) {
        if (lineContent[i].isRead == 1 && lineContent[i].isUpper == 0) {
            return i;
        }
    }
    return -1;
}

void *upper_thread(void *arg){
    int number = *((int*) arg);
    while (1) {
        //Wait for semaphore
        sem_wait(&semUpper);

        //Get next available line
        pthread_mutex_lock(&mutex);
        int nextLine = getNextUpperLine();
        if (nextLine == -1) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        //Getting old line before the converting line to upper
        char line[MAX_LINE_LENGTH];
        sprintf(line, "%s", lineContent[nextLine].line);

        // Convert the line to uppercase
        for (int i = 0; i < lineContent[nextLine].lineSize; i++) {
            lineContent[nextLine].line[i] = toupper(lineContent[nextLine].line[i]);
        }
        lineContent[nextLine].isUpper = 1;

        printf("Upper_%d                         Upper_%d read index %d and converted \"%s\" to \"%s\"\n", number, number, nextLine, line, lineContent[nextLine].line);

        //Release lock on mutex
        pthread_mutex_unlock(&mutex);

        //Signal semaphore
        sem_post(&semReplace);

    }
    return 0;
}

int getNextReplaceLine(){
    for (int i = 0; i < numberOfLines; i++) {
        if (lineContent[i].isRead == 1 && lineContent[i].isUpper ==  1 && lineContent[i].isReplace == 0) {
            return i;
        }
    }
    return -1;
}

void *replace_thread(void *arg){
    int number = *((int*) arg);
    while (1){
        //Wait for semaphore
        sem_wait(&semReplace);

        //Get next available line
        pthread_mutex_lock(&mutex);
        int nextLine = getNextReplaceLine();
        if (nextLine == -1) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        //Getting old line before the replacing
        char line[MAX_LINE_LENGTH];
        sprintf(line, "%s", lineContent[nextLine].line);

        // Replace the line
        for (int i = 0; i < lineContent[nextLine].lineSize; ++i) {
            if (lineContent[nextLine].line[i] == ' '){
                lineContent[nextLine].line[i] = '_';
            }
        }
        lineContent[nextLine].isReplace = 1;

        printf("Replace_%d                         Replace_%d read index %d and converted \"%s\" to \"%s\"\n", number, number, nextLine, line, lineContent[nextLine].line);

        //Release lock on mutex
        pthread_mutex_unlock(&mutex);

        //Signal semaphore
        sem_post(&semWrite);
    }
    return NULL;
}

int getNextWriteLine(){
    for (int i = 0; i < numberOfLines; i++) {
        if (lineContent[i].isRead == 1 && lineContent[i].isUpper == 1 && lineContent[i].isReplace == 1 && lineContent[i].isWrite == 0) {
            return i;
        }
    }
    return -1;
}

void *write_thread(void *arg){
    int number = *((int*) arg);
    while (1) {
        //Wait for the semaphore to be released
        sem_wait(&semWrite);

        //Get the next line to write
        pthread_mutex_lock(&mutex);
        int nextLine = getNextWriteLine();
        if (nextLine == -1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        char *line = lineContent[nextLine].line;
        lineContent[nextLine].isWrite = 1;

        //Open the file with append flag
        int fd_out = open("output.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (fd_out < 0) {
            fprintf(stderr, "Thread %d: Error opening output file\n", (int ) pthread_self());
            return NULL;
        }

        //Write the line to the file
        int written = write(fd_out, line, lineContent[nextLine].lineSize);
        if (written < 0) {
            fprintf(stderr, "Thread %d: Error writing to output file\n", (int ) pthread_self());
            return NULL;
        }

        printf("Writer_%d                         Writer_%d write line %d back which is \"%s\"\n", number, number, nextLine + 1, lineContent[nextLine].line);

        // Close the file
        close(fd_out);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*void *read_thread(void *arg){
    while (1){
        pthread_t pthread = pthread_self();
        srand(time(0) ^ (int) pthread);
        int random_number = rand() % numberOfLines;
        if (lineContent[random_number].isRead == 0){
            pthread_mutex_lock(&mutex);
            int current_offset = 0;
            for (int i = 0; i < random_number; i++) {
                current_offset += (lineContent[i].lineSize + 1) * sizeof(char );
            }
            //printf("offset: %d\n", current_offset);
            //printf("Line Size: %d, line: %d\n", current_offset * sizeof(char ), random_number);
            lseek(fd, current_offset, SEEK_SET);
            int size = lineContent[random_number].lineSize;
            ssize_t sSize = size;
            char line[size];
            read(fd, &line, sSize * sizeof(char));
            //printf("%s size: %d\n", line, sSize);
            lseek(fd, 0, SEEK_SET);
            lineContent[random_number].isRead = 1;
            readSize++;
            //printf("Read from file .%d. %d\n", random_number, (readSize) * sizeof(char ));
            snprintf(lineContent[random_number].line, (lineContent[random_number].lineSize + 1) * sizeof(char ), "%s", line);
            //printf("%s %d\n", lineContent[random_number].line, lineContent[random_number].lineSize);
            pthread_mutex_unlock(&mutex);
        } else if (readSize >= numberOfLines){
            pthread_exit(NULL);
        } else {
            continue;
        }
    }
    return 0;
}*/