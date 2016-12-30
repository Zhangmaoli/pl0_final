// pl0 compiler source code
//for Visual Studio
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "PL0.h"
#include "set.c"
#include "color.h"
//////////////////////////////////////////////////////////////////////

int jdgok(int curBlkNum,int saveBlkNum)  //if saveBlkNum is the prefix of the curBlkNum it's ok
{
	while (curBlkNum>saveBlkNum){
		curBlkNum/=10;
	}
	return curBlkNum==saveBlkNum;
}

void printTable();

void inblock()
{
	block_num=block_num*10+blc[block_level];
	/*
	if (block_num==101111) {
	    int kk;
	    kk=1;
	    printTable();
	}
	*/
	
	color(COLOR_YELLOW); printf("blk_num=%d\n",block_num); color(COLOR_WHITE);
	blc[block_level]++;
	block_level++;
	blc[block_level]=0;
}
void outblock(int saveBlkNum,int saveBlkLvl)
{
	block_level=saveBlkLvl;
	block_num=saveBlkNum;
	color(COLOR_YELLOW); printf("blk_num=%d\n",block_num);color(COLOR_WHITE);
}


// print error message.
void error(int n)
{

    color(12); //red
    int i;
    printf("      ");
    for (i = 1; i <= cc - 1; i++)
        printf(" ");
    printf("^\n");
    printf("Error %3d: %s\n", n, err_msg[n]);
    err++;
    color(7);//black
} // error

//////////////////////////////////////////////////////////////////////
void getch(void)
{
    if (cc == ll)
    {
        if (feof(infile))
        {
            printf("\nPROGRAM INCOMPLETE\n");
            exit(1);
        }
        ll = cc = 0;
        printf("%5d  ", cx);
        while ((!feof(infile)) // added & modified by alex 01-02-09
                && ((ch = getc(infile)) != '\n'))
        {
            printf("%c", ch);
            line[++ll] = ch;
        } // while
        printf("\n");
        line[++ll] = ' ';
    }
    ch = line[++cc];
} // getch

//////////////////////////////////////////////////////////////////////
// gets a symbol from input stream.
void getsym(void)
{
    int i, k;
    char a[MAXIDLEN + 1];

    while (ch == ' '||ch=='\t')
        getch();

    if (isalpha(ch))   // symbol is a reserved word or an identifier.
    {
        k = 0;
        do
        {
            if (k < MAXIDLEN)
                a[k++] = ch;
            getch();
        }
        while (isalpha(ch) || isdigit(ch));   //超出部分不报错自动截断？
        a[k] = 0;
        strcpy(id, a);
        word[0] = id;
        i = NRW;
        while (strcmp(id, word[i--]))
            ;
        if (++i)
            sym = wsym[i]; // symbol is a reserved word
        else
            sym = SYM_IDENTIFIER; // symbol is an identifier
    }
    else if (isdigit(ch))     // symbol is a number.
    {
        k = num = 0;
        sym = SYM_NUMBER;
        do
        {
            num = num * 10 + ch - '0';
            k++;
            getch();
        }
        while (isdigit(ch));
        if (k > MAXNUMLEN)
            error(25); // The number is too great.
    }
    else if (ch == ':')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_BECOMES; // :=
            getch();
        }
        else
        {
            sym = SYM_NULL; // illegal?
        }
    }
    else if (ch == '>')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_GEQ; // >=
            getch();
        }
        else
        {
            sym = SYM_GTR; // >
        }
    }
    else if (ch == '<')
    {
        getch();
        if (ch == '=')
        {
            sym = SYM_LEQ; // <=
            getch();
        }
        else if (ch == '>')
        {
            sym = SYM_NEQ; // <>
            getch();
        }
        else
        {
            sym = SYM_LES; // <
        }
    }
    else if (ch == '/')   //����ע�͹���
    {
        getch();
        if (ch == '/')
        {
            //sym = SYM_LINECOMM;
            while (ll - cc)
            {
                getch();
            }
            getsym();
        }
        else if (ch == '*')
        {
            //sym = SYM_BLOCKCOMM;
            int end = 1;
            char b, a = 0;
            getch();
            while (end)
            {
                if (a != 0)
                    a = b;
                else
                    a = ch;
                getch();
                b = ch;
                if (a == '*' && b == '/')
                    end = 0; //��δ��Ӵ������?
            }
            getch();
            getsym();
        }
        else if(ch=='=')
        {
        	sym=SYM_DIVEQU,
        	getch();
        }
        else
        {
            sym = SYM_SLASH;
        }
    }
    else if(ch=='%')
	{
		getch();
		{
			if(ch=='=')
			{
				sym=SYM_MODEQU;
				getch();
			}
			else error(38);//////�¼� 
		}
	}
    else     // other tokens
    {
        i = NSYM;
        csym[0] = ch;
        while (csym[i--] != ch)
            ;
        if (++i)
        {
            if(ch=='+')
			{
				getch();
				if(ch=='=')
				{
					sym=SYM_ADDEQU;
					getch();
				}
				else  if(ch=='+')
				{
					sym=SYM_ADDADD;
					getch();
				}
				else 
				{
					sym=SYM_PLUS;
				}
			}
			else if(ch=='-')
			{
				getch();
				if(ch=='=')
				{
					sym=SYM_SUBEQU;
					getch();
				}
				else if(ch=='-')
				{
					sym=SYM_SUBSUB;
					getch();
				}
				else sym=SYM_MINUS;
			}
			else if(ch=='*')
			{
				getch();
				{
					if(ch=='=')
					{
						sym=SYM_MULEQU;
						getch();
					}
					else sym=SYM_TIMES;
				}
			}
			else 
			{
				sym = ssym[i];
			    getch();
			}
        }
        else
        {
            color(12);
            printf("Fatal Error: Unknown character.\n");
            color(7);
            exit(1);
        }
    }
    //printf("zq: sym=%d\n",sym);
} // getsym

//////////////////////////////////////////////////////////////////////
// generates (assembles) an instruction.
void gen(int x, int y, int z)
{
    if (cx > CXMAX)
    {
        printf("Fatal Error: Program too long.\n");
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx++].a = z;
} // gen

void dimdeclaration(void)
{
    void enter(int kind);
    char idsaved[MAXIDLEN + 1] = { '\0' };
    int constexpre(symset fsys);
    strcpy(idsaved, id);
    dim = 0; //��ʼ��
    array_size = 1; //��ʼ��
    while (sym == SYM_LSQUARE)   //'['
    {
        getsym();
        /*if (sym == SYM_NUMBER) {
         latit[dim++] = num;
         array_size *= num;
         getsym();
         } else {
         error(26); //lack num
         }*/
        symset set = createset(SYM_RSQUARE, SYM_NULL);
        int nn = constexpre(set);
        latit[dim++] = nn;
        destroyset(set);
        array_size *= nn;
        if (sym == SYM_RSQUARE)   //']'
        {
            getsym();
        }
        else
        {
            error(27); //lack ']'
        }
    }
    strcpy(id, idsaved);
    enter(ID_ARRAY);
    /*	printf("ID_ARRAY:%d\n",ID_ARRAY);
     array * mk=(array*)&table[tx];
     printf("name:%s\n",mk->name);
     printf("dim:%d\n",mk->dim_n);
     printf("kind:%d\n",mk->kind);
     p_dim* p=mk->next;
     for(int b=0;b<mk->dim_n;b++){
     printf("%d,",p->dim_len);
     p=p->next;
     }
     printf("\n");*/
}

//////////////////////////////////////////////////////////////////////
// tests if error occurs and skips all symbols that do not belongs to s1 or s2.
void test(symset s1, symset s2, int n)
{
    symset s;

    if (!inset(sym, s1))
    {
        if (n)
        {
            error(n);
        }
        s = uniteset(s1, s2);
        while (!inset(sym, s))
            getsym();
        destroyset(s);
    }

} // test

//////////////////////////////////////////////////////////////////////
int dx[MAXLEVEL]; // data allocation index
int zx[MAXLEVEL];

