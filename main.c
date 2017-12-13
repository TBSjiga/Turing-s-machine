#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MOVL 1
#define MOVR 2
#define INC 3
#define DEC 4
#define PRINT 5
#define GET 6
#define PRINTC 7
#define BEGIN 8
#define END 9

typedef struct charlist{
    unsigned char value;
    struct charlist *next;
    struct charlist *prev;
} charlist;

typedef struct listline{
    struct charlist *head;
    struct charlist *tail;
    int size;
} listline;

listline *CreateListLine();
void PushBack(listline *list, unsigned char value);
void PushFront(listline *list, unsigned char value);
unsigned char PopFront(listline *list);

listline * CreateListLine(){
    listline *tmp = (listline*)malloc(sizeof(listline));
    tmp->size = 0;
    tmp->head = tmp->tail = 0;
    return tmp;
}

void PushBack(listline *list, unsigned char data){
    charlist *tmp = (charlist*)malloc(sizeof(charlist));
    if (tmp == 0)exit(1);
    tmp->value = data;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail)
        list->tail->next = tmp;

    list->tail = tmp;

    if (list->head == NULL)
        list->head = tmp;
    ++list->size;
}

void PushFront(listline *list, unsigned char data){
    charlist *tmp = (charlist*)malloc(sizeof(charlist));
    if (tmp == 0) exit(1);
    tmp->value = data;
    tmp->next = list->head;
    tmp->prev = NULL;

	if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;
 
    if (list->head == NULL) {
        list->head = tmp;
    }
    ++list->size;
}

unsigned char PopFront(listline *list){
    unsigned char c;
    charlist *prev;

    if (list->head == 0)
        return 0;

    prev = list->head;
    list->head = list->head->next;

    if (list->head){
        list->head->prev = 0;
    }

    if (prev == list->tail){
        list->tail = 0;
    }

    c = prev->value;
    free(prev);
    --list->size;
    return c;
}

charlist* getptr(listline *mlist, charlist *list, int dir){
    if (dir == MOVR){
        if (list->next == 0){
            PushBack(mlist, 0);
        }
        return (list->next);
    }

    else if (dir == MOVL){
        if (list->prev == 0)
            PushFront(mlist, 0);
        return (list->prev);
    }

    return 0;
}

charlist* getNth(listline *list, size_t index) {
    charlist *tmp = list->head;
    size_t i = 0;
 
    while (tmp && i < index) {
        tmp = tmp->next;
        i++;
    }
 
    return tmp;
}

char *GetCommands(FILE *file){
    char data, *commands = 0, tmp[256];
    int i, j, f = 0, size = 0;
    listline *tmplist = 0;
    tmplist = CreateListLine();

    while ((data = fgetc(file))!=EOF){
        if (data == '\n'){
            if (f == 2)
                ++size;
            f = 0;
            continue;
        }

        if (data == '*') f = 1;
        if (f == 1) continue;
        if ((!(data == ' '))&&(!(data == '\t'))) f = 2;
    }

    fseek(file, ftell(file)-2, SEEK_SET);
    data= fgetc(file);
    if ((data == '\n')&&(fgetc(file) == EOF));
    else ++size;

    commands = (char*)malloc((size+1)*sizeof(char));
    commands[size] = 0;
    fseek(file, SEEK_SET, 0);
    f = 0;
    i = 0;

    while ((data = fgetc(file))!=EOF){
        if (data == '\n'){
            if (f == 2){
                for (j=0; tmplist->size; ++j){
                    tmp[j] = PopFront(tmplist);
                }

                tmp[j] = '\0';

				if (!strcmp("movl", tmp)) commands[i] = MOVL;
                if (!strcmp("movr", tmp)) commands[i] = MOVR;
                if (!strcmp("inc", tmp)) commands[i] = INC;
                if (!strcmp("dec", tmp)) commands[i] = DEC;
                if (!strcmp("get", tmp)) commands[i] = GET;
                if (!strcmp("print", tmp)) commands[i] = PRINT;
                if (!strcmp("printc", tmp)) commands[i] = PRINTC;
                if (!strcmp("begin", tmp)) commands[i] = BEGIN;
                if (!strcmp("end", tmp)) commands[i] = END;
                ++i;
            }
            f = 0;
            continue;
        }

        if (data == '*') f = 1;
        if (f == 1) continue;
        if ((data == ' ')||(data == '\t')) continue;
        else f=2;
        PushBack(tmplist, data);
    }

    return commands;
}

char *GetCommands(FILE *file);
charlist* getptr(listline* mlist, charlist *list, int dir);

int main(int argc, char *argv[])
{
    int f=5;
    FILE *file;
    char *commands=0, *data, *fisizeame;
    charlist *ptr=0;
    listline *line=0;

    if (argc > 1){
        if ((file = fopen(argv[1], "r")) == 0){
            printf ("Enter correct file name!\n");
            return 0;
        }
    }

    else {
        fisizeame = (char*)malloc(500*sizeof(char));
        printf("File name\n");
        fgets(fisizeame, 500*sizeof(char), stdin);
        data = fisizeame;
        while ((*(data))!='\n') ++data;
        *data = '\0';
        while ((file = fopen(fisizeame, "r")) == 0){
            fgets(fisizeame, 500*sizeof(char), stdin);
            data = fisizeame;
            while ((*(data))!='\n') ++data;
            *data = '\0';
        }
    }

    line = CreateListLine();
    commands = GetCommands(file);
    PushBack(line, 0);
    data = commands;
    ptr = line->head;
	int x=0;

    while (f){
        switch (*data){
			case INC:{
				if (ptr->value<255 && ptr->value>=0)
					++ptr->value;
				else {
					printf("%s", "Overflow\n");
					ptr->value=0;
				}
				++data;
				break;
			}
			case DEC:{
				if (ptr->value>0)
					--ptr->value;
				else {
					printf("%s", "Overflow2\n");
					ptr->value=255;
				}
				++data;
				break;
			}
			case MOVL:{
				if (x>0) --x;
				getptr(line, ptr, MOVL);
				getNth(line, x);
				++data;
				break;
			}
			case MOVR:{
				++x;
				getptr(line, ptr, MOVR);
				getNth(line, x);
				++data;
				break;
			}
			case PRINT:{
				printf("%d", (int)ptr->value);
				++data;
				break;
			}
			case GET:{
				printf("Enter symbol: \n");
				ptr->value = getchar();
				fflush(stdin);
				++data;
				break;
			}
			case PRINTC:{
				printf("%data", ptr->value);
				++data;
				break;
			}
			case BEGIN:{
				if (ptr->value == 0)
					while ((*data)!=END)
						++data;
				++data;
				break;
			}
			case END:{
				if (ptr->value!=0)
					while ((*data)!=BEGIN)
						--data;
				else
					++data;
				break;
			}			case 0:{
				return 0;
				break;
			}
		}
    }

    printf("\n");

    return 0;
}
