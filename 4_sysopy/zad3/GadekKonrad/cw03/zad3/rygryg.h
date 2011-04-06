#ifndef RYGRYG_H_
#define RYGRYG_H_

int setLock(short int, int, int, short int);
int getLock(short int *, int *, int *, int *, int *);
void printPS(void);
void getFileName(void);

void setWriteLock(void);
void setReadLock(void);
void getLocks(void);
void releaseLock(void);
void readFile(void);
void writeFile(void);
void quit(void);
void help(void);
void unknownCommand(void);


#endif /* RYGRYG_H_ */