// enter object(constant, variable or procedre) into table.
void enter(int kind) //����txָ�룬������������ȫ����ã����ǹ�̵ı�ʶ�����addressδ�����ȫ���?
{
    mask* mk;
    array* ar;
    tx++;
    //search the name before entering
    int i;
    for (i = tx_[level]; i < tx; i++)
    {
        if (!strcmp(id, table[i].name))
        {
            error(36);
            goto l1;
        }
    }

    strcpy(table[tx].name, id);
    table[tx].kind = kind;
	table[tx].cnt=0;  //zq
	table[tx].lpl=loop_level; //
	table[tx].blkNum=block_num;
	table[tx].evl=createevl(0,0);
    table[tx].quote = 0; //cy_quote
    switch (kind)
    {
    case ID_CONSTANT:
        if (num > MAXADDRESS)
        {
            error(25); // The number is too great.
            num = 0;
        }
        table[tx].value = num;
        break;
    case ID_VARIABLE:
        mk = (mask*) &table[tx];
        mk->level = level;
        mk->address = dx[level]++;
        break;
    case ID_PROCEDURE:
        mk = (mask*) &table[tx];
        mk->level = level;
        break;
    case ID_ARRAY:
        ar = (array*) &table[tx];
        ar->dim_n = dim;
        ar->level = level;
        ar->address = dx[level];
        ar->kind = (short) kind;
        dx[level] += array_size;
        p_dim* p = NULL;
        int i;
        for (i = dim - 1; i >= 0; i--)
        {
            p_dim* q = (p_dim*) malloc(sizeof(p_dim));
            q->dim_len = latit[i];
            q->next = p;
            p = q;
        }
        ar->next = p;
        break;
    } // switch
l1:
    ;
} // enter
void enterPar()
{
    mask* mk;
    tx++;
    strcpy(table[tx].name, id);
    table[tx].kind = ID_VARIABLE;
    mk = (mask*) &table[tx];
    mk->level = level + 1; //differences
    mk->address = zx[level]++; //differences
    mk->quote = 1;
}

void modifyTable(int numOfPar)
{
    int i;
    mask* mk;
    for (i = 0; i < numOfPar; i++)
    {
        mk = (mask*) &table[tx - i];
        mk->address = mk->address - numOfPar;
    }
}

void printTable()
{
    int i;
    printf("=========================print Table=========================\n");
    printf("%10s%10s%10s%10s%10s%10s%10s\n", "No.", "name", "level", "address",
           "numOfPar", "quote", "dim");
    for (i = 1; i <= tx; i++)
    {
        array * ar = (array*) &table[i];
        if (table[i].kind == ID_CONSTANT)
            printf("%10d:%10s%10d\n", i, table[i].name, table[i].value);
        else if (ar->kind == ID_ARRAY)
        {
            printf("%10d:%10s%10d%10d%10d%10d%10d\n", i, table[i].name,
                   ar->level, ar->address, ar->numOfPar, ar->quote, ar->dim_n);
            /*	p_dim *p=ar->next;
             while(p){
             printf("%10d",p->dim_len);
             p=p->next;
             }
             printf("\n");*/
        }
        else
        {
            mask *mk = (mask*) &table[i];
            printf("%10d:%10s%10d%10d%10d%10d\n", i, table[i].name, mk->level,
                   mk->address, mk->numOfPar, mk->quote);
        }
    }
}
//////////////////////////////////////////////////////////////////////
// locates identifier in symbol table.
int position(char* id)
{
    int i;
    strcpy(table[0].name, id);
    i = tx + 1;
    while (strcmp(table[--i].name, id) != 0)
        ;
    mask * mk = (mask*) &table[i];
    array * ar = (array*) &table[i];
    if (mk->level == level)   //cy_quote
    {
        table[i].quote = 1;
    }
    else
        table[i].quote = 2;

    return i;
} // position

//////////////////////////////////////////////////////////////////////
void constdeclaration(symset fsys)
{
    int constexpre(symset fsys);
    if (sym == SYM_IDENTIFIER)
    {
    	char idsaved[MAXIDLEN+1];
    	strcpy(idsaved,id);
        getsym();
        if (sym == SYM_EQU || sym == SYM_BECOMES)
        {
            if (sym == SYM_BECOMES)
                error(1); // Found ':=' when expecting '='.
            getsym();
            num = constexpre(
                      uniteset(fsys, createset(SYM_COMMA,SYM_SEMICOLON, SYM_NULL)));
            strcpy(id,idsaved);
            enter(ID_CONSTANT);
        }
        else
        {
            error(3); // There must be an '=' to follow the identifier.
        }
    }
    else
        error(4);
    // There must be an identifier to follow 'const', 'var', or 'procedure'.
} // constdeclaration

//////////////////////////////////////////////////////////////////////
void vardeclaration(void)
{
    if (sym == SYM_IDENTIFIER)
    {
        char idsaved[MAXIDLEN + 1] = { '\0' };
        strcpy(idsaved, id); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        getsym();
        if (sym == SYM_LSQUARE)
        {
            strcpy(id, idsaved);
            dimdeclaration();
        }
        else
        {
            char idsaved2[MAXIDLEN + 1] = { '\0' };
            strcpy(idsaved2, id);
            strcpy(id, idsaved);
            enter(ID_VARIABLE);
            strcpy(id, idsaved2);
        }
    }
    else
    {
        error(4); // There must be an identifier to follow 'const', 'var', or 'procedure'.
    }
} // vardeclaration

//////////////////////////////////////////////////////////////////////
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
int constfactor(symset fsys)
{
    int constexpre(symset fsys);
    int i;
    int n;
    symset set;

	
    test(facbegsys, fsys, 24); // The symbol can not be as the beginning of an expression.
	//test(facbegsys, uniteset(fsys,createset(SYM_IDENTIFIER,SYM_NULL)), 24); 
    while (inset(sym, facbegsys))
    {
        if (sym == SYM_IDENTIFIER)
        {
            if ((i = position(id)) == 0)
            {
                error(11); // Undeclared identifier.
                getsym();
            }
            else
            {
                array* ar = (array*) &table[i];

                switch (table[i].kind)
                {
                    mask* mk;
                case ID_CONSTANT:
                    n = table[i].value;
                    getsym();
                    break;
                case ID_VARIABLE:
                    error(37);
                    getsym();
                    break;
                case ID_PROCEDURE:
                    error(21); // Procedure identifier can not be in an expression.
                    getsym();
                    break;
                default:
                    error(37);
                    break;
                } // switch
            }
        }
        else if (sym == SYM_NUMBER)
        {
            if (num > MAXADDRESS)
            {
                error(25); // The number is too great.
                num = 0; //number̫����ֱ�Ӹ�ֵΪ0
            }
            n = num;
            getsym();
        }
        else if (sym == SYM_LPAREN)   //(expression)
        {
            getsym();
            set = uniteset(createset(SYM_RPAREN, SYM_NULL), fsys);
            n = constexpre(set);
            destroyset(set);
            if (sym == SYM_RPAREN)
            {
                getsym();
            }
            else
            {
                error(22); // Missing ')'.
            }
        }
        //test(fsys, createset(SYM_LPAREN, SYM_NULL), 23);
    } // while
    return n;
} // factor

int constterm(symset fsys)
{
    int mulop;
    symset set;

    set = uniteset(fsys, createset(SYM_TIMES, SYM_SLASH, SYM_NULL));
    int n1 = constfactor(set);
    int n = n1;
    while (sym == SYM_TIMES || sym == SYM_SLASH)
    {
        mulop = sym;
        getsym();
        int n2 = constfactor(set);
        if (mulop == SYM_TIMES)
        {
            n1 = n1 * n2;
        }
        else
        {
            n1 = n1 / n2;
        }
    } // while
    destroyset(set);
    return n1;
} // term

