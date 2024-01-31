#include <cstdint>

int8_t drivetrainMotorL1 = 11;
int8_t drivetrainMotorL2 = 12;
int8_t drivetrainMotorL3 = 1;
int8_t drivetrainMotorR1 = 19;
int8_t drivetrainMotorR2 = 20;
int8_t drivetrainMotorR3 = 10;
int8_t drivetrainImu = 5;
int8_t catapultMotor = 4;
int8_t catapultMotor2 = 7;
int8_t catapultRotation = 9;
int8_t wingLeft = 'A';
int8_t wingRight = 'B';

double drivetrainKp = 7.00;
double drivetrainKi = 0.730;
double drivetrainKd = 8.0;
double drivetrainVelKp = 20.0;
double drivetrainVelKi = 10.0;
double drivetrainVelKd = 0.0;
double drivetrainHeadingKp = 80.0;
double drivetrainHeadingKi = 15.0;
double drivetrainHeadingKd = 30.0;

double tankMoveMultiplier = 0.9;
double arcadeTurnMultiplier = 0.8;
