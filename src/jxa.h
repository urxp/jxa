#ifndef __JXA_H
#define __JXA_H

#include <json.h>

#define cxjson_value 	struct JSON
#define nullptr 		(void *)0x0


typedef enum JSON_Type 			JSON_Type;
typedef enum JXA_Error 			JXA_Error;
typedef struct JXA_ParserPtr 	JXA_ParserPtr;

enum JSON_Type {

	JSON_FALSE, JSON_TRUE, 
	JSON_OBJECT, JSON_ARRAY, 
	JSON_STRING, JSON_NUMBER,
	JSON_NULL, JSON_BOOL
};

enum JXA_Error {

	NO_ERROR,
	EXPECTING_EOF,
	NOT_FOUND,
	CONVERSION_ERROR
};


struct JSON {

	char *			key;
	JSON_Type		type;
	void * 			data;
	JSON 			next;
};

struct JXA_ParserPtr {

	char * 			json_p;
	unsigned int 	line_number;
};

extern IJSON 		STATIC_JXA_INTERFACE;
extern JSON			STATIC_JXA_CURRENT;
extern JXA_Error	error_e;


void JXA_Init();
int JXA_ParseInt(char *);
JSON * jxa_Find(JSON *, const char *);


void printf_json(cxjson_value * json);
void jxa_free(JSON *);

void jxa_parser_error(JXA_Error, JXA_ParserPtr *);
void jxa_interface_error(JXA_Error, JSON_Type);

char * __jxa_strncpy(const char *, unsigned int);

#endif // __JXA_H