int constexpre(symset fsys)
{
    int addop;
    symset set;
    int n = 0;
    set = uniteset(fsys, createset(SYM_PLUS, SYM_MINUS, SYM_NULL));
    if (sym == SYM_PLUS || sym == SYM_MINUS)
    {
        addop = sym;
        getsym();
        n = constterm(set);
        if (addop == SYM_MINUS)
        {
            n = -n;
        }
    }
    else
    {
        n = constterm(set);
    }

    while (sym == SYM_PLUS || sym == SYM_MINUS)
    {
        addop = sym;
        getsym();
        int n1 = constterm(set);
        if (addop == SYM_PLUS)
        {
            n = n + n1;
        }
        else
        {
            n = n - n1;
        }
    } // while

    destroyset(set);
    return n;
} // expression
int sx=0;
void optimize(mask * mk,int saveCx)
{	 
        
    //printf("cx:	%d,	loop_level:	%d,	name:	%s\n",cx,loop_level,mk->name);
    
    //if (loop_level>0) return;
    if (!OPTM) return;
    evl ee=mk->evl;
	ee=ee->next;
	int cnt=0;
	while (ee){
		ee=ee->next;
		cnt++;
	}
	strcpy(id,mk->name);
	if (cnt>1){
		
		int i=position(id);
		if (!i)
		{
			enter(ID_VARIABLE);
			mask* tp=(mask*)&table[tx];
			tp->quote=1;
			table[tx].evl=mk->evl;
			gen(STO,level-tp->level,tp->address);
			tp->cnt++;
			gen(LOD,level-tp->level,tp->address);
			
		}
		else
		{
			int flag=1;
			evl e1=mk->evl;
			evl e2=table[i].evl;
			
			//flag=jdgok(table[i].blkNum,block_num);
			flag=jdgok(block_num,table[i].blkNum);
			table[i].blkNum=block_num;
			
			
			e1=e1->next;
			e2=e2->next;
			while (e1){
				if(e1->cnt > e2->cnt || e1->lpl > e2->lpl )
				{
					e2->cnt = e1->cnt;
					e2->lpl	= e1->lpl; 
					flag=0;
				}
				e1=e1->next;
				e2=e2->next;
			}
			if (flag){
				printf("===============Improved in expression %s==============\n",table[i].name);
				if (sx) //term update 
				{
					cx=sx;
					printf("cx=%d\n",cx);
					sx=0;
				}
				else
				{
					cx=saveCx;
					mask *tp=(mask*)&table[i];
					gen(LOD,level-tp->level,tp->address);
				}
			}
			
			
		}
	}
	
}
void optimize_term(mask * mk,int saveCx)
{	
    //if (loop_level>0) return;\OPTM
    if (!OPTM) return;
	evl ee=mk->evl;
	ee=ee->next;
	int cnt=0;
	while (ee){
		ee=ee->next;
		cnt++;
	}
	strcpy(id,mk->name);
	if (cnt>1){
		
		int i=position(id);
		if (!i)
		{
			//do nothing
			
			enter( ID_VARIABLE);
			mask* tp=(mask*)&table[tx];
			tp->quote=1;
			table[tx].evl=mk->evl;
			gen(STO,level-tp->level,tp->address);
			tp->cnt++;
			gen(LOD,level-tp->level,tp->address);
			sx=cx;
		}
		else
		{
            sx=0;
			int flag=1;
			evl e1=mk->evl;//next,next->next
			evl e2=table[i].evl;//next,next->next
			
			
			
			
			e1=e1->next;
			e2=e2->next;
			while (e1){
				if(e1->cnt > e2->cnt || e1->lpl > e2->lpl )
				{
					//e2->cnt = e1->cnt;
					//e2->lpl	= e1->lpl;
					flag=0;
				}
				e1=e1->next;
				e2=e2->next;
			}
			if (flag){
				printf("===============Improved in term==============\n");
				cx=saveCx;
				mask *tp=(mask*)&table[i];
				gen(LOD,level-tp->level,tp->address);
			}		
		}
	}
	
}

mask* factor(symset fsys) {
	mask *mk=(mask*)malloc(sizeof(mask));
	mk->evl=createevl(0,0);
	mask* expression(symset fsys);
	int i;
	symset set;
	
	test(facbegsys, fsys, 24); // The symbol can not be as the beginning of an expression.

	while (inset(sym, facbegsys)) {
		if (sym == SYM_IDENTIFIER) {
			if ((i = position (id)) == 0) {
				error(11); // Undeclared identifier.
				getsym();
			} else {
				//printf("table[i].name=%s\n",table[i].name);
				strcpy(mk->name,table[i].name); //zq
				//printf("mk->name=%s\n",mk->name);
				mk->evl=createevl(i,table[i].cnt,table[i].lpl,0,0,0);  //0 -> SYM_NULL
				
				array* ar = (array*) &table[i];
				mask* tmk;
				switch (table[i].kind) {
			case ID_CONSTANT:
				
				gen(LIT, 0, table[i].value); //????
				getsym();
				break;
			case ID_VARIABLE:
				tmk = (mask*) &table[i];
				gen(LOD, level - tmk->level, tmk->address);
				getsym();
				break;
			case ID_PROCEDURE:
					;
                	mask *tp;
					tp = (mask*) &table[i];

                    getsym();
                    if (sym == SYM_LPAREN)
                    {
                        getsym();
                        if (sym == SYM_RPAREN)
                        {
                            if (tp->numOfPar == 0)
                                gen(CAL_OP, level - tp->level, tp->address);
                            else
                                error(34); // the number of segement doesn't match
                            getsym();
                            goto ff2;
                        }

                        else
                        {
                            int k = 0;
                            do
                            {

                                if (sym == SYM_COMMA)
                                    getsym();
                                k++;
                                symset set1 = createset(SYM_RPAREN, SYM_NULL);
                                set = uniteset(set1, fsys);
                                expression(set);
                                destroyset(set);
                                destroyset(set1);
                                if (k > tp->numOfPar)
                                {
                                    error(34); //too many segments
                                    break;
                                }
                            }
                            while (sym == SYM_COMMA);

                            if (sym == SYM_RPAREN)
                            {
                                if (tp->numOfPar == k)
                                    gen(CAL_OP, level - tp->level, tp->address);
                                else
                                    error(34); //error "didn't match"
                                getsym();
                                goto ff2;
                            }
                            else
                            {
                                error(22); //')' missing
                                goto ff2;
                            }
                        }
                    }
                    else
                    {
                        if (tp->numOfPar == 0)
                            gen(CAL_OP, level - tp->level, tp->address);
                        else
                            error(34); //didn't match
                        goto ff2;

                    }
                    getsym();
ff2:
                    ;

                    gen(INT_OP,0,-tp->numOfPar+1);
                    //gen(INT_OP,0,1);
                    break;
			default:
				if ((ar->kind == ID_ARRAY)) { //??��?????m??
					getsym();
					int dl = 0;
					p_dim* p = ar->next;
					if (sym == SYM_LSQUARE) {
						gen(LIT, 0, 0);
						gen(LIT, 0, p->dim_len);
						while (p && sym == SYM_LSQUARE) {
							p = p->next;
							gen(OPR, 0, OPR_MUL);
							getsym();
							expression(uniteset(fsys,createset(SYM_COMMA, SYM_NULL)));
							if (sym == SYM_RSQUARE) {
								getsym();
							} else {
								error(27);
							}
							gen(OPR, 0, OPR_ADD);
							if (p) {
								gen(LIT, 0, p->dim_len);
							}
						}
						gen(LAD,level - ar->level, ar->address);
					} else {
						error(28); //need'['
					}
				}
				break;
				}// switch
			}
		} else if (sym == SYM_NUMBER) {
			if (num > MAXADDRESS) {
				error(25); // The number is too great.
				num = 0; //number????????0
			}
			itoa(num,mk->name,10);
			mk->evl=createevl(0,0);
			gen(LIT, 0, num);
			getsym();
		} else if (sym == SYM_LPAREN) //(expression)
				{
			getsym();
			set = uniteset(createset(SYM_RPAREN, SYM_NULL), fsys);
			//expression(set);
			strcpy(mk->name,"("); //zq
			mask *mk2=expression(set);
			strcat(mk->name,mk2->name);
			mk->evl=uniteevl(mk->evl,mk2->evl);
			strcat(mk->name,")");
			destroyset(set);
			if (sym == SYM_RPAREN) {
				getsym();
			} else {
				error(22); // Missing ')'.
			}
		}
		//test(fsys, createset(SYM_LPAREN, SYM_NULL), 23);
	} // while
	
	return mk;
} // factor

