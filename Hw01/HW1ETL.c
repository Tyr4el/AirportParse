#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airPData.h"

#define BUFFER_SIZE 1024
#define MAX_AIRPORTS 1000

int main(int argc, char **argv)
{

	// *fname = argv[1] - Char pointer to fname gets the second argument
	// variable.  First being the program itself.
	char *fname = argv[1];

	if (argc < 2)
	{
		// If the argument count is less than 2, throw an error and quit
		fprintf(stderr, "Error.  Needs filename as argument.\n");
		return -1;
	}

	/* Declare file pointer
	* Pointer magic going on here...don't worry about it */
	FILE *file = fopen(fname, "r");

	/* delimiter is a const char because that is what the function
	* strtok takes and it is set as the comma */
	const char delimiter[2] = ",";

	/* char pointer "token" not initialized */
	char *token = NULL;

	char buffer[BUFFER_SIZE];

	if (file == NULL) // If the file isn't found or something happened
	{
		fprintf(stderr, "Could not open file.\n");
		return 0; // and return 0 to the if
	}
	else {
		printf("File opened successfully.\n\n");

		airPData airports[MAX_AIRPORTS];
		int index = 0;
		
		while ((fgets(buffer, sizeof(buffer), file)) != NULL)
		{
			airPData *airport = &airports[index++];

			airport->siteNumber = malloc(sizeof(char) * (10 + 1));
			airport->LocID = malloc(sizeof(char) * (4 + 1));
			airport->fieldName = malloc(sizeof(char) * (30 + 1));
			airport->city = malloc(sizeof(char) * (27 + 1));
			airport->state = malloc(sizeof(char) * (2 + 1));
			airport->latitude = malloc(sizeof(char) * (15 + 1));
			airport->longitude = malloc(sizeof(char) * (15 + 1));
			airport->controlTower = 0;

			/* token is just one part of the string *
			* strtok splits the string put into it by the delimiter
			* which in this case is a comma */
			token = strtok(buffer, delimiter);		// Get Site Number
			strcpy(airport->siteNumber, token);		// Save Site Number				
			token = strtok(NULL, delimiter);		// Get Location ID
			strcpy(airport->LocID, token);			// Save Location ID
			token = strtok(NULL, delimiter);		// Get Field Name
			strcpy(airport->fieldName, token);		// Save Field Name (Airport Name in PDF)
			token = strtok(NULL, delimiter);		// Get City
			strcpy(airport->city, token);			// Save City
			token = strtok(NULL, delimiter);		// Get State
			strcpy(airport->state, token);			// Save State
			token = strtok(NULL, delimiter);		// Get Region
			token = strtok(NULL, delimiter);		// Get ADO
			token = strtok(NULL, delimiter);		// Get Use
			token = strtok(NULL, delimiter);		// Get Latitude
			strcpy(airport->latitude, token);		// Save Latitude
			token = strtok(NULL, delimiter);		// Get Longitude
			strcpy(airport->longitude, token);		// Save Longitude
			token = strtok(NULL, delimiter);		// Get Airport Ownership
			token = strtok(NULL, delimiter);		// Get Part 139
			token = strtok(NULL, delimiter);		// Get NPIAS
			token = strtok(NULL, delimiter);		// Get NPIAS Hub Type
			token = strtok(NULL, delimiter);		// Get Control Tower
			airport->controlTower = token[0];		// Save Control Tower
			token = strtok(NULL, delimiter);		// Get Fuel
			token = strtok(NULL, delimiter);		// Get Other Services
			token = strtok(NULL, delimiter);		// Get Based Aircraft Total
			token = strtok(NULL, delimiter);		// Get Total Operations
		}

		int length = index;

		printf("SITE NUMBER   LOC ID         FIELD NAME\t\t   CITY\t\t  STATE   LATITUDE\tLONGITUDE\t\tCONTROL TOWER\n");

		for (index = 0; index < length; index++)
		{
			airPData *airport = &airports[index++];
			printf("|%-12s|", airport->siteNumber);
			printf("%6s|", airport->LocID);
			printf("%28s|", airport->fieldName);
			printf("%15s|", airport->city);
			printf("%4s|", airport->state);
			printf("%14s|", airport->latitude);
			printf("%14s|", airport->longitude);

			if (airport->controlTower == 0)
			{
				airport->controlTower = "N";
			}
			else
			{
				airport->controlTower = "Y";
			}

			printf("%15c|", airport->controlTower);
			printf("\n");
		}

		// FREEEEEEDOOOOOMMMMMMMM!!!
		for (index = 0; index < length; index++)
		{
			airPData *airport = &airports[index];
			free(airport->siteNumber);
			free(airport->LocID);
			free(airport->fieldName);
			free(airport->city);
			free(airport->state);
			free(airport->latitude);
			free(airport->longitude);
		}
		
	}

	fclose(file); // Don't forget to close!
	return 0;
}