#include "jxa.h"
#include <stdlib.h>


void jxa_free(JSON * JXA_Value){

	/*
	struct JSON {
		char *			key;
		JSON_Type		type;
		void * 			data;
		JSON 		 	next;
	};
	*/

	JSON __tmp;

	if(JXA_Value && *JXA_Value){

		if((*JXA_Value)->key) free((*JXA_Value)->key);
		
		switch((*JXA_Value)->type){
		case JSON_OBJECT:
		case JSON_ARRAY:
			while((*JXA_Value)->data) jxa_free((JSON *)&(*JXA_Value)->data);
			break;
		case JSON_STRING:
		case JSON_NUMBER:
			free((*JXA_Value)->data);
			break;
		case JSON_NULL:
		case JSON_FALSE:
		case JSON_TRUE:
		default:
			break;
		}

		__tmp = *JXA_Value;
		*JXA_Value = __tmp->next;
		free(__tmp);
	}
}