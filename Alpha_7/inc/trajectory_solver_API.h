#ifndef __TRAJECTORY_SOLVER_API_H__
#define __TRAJECTORY_SOLVER_API_H__

#include <stdint.h>

//#define PORTABLE_DEVICE		/* Should be defined for handhelds*/
#define SLOW_DEVICE 			/* Should be defined for handhelds*/

#define DIST_RANGE 2000			/* Maximum distance in ballistic table, aliquot of TABLE_STEP (!) */
#define TABLE_STEP 50			/* Ballistic table step in meters 25, 50, 100 */
#define WIND_GRANULARITY		5	/* Quantity of wind measurement points */
#define CMD_GRANULARITY			31	/* Quantity of custom drag-function points (0.5 - 3.5 Mach) */
#define USELESS_DATA			0
#define USELESS_COMPLEX_DATA	NULL

#define BALLISTIC_TABLE_SIZE DIST_RANGE/TABLE_STEP
#define OUT

#if defined SLOW_DEVICE
#define MAX_NAME_SIZE 128U
#else
#define MAX_NAME_SIZE 255U
#endif

enum angleUnits {

	MOA_UNITS = 1,
	MRAD_UNITS = 2,
};

enum dragModels {
	
	G1 = 1,
	G7 = 7,
	Gs = 999,	/* For spheres */
	CDM = 42,	/* For custom DF */
};

enum twistDirection {

	RIGHT_TWIST = 0,
	LEFT_TWIST = 1,
};

enum POIDrift {

	POI_LEFT = 0,
	POI_RIGHT = 1,
	POI_UP = 0,
	POI_DOWN = 1,
};

enum inputOptions {

	OPTION_NO = 0,	
	OPTION_YES = 1,
};

enum zeroAtmosphere {

	HERE = 0,
	NOT_HERE = 1,
};

enum TSerrorCodes {

	TS_SUCCESS = 0,
	TS_FAIL = 1,
};

enum reticlePatterns {

    MIL_DOT = 1,
};

enum WindType {

    SIMPLE_CASE = 0, 	/* Equal wind on whole distance */
    COMPLEX_CASE = 1, 	/* Random wind */
};

/****************************************************************
*
*	0 - 360 wind direction angle. Clockwise
*   0 - zero ground angle
*   - - clockwise ground angle
*   + - counterclockwise ground angle  
*
*   see below (ComplexWind)
* 
****************************************************************/

typedef struct ComplexWind {

	uint16_t currentDistance;
	float windSpeed;
	uint16_t windDir;
	float terrainDir;

} windDataArray[WIND_GRANULARITY];

struct Meteo {

	int8_t T;
	uint16_t P;
	uint8_t H;

	/* Simple wind case */
	float windSpeed;
	uint16_t windDir;
	int16_t terrainDir;

	/* Flag for wind type definition */
	int8_t WindType;

	/* Random wind case - here is array of complex 
	wind data (dist, velocity, direction) with 
	dimension equal to WIND_GRANULARITY */
	windDataArray* windData;
};

/* Usage of windData field:

	windDataArray windArray = {{500, 4.0, 90}, {600, 5.5, 180}, ...}

	struct Meteo meteo;
	meteo.windData = &windArray;

	(*meteo.windData)[1].currentDistance
	(*meteo.windData)[1].windSpeed
	(*meteo.windData)[1].windDir

	or put NULL with SIMPLE_CASE

 */

typedef struct CDMportion {

	float MachNumber;
	float CD;
} CDMDataArray[CMD_GRANULARITY];


struct Bullet {

	char bulletName[MAX_NAME_SIZE];
	uint8_t dragFunction;
	/* Data for classic G-functions */
	float BC; 
	float DSF_0_9;
	float DSF_1_0;
	float DSF_1_1;
	uint16_t V0; 
	float length;
	uint16_t mass;
	float caliber;
	int8_t V0temp;
	float thermalSens;
	
	/* Data for CDMs */
	CDMDataArray* cdmData;
};

/* Usage of cdmData field:

	CDMDataArray CDMArray = {{0.5, 0.16}, {0.6, 0.166}, ...}

	struct Bullet bullet;
	bullet.cdmData = &CDMArray;

	(*bullet.cdmData)[1].MachNumber
	(*bullet.cdmData)[1].CD

	or put NULL if use G's
 */


struct Rifle {

	char rifleName[MAX_NAME_SIZE];
	uint16_t zeroDistance;
	float scopeHight;
	float twist;
	uint8_t twistDir;
	uint8_t zeroAtm;
	int8_t zeroTemp;
	uint16_t zeroPress;
	float vertDrift;
	uint8_t vertDrDir;
	float horizDrift;
	uint8_t horizDrDir;
	float rollAngle;
};

struct Scope {

	char scopeName[MAX_NAME_SIZE];
	uint8_t angleUnits;
	float clickVert;
	float clickHoriz;
	uint8_t reticlePattern;
};

struct Inputs {

	uint16_t shotDistance;
	uint8_t terrainAndle;
	float targetSpeedInMILs;
	int16_t targetAzimuth;
	float latitude;
	float magneticIncl;
};

struct Options {

	uint8_t Koriolis;
	uint8_t BallisticTable; 
	uint8_t ThermalCorrection;
	uint8_t AeroJump;
};

struct BallisticTable {

	struct Meteo meteo;
	struct Bullet bullet;
	struct Rifle rifle;
	float Vert[BALLISTIC_TABLE_SIZE + 1][2];
	float Horiz[BALLISTIC_TABLE_SIZE + 1][2];
	float Deriv[BALLISTIC_TABLE_SIZE + 1][2];
	float Time[BALLISTIC_TABLE_SIZE + 1];
} ;

struct Results {

	/* detailed output*/
	char bulletName[MAX_NAME_SIZE];
	char rifleName[MAX_NAME_SIZE];
	int16_t vertSm;
	float vertAngleUnits;
	int16_t vertClicks;
	int16_t horizSm;
	float horizAngleUnits;
	int16_t horizClicks;
	int16_t derivSm;
	float derivAngleUnits;
	int16_t derivClicks;
	float targetAdvance;
	float flightTime;
	uint16_t cineticEnergy;
	float MachNumber;
	uint16_t transsonicDist;
	uint16_t subsonicDist;
	uint16_t deepSubsonic;
	float FGS;	
	struct BallisticTable table;
};

#endif /* __TRAJECTORY_SOLVER_API_H__ */
