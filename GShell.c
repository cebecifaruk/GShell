#include "GShell.h"


void GCreateEnv(Environment *env) {
	uint8_t *ptr=(void*)env;
	unsigned int i;
	for(i=0;i<sizeof(Environment);i++) *(ptr+i)=0;

	env->registers[G_REGISTER_ADDRESS]=-1;
	env->registers[M_REGISTER_ADDRESS]=-1;
	env->registers[COORDINATE_REGISTER_ADDRESS]=COORDINATE_ABSOLUTE;
	env->registers[UNIT_REGISTER_ADDRESS]=1;
	env->registers[PLANE_REGISTER_ADDRESS]=PLANE_XY;
    env->selected_coordinate_system=0;

	env->motors[0].stepPerMM=320;
	env->motors[1].stepPerMM=320;
	#if (AXIS_NUM >= 3)
	env->motors[2].stepPerMM=320;
	#endif
	#if (AXIS_NUM >= 4)
	env->motors[3].stepPerMM=320;
	#endif
	#if (AXIS_NUM >= 5)
	env->motors[4].stepPerMM=320;
	#endif

	#if (AXIS_NUM == 6)
	env->motors[5].stepPerMM=320;
	#endif
}


int GParse (Environment *env) {
	char *code = env->stdio;
    //Açýklama Satýrý desteði eklenecek (açklama) ;Açýklama //Açýklama
    //Satýr numarasý desteði eklenecek
    //Create command and set values


    //Char scan loop
    for (;code[0] != '\0';code++) {

		if(isspace(code[0]));
        else if(code[0] == '(') for(code++;code[0]!=')';code++);
        else if(code[0] == ';') break;
        else if(code[0] == '$') code++;
        else if (isalpha(code[0])){

            char firstChar=toupper(code[0]);
            char numBuffer[10];

            //Multiple Commands in a line
            if((firstChar == 'G'||firstChar == 'M')&& (env->registers[G_REGISTER_ADDRESS]!= -1 || env->registers[M_REGISTER_ADDRESS]!= -1))
							GExec(env);

            numBuffer[0]='\0';
            for (code++;code[0]!='\0'&&(isdigit(code[0])||code[0]=='.'||code[0]=='-');code++)
                strncat(numBuffer,&code[0],1); code--;
            double value = (double) atof(numBuffer);
            switch(firstChar){

                case 'X': env->registers[X_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;
                case 'Y': env->registers[Y_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;
                #if (AXIS_NUM >= 3)
				case 'Z': env->registers[Z_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;
				#endif
				#if (AXIS_NUM >= 4)
                case 'A': env->registers[A_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;
				#endif
				#if (AXIS_NUM >= 5)
                case 'B': env->registers[B_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;
                #endif
				#if (AXIS_NUM == 6)
				case 'C': env->registers[C_REGISTER_ADDRESS]=value; break;
                #endif

                case 'F': env->registers[F_REGISTER_ADDRESS]=(env->registers[UNIT_REGISTER_ADDRESS])*value; break;


                case 'G': env->registers[G_REGISTER_ADDRESS]=value; break;
                case 'M': env->registers[M_REGISTER_ADDRESS]=value; break;

                case 'S': env->registers[S_REGISTER_ADDRESS]=value; break;
                case 'P': env->registers[P_REGISTER_ADDRESS]=value; break;

				case 'D': env->registers[D_REGISTER_ADDRESS]=value; break;
                case 'E': env->registers[E_REGISTER_ADDRESS]=value; break;
                case 'H': env->registers[H_REGISTER_ADDRESS]=value; break;
                case 'I': env->registers[I_REGISTER_ADDRESS]=value; break;
                case 'J': env->registers[J_REGISTER_ADDRESS]=value; break;
                case 'K': env->registers[K_REGISTER_ADDRESS]=value; break;
                case 'L': env->registers[L_REGISTER_ADDRESS]=value; break;
                case 'N': env->registers[N_REGISTER_ADDRESS]=value; break;
                case 'O': env->registers[O_REGISTER_ADDRESS]=value; break;
                case 'Q': env->registers[Q_REGISTER_ADDRESS]=value; break;
                case 'R': env->registers[R_REGISTER_ADDRESS]=value; break;
                case 'T': env->registers[T_REGISTER_ADDRESS]=value; break;
                case 'U': env->registers[U_REGISTER_ADDRESS]=value; break;
                case 'V': env->registers[V_REGISTER_ADDRESS]=value; break;
                case 'W': env->registers[W_REGISTER_ADDRESS]=value; break;
                default: break; //hata
            }
        }
        else {strcpy(env->stdio,"ERROR: SYNTAX ERROR");return -1;}
    }
    return GExec(env);
}






int GExec(Environment *env) {

    if(env->registers[G_REGISTER_ADDRESS]>=0) {

        switch ((unsigned int)env->registers[G_REGISTER_ADDRESS]) {
        // http://www.nist.gov/customcf/get_pdf.cfm?pub_id=823374 - Page 20
        // Group 0
        // {G4, G10, G28, G30, G53, G92, G92.1, G92.2, G92.3}
        case 4:
            if(env->registers[S_REGISTER_ADDRESS] != 0) GDelay((unsigned int)env->registers[S_REGISTER_ADDRESS]);
            else GDelay((unsigned int)env->registers[P_REGISTER_ADDRESS]*1000);
            break;

        // Motion Codes (Group 1)
        // {G0, G1, G2, G3, G38.2, G80, G81, G82, G83, G84, G85, G86, G87, G88, G89}
        case 0:
            env->registers[F_REGISTER_ADDRESS] = F_MAX;
        case 1:
            if(env->registers[COORDINATE_REGISTER_ADDRESS] == COORDINATE_ABSOLUTE) {
                env->motors[0].deltaSteps=(env->motors[0].stepPerMM)*(env->registers[X_REGISTER_ADDRESS] - env->motors[0].position[env->selected_coordinate_system]);
                env->motors[0].position[env->selected_coordinate_system] = env->registers[X_REGISTER_ADDRESS];

                env->motors[1].deltaSteps=(env->motors[1].stepPerMM)*(env->registers[Y_REGISTER_ADDRESS] - env->motors[1].position[env->selected_coordinate_system]);
                env->motors[1].position[env->selected_coordinate_system] = env->registers[Y_REGISTER_ADDRESS];

                #if (AXIS_NUM >= 3)
                env->motors[2].deltaSteps=(env->motors[2].stepPerMM)*(env->registers[Z_REGISTER_ADDRESS] - env->motors[2].position[env->selected_coordinate_system]);
                env->motors[2].position[env->selected_coordinate_system] = env->registers[Z_REGISTER_ADDRESS];
                #endif

                #if (AXIS_NUM >= 4)
                env->motors[3].deltaSteps=(env->motors[3].stepPerMM)*(env->registers[A_REGISTER_ADDRESS] - env->motors[3].position[env->selected_coordinate_system]);
                env->motors[3].position[env->selected_coordinate_system] = env->registers[A_REGISTER_ADDRESS];
                #endif

                #if (AXIS_NUM >= 5)
                env->motors[4].deltaSteps=(env->motors[4].stepPerMM)*(env->registers[B_REGISTER_ADDRESS] - env->motors[4].position[env->selected_coordinate_system]);
                env->motors[4].position[env->selected_coordinate_system] = env->registers[B_REGISTER_ADDRESS];
                #endif

                #if (AXIS_NUM >= 6)
                env->motors[5].deltaSteps=(env->motors[5].stepPerMM)*(env->registers[C_REGISTER_ADDRESS] - env->motors[5].position[env->selected_coordinate_system]);
                env->motors[5].position[env->selected_coordinate_system] = env->registers[C_REGISTER_ADDRESS];
                #endif

            }

            else {
                env->motors[0].deltaSteps=(env->motors[0].stepPerMM)*(env->registers[X_REGISTER_ADDRESS]);
                env->motors[0].position[env->selected_coordinate_system] += env->registers[X_REGISTER_ADDRESS];
                env->registers[X_REGISTER_ADDRESS]=0;

                env->motors[1].deltaSteps=(env->motors[1].stepPerMM)*(env->registers[Y_REGISTER_ADDRESS]);
                env->motors[1].position[env->selected_coordinate_system] += env->registers[Y_REGISTER_ADDRESS];
                env->registers[Y_REGISTER_ADDRESS]=0;

                #if (AXIS_NUM >= 3)
                env->motors[2].deltaSteps=(env->motors[2].stepPerMM)*(env->registers[Z_REGISTER_ADDRESS]);
                env->motors[2].position[env->selected_coordinate_system] += env->registers[Z_REGISTER_ADDRESS];
                env->registers[Z_REGISTER_ADDRESS]=0;
                #endif

                #if (AXIS_NUM >= 4)
                env->motors[3].deltaSteps=(env->motors[3].stepPerMM)*(env->registers[A_REGISTER_ADDRESS]);
                env->motors[3].position[env->selected_coordinate_system] += env->registers[A_REGISTER_ADDRESS];
                env->registers[A_REGISTER_ADDRESS]=0;
                #endif

                #if (AXIS_NUM >= 5)
                env->motors[4].deltaSteps=(env->motors[4].stepPerMM)*(env->registers[B_REGISTER_ADDRESS]);
                env->motors[4].position[env->selected_coordinate_system] += env->registers[B_REGISTER_ADDRESS];
                env->registers[B_REGISTER_ADDRESS]=0;
                #endif

                #if (AXIS_NUM >= 6)
                env->motors[5].deltaSteps=(env->motors[5].stepPerMM)*(env->registers[C_REGISTER_ADDRESS]);
                env->motors[5].position[env->selected_coordinate_system] += env->registers[C_REGISTER_ADDRESS];
                env->registers[C_REGISTER_ADDRESS]=0;
                #endif

            }

            GLinearMotion(env);
			break;

        case 2:
        case 3:  break;

        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89: break;



        // Plane Selection (Group 2)
        // {G17, G18, G19}
        case 17:  env->registers[PLANE_REGISTER_ADDRESS]=PLANE_XY;break;
        case 18:  env->registers[PLANE_REGISTER_ADDRESS]=PLANE_XZ;break;
        case 19:  env->registers[PLANE_REGISTER_ADDRESS]=PLANE_YZ;break;

        // Distance Mode (Group 3)
        // {G90, G91}
        case 90:  env->registers[COORDINATE_REGISTER_ADDRESS]=COORDINATE_ABSOLUTE;break;
        case 91:  env->registers[COORDINATE_REGISTER_ADDRESS]=COORDINATE_RELATIVE;break;

        // FeedRate Mode (Group 4)
        // {G93, G94}
        case 93:
        case 94: break;

        // Units (Group 5)
        // {G20, G21}
        case 20:  env->registers[UNIT_REGISTER_ADDRESS]=25.4;break;
        case 21:  env->registers[UNIT_REGISTER_ADDRESS]=1;break;


        // (Group 6)
        // (Group 7)
        // (Group 8)
        // (Group 9)
        // (Group 10)
        // (Group 11)

        // Coordinate System Selection (Group 12)
        // {G54, G55, G56, G57, G58, G59, G59.1, G59.2, G59.3}
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:break;

        // (Group 13)


        default:
			env->registers[G_REGISTER_ADDRESS]=-1;
			env->registers[M_REGISTER_ADDRESS]=-1;
			strcpy(env->stdio,"ERROR: UNSPPORTED COMMAND");
			return ERROR_UNSPPORTED_COMMAND;

        }
    }
    else if(env->registers[M_REGISTER_ADDRESS]>=0) {
        switch((unsigned int) env->registers[M_REGISTER_ADDRESS]) {

        case 17:
            GEnableMotors();
            break;
        case 18:
        case 84:
            GDisableMotors();
            break;
        default:
			env->registers[G_REGISTER_ADDRESS]=-1;
			env->registers[M_REGISTER_ADDRESS]=-1;
			strcpy(env->stdio,"ERROR: UNSPPORTED COMMAND");
			return ERROR_UNSPPORTED_COMMAND;

        }
    }
    else {
        env->registers[G_REGISTER_ADDRESS]=-1;
        env->registers[M_REGISTER_ADDRESS]=-1;
        strcpy(env->stdio,"ERROR: NO COMMAND");
        return ERROR_NO_COMMAND;
    }
	env->registers[G_REGISTER_ADDRESS]=-1;
	env->registers[M_REGISTER_ADDRESS]=-1;
	strcpy(env->stdio,"OK");
	return 0;
}


static int max(int a, int b) {if(a>=b)return a;else return b;}
void GLinearMotion(Environment *env)
{

	unsigned int  i,k,intstep[AXIS_NUM];
	float m[AXIS_NUM], steps[AXIS_NUM];

	//ENABLE MOTORS
	GEnableMotors();
	//Set direction of motors
	GSetDir(env);
	//Calculate max step
	//int max_delta = env->motors[0].deltaSteps >= env->motors[1].deltaSteps ? env->motors[0].deltaSteps : env->motors[1].deltaSteps;
	int max_delta = max(env->motors[0].deltaSteps, env->motors[1].deltaSteps);
	max_delta = max(env->motors[2].deltaSteps, max_delta);
    if (max_delta == 0) return;

    for(i=0;i<AXIS_NUM;i++) steps[i]=0;

	for(i=0;i<AXIS_NUM;i++) m[i]=(float)env->motors[i].deltaSteps/(float)max_delta;

	for (k = 0;k < max_delta;k++){
		for(i=0;i<AXIS_NUM;i++) steps[i] += m[i];
		// Round float
		for(i=0;i<AXIS_NUM;i++) {if ((int) (steps[i] + 0.5) >= 1)env->motors[i].step =true, steps[i]-=1;
        else env->motors[i].step =false;}
		GStepper(env);
		//GDelay(1);
	}
    for(i=0;i<AXIS_NUM;i++) env->motors[i].deltaSteps=0;
	GDisableMotors();
}
