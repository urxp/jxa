#define DIGIT_0_9(d)		((d) >= 0x30 && (d) <= 0x39)
#define DIGIT_1_9(d)		((d) >= 0x31 && (d) <= 0x39)
#define DIGIT_0(d)			((d) == 0x30)
//#define EXP_e_E(d)		((d) == 0x45 || (d) == 0x65)
#define EXP_e_E(d)			(((d) | 0x20) == 0x65)
#define DEC_POINT(d) 		((d) == 0x2E)
#define MINUS(d) 			((d) == 0x2D)
#define PLUS_MINUS(d) 		((d) == 0x2D || (d) == 0x2B)

#define nullptr 			(void *)0x0

/*
char * __jxa_avalaible_number_a(char * str){

loc_Q1:

	if(MINUS(*str)) ++str;

	goto loc_Q2;

loc_Q2:

	if(DIGIT_0(*str)){

		++str;
		goto loc_Q3;
	}

	if(DIGIT_1_9(*str)){

		++str;
		goto loc_Q4;
	}

	goto loc_K3;

loc_Q3:

	if(*str == '.'){

		++str;
		goto loc_Q5;
	}

	goto loc_K1;

loc_Q4:

	if(DIGIT_0_9(*str)){

		++str;
		goto loc_Q4;
	}

	goto loc_Q3;

loc_Q5:

	if(DIGIT_0_9(*str)){

		++str;
		goto loc_Q6;
	}

	goto loc_K3;

loc_Q6:

	if(DIGIT_0_9(*str)){

		++str;
		goto loc_Q6;
	}

	goto loc_K1;

loc_Q7:

	if(PLUS_MINUS(*str)) ++str;

	goto loc_Q8;

loc_Q8:

	if(DIGIT_0_9(*str)){

		++str;
		goto loc_Q9;
	}

	goto loc_K3;

loc_Q9:

	if(DIGIT_0_9(*str)){

		++str;
		goto loc_Q9;
	}

	goto loc_K2;

loc_K1:

	if(EXP_e_E(*str)){

		++str;
		goto loc_Q7;
	}

	return str;

loc_K2:

	return str;

loc_K3:

	return nullptr;
}
*/

char * __jxa_FakeParseNumber(char * str){

	if(MINUS(*str)) ++str;

	if(DIGIT_0(*str)) ++str;
	else {

		if(DIGIT_1_9(*str)) ++str;
		else return nullptr;

		while(DIGIT_0_9(*str)) ++str;
	}

	if(DEC_POINT(*str)){

		++str;

		if(DIGIT_0_9(*str)) ++str;
		else return nullptr;

		while(DIGIT_0_9(*str)) ++str;
	}

	if(EXP_e_E(*str)){

		++str;

		if(PLUS_MINUS(*str)) ++str;

		if(DIGIT_0_9(*str)) ++str;
		else return nullptr;

		while(DIGIT_0_9(*str)) ++str;
	}

	return str;
}









#include <stdlib.h>
#include <string.h>

int jxa_QIsNumber(char * num_string){

	char * p = __jxa_FakeParseNumber(num_string);

	return p && !*p;
}

int jxa_QStartsWithNumber(char * num_string){

	return !!__jxa_FakeParseNumber(num_string);
}


char * __jxa_StringCopyUnsafe(char * src, size_t len){

	char * dst = calloc((len + 1) * sizeof(char), 0x1);
	while(len--) *(dst + len) = *(src + len);

	return dst;
}

char * jxa_ParseNumber(char * str){

	char * fakestr = __jxa_FakeParseNumber(str);
	
	return fakestr ? __jxa_StringCopyUnsafe(str, fakestr - str) : nullptr;
}



#include <stdio.h>

#define json_number(num) 					\
	printf("::: %15s - %d/%d ::: %s\n",		\
		num, 								\
		jxa_QIsNumber(num),					\
		jxa_QStartsWithNumber(num),			\
		jxa_ParseNumber(num))


int main(){

	json_number("1000");
	json_number("-1.567");
	json_number("m");
	json_number("-.5");
	json_number("-0");
	json_number("-m1000");
	json_number("-1000.8e5");
	json_number("1.-000");
	json_number("0e+");
	json_number("0e00000");
	json_number("50m");

	return 0;
}