//////////////////////////////////////////////////////////////////////
mask * term(symset fsys)
{
	int saveCx=cx;
	mask *mk=(mask*)malloc(sizeof(mask));
	mk->evl=createevl(0,0);
	int mulop;
	symset set;
	
	set = uniteset(fsys, createset(SYM_TIMES, SYM_SLASH, SYM_NULL));
	
	mask *mk2=factor(set);
	strcpy(mk->name,mk2->name);
	mk->evl=uniteevl(mk->evl,mk2->evl);
	while (sym == SYM_TIMES || sym == SYM_SLASH)
	{
		mulop = sym;
		if (sym==SYM_TIMES)
			strcat(mk->name,"*");
		else
			strcat(mk->name,"/");
			
		getsym();
		mask *mk2=factor(set);
		strcat(mk->name,mk2->name);
		mk->evl=uniteevl(mk->evl,mk2->evl);
		if (mulop == SYM_TIMES)
		{
			gen(OPR, 0, OPR_MUL);
		}
		else
		{
			gen(OPR, 0, OPR_DIV);
		}
	} // while
	destroyset(set);
	if (OPTM)
	  optimize_term(mk,saveCx);
	return mk;
} // term

//////////////////////////////////////////////////////////////////////

mask* expression(symset fsys)
{
	int saveCx=cx;
	mask *mk=(mask*)malloc(sizeof(mask));
	mk->evl=createevl(0,0);
	int addop;
	symset set;

	set = uniteset(fsys, createset(SYM_PLUS, SYM_MINUS, SYM_NULL));
	if (sym == SYM_PLUS || sym == SYM_MINUS)
	{
		if (sym==SYM_PLUS)
			strcpy(mk->name,"+");
		else
			strcpy(mk->name,"-");
		addop = sym;
		getsym();
		mask *mk2=term(set);
		strcat(mk->name,mk2->name);
		mk->evl=uniteevl(mk->evl,mk2->evl);
		if (addop == SYM_MINUS)
		{
			gen(OPR, 0, OPR_NEG);
		}
	}
	else
	{
		mask *mk2=term(set);
		strcpy(mk->name,mk2->name);
		mk->evl=uniteevl(mk->evl,mk2->evl);
	}

	while (sym == SYM_PLUS || sym == SYM_MINUS)
	{
		optimize(mk,saveCx);
		if (sym==SYM_PLUS)
			strcat(mk->name,"+");
		else
			strcat(mk->name,"-");
		addop = sym;
		getsym();
		mask *mk2=term(set);
		strcat(mk->name,mk2->name);
		mk->evl=uniteevl(mk->evl,mk2->evl);
		if (addop == SYM_PLUS)
		{
			gen(OPR, 0, OPR_ADD);
		}
		else
		{
			gen(OPR, 0, OPR_MIN);
		}
	} // while

	destroyset(set);
	//printf("mk->name: %s\n",mk->name);
	//printevl(mk->evl);
	optimize(mk,saveCx);
	
	
	return mk;
} // expression
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// expression

//////////////////////////////////////////////////////////////////////
/*
 void condition(symset fsys)
 {
 int relop;
 symset set;

 if (sym == SYM_ODD)
 {
 getsym();
 expression(fsys);
 gen(OPR, 0, 6);
 }
 else
 {
 set = uniteset(relset, fsys);
 expression(set);
 destroyset(set);
 if (! inset(sym, relset))
 {
 error(20);
 }
 else
 {
 relop = sym;
 getsym();
 expression(fsys);
 switch (relop)
 {
 case SYM_EQU:
 gen(OPR, 0, OPR_EQU);
 break;
 case SYM_NEQ:
 gen(OPR, 0, OPR_NEQ);
 break;
 case SYM_LES:
 gen(OPR, 0, OPR_LES);
 break;
 case SYM_GEQ:
 gen(OPR, 0, OPR_GEQ);
 break;
 case SYM_GTR:
 gen(OPR, 0, OPR_GTR);
 break;
 case SYM_LEQ:
 gen(OPR, 0, OPR_LEQ);
 break;
 } // switch
 } // else
 } // else
 } // condition
 */
void con_factor(symset fsys)
{
	void condition(symset fsys);
	int i;
	symset set,set1;
	
	
	symset con_facbegsys=uniteset(facbegsys,createset(SYM_ODD,SYM_NOT,SYM_LPAREN,SYM_NULL));
	test(con_facbegsys,fsys,29); // the symbol can not be as the beginning of the condition
	
	while (inset(sym,con_facbegsys)) //conbeg
	{
		if (sym==SYM_LPAREN)
		{
			getsym();
			set1=createset(SYM_RPAREN,SYM_NULL);
			set=uniteset(set1,fsys);			
			condition(set);
			destroyset(set);
			destroyset(set1);
			if (sym==SYM_RPAREN)
			{
				getsym();
			}
			else
			{
				error(22); // Missing ')'
			}
		}
		else if (sym==SYM_ODD || sym==SYM_NOT)
		{
			int saveSym=sym;
			getsym();
			condition(fsys);
			if (saveSym==SYM_ODD)
				gen(OPR,0,OPR_ODD);
			else
				gen(OPR,0,OPR_NOT);
		}
		else
		{
			set=uniteset(relset,fsys);
			expression(fsys);
			destroyset(set);
			if (!inset(sym,relset))
			{
				//error(20);
				test(fsys,phi,20);
			}
			else
			{
				int relop=sym;
				getsym();
				expression(fsys);
				switch (relop)
				{
				case SYM_EQU:
					gen(OPR, 0, OPR_EQU);
					break;
				case SYM_NEQ:
					gen(OPR, 0, OPR_NEQ);
					break;
				case SYM_LES:
					gen(OPR, 0, OPR_LES);
					break;
				case SYM_GEQ:
					gen(OPR, 0, OPR_GEQ);
					break;
				case SYM_GTR:
					gen(OPR, 0, OPR_GTR);
					break;
				case SYM_LEQ:
					gen(OPR, 0, OPR_LEQ);
					break;
				} // switch
			}
					
		}
	}
	
}

void con_term(symset fsys)
{
	int saveCx[MAXLEVEL];
	int k=0;
	symset set,set1;
	set1=createset(SYM_AND,SYM_NULL);
	set=uniteset(fsys,set1);
	con_factor(set);
	saveCx[k++]=cx;
	gen(JPC,0,0);
	while (sym==SYM_AND){
		inblock();
		getsym();
		con_factor(set);	
		saveCx[k++]=cx;
		gen(JPC,0,0);
	}
	destroyset(set);
	destroyset(set1);
	gen(LIT,0,1);
	int cx0=cx;
	gen(JMP,0,0);
	while (k>0)
		code[saveCx[--k]].a=cx;
	gen(LIT,0,0);
	code[cx0].a=cx;
}

