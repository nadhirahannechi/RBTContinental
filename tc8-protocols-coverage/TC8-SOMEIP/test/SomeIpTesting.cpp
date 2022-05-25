#include <stdio.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#define MAXSIZE 50

int main()
{
	printf("\n\t*******Starting Tests*******\n\n");

	int test_number = -1;
	int test_sd_behavior_number;
	int continue_testing = 1;
	while (continue_testing)
	{
		//printf("\n\t****************************\n\n");
		printf("Here is the list of test types : \n\n");
		printf("1- SOMEIPSRV_FORMAT\n");
		printf("2- SOMEIPSRV_OPTIONS\n");
		printf("3- SOMEIPSRV_SD_MESSAGE\n");
		printf("4- SOMEIPSRV_SD_BEHAVIOR\n"); // 1-4
		printf("5- SOMEIPSRV_BASIC\n");		  //1-3
		printf("6- SOMEIPSRV_ONWIRE\n");	  //1- 12 EXCEPT 8/9
		printf("7- SOMEIPSRV_RPC\n");		  //1-20  EXCEPT 12/15/16
		printf("\n\n Type the number of test you wanna perform\n\t>");
		scanf("%d", &test_number);
		char cmd[MAXSIZE];
		switch (test_number)
		{
		case 1:
			printf("\n\tChoose the number of the SOMEIPSRV_FORMAT test you wanna perform ( From 1 to 28 EXCEPT 22): \n\t>");
			char test_format_number_char[3];
			scanf(" %s", test_format_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_FORMAT_client ");
			strcat(cmd, test_format_number_char);
			system(cmd);
			break;
		case 2:
			printf("\n\tChoose the number of the SOMEIPSRV_OPTIONS test you wanna perform ( From 1 to 15): \n\t>");
			char test_option_number_char[3];
			scanf(" %s", test_option_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_OPTIONS_client ");
			strcat(cmd, test_option_number_char);
			system(cmd);
			break;
		case 3:
			printf("\n\tChoose the number of the SOMEIPSRV_SD_MESSAGE test you wanna perform ( From 1 to 19 EXCEPT 10/12): \n\t>");
			//scanf("%d",&test_sd_message_number);
			char test_sd_message_number_char[3];
			scanf(" %s", test_sd_message_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_SD_MESSAGE_client ");
			strcat(cmd, test_sd_message_number_char);
			system(cmd);
			break;
		case 4:
			printf("\n\tChoose the number of the SOMEIPSRV_SD_BEHAVIOR test you wanna perform ( From 1 to 4 ): \n\t>");
			char test_sd_behavior_number_char[3];
			scanf(" %s", test_sd_behavior_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_SD_BEHAVIOR_client ");
			strcat(cmd, test_sd_behavior_number_char);
			system(cmd);
			break;
		case 5:
			printf("\n\tChoose the number of the SOMEIPSRV_BASIC test you wanna perform ( From 1 to 3 ): \n\t>");
			char test_basic_number_char[3];
			scanf(" %s", test_basic_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_BASIC_client ");
			strcat(cmd, test_basic_number_char);
			system(cmd);
			break;
		case 6:
			printf("\n\tChoose the number of the SOMEIPSRV_ONWIRE test you wanna perform ( From 1 to 12 EXCEPT 8/9): \n\t>");
			char test_onwire_number_char[3];
			scanf(" %s", test_onwire_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_ONWIRE_client ");
			strcat(cmd, test_onwire_number_char);
			system(cmd);
			break;

		case 7:
			printf("\n\tChoose the number of the  SOMEIPSRV_RPC test you wanna perform ( From 1 to 20 EXCEPT 12/15/16): \n\t>");
			char test_rpc_number_char[3];
			scanf(" %s", test_rpc_number_char);
			strcpy(cmd, "env VSOMEIP_CONFIGURATION=../config/myClientConfig.json VSOMEIP_APPLICATION_NAME=testClient ./SOMEIPSRV_RPC_client ");
			strcat(cmd, test_rpc_number_char);
			system(cmd);
			break;
		}
		char response[10];
		printf("\nWanna try an other test ?  (yes/no)\n\t>");
		scanf("%s", response);
		if (strcmp(response, "yes") == 0 || strcmp(response, "y") == 0 || strcmp(response, "Y") == 0 || strcmp(response, "YES") == 0)
		{
			continue_testing = 1;
		}
		else
		{
			continue_testing = 0;
		}
	}
	return 0;
}