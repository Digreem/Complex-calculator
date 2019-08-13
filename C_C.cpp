//Complex number calculator, a symbolic expression is input
#include <iostream>
#include <math.h>

using namespace std;

#define PI 3.14159

class oper_stek //Stack class for operators
{
private:
	char oper[100];
	int size;
public:
	oper_stek() : size(0)
	{}
	void push(char);
	void print_stek();
	char pop();
	char last();
	int top()
	{
		return size;
	}
	int priority(char oper)
	{
		if (oper == '+' || oper == '-')
			return 0;
		if (oper == '*' || oper == '/')
			return 1;
	}
};
void oper_stek::push(char v)
{
	oper[size++] = v;
}
void oper_stek::print_stek()
{
	if (size > 0)
	{
		for (int i = size - 1; i >= 0; i--)
			cout << oper[i] << "  ";
		cout << endl;
	}
}
char oper_stek::pop()
{
	if (size > 0)
		return oper[--size];
}
char oper_stek::last()
{
	if (size > 0)
		return oper[size-1];
}

struct complex
{
	float re, im;
};

class num_stek  //Stack class for values
{
private:
	complex val[100];
	int size;
public:
	num_stek() : size(0)
	{}
	void push(complex);
	void print_stek();
	complex pop();
	complex last();
	int top()
	{
		return size;
	}
	complex inv(complex Z)
	{
		Z.im = -Z.im;
		Z.re = -Z.re;
		return Z;
	}
	void calc(char operation)
	{
		complex B = pop();
		complex A = pop();
		complex C;
		switch (operation)
		{ 
			case '+':
			{
				C.re = A.re + B.re;
				C.im = A.im + B.im;
			} break;
			
			case '-':
			{
				C.re = A.re - B.re;
				C.im = A.im - B.im;
			} break;
			case '*':
			{
				C.re = A.re*B.re - A.im*B.im;
				C.im = A.re*B.im + A.im*B.re;
			} break;
			case '/':
			{
				C.re = (A.re*B.re + A.im*B.im) / (B.im*B.im +B.re*B.re);
				C.im = (B.re*A.im - A.re*B.im) / (B.im*B.im + B.re*B.re);
			} break;
			
		}
		push(C);
	}
};

void num_stek::push(complex v)
{ val[size++] = v;}
complex num_stek::pop()
{
	if (size > 0)
		return val[--size];
}

 
class result //String parsing class
			//Calculates expression recursievly using stacks
{
private:
	complex res;
	char line[500];
	int i;
	float arg;
	float mod;
public:
	result() : i(0)
	{}
	void enter_line()
	{cin.get(line, 500);}
	void exp_form()
	{
		arg = atan(res.im / res.re) * 180 / PI;
		mod = sqrt(res.im*res.im + res.re*res.re);
	}
	complex get_res()
	{return res;}
	complex calc();
	void show_res()
	{
		char op;
		if (res.im >= 0)
			op = '+';
		else
			op = '-';
		cout <<'\n' << res.re <<  op << abs(res.im) << 'i'<<endl;
		cout << '\n' << "Exponential form is: " << mod << "exp(i" << arg << ')' << '\n' << endl;
	}
};

complex result::calc()
{
	bool fractional = false;
	float num;
	complex z;
	int k;
	num_stek S;
	oper_stek O;
	for (; line[i] != '\0'&& line[i] != ')'; i++)
	{
		if (line[i] >= '0'&& line[i] <= '9')
		{
			fractional = false;
			num = 0.0;
			z.im = 0;
			z.re = 0;
			k = 10;
			for (; line[i] >= '0'&& line[i] <= '9'; i++)
			{
				if (!fractional)
					num = num * 10 + static_cast<float>(line[i] - '0');
				else
				{
					num += static_cast<float>(line[i] - '0') / k;
					k *= 10;
				}
				if (line[i + 1] == '.')
				{
					i++;
					fractional = true;
				}	
			}
			if (O.top() > 0 && S.top() == 0)
			{
				if (O.pop() == '-')
					num =-num;
			}
			if (line[i] == 'i' || line[i] == 'j')
				z.im = num;
			else
				z.re = num;
			S.push(z);
			i--;
		}
		if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/')
		{
			if (O.top() > 0)
			{
				if (O.priority(line[i]) < O.priority(O.last()))
				{
					while (O.top() > 0)
						S.calc(O.pop());
				}
				if (O.priority(line[i]) == O.priority(O.last()))
				{
					S.calc(O.pop());
				}


			}
			O.push(line[i]);
		}
		if (line[i] == '(')
		{
			i++;
			if (O.top() > 0 && S.top() == 0)
			{
				if (O.pop() == '-')
					S.push(S.inv(calc()));
			}
			else
			S.push(calc());

		}
	}
	while (O.top() > 0)
		S.calc(O.pop());
	res = S.pop();
	exp_form();
	return res;
}

void main()
{
	result R;
	cout << "  Welcom to ULTIMATE COMPLEX CALCULATOR!!!" << endl;
	cout << "Now you can calculate any complex expression using operations +, -, *, /" << endl;
	cout  << "And do not forget about brackets! For example: (3.4+2i)/(5-8.93i) + 1+64i " << endl;
	cout << '\n' << "Enter line" << endl;
	R.enter_line();
	R.calc();
	cout << '\n' << " Result is:" << endl;
	R.show_res();
}