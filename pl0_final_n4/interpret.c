// pl0 compiler source code
//for Visual Studio
#pragma warning(disable:4996) 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "PL0.h"
#include "set.c"

/////////////////////////////////////////////////////////////////////
void listcode(int from, int to)
{
    int i;

    printf("\n");
    for (i = from; i < to; i++)
    {
        printf("%5d %s\t%d\t%d\n", i, mnemonic[code[i].f], code[i].l,
               code[i].a);
    }
    printf("\n");
} // listcode
//////////////////////////////////////////////////////////////////////
int base(int stack[], int currentLevel, int levelDiff)
{
    int b = currentLevel;

    while (levelDiff--)
        b = stack[b];
    return b;
} // base

//////////////////////////////////////////////////////////////////////
// interprets and executes codes.
void interpret()
{
    int pc; // program counter
    int stack[STACKSIZE];
    int top; // top of stack
    int b; // program, base, and top-stack register
    instruction i; // instruction register

    printf("Begin executing PL/0 program.\n");

    pc = 0;
    b = 1;
    top = 3;
    stack[1] = stack[2] = stack[3] = 0;
    do
    {
        i = code[pc++];
        switch (i.f)
        {
        case LIT:
            stack[++top] = i.a;
            break;
        case OPR:
            switch (i.a) // operator
            {
            case OPR_RET:
                top = b - 1;
                pc = stack[top + 3];
                b = stack[top + 2];
                break;
            case OPR_NEG:
                stack[top] = -stack[top];
                break;
            case OPR_ADD:
                top--;
                stack[top] += stack[top + 1];
                break;
            case OPR_MIN:
                top--;
                stack[top] -= stack[top + 1];
                break;
            case OPR_MUL:
                top--;
                stack[top] *= stack[top + 1];
                break;
            case OPR_DIV:
                top--;
                if (stack[top + 1] == 0)
                {
                    fprintf(stderr, "Runtime Error: Divided by zero.\n");
                    fprintf(stderr, "Program terminated.\n");
                    continue;
                }
                stack[top] /= stack[top + 1];
                break;
            case OPR_ODD:
                stack[top] %= 2;
                break;
            case OPR_NOT:
                stack[top] = !stack[top];
                break;
            case OPR_EQU:
                top--;
                stack[top] = stack[top] == stack[top + 1];
                break;
            case OPR_NEQ:
                top--;
                stack[top] = stack[top] != stack[top + 1];
                break;
            case OPR_LES:
                top--;
                stack[top] = stack[top] < stack[top + 1];
                break;
            case OPR_GEQ:
                top--;
                stack[top] = stack[top] >= stack[top + 1];
                break;
            case OPR_GTR:
                top--;
                stack[top] = stack[top] > stack[top + 1];
                break;
            case OPR_LEQ:
                top--;
                stack[top] = stack[top] <= stack[top + 1];
                break;
            case OPR_AND:
                top--;
                stack[top] = stack[top] & stack[top + 1];
                break;
            case OPR_OR:
                top--;
                stack[top] = stack[top] | stack[top + 1];
                break;
            case OPR_PRT:
                printf("%d ", stack[top]);
                top--;
                ;
                break;
            case OPR_WTL:
                printf("\n");
                break;
            case OPR_RED:
                scanf("%d", &stack[++top]);
                break;

            default:
                fprintf(stderr, "No Such OPR_CODE=%d.\n", i.a);
                break;
            } // switch
            break;
        case LOD:
            stack[++top] = stack[base(stack, b, i.l) + i.a];
            break;
        case LAD:
            stack[top] = stack[base(stack, b, i.l) + stack[top] + i.a];
            break;
        case STO:
            stack[base(stack, b, i.l) + i.a] = stack[top];
            //printf("%d\n", stack[top]);
            top--;
            break;
        case STA: //ƫ����ȡ��ջ��Ԫ��
            stack[base(stack, b, i.l) + stack[top - 1] + i.a] = stack[top];
            //printf("%d\n", stack[top]);
            top -= 2;
            break;
        case CAL_OP:
            stack[top + 1] = base(stack, b, i.l);
            // generate new block mark
            stack[top + 2] = b;
            stack[top + 3] = pc;
            b = top + 1;
            pc = i.a;
            break;
        case INT_OP:
            top += i.a;
            break;
        case JMP:
            pc = i.a;
            break;
        case JPC:
            if (stack[top] == 0)
                pc = i.a;
            top--;
            break;
        } // switch
    }
    while (pc);

    printf("End executing PL/0 program.\n");
} // interpret

void main(int argc,char *argv[]) {
	FILE* hbin;
	int i;
    char s[80];
	if (argc==1)
		strcpy(s,"a.txt");
	else
		strcpy(s,argv[1]);
	printf("file name=%s\n",s);
	getch();
	if ((hbin = fopen(s, "r")) == NULL)
	{	
		printf("File %s can't be opened.\n", s);
		exit(1);
	}
	hbin = fopen(s, "r");
	int cx;
	//fread(&cx,sizeof(cx),1,hbin);
	fscanf(hbin,"%d",&cx);
	//printf("cx=%d\n",cx);
	for (i = 0; i < cx; i++){
		//fread(&code[i], sizeof(instruction), 1, hbin);
		fscanf(hbin,"%d%d%d",&code[i].f,&code[i].l,&code[i].a);
	}
	
	fclose(hbin);
	interpret();
	 
} // main

