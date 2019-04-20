#include "jxa.h"

#define JSON_GET_INIT(_0, _1) 		\
	STATIC_JXA_INTERFACE.get._0 = JSON_GET_ ## _0



void jxa_interface_error(JXA_Error e, JSON_Type type_e){

	error_e = e;
}


void JXA_SetCurrent(const char * jxl){

	JSON * current_p = jxa_Find(&STATIC_JXA_CURRENT, jxl);

	if(current_p) STATIC_JXA_CURRENT = *current_p;
}

int JXA_ParseInt(char * int_str){

	int sign = 1;
	int result = 0;

	if(*int_str == '-'){

		sign = -1;
		++int_str;
	}

	while(*int_str){

		result *= 10;
		result += *int_str - 0x30;
		++int_str;
	}

	return sign * result;
}

double JXA_ParseDouble(char * double_str){

	return 77.7;
}

float JXA_ParseFloat(char * float_str){

	return (float)JXA_ParseDouble(float_str);
}


char * JSON_GET_STRING(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return nullptr;

	if(STATIC_JXA_CURRENT->type == JSON_STRING) 	return STATIC_JXA_CURRENT->data;
	if(STATIC_JXA_CURRENT->type == JSON_NULL) 		return nullptr;
	
	jxa_interface_error(CONVERSION_ERROR, JSON_STRING);
	return nullptr;
}

JSON JSON_GET_RAW(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return nullptr;

	return STATIC_JXA_CURRENT;
}

IJSON * JSON_GET_ROOT(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return nullptr;

	return &STATIC_JXA_INTERFACE;
}

int JSON_GET_INT(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return 0;

	if(STATIC_JXA_CURRENT->type == JSON_NUMBER) return JXA_ParseInt(STATIC_JXA_CURRENT->data);

	jxa_interface_error(CONVERSION_ERROR, JSON_NUMBER);
	return 0;
}

float JSON_GET_FLOAT(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return 0.0f;

	if(STATIC_JXA_CURRENT->type == JSON_NUMBER) return JXA_ParseFloat(STATIC_JXA_CURRENT->data);

	jxa_interface_error(CONVERSION_ERROR, JSON_NUMBER);
	return 0.0f;
}

double JSON_GET_DOUBLE(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return 0.0;

	if(STATIC_JXA_CURRENT->type == JSON_NUMBER) return JXA_ParseDouble(STATIC_JXA_CURRENT->data);

	jxa_interface_error(CONVERSION_ERROR, JSON_NUMBER);
	return 0.0;
}

int JSON_GET_BOOL(const char * jxl){

	JXA_SetCurrent(jxl);

	JSON_ERROR return 0;

	if(STATIC_JXA_CURRENT->type == JSON_FALSE) 	return 0;
	if(STATIC_JXA_CURRENT->type == JSON_TRUE) 	return 1;

	jxa_interface_error(CONVERSION_ERROR, JSON_BOOL);
	return 0;
}

void JSON_GET_PRINT(const char * jxl){

	JXA_SetCurrent(jxl);

	printf_json(STATIC_JXA_CURRENT);
}

void JXA_Init(){

	JSON_INTERFACE(JSON_GET_INIT)
}