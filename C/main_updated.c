#include "mpi.h"
//#include "omp.h"
//#include <string>
//#include <string.h>
//#include <vector>
//#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <cstdlib>

MPI_Status status;
FILE *fp0;
FILE *fp1;
FILE *fp2;
FILE *fp3;
FILE *fp4;
FILE *fp5;
FILE *fp6;
FILE *fp7;



int main(int argc,char **argv) {
  
	int myid, nprocs, row, col; // node id  
  
  MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  
  // keep track of cpu time
  double cpu1 = MPI_Wtime();
  
  double m = 0.001;
	
  // global variables
  //long long int N = 1e12;
  //long long int totalTime = 1e11;
  
  long long int N = 1e12;
  long long int totalTime = 3e13;
  
  
  	// clearFile
	
  switch(myid) {
    // choose which file to open up
    case 0: 
      fp0 = fopen("out0.d","w");
      fclose(fp0);
      break;
    case 1: 
      fp1 = fopen("out1.d","w");
      fclose(fp1);
      break;
    case 2: 
      fp2 = fopen("out2.d","w");
      fclose(fp2);
      break;
    case 3: 
      fp3 = fopen("out3.d","w");
      fclose(fp3);
      break;
    case 4: 
      fp4 = fopen("out4.d","w");
      fclose(fp4);
      break;
    case 5: 
      fp5 = fopen("out5.d","w");
      fclose(fp5);
      break;
    case 6: 
      fp6 = fopen("out6.d","w");
      fclose(fp6);
      break;
    case 7: 
      fp7 = fopen("out7.d","w");
      fclose(fp7);
      break;
  }
  
  
  
	
	// set up initial params
	double w = 0.1;
	double a = 3;
  double b = 0;
  double c = 5;
  double d = 1;
  
  // build Q
  double q_matrix[16][16] = {0.00};
  double q_pos[16][16] = {0.00};
  double q_pass[16][16] = {0.00};
  double q_driver[16][16] = {0.00};

  int n = 4; // bit size
  double mp = m/2;
  double md = m/2;
    
  int nTypes = pow(2,n);
  int divider = 11; // 4 bit
    
  // update q_pos
  int myOwnID;
  int bitmask; 
  for (myOwnID = 0; myOwnID < nTypes; myOwnID++) {
    for (bitmask = 1; bitmask < nTypes; bitmask *= 2) {
      int partner = myOwnID ^ bitmask;
      
      if ((myOwnID >= divider) && (partner < divider)) {
        // cancer -> healthy
        q_pos[myOwnID][partner] = 0.00;
      } else {
        q_pos[myOwnID][partner] = 1.00;
      }
    }
  }
  
  for (row = 0; row < divider; row++) {
    for (col = 0; col < divider; col++) {
      q_pass[row][col] = q_pos[row][col];
    }
    for (col = divider; col < nTypes; col++) {
      q_driver[row][col] = q_pos[row][col];
    }
  }
    
  for (row = divider; row < nTypes; row++) {
      for (col = divider; col < nTypes; col++) {
          q_pass[row][col] = q_pos[row][col];
      }
  }

  // normalize passenger & driver
  for (row = 0; row < nTypes; row++) {
    double passengerRowSum = 0.0;
    double driverRowSum = 0.0;
    
    for (col = 0; col < nTypes; col++) {
      passengerRowSum += q_pass[row][col];
      driverRowSum += q_driver[row][col];
    }
    
    for (col = 0; col < nTypes; col++) {
      if (passengerRowSum > 0) {
        q_pass[row][col] = q_pass[row][col]/passengerRowSum*mp;
      }
      if (driverRowSum > 0) {
        q_driver[row][col] = q_driver[row][col]/driverRowSum*md;
      }
    }
  }

  // add two matrices
  for (row = 0; row < nTypes; row++) {
    for (col = 0; col < nTypes; col++) {
        q_matrix[row][col] = q_driver[row][col] + q_pass[row][col];
    }
  }
  
  // fix diagonal
  for (row = 0; row < nTypes; row++) {
    double rowSum = 0.0;
    for (col = 0; col < nTypes; col++) { rowSum += q_matrix[row][col]; }
    q_matrix[row][row] = 1.0000000000 - rowSum;
  }
	
	// CHANGE NTHREADS
	//long long int x[16] = {0};
	long long int x[16] = {N, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	
	long long int i = 0;
  
		
	// i think this will probably be the same seed each time
  srand ( myid );
  
  
  
  // define everything before time loop
  double f, g, denom, r1, r2, r3, cdf_sum, nextSum;
  double p_birth[16] = {0.0};
  double p_death[16] = {0.0};
  
	int j, birthType, newBirthType;
	
	i = x[11] + x[12] + x[13] + x[14] + x[15];
	
  long long int time;
  for (time = 0; time < totalTime; time++) {

    // now calculate i
    i = x[11] + x[12] + x[13] + x[14] + x[15];
    
    // only update p_birth, p_death every so often
    if (time % 1000000 == 0) {
      // fitness of healthy cells
      f = 1 - w + w*((a*(N - i - 1) + b*i)/(N - 1));
    
      // fitnes of cancer cells
      g = 1 - w + w*((c*(N - i) + d*(i - 1))/(N - 1));
  
      // birth and death
      
      denom = 0.0;
      
      for (j = 0; j < 11; j++) { denom += x[j]*f; }
      for (j = 11; j < 16; j++) { denom += x[j]*g; }
    
      for (j = 0; j < 11; j++) { 
        p_birth[j] = x[j]*f/denom;
        p_death[j] = x[j]/(long double)N;
      }
      
      for (j = 11; j < 16; j++) { 
        p_birth[j] = x[j]*g/denom;
        p_death[j] = x[j]/(long double)N;
      }
    } 	
  	
    // generate 3 rando's
    r1 = (double)rand()/(double)RAND_MAX; //BIRTH
    r2 = (double)rand()/(double)RAND_MAX; // MUTATION
    r3 = (double)rand()/(double)RAND_MAX; // DEATH
    
    // BIRTH
    cdf_sum = p_birth[0];

    if (r1 < cdf_sum) {
        birthType = 0;
    } else {
      for (j = 1; j < 16; j++) {
        birthType = j;
        double nextSum = cdf_sum + p_birth[j];
        
        if ((r1 >= cdf_sum) && (r1 < nextSum)) {
          break;
        }
        cdf_sum = nextSum;
      }
    }

    
    // MUTATION (assume birthType is already calculated
    
    cdf_sum = q_matrix[birthType][birthType];

    // put this guy first, by far the most common scenario when m << 1
    if (r2 < cdf_sum) {
      newBirthType = birthType;
    } else {
      // cycle through only relevant q
      for (bitmask = 1; bitmask < nTypes; bitmask *= 2) {
        newBirthType = birthType ^ bitmask;
        nextSum = cdf_sum + q_matrix[birthType][newBirthType];
        
        if ((r2 >= cdf_sum) && (r2 < nextSum)) {
          break;
        }
        cdf_sum = nextSum;        
      }
    }
    
    birthType = newBirthType;
    
    
    
    // DEATH
    int deathType;
    cdf_sum = p_death[0];

    if (r3 < cdf_sum) {
      deathType = 0;
    } else {
      for (j = 1; j < 16; j++) {
        deathType = j;
        nextSum = cdf_sum + p_death[j];
        
        if ((r3 >= cdf_sum) && (r3 < nextSum)) {
          break;
        }
        cdf_sum = nextSum;
      }
    }


    // this shouldn't happen, but check that
    // we won't kill a cell type of 0 count:
    if (x[deathType] > 0) {
      // enact the next time step        
      x[birthType] += (long long int)1;
      x[deathType] -= (long long int)1;  
    }
    
    if (time % 50000000 == 0) {
      if (myid == 0) {
        printf("%lld, %lld\n", time, i);
      }
      
      // write out first time step
      // myid = [0, 7] because I have 8 processors locally
      switch(myid) {
        // choose which file to open up
        case 0: fp0 = fopen("out0.d","a");
                fprintf(fp0,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp0);
                break;
        case 1: fp1 = fopen("out1.d","a");
                fprintf(fp1,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp1);
                break;
        case 2: fp2 = fopen("out2.d","a");
                fprintf(fp2,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp2);
                break;
        case 3: fp3 = fopen("out3.d","a");
                fprintf(fp3,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp3);
                break;
        case 4: fp4 = fopen("out4.d","a");
                fprintf(fp4,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp4);
                break;
        case 5: fp5 = fopen("out5.d","a");
                fprintf(fp5,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp5);
                break;
        case 6: fp6 = fopen("out6.d","a");
                fprintf(fp6,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp6);
                break;
        case 7: fp7 = fopen("out7.d","a");
                fprintf(fp7,"%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld\n",time,x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]);
                fclose(fp7);  
                break;
      }
    }
    
    
  } // end time loop
	
	// after all is said and done (one node)
	double cpu2 = MPI_Wtime();
	printf("Node %d took time: %f\n", myid, cpu2-cpu1);
	
	MPI_Finalize();
	return 0;
}










