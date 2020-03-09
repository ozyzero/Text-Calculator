#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//double atof(const char* s);//不用重复定义，存在于STDLIB.H
int Initialize();
int Arithtest(int Phy_index, int Log_index); /*参数1:物理下标 参数2:逻辑下标*/
int ConvertValue(int Phy_index, int Log_index); /*参数1:物理下标 参数2:逻辑下标*/
void clear(int n);
int JudgeSymbol(int data, int mode);	/*1:判定字符 2:判定运算符 3:判定数值*/

int Calculate();
int JudgePrio(int index, int* level, int* ins);			/*判断优先级*/
int JJud(int index, int* level, int* ins, int n, int bound);	/*反复退栈*/
void pop(int sta, int ins, int n);	/*出栈*/
void Cal(int index, int n);	/*运算*/
void ShowCal();			/*输出结果*/
void ShowList(int mode);/*1：输出算式 2：输出临时转换的字符串*/

double add(double a, double b);
double sub(double a, double b);
double mul(double a, double b);
double _div(double a, double b);
//double pow(double a, double b); //不用重复定义，存在于MATH.H

double(*funs[])(double, double) = { add,sub,mul,_div,pow };
double (**f)(double, double) = funs;

char Symbol[50] = { 0 };
double Value[50] = { 0 };	/*类型错误*/
char Stack[50] = { 0 };
char Arith[50] = { 0 };	/*临时存储*/

char str_symbol[9] = { "0+-*/^()" }; 
char str_value[12] = { "01234567890" };



/*========================*/
int Initialize() {
	int i = 0;
	int p = 0;

	gets_s(Arith);  
	if (Arith[0] == '-') {
		Symbol[0] = 1;
		Value[0] = 0;
		p++;
	}
	for (; i < 50; i++) {
		if (Arith[i] == '\0') break;
		if (JudgeSymbol(Arith[i], 1)) {			/*判定字符*/
			if (!Arithtest(i, p)) { printf("Arithtest error"); return 0; }
			if (Arith[i] == '-' && Symbol[p - 1] == '(') {
				Symbol[p] = 1;
				Value[p] = 0;
				p++;
			}
			Symbol[p] = Arith[i];
			p++;
			continue;
		}
		if (!JudgeSymbol(Arith[i], 3)) { printf("not Value1 error\n"); return 0; }	/*判定数值*/

		i = ConvertValue(i, p);
		if (i < 0) return 0;
		Symbol[p] = 1;	/*标志为1*/

		p++;
	}

	/*末位必须为数值*/
	if (Symbol[p - 1] != 1 && Symbol[p - 1] != ')') { printf("Value error\n"); return 0; }
	return p;
}
int Arithtest(int Phy_index, int Log_index) { /*参数1:物理下标 参数2:逻辑下标*/
	int i = Phy_index, p = Log_index;

	switch (Arith[i]) {
	case '(': {	/*左不能有数值 ，右不能有右括号运算符*/
		if (Symbol[p - 1] == 1) return 0;
		if (Arith[i + 1] == '-') return 1;
		if (JudgeSymbol(Arith[i + 1], 2) || Arith[i + 1] == ')') return 0;
		break;
	}
	case ')': {	/*左不能有运算符与左括号，右不能有数值*/
		if (JudgeSymbol(Symbol[p - 1], 2) && Symbol[p - 1] == '(') return 0;
		if (JudgeSymbol(Arith[i + 1], 3)) return 0;
		break;
	}
	default: {
		/*运算符连续*/
		if (JudgeSymbol(Arith[i + 1], 2)) return 0;

	}
	}
	return 1;
}
int ConvertValue(int Phy_index, int Log_index) { /*参数1:物理下标 参数2:逻辑下标*/
	int i = Phy_index;
	int p = Log_index;
	int n = 0, kp = 0;
	for (; i < 50; i++) {
		if (!JudgeSymbol(Arith[i], 3) && Arith[i] != '.') break;
		if (Arith[i] == '.') kp++;
		Stack[n++] = Arith[i];
	}

	if (kp > 1) return -1; /*小数点限制*/
	Value[p] = atof(Stack);

	clear(n);

	return i - 1;
}
void clear(int n) {
	int i;
	for (i = 0; i < n + 1; i++) {
		Stack[i] = 0;
	}
}
int JudgeSymbol(int data, int mode) {	/*1:判定字符 2:判定运算符 3:判定数值*/
	int i;
	switch (mode) {
	case 1: {
		for (i = 1; i < 8; i++) {
			if (data == str_symbol[i]) return i;
		}
		break;
	}
	case 2: {
		for (i = 1; i < 6; i++) {
			if (data == str_symbol[i]) return i;
		}
		break;
	}
	case 3: {
		for (i = 1; i < 11; i++) {
			if (data == str_value[i]) return i;
		}
		break;
	}
	default:
		printf(" error in mode \n");
	}
	return 0;
}

