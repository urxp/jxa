#include <stdio.h>
#include <json.h>

int main(){

	JSON cities_json;
	char * GUID;
	JSON json_test;
	char * name;

	int testInt;


	cities_json = JSON_ParseFile("cities.json");
	
	GUID = JSON(cities_json)
			->get.STRING("\\0\\.Boston\\.ID");

	JSON_ERROR{
		// error
		printf("::: Error: %s\n", JSON_ErrorMsg());
	}

	printf("::: Boston GUID: %s\n", GUID);

	GUID = JSON(cities_json)
			->get.STRING("\\[1\\{St.Louis\\{ID");

	JSON_ERROR{
		// error
		printf("::: Error: %s\n", JSON_ErrorMsg());
	}

	printf("::: St.Louis GUID: %s\n", GUID);
	
#define JSON_I_GET(_0, _1, _2)	JSON(_0)->get._2(_1)

	//JSON testJSON 	= JSON(cities_json)
	//					->get.RAW("\\[2\\[0\\[0");
	JSON testJSON = JSON_I_GET(cities_json, "\\[2\\[0\\[0", RAW);

	//testInt 		= JSON(testJSON)
	//					->get.INT("\\");
	testInt = JSON_I_GET(testJSON, "\\", INT);

	JSON_ERROR{
		// error
		printf("::: Error: %s\n", JSON_ErrorMsg());
	}

	printf("::: test int: %d\n", testInt);

	printf("\n");
	JSON(cities_json)->get.PRINT("");
	JSON(cities_json)->get.PRINT("\\0\\.Boston\\{ID");

	JSON_Free(&cities_json);


	


	json_test = JSON_Parse("{\"name\": \"Avoozl\"}");
	name = JSON_(json_test)
					->get.STRING("\\.name");

	JSON_ERROR{
		// error
		printf("::: Error: %s\n", JSON_ErrorMsg());
	}

	printf("::: name: %s\n", name);
	JSON_Free(&json_test);


	printf("Press enter to leave ...\n");
	getchar();

	return 0;
}