void condition(symset fsys)
{
	int saveCx[MAXLEVEL];
	int k=0;
	symset set,set1;
	set1=createset(SYM_OR,SYM_NULL);
	set=uniteset(set1,fsys);
	con_term(set);
	gen(OPR,0,OPR_NOT);
	saveCx[k++]=cx;
	gen(JPC,0,0);
	while (sym==SYM_OR)
	{
		inblock();// 
		getsym();
		con_term(set);
		gen(OPR,0,OPR_NOT);
		saveCx[k++]=cx;
		gen(JPC,0,0);
	}
	destroyset(set1);
	destroyset(set);
	gen(LIT,0,0);
	int cx0=cx;
	gen(JMP,0,0);
	while (k>0)
		code[saveCx[--k]].a=cx;
	gen(LIT,0,1);
	code[cx0].a=cx;
}
//////////////////////////////////////////////////////////////////////
void statement(symset fsys)
{
    int i, cx1, cx2;
    symset set1, set;


    if (sym == SYM_IDENTIFIER)   // variable assignment
    {
        mask* mk;
        i = position(id);
        array* ar = (array*) &table[i];
        if (!i)
        {
            error(11); // Undeclared identifier.
            getsym();
        }
        else if ((table[i].kind != ID_VARIABLE) && (ar->kind != ID_ARRAY))
        {
            error(12); // Illegal assignment.
            i = 0;
        }
        getsym();
        if ((ar->kind == ID_ARRAY))   
        {
            int dl = 0;
            p_dim* p = ar->next;
            if (sym == SYM_LSQUARE)
            {
                gen(LIT, 0, 0);
                gen(LIT, 0, p->dim_len);
                while (sym == SYM_LSQUARE)   //cy_array
                {
                    if (p)   //cy_array
                    {
                        p = p->next;
                    }
                    gen(OPR, 0, OPR_MUL);
                    getsym();
                    expression(uniteset(fsys, createset(SYM_COMMA, SYM_NULL)));
                    dl++; //cy_array
                    if (sym == SYM_RSQUARE)
                    {
                        getsym();
                    }
                    else
                    {
                        error(27);
                    }
                    gen(OPR, 0, OPR_ADD);
                    if (p)
                    {
                        gen(LIT, 0, p->dim_len);
                    }
                }
                if (sym != SYM_BECOMES)   //cy_array
                {
                    set1 = createset(SYM_BECOMES, SYM_NULL);
                    test(set1, fsys, 0);
                    destroyset(set1);
                }
                if (dl != ar->dim_n)   //cy_array
                {
                    error(26);
                }
            }
            else
            {
                error(28); //need'['
            }
        } 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*if (sym == SYM_BECOMES)
        {
            getsym();
        }
        else
        {
            error(13); // ':=' expected.
        }
        expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
        mk = (mask*) &table[i];
        if (i)
        {
            if ((table[i].kind == ID_VARIABLE))
            {
                gen(STO, level - mk->level, mk->address); //�洢ָ��
                mk->cnt++;
            }
            else
            {
                gen(STA, level - mk->level, mk->address); //������ָ��
                mk->cnt++;
            }
        }*/
         if (i)
        {
            if ((table[i].kind == ID_VARIABLE))
            {
               if (sym == SYM_BECOMES) 
			   {
			   	getsym();
			   	expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
                mk = (mask*) &table[i];
                gen(STO, level - mk->level, mk->address); 
			   }
			   else if(sym==SYM_ADDEQU)
		          {
		          	
		          	getsym();
			   	    expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
                    mk = (mask*) &table[i];
			        gen(LOD,level-mk->level,mk->address);
			        gen(OPR,0,OPR_ADD);
			        gen(STO,level-mk->level,mk->address);
		          }
		        else if(sym==SYM_MULEQU)
		         {
		           getsym();
			   	   expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
                   mk = (mask*) &table[i];
		   	       gen(LOD,level-mk->level,mk->address);
			       gen(OPR,0,OPR_MUL);
			       gen(STO,level-mk->level,mk->address);
		         }
		         else if(sym==SYM_SUBEQU)
		         {
		           getsym();
			   	   expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
                   mk = (mask*) &table[i];
			       gen(LOD,level-mk->level,mk->address);
			       gen(OPR,0,OPR_MIN);
			       gen(OPR,0,OPR_NEG);
			       gen(STO,level-mk->level,mk->address);
		         }
		         else if(sym==SYM_DIVEQU)
		        {
		           getsym();
		           mk = (mask*) &table[i];
		   	       gen(LOD,level-mk->level,mk->address);
		   	       expression(uniteset(fsys,createset(SYM_SEMICOLON,SYM_NULL)));
			       gen(OPR,0,OPR_DIV);
			       gen(STO,level-mk->level,mk->address);
		        }
		        else if(sym==SYM_MODEQU)
		       {
		   	       getsym();
		   	       mk = (mask*) &table[i];
		   	       gen(LOD,level-mk->level,mk->address);
		   	       expression(uniteset(fsys,createset(SYM_SEMICOLON,SYM_NULL)));
			       gen(OPR,0,OPR_MOD);
			       gen(STO,level-mk->level,mk->address);
		   	   }
		   	   else if(sym==SYM_ADDADD)
		       {
			    getsym();
			    mk = (mask*) &table[i];
			    gen(LOD,level-mk->level,mk->address);
			    gen(LIT,0,1);
			    gen(OPR,0,OPR_ADD);
				gen(STO,level-mk->level,mk->address);
			   }
			   else if(sym==SYM_SUBSUB)
		       {
			     getsym();
			     mk = (mask*) &table[i];
			     gen(LOD,level-mk->level,mk->address);
			     gen(LIT,0,1);
			     gen(OPR,0,OPR_MIN);
				 gen(STO,level-mk->level,mk->address);
			      
		       }
			   else error(13);
                
            }
            else
            {
               if (sym == SYM_BECOMES)
			   {
			   	 getsym();
			   	 expression(uniteset(fsys, createset(SYM_SEMICOLON, SYM_NULL)));
                 mk = (mask*) &table[i];
                 gen(STA, level - mk->level, mk->address);
			   }
			   
               else error(13); // ':=' expected.
                // 
            }
        }
    } // if sym==SYM_IDENTIFIER
    else if (sym == SYM_EXIT)
    {
        getsym();
        if (sym==SYM_LPAREN)
        {
            getsym();
            expression(uniteset(fsys,createset(SYM_RPAREN,SYM_NULL)));
            //gen(STO,1,dx[level-1]-1);
            gen(STO,0,-zx[level-1]); //////////////////
            if (sym==SYM_RPAREN)
            {
                getsym();
            }
            else
            {
                error(22);
            }
        }
        gen(OPR, 0, OPR_RET); // return
    }
    else if (sym == SYM_BREAK) //cy
    {
        if (cxbreak.sign)
        {
            cxbrklink pp;
            if (cxbreak.flag)
            {
                pp = cxbreak.then;
                while (pp->next)
                {
                    pp = pp->next;
                }
                pp->next = (cxbrklink) malloc(sizeof(struct cxlink));
                pp = pp->next;
                pp->next = NULL;
            }
            else
            {
                cxbreak.flag = 1; //置有�?
                pp = (cxbrklink) malloc(sizeof(struct cxlink));
                cxbreak.then = pp;
                pp->next = NULL;
            }
            pp->cxbrk = cx;
            gen(JMP, 0, 0);
            getsym();
        }
        else
        {
            error(35);
            if (sym == SYM_SEMICOLON)
            {
                getsym();
            }
        }
////////////////////////////////////////////////////////////
    }
    else if (sym == SYM_CALL)     // procedure call
    {
        getsym();
        if (sym != SYM_IDENTIFIER)
        {
            error(14); // There must be an identifier to follow the 'call'.
        }
        else
        {
            if (!(i = position(id)))
            {
                error(11); // Undeclared identifier.
                getsym();
            }
            else if (table[i].kind == ID_PROCEDURE)
            {
                mask* mk;
                mk = (mask*) &table[i];

                getsym();
                if (sym == SYM_LPAREN)
                {
                    getsym();
                    if (sym == SYM_RPAREN)
                    {
                        if (mk->numOfPar == 0)
                            gen(CAL_OP, level - mk->level, mk->address);
                        else
                            error(34); // the number of segement doesn't match
                        getsym();
                        goto ff2;
                    }

                    else
                    {
                        int k = 0;
                        do
                        {

                            if (sym == SYM_COMMA)
                                getsym();
                            k++;
                            set1 = createset(SYM_RPAREN, SYM_NULL);
                            set = uniteset(set1, fsys);
                            expression(set);
                            destroyset(set);
                            destroyset(set1);
                            if (k > mk->numOfPar)
                            {
                                error(34); //too many segments
                                break;
                            }
                        }
                        while (sym == SYM_COMMA);

                        if (sym == SYM_RPAREN)
                        {
                            if (mk->numOfPar == k)
                                gen(CAL_OP, level - mk->level, mk->address);
                            else
                                error(34); //error "didn't match"
                            getsym();
                            goto ff2;
                        }
                        else
                        {
                            error(22); //')' missing
                            goto ff2;
                        }
                    }
                }
                else
                {
                    if (mk->numOfPar == 0)
                        gen(CAL_OP, level - mk->level, mk->address);
                    else
                        error(34); //didn't match
                    goto ff2;

                }
            }
            else
            {
                error(15); // A constant or variable can not be called.
            }
            getsym();
ff2:		;
            mask* mk;
            mk = (mask*) &table[i];
            gen(INT_OP,0,-mk->numOfPar);
        }

    }
    else if (sym == SYM_REPEAT)
    {
	    int saveBlkNum=block_num;
	    int saveBlkLvl=block_level;
        set1 = createset(SYM_UNTIL, SYM_SEMICOLON, SYM_NULL);
        set = uniteset(set1, fsys);
        cx1 = cx;
        getsym();

        cxb cxbsaved = cxbreak; //cy
        cxbreak.flag = 0; //cy
        cxbreak.sign = 1; //cy
        cxbreak.then = NULL; //cy
        statement(set1);
        if (sym != SYM_SEMICOLON)
        {
            error(10); // "';' expected."
        }
        else
        {
            getsym();
            if (sym == SYM_UNTIL)
            {
                getsym();

                condition(set);
                gen(JPC, 0, cx1);

                if (cxbreak.flag)   //cy
                {
                    cxbrklink p = cxbreak.then;
                    while (p)
                    {
                        code[p->cxbrk].a = cx;
                        cxbrklink q = p;
                        free(p);
                        p = q->next;
                    }

                }
                cxbreak.flag = cxbsaved.flag; //cy
                cxbreak.then = cxbsaved.then; //cy
                cxbreak.sign = cxbsaved.sign; //cy

            }
            else
                error(31); //missing repeat
            
	        outblock(saveBlkNum,saveBlkLvl);
        }
    }
//test(fsys, phi, 19);
    else if (sym == SYM_IF) 
    {
	    int saveBlkNum=block_num;
	    int saveBlkLvl=block_level;
        getsym();
        set1 = createset(SYM_THEN, SYM_NULL); //modified by ZQ
        set = uniteset(set1, fsys);
        condition(set);
        inblock();
        destroyset(set1);
        destroyset(set);
        if (sym == SYM_THEN)
            getsym();
        else
            error(16); // 'then' expected.
        cx1 = cx;
        gen(JPC, 0, 0);
        statement(uniteset(fsys, createset(SYM_ELSE, SYM_NULL)));

        if (sym == SYM_ELSE)   
        {
            cx2 = cx; 
            gen(JMP, 0, 0); 
            getsym();
            code[cx1].a = cx; 
            statement(fsys);
            code[cx2].a = cx; 
        }
        else    
        {
            code[cx1].a = cx; 
        }
        
	    outblock(saveBlkNum,saveBlkLvl);
    }
    else if (sym == SYM_BEGIN)     // block
    {
        getsym();
        if (sym==SYM_VAR)  {
            getsym();
            do
            {
                //varnum++;
                vardeclaration();
                while (sym == SYM_COMMA)
                {
                    getsym();
                    //varnum++;
                    vardeclaration();
                }
                if (sym == SYM_SEMICOLON)
                {
                    getsym();
                }
                else
                {
                    error(5); // Missing ',' or ';'.
                }
            }  while (sym == SYM_IDENTIFIER);
            
    	};

        set1 = createset(SYM_SEMICOLON, SYM_END, SYM_NULL);
        set = uniteset(set1, fsys);
        statement(set);
        while (sym == SYM_SEMICOLON || inset(sym, statbegsys))
        {
            if (sym == SYM_SEMICOLON)
            {
                getsym();
            }
            else
            {
                error(10); 
            }
            statement(set);
        } // while
        destroyset(set1);
        destroyset(set);
        if (sym == SYM_END)
        {
            getsym();
        }
        else
        {
            error(17); // ';' or 'end' expected.
        }
    }
    else if (sym == SYM_WHILE)     // while statement
    {
	    int saveBlkNum=block_num;
	    int saveBlkLvl=block_level;
        cx1 = cx;
        getsym();
        set1 = createset(SYM_DO, SYM_NULL);
        set = uniteset(set1, fsys);
        condition(set);
        inblock();
        destroyset(set1);
        destroyset(set);
        cx2 = cx;
        gen(JPC, 0, 0);
        if (sym == SYM_DO)
        {
            getsym();
        }
        else
        {
            error(18); // 'do' expected.
        }

        cxb cxbsaved = cxbreak; //cy
        cxbreak.flag = 0; //cy
        cxbreak.sign = 1; //cy
        cxbreak.then = NULL; //cy
        statement(fsys);
        gen(JMP, 0, cx1);
        code[cx2].a = cx;
	    outblock(saveBlkNum,saveBlkLvl);
        if (cxbreak.flag)   //cy
        {
            cxbrklink p = cxbreak.then;
            while (p)
            {
                code[p->cxbrk].a = cx;
                cxbrklink q = p;
                //free(p);
                p = q->next;
            }
            cxbreak.then = NULL;
        }
        cxbreak.flag = cxbsaved.flag; //cy
        cxbreak.then = cxbsaved.then; //cy
        cxbreak.sign = cxbsaved.sign; //cy
    }
    else if (sym == SYM_WRITE || sym == SYM_WRITELN)   //added by zq
    {
        int saveSym = sym;
        getsym();
        
        if (sym == SYM_LPAREN)
        {
            getsym();
        }
        else
        {
            error(33); //'(' expected
        }
        
        if (sym!=SYM_RPAREN){
        	
			
	        
	        set1 = createset(SYM_COMMA, SYM_RPAREN, SYM_NULL);
	        set = uniteset(set1, fsys);
	        expression(set);
	        destroyset(set1);
	        destroyset(set);
	        gen(OPR, 0, OPR_PRT); //what about the previous expression didn't make a propriate value
	
	        while (sym == SYM_COMMA)
	        {
	            getsym();
	            set1 = createset(SYM_RPAREN, SYM_NULL);
	            set = uniteset(set1, fsys);
	            expression(set);
	            destroyset(set1);
	            destroyset(set);
	            gen(OPR, 0, OPR_PRT);
	        }
		}
        if (sym == SYM_RPAREN)
        {
            getsym();
            if (saveSym == SYM_WRITELN)
            {
                gen(OPR, 0, OPR_WTL); // new line
            }
        }
        else
        {
            error(22); //"Missing ')'."
        }

    }
    else if (sym == SYM_READ)   //added by zq
    {
        getsym();
        if (sym == SYM_LPAREN)
        {
            getsym();
        }
        else
        {
            error(33); //'(' expected
        }
        //set1=createset(SYM_COMMA,SYM_PAREN,SYM_NULL);
        //set=uniteset(set1,fsys);
        if (sym == SYM_IDENTIFIER)
        {
            if ((i = position(id)) == 0)
            {
                error(11); //Undeclared identifier
                getsym();
            }
            else
            {
                switch (table[i].kind)
                {
                    mask *mk;
                case ID_CONSTANT:
                    error(12); //Illegal assignment
                    break;
                case ID_PROCEDURE:
                    error(12);
                    break;
                case ID_VARIABLE:
                    mk = (mask*) &table[i];
                    gen(OPR, 0, OPR_RED);
                    gen(STO, level - mk->level, mk->address);
					mk->cnt++;
					break;
                }
            }
        }
        else
        {
            error(19); //incorrect symbol
        }
        getsym();
        while (sym == SYM_COMMA)
        {
            getsym();
            if (sym == SYM_IDENTIFIER)
            {
                if ((i = position(id)) == 0)
                {
                    error(11);
                    getsym();
                }
                else
                {
                    switch (table[i].kind)
                    {
                        mask *mk;
                    case ID_CONSTANT:
                        error(12);
                        break;
                    case ID_PROCEDURE:
                        error(12);
                        break;
                    case ID_VARIABLE:
                        mk = (mask*) &table[i];
                        gen(OPR, 0, OPR_RED);
                        gen(STO, level - mk->level, mk->address);
						mk->cnt++;
                        break;

                    }
                }
            }
            else
            {
                error(19);
            }
            getsym();
        }

        if (sym == SYM_RPAREN)
        {
            getsym();
        }
        else
        {
            error(22); //"Missing ')'."
        }

    }
    else if (sym == SYM_FOR)   
    {
    	
	    int saveBlkNum=block_num;
	    int saveBlkLvl=block_level;
        inblock();
        getsym();
        mask* mk;
        if (sym != SYM_IDENTIFIER)
            error(4); 
        i = position(id);
        mk = (mask*) &table[i];
        if (i == 0)
            error(11);
        else if (table[i].kind != ID_VARIABLE) //ASSIGNMENT TO NON-VARIABLE
            error(12); 
        getsym();
        if (sym != SYM_BECOMES) //:=
            error(13);
        getsym();
        set1 = createset(SYM_DOWNTO, SYM_DO, SYM_TO, SYM_NULL);
        set = uniteset(fsys, set1);
        expression(set); 
        destroyset(set1);
        destroyset(set);
        if (sym == SYM_DOWNTO)
        {
            getsym();
            cx1 = cx; 
            gen(STO, level - mk->level, mk->address); 
            mk->cnt++;
            gen(LOD, level - mk->level, mk->address);
            set1 = createset(SYM_DO, SYM_NULL);
            set = uniteset(fsys, set1);
            expression(set); 
            destroyset(set1);
            destroyset(set);
   
            gen(OPR, 0, OPR_GEQ);
            cx2 = cx; 
            gen(JPC, 0, 0);
            if (sym == SYM_DO)   
            {
                getsym();
                cxb cxbsaved = cxbreak; //cy
                cxbreak.flag = 0; 
                cxbreak.sign = 1; 
                cxbreak.then = NULL;
                ; 

                set1 = createset(SYM_SEMICOLON, SYM_NULL);
                set = uniteset(set1, fsys);
                statement(set);
                destroyset(set1);
                destroyset(set);
                gen(LOD, level - mk->level, mk->address);
                gen(LIT, 0, STEP); 
                gen(OPR, 0, OPR_MIN); 
                gen(JMP, 0, cx1);
                code[cx2].a = cx;
                if (cxbreak.flag)  
                {
                    cxbrklink p = cxbreak.then;
                    while (p)
                    {
                        code[p->cxbrk].a = cx;
                        cxbrklink q = p;
                        free(p);
                        p = q->next;
                    }

                }
                cxbreak.flag = cxbsaved.flag; //cy
                cxbreak.then = cxbsaved.then; //cy
                cxbreak.sign = cxbsaved.sign; //cy

            }
            else
                error(18); //do expected
        }
        else if (sym == SYM_TO)
        {
            getsym();
            cxb cxbsaved = cxbreak; 
            cxbreak.flag = 0; 
            cxbreak.sign = 1; //cy
            cxbreak.then = NULL;
            ; //cy
            cx1 = cx; 
            gen(STO, level - mk->level, mk->address);
			mk->cnt++;
            gen(LOD, level - mk->level, mk->address);
            set1 = createset(SYM_DO, SYM_NULL);
            set = uniteset(fsys, set1);
            expression(set);
            destroyset(set1);
            destroyset(set);
            gen(OPR, 0, OPR_LEQ); 
            cx2 = cx; 
            gen(JPC, 0, 0); 
            if (sym == SYM_DO)
            {
                getsym();
                set1 = createset(SYM_SEMICOLON, SYM_NULL);
                set = uniteset(set1, fsys);
                statement(set); 
                destroyset(set1);
                destroyset(set);
                gen(LOD, level - mk->level, mk->address); 
                gen(LIT, 0, STEP);
                gen(OPR, 0, OPR_ADD); 
                gen(JMP, 0, cx1); 
                code[cx2].a = cx; 
                if (cxbreak.flag)   //cy
                {
                    cxbrklink p = cxbreak.then;
                    while (p)
                    {
                        code[p->cxbrk].a = cx;
                        cxbrklink q = p;
                        free(p);
                        p = q->next;
                    }

                }
                cxbreak.flag = cxbsaved.flag; //cy
                cxbreak.then = cxbsaved.then; //cy
                cxbreak.sign = cxbsaved.sign; //cy

            }
            else
                error(18); //do expected
        }
        else
            error(30); //to or downto expected
        
	    outblock(saveBlkNum,saveBlkLvl);
    }
    test(fsys, phi, 19);
} // statement

