#include <stdio.h>
#include <setjmp.h>

jmp_buf begin;

char curlex;

void getlex(void);

int expr(void);

int add(void);

int mult(void);

int stepen(void);

void error();

int main()
{
	int result;
	setjmp(begin);
	printf("==>");
	getlex();
	result = expr();
	if (curlex != '\n')
		error();
	printf("\n%d\n",result);
	return 0;
}

void getlex(void)
{
	while ((curlex = getchar()) == ' ');
}

void error(void)
{
	printf("\nОШИБКА\n");
	while (getchar() != '\n')
			longjmp(begin, 1);
}

int expr()
{
	int e = add();
	while (curlex =='+' || curlex=='-')
	{
		if(curlex == '+')
		{
			getlex();
			e = e + add();
		}
		else
		{
			getlex();
			e = e - add();
		}
	}
	return e;
}

int add()
{
	int st = stepen();
	int dv, md;
	while (curlex == '*' || curlex == '/')
	{
		if (curlex == '*')
		{
			getlex();
			st = st*stepen();
		}
		else
		{
			getlex();
			st = st/stepen();
		}
	}
	return st;
}

int pwr(int a, int b)
{
	int r = a;
	if (b<0)
	{
		error();
		longjmp(begin, 1);
	}
	if (b == 0)
		return 1;
	--b;
	while (b>0)
	{
		r *= a;
		--b;
	}
	return r;
}

int stepen()
{
	int s = mult();
	while (curlex == '^')
	{
		getlex();
		int stp = stepen();
		s = pwr(s, stp);
	}
	return s;
}


int mult()
{
	int m;
	switch(curlex)
	{
		case '0':case'1':case'2':case '3':case'4':case'5':
		case '6':case'7':case'8':case'9': m = curlex - '0'; break;
		case '(':getlex(); m = expr();
		if (curlex == ')') break;
		default : error();
	}
	getlex();
	return m;
}
