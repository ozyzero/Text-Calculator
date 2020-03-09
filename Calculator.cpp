// 计算器.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

//存储res,即上一次计算结果

#include "Calculate.h"
#include<string.h>  //获取memset()函数，以清零

void Clears();

int main() {
	printf("以下输入算式：例如 1+1 ,支持加减乘除幂，可自己添加计算模块。\n");
	while (1) {
		if (!Calculate()) { printf("error"); }
		printf("\n\n");
		Clears(); //清空计算数组
	}
	return 0;
}

void Clears() {
	memset(Symbol, 0, 50);
	memset(Value, 0, 50);
	memset(Stack, 0, 50);
	memset(Arith, 0, 50);
}