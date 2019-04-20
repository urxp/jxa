#include "jxa.h"

void printf_tab(int tab){

	while(tab--) printf("\t");
}



void printf_json_tab(cxjson_value * json, int tab);


void printf_json(cxjson_value * json){

	if(!json) return;

	printf_json_tab(json, 0);
	printf("\n");
}

void printf_json_tab(cxjson_value * json, int tab){

	switch(json->type){
	case JSON_OBJECT:
	{
		cxjson_value * item = json->data;

		if(json->key && tab){
			printf_tab(tab);
			if(item) 	printf("\"%s\": {\n", json->key);
			else 		printf("\"%s\": {}", json->key);
		}
		else {
			printf_tab(tab);
			if(item) 	printf("{\n");
			else 		printf("{}");
		}

		while(item){
			printf_json_tab(item, tab + 1);
			item = item->next;
			if(item) printf(",\n");
			else {
				printf("\n");
				printf_tab(tab);
				printf("}");
			}
		}

		break;
	}
	case JSON_ARRAY:
	{
		cxjson_value * item = json->data;

		if(json->key && tab){
			printf_tab(tab);
			if(item) 	printf("\"%s\": [\n", json->key);
			else 		printf("\"%s\": []", json->key);
		}
		else {
			printf_tab(tab);
			if(item) 	printf("[\n");
			else 		printf("[]");
		}

		while(item){
			printf_json_tab(item, tab + 1);
			item = item->next;
			if(item) printf(",\n");
			else {
				printf("\n");
				printf_tab(tab);
				printf("]");
			}
		}

		break;
	}
	case JSON_STRING:
		printf_tab(tab);
		if(json->key && tab)	printf("\"%s\": \"%s\"", json->key, (char *)json->data);
		else					printf("\"%s\"", (char *)json->data);
		break;
	case JSON_NUMBER:
		printf_tab(tab);
		if(json->key && tab)	printf("\"%s\": %s", json->key, (char *)json->data);
		else					printf("%s", (char *)json->data);
		break;
	case JSON_FALSE:
		printf_tab(tab);
		if(json->key && tab)	printf("\"%s\": false", json->key);
		else					printf("false");
		break;
	case JSON_TRUE:
		printf_tab(tab);
		if(json->key && tab)	printf("\"%s\": true", json->key);
		else					printf("true");
		break;
	case JSON_NULL:
		printf_tab(tab);
		if(json->key && tab)	printf("\"%s\": null", json->key);
		else					printf("null");
		break;
	default:
		printf("Error :: invalid JSON value!\n");
		break;
	}
}