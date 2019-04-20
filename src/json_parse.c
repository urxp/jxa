#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jxa.h"



#define new_(dt)	calloc(0x1, sizeof(dt))


typedef enum cxjson_state		cxjson_state;


cxjson_state cxjson_parse_string(JXA_ParserPtr *, char **);
cxjson_state jxa_parse_value(JXA_ParserPtr *, cxjson_value **);




enum cxjson_state { CXJSON_INVALID, CXJSON_VALID };











void cxjson_trim(JXA_ParserPtr * cxjson_p){

	if(cxjson_p == nullptr || cxjson_p->json_p == nullptr) return;

loc_next:

	switch(*cxjson_p->json_p){
	case 0x0A:
		++(cxjson_p->line_number);
	case 0x09:
	case 0x0D: 
	case 0x20:
		++(cxjson_p->json_p);
		goto loc_next;		
	default:
		return;
	}
}





#define IS_VALID(op)	\
	if((op) == CXJSON_INVALID) return CXJSON_INVALID



cxjson_state cxjson_parse_colon(JXA_ParserPtr * cxjson_p){

	if(*cxjson_p->json_p != ':'){

		return CXJSON_INVALID;
	}


	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);

	return CXJSON_VALID;
}

cxjson_state cxjson_parse_pair(JXA_ParserPtr * cxjson_p, cxjson_value ** __cxjson){

	char * key;

	// parse key
	IS_VALID(cxjson_parse_string(cxjson_p, &key));

	// check for colon
	IS_VALID(cxjson_parse_colon(cxjson_p));

	// parse value
	IS_VALID(jxa_parse_value(cxjson_p, __cxjson));

	// assign key
	(*__cxjson)->key = key;

	return CXJSON_VALID;
}


cxjson_state cxjson_parse_object(JXA_ParserPtr * cxjson_p, cxjson_value ** __cxjson){

	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);

	if(*cxjson_p->json_p == '}') goto loc_return;

loc_loop:

	if(*cxjson_p->json_p == '"'){

		IS_VALID(cxjson_parse_pair(cxjson_p, __cxjson));

		if(*cxjson_p->json_p == '}') goto loc_return;

		if(*cxjson_p->json_p == ','){

			++(cxjson_p->json_p);
			cxjson_trim(cxjson_p);
			__cxjson = &(*__cxjson)->next;
			goto loc_loop;
		}

		return CXJSON_INVALID;
	}

	return CXJSON_INVALID;

loc_return:

	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);
	return CXJSON_VALID;
}


cxjson_state cxjson_parse_array(JXA_ParserPtr * cxjson_p, cxjson_value ** __cxjson){

	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);

	// empty array
	if(*cxjson_p->json_p == ']') goto loc_return;

	// non-empty array
loc_loop:

	// parse item
	IS_VALID(jxa_parse_value(cxjson_p, __cxjson));

	// end of array
	if(*cxjson_p->json_p == ']') goto loc_return;

	// next item
	if(*cxjson_p->json_p == ','){
	
		++(cxjson_p->json_p);
		__cxjson = &(*__cxjson)->next;
		goto loc_loop;
	}

	return CXJSON_INVALID;

loc_return:

	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);
	return CXJSON_VALID;
}


























cxjson_state cxjson_parse_const(JXA_ParserPtr * cxjson_p, const char * const_val){

	size_t strlen_const_val = strlen(const_val);

	if(!strncmp(cxjson_p->json_p, const_val, strlen_const_val)){

		cxjson_p->json_p += strlen_const_val;
		cxjson_trim(cxjson_p);

		return CXJSON_VALID;
	}

	return CXJSON_INVALID;
}


cxjson_state cxjson_parse_false(JXA_ParserPtr * cxjson_p){

	return cxjson_parse_const(cxjson_p, "false");
}

cxjson_state cxjson_parse_true(JXA_ParserPtr * cxjson_p){

	return cxjson_parse_const(cxjson_p, "true");
}

cxjson_state cxjson_parse_null(JXA_ParserPtr * cxjson_p){

	return cxjson_parse_const(cxjson_p, "null");
}






















#define CONTROL_CHAR											0x00:	\
	case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: 	\
	case 0x07: case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C:	\
	case 0x0D: case 0x0E: case 0x0F: case 0x10: case 0x11: case 0x12: 	\
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17: case 0x18:	\
	case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1E: 	\
	case 0x1F

#define HEX_DIGIT	'0':								\
	case '1': case '2': case '3': case '4':	case '5':	\
	case '6': case '7': case '8': case '9': case 'a':	\
	case 'b': case 'c': case 'd': case 'e': case 'f':	\
	case 'A': case 'B': case 'C': case 'D': case 'E': 	\
	case 'F'



