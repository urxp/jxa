struct {

	int _KEY;
	size_t _KEY_SIZE;
	int _OBJECT;
	size_t _OBJECT_SIZE;
	int _ARRAY;
	size_t _ARRAY_SIZE;
	int _STRING;
	size_t _STRING_SIZE;
	int _NUMBER;
	size_t _NUMBER_SIZE;
	int _FALSE;
	size_t _FALSE_SIZE;
	int _TRUE;
	size_t _TRUE_SIZE;
	int _NULL;
	size_t _NULL_SIZE;
} stats;




void _cxjson_stats(cxjson_value * _cxjson){

	if(_cxjson){

		if(_cxjson->key){
			stats._KEY++;
			stats._KEY_SIZE += strlen(_cxjson->key) + 1;
		}

		switch(_cxjson->type){
		case CXJSON_OBJECT:
			stats._OBJECT++;
			stats._OBJECT_SIZE += sizeof(*_cxjson);
			_cxjson_stats(_cxjson->data);
			break;
		case CXJSON_ARRAY:
			stats._ARRAY++;
			stats._ARRAY_SIZE += sizeof(*_cxjson);
			_cxjson_stats(_cxjson->data);
			break;
		case CXJSON_STRING:
			stats._STRING++;
			stats._STRING_SIZE += sizeof(*_cxjson);
			stats._STRING_SIZE += strlen(_cxjson->data) + 1;
			break;
		case CXJSON_NUMBER:
			stats._NUMBER++;
			stats._NUMBER_SIZE += sizeof(*_cxjson);
			stats._NUMBER_SIZE += strlen(_cxjson->data) + 1;
			break;
		case CXJSON_FALSE:
			stats._FALSE++;
			stats._FALSE_SIZE += sizeof(*_cxjson);
			break;
		case CXJSON_TRUE:
			stats._TRUE++;
			stats._TRUE_SIZE += sizeof(*_cxjson);
			break;
		case CXJSON_NULL:
			stats._NULL++;
			stats._NULL_SIZE += sizeof(*_cxjson);
			break;
		default:
			return;
		}

		_cxjson_stats(_cxjson->next);
	}
}

void cxjson_stats(cxjson_value * _cxjson){

	stats._KEY = 0;
	stats._OBJECT = 0;
	stats._ARRAY = 0;
	stats._STRING = 0;
	stats._NUMBER = 0;
	stats._FALSE = 0;
	stats._TRUE = 0;
	stats._NULL = 0;
	stats._KEY_SIZE = 0;
	stats._OBJECT_SIZE = 0;
	stats._ARRAY_SIZE = 0;
	stats._STRING_SIZE = 0;
	stats._NUMBER_SIZE = 0;
	stats._FALSE_SIZE = 0;
	stats._TRUE_SIZE = 0;
	stats._NULL_SIZE = 0;

	_cxjson_stats(_cxjson);
}

void print_stats(){

	printf("\n");
	printf("------------------------------------------\n");
	printf("::: KEY ::::::::::::: %6d [%9d b]\n", stats._KEY, stats._KEY_SIZE);
	printf("::: CXJSON_OBJECT ::: %6d [%9d b]\n", stats._OBJECT, stats._OBJECT_SIZE);
	printf("::: CXJSON_ARRAY :::: %6d [%9d b]\n", stats._ARRAY, stats._ARRAY_SIZE);
	printf("::: CXJSON_STRING ::: %6d [%9d b]\n", stats._STRING, stats._STRING_SIZE);
	printf("::: CXJSON_NUMBER ::: %6d [%9d b]\n", stats._NUMBER, stats._NUMBER_SIZE);
	printf("::: CXJSON_FALSE :::: %6d [%9d b]\n", stats._FALSE, stats._FALSE_SIZE);
	printf("::: CXJSON_TRUE ::::: %6d [%9d b]\n", stats._TRUE, stats._TRUE_SIZE);
	printf("::: CXJSON_NULL ::::: %6d [%9d b]\n", stats._NULL, stats._NULL_SIZE);
	printf("------------------------------------------\n");


	size_t structure_size = 0;
	structure_size += stats._OBJECT;
	structure_size += stats._ARRAY;
	structure_size += stats._STRING;
	structure_size += stats._NUMBER;
	structure_size += stats._FALSE;
	structure_size += stats._TRUE;
	structure_size += stats._NULL;

	structure_size *= sizeof(cxjson_value);

	size_t data_size = 0;
	data_size += stats._KEY_SIZE;
	data_size += stats._OBJECT_SIZE;
	data_size += stats._ARRAY_SIZE;
	data_size += stats._STRING_SIZE;
	data_size += stats._NUMBER_SIZE;
	data_size += stats._FALSE_SIZE;
	data_size += stats._TRUE_SIZE;
	data_size += stats._NULL_SIZE;

	data_size -= structure_size;


	printf("::: STRUCT SIZE :::::::::::: [%9d b]\n", structure_size);
	printf("::: DATA SIZE :::::::::::::: [%9d b]\n", data_size);
	printf("------------------------------------------\n\n");
}