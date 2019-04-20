#ifndef __JSON_H
#define __JSON_H

typedef struct JSON * JSON;

#define IJSON 					struct JSON_Interface
#define JSON_GET(_0, _1)		_1 (*_0)(const char *)
#define JSON_INTERFACE(_0)		\
	_0(RAW,		JSON);			\
	_0(ROOT, 	IJSON *); 		\
	_0(STRING,	char *);		\
	_0(INT,		int);			\
	_0(FLOAT,	float);			\
	_0(DOUBLE,	double);		\
	_0(BOOL,	int);			\
	_0(PRINT,  	void);

IJSON { struct { JSON_INTERFACE(JSON_GET) } get; };

JSON 		JSON_Parse(const char *);
JSON 		JSON_ParseFile(const char *);
void		JSON_Free(JSON *);
int 		JSON_Error();
char * 		JSON_ErrorMsg();
IJSON *		JSON_(JSON);

#define JSON(_0)				JSON_(_0)
#define JSON_ERROR				if(JSON_Error())
#define JSON_FREE(_0) 			JSON_Free(&(_0))

#endif // __JSON_H