#ifndef _SOLVER_STRUCTS_AND_CONSTS_H_
#define _SOLVER_STRUCTS_AND_CONSTS_H_

const float STEP_f = 3.28084;
const float DegToRad = 0.0174533;
const float hPa2mmHg = 0.75;
const float mmToInch = 0.03937;
const float MOAToMRAD = 0.2909;
const float grain2Pounds = 0.00014286;

const float Pi = 3.14159;
const float Inch = 25.4;
const float InchInSm = 2.54;
const float EarthRotation = 0.00007292;
const float MOA_ = 2.9089;
const float _MPHtoMPS = 2.237;

/* Это умноженные на 10 дистанции для калибровки */
const uint16_t transsonicMach_ = 11;
const uint16_t subsonicMach_ = 10;
const uint16_t deepSubsonicMach_ = 9;

typedef struct {

	uint16_t Dist;		/* Dist, (meters) */
	float Dst_f;		/* Dist, (feets) */
	float Yabs;			/* Y, (sm) absolute */
	float Yrz;			/* Y, (sm) relative to zero point (ZP) */
	float Yrt;			/* Y', (sm) relative to ZP and terrain angle */
	float W;			/* windage (sm) */
	float Time;			/* (sec) */
	float Deriv;		/* (sm) */
	float Yrt_moa;		/* in MOA */
	float Yrt_mrad;		/* in MRAD */
	float W_moa;		/* in MOA */
	float W_mrad;		/* in MRAD */
	float Deriv_moa;	/* in MOA */
	float Deriv_mrad;	/* in MRAD */
	float Koriol_h;		/* Horizontal Koriolis */
	float MachNumber;	/* Mach number at distance */
	float windSpeed; 

} solverUnit;

struct solveCompactData {

	uint16_t dist;
	float Yabs; 
	float W; 
	float Time;
	float Deriv;
	float Mach;
	float Wz;
};

struct windPortion {

	float Wx;
	float Wy;
	float Wz;
};

struct dragAndBCInfo {

	float CD;
	float C3;
	float BCzero;
};

struct calibrationDistances {

	uint16_t DistTrans;
	uint16_t DistSubsonic; 
	uint16_t DistDeepSubsonic;
};

struct zeroingInfo {

	float DistFeet;
	float Yzero;
};

struct CalibDists {

	uint16_t DistTrans; 		/* transsonic distance (@ 1.1 Mach) */
	uint16_t DistSubsonic; 		/* subsonic distance (@ 1.0 Mach)*/
	uint16_t DistDeepSubsonic;	/* deep subsonic distance (@ 0.9 Mach) */
};

struct CalibCoefficients {

	float atTrassonic;
	float atSubsonic;
	float atDeepSubsonic;
};

struct VertCorrsAtCalibDists { /* vertical corrections at 1.1, 1.0 and 0.9 Mach */

	float verCorrAtTranssonic;
	float verCorrAtSubsonic;
	float verCorrAtDeepSubsonic;
};

struct terminalData {

	float VxDist;
	float MachDist;
	uint16_t cineticEnergy;
};

#endif /* _SOLVER_STRUCTS_AND_CONSTS_H_ */