cxjson_state cxjson_parse_string(JXA_ParserPtr * cxjson_p, char ** dst){

	char * string_start;
	size_t string_size;
	int u4hex;

	string_start = ++(cxjson_p->json_p);

loc_loop:

	switch(*cxjson_p->json_p){
	case CONTROL_CHAR:
		return CXJSON_INVALID;
	case 0x5C:				// '\\'
		++(cxjson_p->json_p);
		goto loc_escape;
	case 0x22:				// '"'
		goto loc_break;
	default:
		++(cxjson_p->json_p);
		goto loc_loop;
	}


loc_escape:
	
	switch(*cxjson_p->json_p){
	case 0x22:				// '"'
	case 0x5C:				// '\\'
	case 0x2F:				// '/'
	case 0x62:				// 'b'
	case 0x66:				// 'f'
	case 0x6E:				// 'n'
	case 0x72:				// 'r'
	case 0x74:				// 't'
		++(cxjson_p->json_p);
		goto loc_loop;
	case 0x75:				// 'u'
		++(cxjson_p->json_p);
		u4hex = 0;
		goto loc_4hex;
	default:
		return CXJSON_INVALID;
	}


loc_4hex:
	
	if(u4hex > 3) goto loc_loop;

	switch(*cxjson_p->json_p){
	case HEX_DIGIT:
		++(cxjson_p->json_p);
		++u4hex;
		goto loc_4hex;
	default:
		return CXJSON_INVALID;
	}
	

loc_break:

	string_size = cxjson_p->json_p - string_start;

	// copy string from buffer
	*dst = calloc(string_size + 0x1, sizeof(char));
	strncpy(*dst, string_start, string_size);

	++(cxjson_p->json_p);
	cxjson_trim(cxjson_p);

	return CXJSON_VALID;
}




































































#define DIGIT_0_9(d)		((d) >= 0x30 && (d) <= 0x39)
#define DIGIT_1_9(d)		((d) >= 0x31 && (d) <= 0x39)
#define DIGIT_0(d)			((d) == 0x30)
//#define EXP_e_E(d)		((d) == 0x45 || (d) == 0x65)
#define EXP_e_E(d)			(((d) | 0x20) == 0x65)
#define DEC_POINT(d) 		((d) == 0x2E)
#define MINUS(d) 			((d) == 0x2D)
#define PLUS_MINUS(d) 		((d) == 0x2D || (d) == 0x2B)



/*	char * __jxa_does_start_w_number(char * str)
*
*	checks if str starts with a valid JSON number
*
*	returns 
*		if doesn't: nullptr
		if does: pointer to the first character of str that isn't part of JSON number
*/

char * __jxa_does_start_w_number(char * str){

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




cxjson_state cxjson_parse_number(JXA_ParserPtr * cxjson_p, char ** dst){

	char * number_start = cxjson_p->json_p;
	char * number_end = __jxa_does_start_w_number(number_start);

	if(!number_end) return CXJSON_INVALID;

	*dst = __jxa_strncpy(number_start, number_end - number_start);

	cxjson_p->json_p = number_end;

	cxjson_trim(cxjson_p);

	return CXJSON_VALID;
}





















#define DIGIT	\
	'0': case '1': case '2': case '3': case '4':	\
	case '5': case '6': case '7': case '8': case '9'


cxjson_state jxa_parse_value(JXA_ParserPtr * cxjson_p, cxjson_value ** __cxjson){

	cxjson_trim(cxjson_p);

	// create new json value
	*__cxjson = new_(cxjson_value);
	
	switch(*cxjson_p->json_p){
	case '{':
		(*__cxjson)->type = JSON_OBJECT;
		return cxjson_parse_object(cxjson_p, (cxjson_value **)&(*__cxjson)->data);
	case '[':
		(*__cxjson)->type = JSON_ARRAY;
		return cxjson_parse_array(cxjson_p, (cxjson_value **)&(*__cxjson)->data);
	case '"':
		(*__cxjson)->type = JSON_STRING;
		return cxjson_parse_string(cxjson_p, (char **)&(*__cxjson)->data);
	case '-':
	case DIGIT:
		(*__cxjson)->type = JSON_NUMBER;
		return cxjson_parse_number(cxjson_p, (char **)&(*__cxjson)->data);
	case 'f':
		(*__cxjson)->type = JSON_FALSE;
		return cxjson_parse_false(cxjson_p);
	case 't':
		(*__cxjson)->type = JSON_TRUE;
		return cxjson_parse_true(cxjson_p);
	case 'n':
		(*__cxjson)->type = JSON_NULL;
		return cxjson_parse_null(cxjson_p);
	default:
		return CXJSON_INVALID;
	}
}





















































































void jxa_parser_error(JXA_Error e, JXA_ParserPtr * parser_p){

	error_e = e;
}



























JSON JSON_ParseFile(const char * file_json){

	JSON result;
	char * buffer = nullptr;

	long size_f;
	size_t elements_read;

	FILE * json_f = fopen(file_json, "rb");

	if(json_f){

		fseek(json_f, 0x0, 0x2);
		size_f = ftell(json_f);
		fseek(json_f, 0x0, 0x0);
		buffer = calloc(size_f + 0x1, sizeof(char));
		elements_read = fread(buffer, 0x1, size_f, json_f);
		fclose(json_f);
	}

	result = JSON_Parse(buffer);

	free(buffer);
	return result;
}


JSON JSON_Parse(const char * json_str){

	error_e = NO_ERROR;

	JXA_ParserPtr parser_p = {

		.json_p 		= (char *)json_str,
		.line_number 	= 1
	};

	JSON result;

	jxa_parse_value(&parser_p, &result);

	JSON_ERROR{

		jxa_free(&result);
		return nullptr;
	}	

	if(*(parser_p.json_p)){

		jxa_parser_error(EXPECTING_EOF, &parser_p);

		jxa_free(&result);
		return nullptr;
	}

	return result;
}