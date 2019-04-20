#include <stdlib.h>
#include <string.h>
#include "jxa.h"


void jxa_query_error(JXA_Error e, const char * query_p){

	error_e = e;
}

/*
	JXA_FindKey(JSON, "Errorcode");


	JSON ->	| key
			| type	(Object)
			| data	->	| key	("Errorcode")
			| next		| type
						| data	( ->	& ???) 
						| next 	->

	&(JSON->data->data)
	&(JSON->data->next->data)
*/

JSON * JXA_FindKey(JSON * _cxjson, const char * _key){

	JSON * _return = nullptr;
	
	if(!_cxjson || !(*_cxjson)) return nullptr;

	if((*_cxjson)->type == JSON_OBJECT){

		_return = (JSON *)&(*_cxjson)->data;

		while(*_return && strcmp((*_return)->key, _key)){

			_return = &(*_return)->next;
		}

		return *_return ? _return : nullptr;
	}

	return nullptr;
}

JSON * JXA_FindIndex(JSON * _cxjson, int _idx){

	int arr_idx = _idx;
	JSON * _return = nullptr;
	
	if(!_cxjson || !(*_cxjson)) return nullptr;

	if((*_cxjson)->type == JSON_ARRAY){

		_return = (JSON *)&(*_cxjson)->data;

		while(*_return && arr_idx){

			--arr_idx;
			_return = &(*_return)->next;
		}

		return *_return ? _return : nullptr;
	}

	return nullptr;
}






JSON * jxa_FindLoop(JSON * jxObject, char * jxPath){

	char * end;

find_loop:

	if(*jxPath == 0x0) return jxObject;

	if(jxObject == nullptr) return nullptr;


	if((*jxPath == '.') || (*jxPath == '{')){ 	// object key

		end = ++jxPath;

	loc_loop:
		if(*end == 0x0) return JXA_FindKey(jxObject, jxPath);
		if(*end == '\\'){

			if(*(end + 1) == '.') goto loc_end_loop;
			if(*(end + 1) >= '0' && *(end + 1) <= '9') goto loc_end_loop;
			if(*(end + 1) == '{') goto loc_end_loop;
			if(*(end + 1) == '[') goto loc_end_loop;
		}
		++end;
		goto loc_loop;

	loc_end_loop:
		*end = 0x0;
		jxObject = JXA_FindKey(jxObject, jxPath);
	}
	else{	// array index

		end = (*jxPath == '[') ? ++jxPath : jxPath;

		while(*end != '\\'){

			if(*end++ == 0x0) return JXA_FindIndex(jxObject, JXA_ParseInt(jxPath));
		}

		*end = 0x0;
		if(*jxPath) jxObject = JXA_FindIndex(jxObject, JXA_ParseInt(jxPath));
	}

	jxPath = ++end;
	goto find_loop;
}



char * __jxa_strncpy(const char * src, unsigned int len){

	//printf("::: DEPRECATED json_find.c :: __jxa_strncpy\n");

	char * dst = calloc(len + 1, sizeof(char));
	while(len--) *(dst + len) = *(src + len);

	return dst;
}









unsigned int jxa_StringLength(const char * str){

	unsigned int result = 0;

	while(*str++) ++result;

	return result;
}

char * jxa_StringCopyFirstN(const char * src, unsigned int n){

	char * dst = calloc(n + 1, sizeof(char));
	while(n--) *(dst + n) = *(src + n);

	return dst;
}

char * jxa_StringCopy(const char * src){

	return jxa_StringCopyFirstN(src, jxa_StringLength(src));
}


JSON * jxa_Find(JSON * jxObject, const char * jxPath){

	if(jxPath == nullptr) 	return nullptr;
	if(*jxPath == 0x0) 		return jxObject;
	if(*jxPath != '\\') 	return nullptr;

	char * jxPath_cp = jxa_StringCopy(jxPath);
	JSON * result = jxa_FindLoop(jxObject, jxPath_cp + 1);

	free(jxPath_cp);
	return result;
}