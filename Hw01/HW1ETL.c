#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "airPData.h"

#define BUFFER_SIZE 1024 // Define a buffer size for the airports
#define MAX_AIRPORTS 1000
#define LOC_BUFFER_SIZE 15

void my_strtok(char **buffer, char delimiter, char* destination);
float sexag2decimal(char *degreeString);

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
		airPData airports[MAX_AIRPORTS];
		int index = 0;

		char *latLonBuffer = malloc(sizeof(char) * (15 + 1));
		
		while ((fgets(buffer, sizeof(buffer), file)) != NULL) // Read the lines of the csv file
		{
			airPData *airport = &airports[index++]; // Create an array of airPData structs to be able to store the data

			airport->LocID = malloc(sizeof(char) * (4 + 1));
			airport->fieldName = malloc(sizeof(char) * (60 + 1));
			airport->city = malloc(sizeof(char) * (35 + 1));

			/* token is just one part of the string *
			* strtok splits the string put into it by the delimiter
			* which in this case is a comma */
			char *bufferPtr = buffer;

			my_strtok(&bufferPtr, ',', NULL);						// SKIP Site Number
			my_strtok(&bufferPtr, ',', airport->LocID);				// Get and Save Location ID
			my_strtok(&bufferPtr, ',', airport->fieldName);			// Get and Save Field Name
			my_strtok(&bufferPtr, ',', airport->city);				// Get and Save City
			my_strtok(&bufferPtr, ',', NULL);						// SKIP State
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', latLonBuffer);				// Get and Save Latitude
			airport->latitude = sexag2decimal(latLonBuffer);		// Convert Latitude to decimal
			my_strtok(&bufferPtr, ',', latLonBuffer);				// Get and Save Longitude
			airport->longitude = sexag2decimal(latLonBuffer);		// Convert Longitude to decimal
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP
			my_strtok(&bufferPtr, ',', NULL);						// SKIP Control Tower	

		}

		int length = index;

		printf("code,name,city,lat,lon\n");

		for (index = 0; index < length; index++)
		{
			airPData *airport = &airports[index];
			printf("%s,", airport->LocID);
			printf("%s,", airport->fieldName);
			printf("%s,", airport->city);
			printf("%.4f,", airport->latitude);
			printf("%.4f", airport->longitude);
			printf("\n");
		}

		// FREEEEEEDOOOOOMMMMMMMM!!!
		for (index = 0; index < length; index++)
		{
			airPData *airport = &airports[index];
			free(airport->LocID);
			free(airport->fieldName);
			free(airport->city);
		}

		free(latLonBuffer); // Free the latlonBuffer just once
		
	}
	
	fclose(file); // Don't forget to close!

	return 0;
}

void my_strtok(char **buffer, char delimiter, char *destination)
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

float sexag2decimal(char *degreeString)
{
	// Check if degreeString is NULL
	if (degreeString == NULL)
	{
		return 0.0;
	}

	// START DEGREES
	// Degrees needs no conversion because it's just a number from 0 to 1880
	char *ddStr = strtok(degreeString, "-");
	if (ddStr == NULL)
	{
		return 0.0;
	}
	int dd = atoi(ddStr);
	if (dd < 0 || dd > 180) // Check for out of bounds
	{
		return 0.0;
	}
	// END DEGREES

	// START MINUTES
	char *mmStr = strtok(NULL, "-");
	if (mmStr == NULL)
	{
		return 0.0;
	}
	int mm = atoi(mmStr); // Minutes
	if (mm < 0 || mm > 59) // Check for out of bounds
	{
		return 0.0;
	}
	// END MINUTES

	// START SECONDS
	char *ssStr = strtok(NULL, ".");
	if (ssStr == NULL)
	{
		return 0.0;
	}
	int ss = atoi(ssStr); // Seconds
	if (ss < 0 || ss > 59) // Check for out of bounds
	{
		return 0.0;
	}
	// END SECONDS

	// START MAS
	char *masDStr = strtok(NULL, "");
	char direction;
	if (masDStr == NULL)
	{
		return 0.0;
	}
	int mas = atoi(masDStr); // Milli-Arc Seconds
	if (mas < 0 || mas > 9999) // Check for out of bounds
	{
		return 0.0;
	}
	// END MAS

	// Check the length of masDStr and get the direction char out of it
	if (strlen(masDStr) == 5)
	{
		direction = masDStr[4]; // Direction
	} 
	else
	{
		return 0.0;
	}

	// Conversions per specs pg 5
	float ssMasCombined = ss + mas / 1000.0;
	float mmFinal = mm / 60.0;
	float ssMasFinal = ssMasCombined / (pow(60.0, 2));

	// Define the result and perform the maths
	float result = dd + mmFinal + ssMasFinal;

	// Check for directional things
	// Check to make sure an invalid direction wasn't entered
	if (direction == 'S')
	{
		result = result * -1;
	}

	if (direction == 'W')
	{
		result = result * -1;
	}

	if (!(direction == 'N' || direction == 'S' || direction == 'E' || direction == 'W'))
	{
		return 0.0;
	}
	
	// Return
	return result;
}