#include <cstdint>

int8_t drivetrainMotorL1 = 10;
int8_t drivetrainMotorL2 = 9;
int8_t drivetrainMotorL3 = 0;
int8_t drivetrainMotorR1 = 1;
int8_t drivetrainMotorR2 = 2;
int8_t drivetrainMotorR3 = 0;
int8_t drivetrainImu = 6;
int8_t catapultMotor = 4;
int8_t catapultMotor2 = 7;
int8_t catapultRotation = 5;
int8_t wingLeft = 'A';
int8_t wingRight = 'B';

double drivetrainKp = 7.50;
double drivetrainKi = 0.730;
double drivetrainKd = 8.0;
double drivetrainVelKp = 20.0;
double drivetrainVelKi = 10.0;
double drivetrainVelKd = 0.0;
double drivetrainHeadingKp = 80.0;
double drivetrainHeadingKi = 15.0;
double drivetrainHeadingKd = 30.0;

double tankMoveMultiplier = 1.0;
double arcadeTurnMultiplier = 1.0;
