#ifndef __TRAJECTORY_SOLVER_ROUTINES_H__
#define __TRAJECTORY_SOLVER_ROUTINES_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "trajectory_solver_API.h"
#include "solver_structs_and_consts.h"

/* INFO - should be in actual state */
#define LIB_VERSION "2.0.0.6"
#define LIB_TYPE "x86_64bit"

const char* libInfo (void);
const char* devInfo (void);

inline float OverallAirDensity(float Pa, float Ph, int8_t T);
float SpeedOfSound (float Pa, float Ph, int8_t T);

float speedOfSoundFeetRatio(const struct Meteo* const meteo);
float VapourPressure (const struct Meteo* const meteo);
float ConditionCorrectionFactor (const struct Meteo* const meteo);

float ZeroingAngleforNumeric(float G_f, const struct Rifle* const rifle, const struct Bullet* const bullet, 
	const struct Meteo* const meteo, const struct dragAndBCInfo* const dragInfo);

float throwAngleCalculation (float Y, float Dst_f);
float DragCoefficient (uint8_t DragFunction, float M);
float DragCoefficientForCDM (const struct Bullet* bullet, float Mach);
float linearInterpolationDSF (const struct Bullet* bullet, float Mach);
float KleroFormula (float Lat);
float V0dueToSensivity(const struct Meteo* const meteo, const struct Bullet* const bullet, const struct Options* const options);
float VerticalCoriolis(float V0, const struct Inputs* const inputs, const struct Options* const options);
void getCalibrationCoeffs(const struct VertCorrsAtCalibDists* calcCorrs, 
const struct VertCorrsAtCalibDists* realCorrs, struct OUT CalibCoefficients* callibCoeff);

float CorrFactorAndMOAorMRAD (uint8_t ScopeIn);
float YwithPOIdrift (float Y, float VertDriftAngular, uint16_t Dist, float CorrectionFactor);
float WdwithPOIdrift (float Wd, float HorizDriftAngular, uint16_t Dist, float CorrectionFactor);

float MillersFGS(float V0, const struct Meteo* const meteo, const struct Bullet* const bullet, 
const struct Rifle* const rifle);
float DerivationCalculation (float SG, float Time, uint8_t TwistDir);
uint16_t CineticEnergy (float Vx, float Weight);

void ResultStructFullfilment (uint16_t Dist, float CorrectionFactor, float Time, float targetAdvanceInMils, 
	uint16_t cineticEnergy, float Mach, const struct calibrationDistances* const calibDists, float SG, float Y, float Wd, 
	float Deriv, float ClickVert, float ClickHoriz, const char* bulletName, const char* rifleName, struct Results* OUT results);

float koriolisHoriz (uint16_t Dist, float Time, float Latitude);
float koriolisVert (float V0, uint16_t Azimuth, float MagIncl, float Latitude, float G);
float convertSmToMOA (float Yrt, uint16_t Dist);
float convertMOAToMRAD (float Yrt_MOA_);
float absoluteDropToZeroing (float Yabs, float Dst_f, float throwAngle);
float relativeDropWithTerrainAngle (float Yrz, float Yabs, float Alpha);
float targetAdvance(float Vtarget, float flightTime);
uint8_t startIndexForCDM(float Mach);
float gravityAccelerFeets(const struct Inputs* const inputs);
float getVertDriftAngular(const struct Rifle* const rifle, const struct Scope* const scope);
float getHorizDriftAngular(const struct Rifle* const rifle, const struct Scope* const scope);
float calculateFakeG7BC(const uint16_t bulletMass, float caliberInch, float i_7);
bool machAtTranssonic(const float MachNum);
bool machAtSubsonic(const float MachNum);
bool machAtDeepSubsonic(const float MachNum);
void fullfillBallisticTable(const struct Meteo* meteo, const struct Bullet* bullet, 
	const struct Rifle* rifle, const solverUnit solver[], struct Results* OUT results);
void getWindComponents(const struct Meteo* meteo, const uint16_t dist, struct OUT windPortion* wind);
float aeroJmpCorrector(float MillersFGS, const struct Bullet* bullet, const struct Rifle* const rifle, 
	const struct Meteo* const meteo, const struct Options* const options);
void initStartWindComponents(const struct Meteo* const meteo, struct windPortion* windComps);
void windComponentsForComplexCase(const struct Meteo* const meteo, struct windPortion* windComps, uint16_t currentDist);
void addSomeSolutionDataToSolverStruct(float KoriolisVert, float YaeroJump, float throwAngle, const struct Inputs* const inputs, 
	const struct Options* const options, const struct Bullet* const bullet, solverUnit* solver, size_t index);

void defineDragInfoForCDM(uint16_t dist, float Mach, float CCF, const struct Bullet* const bullet, 
	const struct Rifle* const rifle, struct dragAndBCInfo* dragInfo, struct calibrationDistances* calibDists);
float calculateC3(float CCF, float BC);

#endif /* __TRAJECTORY_SOLVER_ROUTINES_H__ */
