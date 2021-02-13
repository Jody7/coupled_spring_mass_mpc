#include "solver.h"

Vars vars;
Params params;
Workspace work;
Settings settings;

const float time_step = 0.01;
const int mpc_sim_iterations = 1000;

int main(int argc, char ** argv) {

	FILE *fp;
	fp = fopen("mpc.data", "w+");

	int num_iters_solve;

	set_defaults();
	setup_indexing();
	load_default_data();

	settings.verbose = 0;
	printf("start\n");

	for (int i = 0; i < mpc_sim_iterations; i++) {
			float setpoint = 0.0;

			if(i>500) {
				setpoint = -5.0;
			} else if(i>100) {
				setpoint = 5.0;
			}

			for(int ii=0; ii<52; ii++) {
				params.ref[ii][0] = setpoint;
				params.ref[ii][1] = 0.0;
				params.ref[ii][2] = 0.0;

				params.ref[ii][3] = setpoint;
				params.ref[ii][4] = 0.0;
				params.ref[ii][5] = 0.0;
			}

			num_iters_solve = solve();

			fprintf(fp, "%f %f %f %f %f %f %f %f %f\n", i*time_step, vars.u[0][0], vars.x[1][0], vars.x[1][1], vars.x[1][2], 
				vars.x[1][3], vars.x[1][4], vars.x[1][5]);
			
			params.x_0[0] = vars.x[1][0];
			params.x_0[1] = vars.x[1][1];
			params.x_0[2] = vars.x[1][2];
			params.x_0[3] = vars.x[1][3];
			params.x_0[4] = vars.x[1][4];
			params.x_0[5] = vars.x[1][5];
		}

	printf("end\n");

	fclose(fp);
	return 0;
}
void load_default_data(void) {
	float springConstK = 0.25;
	float dampeningConstC = 0.05;
	float mass_1 = 1.0;
	float mass_2 = 9.0;

	params.x_0[0] = 0.0;
	params.x_0[1] = 0.0;
	params.x_0[2] = 0.0;
	params.x_0[3] = 0.0;
	params.x_0[4] = 0.0;
	params.x_0[5] = 0.0;

	params.Q[0] = 0.0; // x1[0] pos
	params.Q[6] = 0;
	params.Q[12] = 0;
	params.Q[18] = 0;
	params.Q[24] = 0;
	params.Q[30] = 0;
	params.Q[1] = 0;
	params.Q[7] = 0.0; // x1[1] vel
	params.Q[13] = 0;
	params.Q[19] = 0;
	params.Q[25] = 0;
	params.Q[31] = 0;
	params.Q[2] = 0;
	params.Q[8] = 0;
	params.Q[14] = 0.0; // x1[2]
	params.Q[20] = 0;
	params.Q[26] = 0;
	params.Q[32] = 0;
	params.Q[3] = 0;
	params.Q[9] = 0;
	params.Q[15] = 0;
	params.Q[21] = 3.0; // x2[0] pos
	params.Q[27] = 0;
	params.Q[33] = 0;
	params.Q[4] = 0;
	params.Q[10] = 0;
	params.Q[16] = 0;
	params.Q[22] = 0;
	params.Q[28] = 0.1; // x2[1] vel
	params.Q[34] = 0;
	params.Q[5] = 0;
	params.Q[11] = 0;
	params.Q[17] = 0;
	params.Q[23] = 0;
	params.Q[29] = 0;
	params.Q[35] = 0.5; // x2[2]

	params.R[0] = 0.001;

	params.Q_final[0] = 0.0;
	params.Q_final[6] = 0;
	params.Q_final[12] = 0;
	params.Q_final[18] = 0;
	params.Q_final[24] = 0;
	params.Q_final[30] = 0;
	params.Q_final[1] = 0;
	params.Q_final[7] = 0.0;
	params.Q_final[13] = 0;
	params.Q_final[19] = 0;
	params.Q_final[25] = 0;
	params.Q_final[31] = 0;
	params.Q_final[2] = 0;
	params.Q_final[8] = 0;
	params.Q_final[14] = 0.0;
	params.Q_final[20] = 0;
	params.Q_final[26] = 0;
	params.Q_final[32] = 0;
	params.Q_final[3] = 0;
	params.Q_final[9] = 0;
	params.Q_final[15] = 0;
	params.Q_final[21] = 1.0;
	params.Q_final[27] = 0;
	params.Q_final[33] = 0;
	params.Q_final[4] = 0;
	params.Q_final[10] = 0;
	params.Q_final[16] = 0;
	params.Q_final[22] = 0;
	params.Q_final[28] = 0.0;
	params.Q_final[34] = 0;
	params.Q_final[5] = 0;
	params.Q_final[11] = 0;
	params.Q_final[17] = 0;
	params.Q_final[23] = 0;
	params.Q_final[29] = 0;
	params.Q_final[35] = 0.0;

	params.A[0] = 1.0;	
	params.A[1] = 0;
	params.A[2] = -springConstK;
	params.A[3] = 0;
	params.A[4] = 0;
	params.A[5] = springConstK;

	params.A[6] = time_step;	
	params.A[7] = 1.0;
	params.A[8] = -dampeningConstC;
	params.A[9] = 0;
	params.A[10] = 0;
	params.A[11] = dampeningConstC;

	params.A[12] = 0;	
	params.A[13] = 1.0 / mass_1;
	params.A[14] = 0;
	params.A[15] = 0;
	params.A[16] = 0;
	params.A[17] = 0;

	params.A[18] = 0;
	params.A[19] = 0;
	params.A[20] = springConstK;
	params.A[21] = 1.0;
	params.A[22] = 0;
	params.A[23] = -springConstK;

	params.A[24] = 0;
	params.A[25] = 0;
	params.A[26] = dampeningConstC;
	params.A[27] = time_step;
	params.A[28] = 1.0;
	params.A[29] = -dampeningConstC;

	params.A[30] = 0;
	params.A[31] = 0;
	params.A[32] = 0;
	params.A[33] = 0;
	params.A[34] = 1.0 / mass_2;
	params.A[35] = 0;

	params.B[0] = 0.0;
	params.B[1] = 0.0;
	params.B[2] = 1.0;
	params.B[3] = 0.0;
	params.B[4] = 0.0;
	params.B[5] = 0.0;

	params.u_max[0] = 1.0;
	params.S[0] = 0.25 * time_step;
}