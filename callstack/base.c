#include <stdio.h>
#include <string.h>
#define STACK_SIZE 50

int     call_stack[STACK_SIZE];
char    stack_info[STACK_SIZE][20];

int SP = -1; 
int FP = -1;

void func1(int arg1, int arg2, int arg3);
void func2(int arg1, int arg2);
void func3(int arg1);

void push(int value, char *str);
int pop();
void prologue(char *str);
void epilogue(int num_args);

void print_stack()
{
    if (SP == -1)
    {
        printf("Stack is empty.\n");
        return;
    }

    printf("====== Current Call Stack ======\n");
    
    for (int i = SP; i >= 0; i--)
    {
        if (call_stack[i] != -1)
            printf("%d : %s = %d", i ,stack_info[i], call_stack[i]);
        else
            printf("%d : %s", i, stack_info[i]);

        if (i == SP)
            printf("    <=== [esp]\n");
        else if (i == FP)
            printf("    <=== [ebp]\n");
        else
            printf("\n");
    }
    printf("================================\n\n");
}

void push(int value, char *str) 
{
    
    SP++;
    call_stack[SP] = value;
    strcpy(stack_info[SP], str);
}

int pop()
{
    if(SP==-1)
        return -1;
    int value=call_stack[SP];
    SP--;
    return value;
}

void prologue(char *str)
{
    push(-1, "Return Address");
    push(FP, str);
    FP = SP;
}

void epilogue(int num_args)
{
    SP = FP;
    FP = pop();
    pop();
    
    for (int i = 0; i < num_args; i++) {
        pop();
    }
}

void func1(int arg1, int arg2, int arg3)
{

    push(arg3,"arg3");
    push(arg2,"arg2");
    push(arg1,"arg1");
    prologue("func1");
    int var_1 = 100;
    push(var_1, "var_1");
    print_stack();
    func2(11, 13);
	print_stack();
    epilogue(3);
}


void func2(int arg1, int arg2)
{
    push(arg2,"arg2");
    push(arg1,"arg1");
    prologue("func2");
	int var_2 = 200;
    push(var_2,"var_2");
    print_stack();
    func3(77);
    print_stack();
    epilogue(2);
}


void func3(int arg1)
{
    push(arg1,"arg1");
	prologue("func3");
    int var_3 = 300;
    int var_4 = 400;
    push(var_3,"var_3");
    push(var_4,"var_4");
    print_stack();
    epilogue(1);
}


int main()
{
    func1(1, 2, 3);
    print_stack();
    return 0;
}
