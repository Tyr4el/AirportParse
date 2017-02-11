#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airPData.h"

#define BUFFER_SIZE 1024 // Define a buffer size for the airports
#define MAX_AIRPORTS 1000

void my_strtok(char **buffer, char delimiter, char* destination);

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
		int count = 0;
		int index = 0;
		char *controlTower = malloc(sizeof(char) * 2);
		
		while ((fgets(buffer, sizeof(buffer), file)) != NULL) // Read the lines of the csv file
		{
			count += 1;
			airPData *airport = &airports[index++]; // Create an array of airPData structs to be able to store the data

			airport->siteNumber = malloc(sizeof(char) * (10 + 1));
			airport->LocID = malloc(sizeof(char) * (4 + 1));
			airport->fieldName = malloc(sizeof(char) * (60 + 1));
			airport->city = malloc(sizeof(char) * (27 + 1));
			airport->state = malloc(sizeof(char) * (2 + 1));
			airport->latitude = malloc(sizeof(char) * (15 + 1));
			airport->longitude = malloc(sizeof(char) * (15 + 1));
			airport->controlTower = 0;

			/* token is just one part of the string *
			* strtok splits the string put into it by the delimiter
			* which in this case is a comma */
			char *bufferPtr = buffer;

			my_strtok(&bufferPtr, ',', airport->siteNumber);		// Get and Save Site Number
			my_strtok(&bufferPtr, ',', airport->LocID);				// Get and Save Location ID
			my_strtok(&bufferPtr, ',', airport->fieldName);			// Get and Save Field Name
			my_strtok(&bufferPtr, ',', airport->city);				// Get and Save City
			my_strtok(&bufferPtr, ',', airport->state);				// Get and Save State
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', airport->latitude);			// Get and Save Latitude
			my_strtok(&bufferPtr, ',', airport->longitude);			// Get and Save Longitude
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', controlTower);				// Get and Save Control Tower
			airport->controlTower = controlTower[0];				
		}

		int length = index;

		printf(" SITE NUMBER\t LOC ID\t\t\t\tFIELD NAME\t\t\t\t\t\tCITY\t\t    STATE    LATITUDE      LONGITUDE     CONTROL TOWER\n");

		for (index = 0; index < length; index++)
		{
			airPData *airport = &airports[index];
			printf("|%-12s|", airport->siteNumber);
			printf("%12s|", airport->LocID);
			printf("%55s|", airport->fieldName);
			printf("%40s|", airport->city);
			printf("%4s|", airport->state);
			printf("%14s|", airport->latitude);
			printf("%14s|", airport->longitude);
			printf("%15c|", airport->controlTower);
			printf("\n");
		}
		printf("The total airport count was: %d\n", count);

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

void my_strtok(char **buffer, char delimiter, char* destination)
{
	// Search through the buffer and return a pointer to the first occurence of the delim
	// in the buffer
	char *end = strchr(*buffer, delimiter); 
	if (destination != NULL)
	{
		memcpy(destination, *buffer, (end - *buffer)); // Copy the token using memcpy
		destination[end - *buffer] = 0;
	}
	*buffer = end + 1; // Advance the buffer to the character after the delimiter
}