int Calculate() {
	int n = 0, level = 0;		/*n:逻辑数据总数 level：优先级*/

	int bracks[10] = { 0 };
	int brack = 0;
	int brack_level[10] = { 0 };

	int index = 0, ins = 0;		/*index:记录下标 ins:入栈数据量*/

	n = Initialize();

	if (n < 1) { printf("Initialize error\n"); return 0; }
	ShowList(1);

	for (; index < n; index++) {
		if (Symbol[index] == 1 || Symbol[index] == 0) continue;

		if (Symbol[index] == '(') {
			bracks[brack] = ins;
			brack++;
			continue;
		}
		if (Symbol[index] == ')') {
			/*左括号不存在*/
			if (!brack) { printf(" brack error \n"); return 0; }

			brack--;
			pop(bracks[brack], ins - 1, n);	/*释放当前括号栈*/
			ins = bracks[brack];
			brack_level[brack] = 0;		/*优先级置零*/
			continue;
		}
		if (brack) {
			JJud(index, &brack_level[brack - 1], &ins, n, bracks[brack - 1]);
			continue;
		}
		JJud(index, &level, &ins, n, 0);
	}
	if (brack) return 0;
	if (ins > 0) pop(0, ins - 1, n);

	ShowCal();
	return 1;
}
int JudgePrio(int index, int* level, int* ins) {			/*判断优先级*/
	int i = 0;
	i = JudgeSymbol(Symbol[index], 2);
	if (*level == 0 || i > * level) {	/*判定录入*/
		Stack[*ins] = index;
		(*ins)++;
		*level = i;
		return 1;
	}
	return 0;
}
int JJud(int index, int* level, int* ins, int n, int bound) {	/*反复退栈*/

	while (!JudgePrio(index, level, ins) && *ins > bound) {
		Cal(Stack[*ins - 1], n);
		Stack[*ins - 1] = index;
		(*ins)--;

		if (*ins == bound) {
			*level = 0;
			if (JudgePrio(index, level, ins)) return 1;
			return 0;
		}
		*level = JudgeSymbol(Symbol[Stack[*ins - 1]], 2);
	}
	*level = JudgeSymbol(Symbol[index], 2);
	return 1;
}
void pop(int sta, int ins, int n) {	/*出栈*/
	// printf("\n======pop=======\n"); //取消调试注释
	for (; sta <= ins; ins--) {
		Cal(Stack[ins], n);
	}
}
void Cal(int index, int n) {	/*运算*/
	int k = 0;
	double ros = 0;
	int lef = index - 1, rig = index + 1;

	while (Symbol[lef] != 1 && lef > 0) lef--;
	while (Symbol[rig] != 1 && rig < n - 1) rig++;

	k = JudgeSymbol(Symbol[index], 2) - 1;
	//printf("k=%d lef=%d rig=%d\n", k, lef, rig); //取消调试注释
	ros = f[k](Value[lef], Value[rig]);

	Symbol[index] = 1;
	Value[index] = ros;
	Symbol[lef] = 0;
	Symbol[rig] = 0;

}
void ShowCal() {			/*输出结果*/
	int i;
	for (i = 0; i < 50; i++) {
		if (Symbol[i] == 1) printf("=%f", Value[i]);
	}
}
void ShowList(int mode) { /*1：输出算式 2：输出临时转换的字符串*/
	int i;
	switch (mode) {
	case 1: {
		for (i = 0; i < 50; i++) {
			if (Symbol[i] == '\0') continue;
			if (Symbol[i] == 1) { printf("%f", Value[i]); }
			else printf("%c", Symbol[i]);
		}
		break;
	}
	case 3: {
		for (i = 0; i < 50; i++) {
			if (Stack[i] == '\0') break;
			else printf("%c", Stack[i]);
		}
		break;
	}
	default:
		break;
	}

}
double add(double a, double b) {
	//printf("\n 			a=%f add b=%f \n", a, b);//取消调试注释
	return a + b;
}
double sub(double a, double b) {
	//printf("\n 			a=%f sub b=%f \n", a, b)//取消调试注释
	return a - b;

}
double mul(double a, double b) {
	//printf("\n 			a=%f mul b=%f \n", a, b);//取消调试注释
	return a * b;
}
double _div(double a, double b) {
	if (b < 0) printf("div error");
	return a / b;
}
