#ifndef GCLIB
#define GCLIB

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*----- USER DEFINATIONS -----------------------*/
#define AXIS_NUM 4
#define F_MAX 1000

#define USER_LEN 1
#define MESSAGE_LEN 100
#define USE_DDA_ALGORITHM
#define COORDINATE_MEM_LEN 2

/*------------------*/

#define COORDINATE_ABSOLUTE 0
#define COORDINATE_RELATIVE 1

#define PLANE_XY            0
#define PLANE_YZ            1
#define PLANE_XZ            2

/* ERROR CODE DEFINATION */
#define ERROR_UNSPPORTED_COMMAND -1
#define ERROR_NO_COMMAND -2

#if (AXIS_NUM > 6 || AXIS_NUM < 2)
#error "GShell unsupported more than 6 axis or less 2 axis..!"
#endif

#define REGISTER_LEN 30

#define A_REGISTER_ADDRESS 0
#define B_REGISTER_ADDRESS 1
#define C_REGISTER_ADDRESS 2
#define D_REGISTER_ADDRESS 3
#define E_REGISTER_ADDRESS 4
#define F_REGISTER_ADDRESS 5
#define G_REGISTER_ADDRESS 6
#define H_REGISTER_ADDRESS 7
#define I_REGISTER_ADDRESS 8
#define J_REGISTER_ADDRESS 9
#define K_REGISTER_ADDRESS 10
#define L_REGISTER_ADDRESS 11
#define M_REGISTER_ADDRESS 12
#define N_REGISTER_ADDRESS 13
#define O_REGISTER_ADDRESS 14
#define Q_REGISTER_ADDRESS 15
#define P_REGISTER_ADDRESS 16
#define R_REGISTER_ADDRESS 17
#define S_REGISTER_ADDRESS 18
#define T_REGISTER_ADDRESS 19
#define U_REGISTER_ADDRESS 20
#define V_REGISTER_ADDRESS 21
#define W_REGISTER_ADDRESS 22
#define X_REGISTER_ADDRESS 23
#define Y_REGISTER_ADDRESS 24
#define Z_REGISTER_ADDRESS 25
#define COORDINATE_REGISTER_ADDRESS 26
#define UNIT_REGISTER_ADDRESS 27
#define PLANE_REGISTER_ADDRESS 28


/*----- Types ----------------------------------*/
typedef enum {false = 0,true = 1}bool;

typedef struct {
	char		*name;
	float		data;
}Variable;

typedef struct {
	int32_t 	deltaSteps;
	uint8_t 	stepPerMM;
	float	position[COORDINATE_MEM_LEN];
	bool 	step;
}Motor;

typedef struct {
    float 		registers[REGISTER_LEN];
    Variable	vars[USER_LEN];
	Motor 		motors[AXIS_NUM];
	char		stdio[MESSAGE_LEN];
	uint8_t 	selected_coordinate_system;
}Environment;

/*----- USER DEFINED FUNCTIONS ---------------*/


void 	GDelay(unsigned int ms);
void	GDelayus(unsigned int us);
void	GEnableMotors(void);
void	GDisableMotors(void);
void	GSetDir(Environment *env);
void	GStepper(Environment *env);

/*----- GSHELL FUNCTIONS --------------------*/

void 	GCreateEnv(Environment *env);
int     GParse(Environment *env);
int     GExec(Environment *env);
void 	GLinearMotion(Environment *env);


#endif