//////////////////////////////////////////////////////////////////////
int search_var(int len, int from)   //cy_quote
{
    int count = 0;
    int arsize(int i);
    int i;
    for (i = tx - from; len > 0; i--, len--)
    {
        array* ar = (array*) &table[i];
        if (table[i].quote == 0)
        {
            if (ar->kind == ID_ARRAY)
            {
                mask *mk = (mask*) &table[i];
                count += arsize(i);
            }
            else if (ar->kind == ID_VARIABLE)
            {
                count++;
            }
        }
    }
    return count;
}

int search_pro(int n)   //cy_quote
{
    int count = 0;
    int con = (tx - n + 1);
    for (int i = tx - n + 1; n; i++, n--)
    {
        if (table[i].kind == ID_PROCEDURE)
        {
            if (table[i].quote == 0)
            {
                proth[count] = i - con;
                count++;

            }
        }
    }
    return count;
}

void cutcode(int start, int end)   //cy_quote
{
    int len = end - start;
    for (int c = start; c + len < cx; c++)
    {
        code[c].a = code[c + len].a;
        code[c].f = code[c + len].f;
        code[c].l = code[c + len].l;
        if ((code[c].f == JMP) || (code[c].f == JPC))
        {
            code[c].a -= len;
        }
        else if (code[c].f == CAL_OP)
        {
            if (code[c].a >= start)
            {
                code[c].a -= len;
            }
        }
    }
//printf("******************cx:%d\n", cx);
    cx -= len;
}

