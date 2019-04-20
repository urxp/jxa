#include "jxa.h"


IJSON   STATIC_JXA_INTERFACE;
JSON 	STATIC_JXA_CURRENT;

char ErrorMsgBuffer[512] = "Unidentified error occured!";//{0};


void JSON_Free(JSON * JXA_Value){

	jxa_free(JXA_Value);
}



/* int JSON_Error() ********************************************** */

JXA_Error error_e;
int JSON_Error(){ 

	return error_e;
}

char * JSON_ErrorMsg(){

	return ErrorMsgBuffer;
}

/* ********************************************** int JSON_Error() */

/* IJSON * JSON_(JSON) ***************************** */

IJSON * JXA_Inited(JSON);
IJSON * JXA_Uninited(JSON);
IJSON * (*__JSON__)(JSON) = &JXA_Uninited;

IJSON * JXA_Inited(JSON JXA_Value){

	STATIC_JXA_CURRENT = JXA_Value;

	return &STATIC_JXA_INTERFACE;
}

IJSON * JXA_Uninited(JSON JXA_Value){

	JXA_Init();
	__JSON__ = &JXA_Inited;

	return __JSON__(JXA_Value);
}

IJSON * JSON_(JSON JXA_Value){

	error_e = NO_ERROR;

	return __JSON__(JXA_Value);
}

/* ***************************** IJSON * JSON_(JSON) */