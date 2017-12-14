#include <stdio.h>
#include <stdlib.h>

struct list
{
   unsigned char value;
   struct list* right;
   struct list* left;
};

char str[256];
FILE *file=NULL;
int get_code(char *com);
struct list* ptr;
void command(int code);
int line();

int line()
{
  int index = 0;
  if (!feof(file))
  {
    fgets(str, sizeof(str),file);
    while((str[index] == '\t') || (str[index] == ' '))
    {
      index++;
    }
    return get_code(str + index);
  }
  return 0;
}

int get_code(char * com){
    if((com[0] == 'm') && (com[3] == 'l'))    /*movl*/
        {
          return 1;
        }
    if((com[0] == 'm') && (com[3] == 'r'))    /*movr*/
        {
          return 2;
        }
    if (com[0] == 'i')   /*inc*/
        {
          return 3;
        }
    if (com[0] == 'd')   /*dec*/
        {
          return 4;
        }
    if((com[0] == 'p') && (com[5] == 'c'))   /*printc*/
        {
          return 7;
        }
    if(com[0] == 'p')   /*print*/
        {
          return 5;
        }
    if(com[0] == 'g')   /*get*/
        {
          return 6;
        }
    if(com[0] == 'b')   /*begin*/
        {
          return 8;
        }
    if(com[0] == 'e')   /*end*/
        {
          return 9;
        }
    return 0;
}

void command(int code)
{
  switch(code)
  {
    case 1:
    if (ptr -> left != NULL)
    {
      ptr = ptr -> left;
    }
    else
    {
      struct list* temp = ptr;
      ptr -> left = (struct list*)malloc(sizeof(struct list));
      ptr = ptr -> left;
      ptr -> value = 0;
      ptr -> right = temp;
      ptr -> left = NULL;
    }
    break;
	
    case 2:
    if(ptr -> right != NULL)
    {
      ptr = ptr-> right;
    }
    else
    {
      struct list* temp = ptr;
            ptr -> right = (struct list*)malloc(sizeof(struct list));
            ptr = ptr -> right;
            ptr -> left = temp;
            ptr -> value = 0;
            ptr -> right = NULL;
    }
    break;
	
    case 3:
    if (ptr -> value == 255)
    {
      ptr -> value = 0;
      printf("overflow \n");
    }
    else
    {
      ptr -> value++;
    }
    break;
	
    case 4:
        if(ptr -> value != 0)
            {
              ptr -> value --;
            }
    break;
		
    case 5:
        printf("%d \n", ptr -> value);
    break;
		
    case 6:
        {
            printf("Please enter a number from 0 to 255: \n ");
            unsigned int temp;
            scanf("%u", &temp);
            ptr -> value = temp;
            printf("\n");
        }
    break;
		
    case 7:
        printf("%c \n", (ptr -> value));
    break;

    case 8:
        {
            int i = 0;
            char s[256];
            s[0] = line();
            while((s[i] != 9) && (!feof(file)))
            {
                command(s[i]);
                i++;
                s[i] = line();
            }
            int max = i;
            while(ptr -> value)
            {
                for(i = 0; i < max; i++)
                {
                    command(s[i]);
                }
            }
        }
        break;
  }
}

int main(int argc, char *argv[])
{
  ptr = (struct list*)malloc(sizeof(struct list));
  ptr -> left = NULL;
  ptr -> right = NULL;
  ptr -> value = 0;

  if (argc != 2)
  {
    printf("File name not found.");
  }
    else
    {
      file = fopen(argv[1],"r");
    }

    if(file)
    {
      while(!feof(file))
      {
        command(line());
      }
    }
    else
    {
      printf("\n Can't read this file.");
    }
    fclose(file);
    return 0;
}