int arsize(int i)
{
    array *ar = (array*) &table[i];
    p_dim *p = ar->next;
    int c = 1;
    while (p)
    {
        c *= p->dim_len;
        p = p->next;
    }
    /*	for (int i = ar->dim_n; i; i--) {
     printf("i:%d",i);
     c *= p->dim_len;
     p=p->next;
     }*/
    return c;
}

int locate(int n, int star_tx, int end_tx, int f)
{
    int c = 0;
    for (int i = star_tx; i < end_tx; i++)
    {
        mask* mk = (mask*) &table[i];
        if (mk->address == n)
        {
            break;
        }
        if ((mk->quote == 0) && (table[i].kind != ID_CONSTANT))
        {
            if ((f == STA) || (f == LAD))
            {
                c += arsize(i);
            }
            else if ((f == STO) || (f == LOD))
            {
                c++;
            }
        }
    }
    return c;
}

void cutprovarcode(int star_cx, int end_cx, int star_tx, int end_tx)
{
    if (star_cx < end_cx)
    {
        int l = 0;
        for (int i = star_cx; i < end_cx; i++)
        {
            if (code[i].f == JMP)
            {
                if (code[code[i].a].f == INT_OP)
                {
                    l++;
                }
            }
            if ((code[i].f == OPR) && (code[i].a == OPR_RET))
            {
                if (code[i + 1].f == INT_OP)
                {
                    l--;
                }
                if (code[i + 1].f == JMP)
                {
                    i++;
                }
            }
            if (code[i].l == l)
            {
                if ((code[i].f == LOD) || (code[i].f == LAD)
                        || (code[i].f == STA) || (code[i].f == STO))
                {
                    //again locate the statue
                    code[i].a -= locate(code[i].a, star_tx, end_tx, code[i].f);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void block(symset fsys)
{
	int saveBlkNum=block_num;
	int saveBlkLvl=block_level;
    inblock();
    int cx0; // initial code index
    mask* mk;
    int savedTx;
    int var_n = 0; //cy_quote
    int pro_n = 0; //cy_quote
    symset set1, set;
    dx[level] = 3; //iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
    if (level == 0)
        mk = (mask*) &table[tx]; //mk -> procedure in the table
    else
        mk = (mask*) &table[tx - zx[level - 1]];
    mk->address = cx;
    int jmpcx = cx; //cy_quote
    gen(JMP, 0, 0);
    int varnum = 0; //cy_quote
    int pronum = 0; //cy_quote
    prolink* pro = NULL; //cy_quote
    tx_[level] = tx + 1;
    if (level > MAXLEVEL)
    {
        error(32); // There are too many levels.
    }
    do
    {
        if (sym == SYM_CONST)   // constant declarations
        {
            getsym();
            do
            {
                constdeclaration(fsys);
                while (sym == SYM_COMMA)
                {
                    getsym();
                    constdeclaration(fsys);
                    printf("OK\n");
                }
                if (sym == SYM_SEMICOLON)
                {
                    getsym();
                }
                else
                {
                    error(5); // Missing ',' or ';'.
                    //	printf("there\n");
                }
            }
            while (sym == SYM_IDENTIFIER);
        } // if
        if (sym == SYM_VAR)   // variable declarations
        {
            getsym();
            do
            {
                varnum++;
                vardeclaration();
                while (sym == SYM_COMMA)
                {
                    getsym();
                    varnum++;
                    vardeclaration();
                }
                if (sym == SYM_SEMICOLON)
                {
                    getsym();
                }
                else
                {
                    error(5); // Missing ',' or ';'.
                }
            }
            while (sym == SYM_IDENTIFIER);
//			block = dx;
        } // if
        prolink* head = NULL;
        while (sym == SYM_PROCEDURE)   // procedure declarations
        {
            pronum++; //cy_quote
            prolink* p = (prolink*) malloc(sizeof(prolink)); //cy_quote
            p->next = NULL;
            if (pro == NULL)   //cy_quote
            {
                pro = p;
            }
            else
            {
                head->next = p;
            }
            head = p;
            zx[level] = 0; //�������?
            getsym();
            if (sym == SYM_IDENTIFIER)
            {
                enter(ID_PROCEDURE);
                head->table_adr = tx;
                getsym();
            }
            else
            {
                error(4); // There must be an identifier to follow 'const', 'var', or 'procedure'.
            }

            if (sym == SYM_LPAREN) //added by zq
            {
                do
                {
                    getsym();
                    if (sym == SYM_IDENTIFIER)
                    {
                        //declare
                        enterPar();
                        getsym();
                    }
                    else if (sym == SYM_RPAREN)
                    {
                        /*break;*/
                    }
                    else
                    {
                        error(19); //incorrect symbol
                        getsym();
                    }
                }
                while (sym == SYM_COMMA);

                if (sym == SYM_RPAREN)
                {
                    modifyTable(zx[level]);
                }
                else
                {
                    error(22); //')' missing
                }
                getsym();
            }
            else
            {
            }

            if (sym == SYM_SEMICOLON)
            {
                getsym();
            }
            else
            {
                error(5); // Missing ',' or ';'.
            }
            level++;
            savedTx = tx;
            set1 = createset(SYM_SEMICOLON, SYM_NULL);
            set = uniteset(set1, fsys);
            head->start = cx; //cy_quote
            block(set);
            head->end = cx; //cy_quote
            destroyset(set1);
            destroyset(set);
            level--;
            tx = savedTx - zx[level]; //modified by zq.  need to substract the parameters.

            if (sym == SYM_SEMICOLON)
            {
                getsym();
                set1 = createset(SYM_IDENTIFIER, SYM_PROCEDURE, SYM_NULL);
                set = uniteset(statbegsys, set1);
                test(set, fsys, 6);
                destroyset(set1);
                destroyset(set);
            }
            else
            {
                error(5); // Missing ',' or ';'.
            }
        } // while
        set1 = createset(SYM_IDENTIFIER, SYM_NULL);
        set = uniteset(statbegsys, set1);
        test(set, declbegsys, 7);
        destroyset(set1);
        destroyset(set);
    }
    while (inset(sym, declbegsys));

    code[mk->address].a = cx;
    mk->address = cx;
    if (level == 0)
        mk->numOfPar = zx[level]; //added by zq
    else
        mk->numOfPar = zx[level - 1];
    cx0 = cx; //procedure enter address
    gen(INT_OP, 0, dx[level]);
    set1 = createset(SYM_SEMICOLON, SYM_END, SYM_NULL);
    set = uniteset(set1, fsys);
    statement(set);
    destroyset(set1);
    destroyset(set);
	
	code[cx0].a=dx[level];
	if (OPTM_CY)
	{
		varnum=dx[level]-2;
	    var_n = search_var(varnum, pronum); //cy_quote
		//int var_n2 = search_var(varnum, pronum, 2); //cy_quote
	    pro_n = search_pro(pronum); //cy_quote
	    if (var_n)
	    {
	        cutprovarcode(cx0, cx, tx_[level], tx - pro_n); //cy_quote
	        cutprovarcode(jmpcx + 1, cx0, tx_[level], tx - pro_n); //cy_quote
	    }
	    
	    code[cx0].a -= var_n; //cy_quote
	    if (pro_n)   //如果有无用的过程
	    {
	        prolink* q = pro;
	        int proth_formor = -1;
	        for (int k = 0; k < pro_n; k++)
	        {
	            for (int n = proth[k] - proth_formor; n != 1; n--)
	            {
	                q = q->next;
	            }
	            proth_formor = proth[k];
	            int codelen = q->end - q->start; //对应过程的长�?
	            for (prolink* qq = q->next; qq; qq = qq->next)
	            {
	                qq->start -= codelen;
	                qq->end -= codelen;
	                //			mask* mk = (mask*) &table[qq->table_adr]; //已被调用过程的table
	                //			mk->address -= codelen;
	            }
	            cutcode(q->start, q->end); //剪掉代码段，改变jmp\jpc\call
	            code[jmpcx].a -= codelen;
	            cx0 -= codelen; //cx0为第一条INT指令，相当于本block的statement的首地址
	        }
	    }
	}
    gen(OPR, 0, OPR_RET); // return
    test(fsys, phi, 8); // test for error: Follow the statement is an incorrect symbol.
    listcode(cx0, cx);
    printTable();
    
   	outblock(saveBlkNum,saveBlkLvl);
} // block

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
            case OPR_MOD:
            	  top--;
            	  stack[top]%=stack[top+1];
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

//////////////////////////////////////////////////////////////////////
void main(int argc,char *argv[])
{
    FILE* hbin;
    FILE* hasm;
    char s[80];
    int i;
    symset set, set1, set2;

    if (argc==1)
		strcpy(s,"input.pl");
	else
		strcpy(s,argv[1]);
	if ((infile = fopen(s, "r")) == NULL)
	{	
		printf("File %s can't be opened.\n", s);
		exit(1);
	}
    phi = createset(SYM_NULL);
    relset = createset(SYM_EQU, SYM_NEQ, SYM_LES, SYM_LEQ, SYM_GTR, SYM_GEQ,
                       SYM_NULL);

    // create begin symbol sets
    declbegsys = createset(SYM_CONST, SYM_VAR, SYM_PROCEDURE, SYM_NULL);
    statbegsys = createset(SYM_BEGIN, SYM_CALL, SYM_IF, SYM_WHILE, SYM_NULL);
    facbegsys = createset(SYM_IDENTIFIER, SYM_NUMBER, SYM_LPAREN, SYM_NULL);

    err = cc = cx = ll = 0; // initialize global variables
    ch = ' ';
    kk = MAXIDLEN;

    getsym();
    cxbreak.flag = 0; //cy
    cxbreak.sign = 0; //cy
    cxbreak.then = NULL; //cy
    set1 = createset(SYM_PERIOD, SYM_NULL);
    set2 = uniteset(declbegsys, statbegsys);
    set = uniteset(set1, set2);
    block(set);
    destroyset(set1);
    destroyset(set2);
    destroyset(set);
    destroyset(phi);
    destroyset(relset);
    destroyset(declbegsys);
    destroyset(statbegsys);
    destroyset(facbegsys);

    if (sym != SYM_PERIOD)
        error(9); // '.' expected.
    color(COLOR_GREEN);
    if (err == 0)
    {
    	char s1[80];
    	char s2[80];
    	if (argc<3) 
    		strcpy(s1,"a.txt");
    	else
    		strcpy(s1,argv[2]); 
    	if (argc<4)
    		strcpy(s2,"a.asm");
    	else
    		strcpy(s2,argv[2]);
        hbin = fopen(s1, "w");
        hasm = fopen(s2,"w");
        //fwrite(&cx,sizeof(cx),1,hbin);
        fprintf(hbin,"%d\n",cx);
        printf("cx=%d\n",cx);
        for (i = 0; i < cx; i++)
        {
            //fwrite(&code[i], sizeof(instruction), 1, hbin);
            fprintf(hasm,"%5d %s\t%d\t%d\n", i, mnemonic[code[i].f], code[i].l,code[i].a);
            
			fprintf(hbin,"%d %d %d\n",code[i].f,code[i].l,code[i].a);
        }

        fclose(hbin);
        fclose(hasm);
        printf(
            "==================Compiling is complished !==================\n");
    }
    if (err)
        printf("There are %d error(s) in PL/0 program.\n", err);
    printf("OPTM=%d,	OPTM_CY=%d\n",OPTM,OPTM_CY);
    color(COLOR_WHITE);
    //interpret();
    //system("interpret");
    //printf("%d\n",cx);
    //listcode(0,cx);
//	listcode(0, cx);
} // main

//////////////////////////////////////////////////////////////////////
// eof pl0.c
