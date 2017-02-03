typedef struct airPData
{
	char *siteNumber; //FAA Site Number
	char *LocID; //Airport’s ‘‘Short Name’’, ie MCO
	char *fieldName; //Airport Name
	char *city; //Associated City
	char *state; //State
	char *latitude; //Latitude
	char *longitude; //Longitude
	char controlTower;//Control Tower (Y/N)
} airPData;
