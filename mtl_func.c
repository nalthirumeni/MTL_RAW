
/*
			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                                                MTL_RAW

			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Function: This program Evalauted MTL operators with time reference from 'NOW' - the current time and continues to evalaute the results for every time unit.


// Developed by P Thirumeni -203085002, PhD, student IIT Dharwad India.

// Under the guidance of PhD. Supervisors, Dr.Rajshekar.K (IIT-Dh), Dr.C.M.Ananda (CSIR-NAL)

//   P THIRUMENI  date :10.05.23

// Functions : G,F,U,W,H,P,S,X,Y

Test G and F in MODE_1

csv1.txt files containts the APs. with time stamp
mtl.txt contains the MTL formula. (simple/single formulas)

both csv1.txt and mtl.txt to be kept in the same directory of the exe file.

/**************************************************************************************************************************************************************/






#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10000  // 
#define BUFFER_2_SIZE 20

char line[100];
int num_formulas = 6; // change this to the number of formulas in your file
char* formula_line;
int time, p1, p2, p3, p4, p5;
int fids[BUFFER_2_SIZE], low_times[BUFFER_2_SIZE], up_times[BUFFER_2_SIZE], intervals[BUFFER_2_SIZE];
char letters[BUFFER_2_SIZE];

int buffer_g[BUFFER_SIZE] = {0};
int buffer_f[BUFFER_SIZE] = {0};
int front = 0, rear = 0;
int front_f = 0, rear_f = 0;
int fid;


//Function declarations here.


int MTL_G(int p, int interval);
int MTL_F(int p, int interval);


// Main function here

int main() {


     printf("\n\n");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n                                               MTL_RAW ONLINE MONITORING ");
    printf("\n                              +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\n\n");


FILE* fp;

fp = fopen("mtl.txt", "r");

if (fp == NULL) {
    printf("Error opening file\n");
    return 1;
}
printf("\n\nReading MTL formula from mtl.txt \n\n");
for (int i = 0; i < num_formulas; i++) {
    // read formula line from file
    fgets(line, sizeof(line), fp);
    formula_line = strtok(line, "\n");

    // parse formula line and store values in arrays
    char letter;
    sscanf(formula_line, "Formula ID %d : %c[%d,%d]", &fids[i], &letter, &low_times[i], &up_times[i]);
    intervals[i] = up_times[i] - low_times[i] + 1;
    letters[i] = letter;
}

fclose(fp);

// print parsed data for each formula
for (int i = 0; i < num_formulas; i++) {

 //   printf("i %d\n", i+1);
    printf("MTL Formula %d: Formula ID %d : %c[%d,%d]\n", i+1, fids[i], letters[i], low_times[i], up_times[i]);
    printf("Formula %d Interval: [%d,%d] (%d time units)\n", fids[i], low_times[i], up_times[i], intervals[i]);
}
printf("\nAll MTL formuals read.\n");
// reading mtl formual completed


// read csv from now

    fp = fopen("csv1.txt", "r");

    if (fp == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("\ncsv1 opened \n");
    // skip header line
    fgets(line, sizeof(line), fp);

    int i;
    i=0; // for globally
    int send_interval = intervals[i];
    int send_interval_F = intervals[i+1];//finally

    while (fgets(line, sizeof(line), fp))

    {


    sscanf(line, "%d,%d,%d,%d,%d,%d", &time, &p1, &p2, &p3, &p4, &p5);



//MTL_G
    int mtl_g_output = MTL_G(p2, send_interval);
    printf("\n");
    printf("\nTimestamp: %d", time);
    printf("\nMTL_G Output %d",mtl_g_output);
    printf("\nP2: %d\n", p2);
    printf("MTL_G interval: %d", send_interval );
    printf("\nMTL_G effective buffer size: %d", rear - front);
  //  printf("Upper time limit: %d\n", up_times[i]);

      if (time < up_times[i])
            {
             printf("\nNOT ENOUGH INFO. G_WAIT  for Formula ID %d",fids[i]);
            }


        if (time >= up_times[i])

            {

//            printf("\n intervals[i] %d,intervals[i]+1 %d,fids[i] %d,((time-intervals[i])+1) %d", intervals[i],intervals[i]+1,fids[i],((time-intervals[i])+1));
            printf("\nGlobally output for Timestamp=%d and Formula ID %d for the input values of p for evaluation timestamp range (%d to %d) => Output = %d\n",  time-up_times[i],fids[i],((time-intervals[i])+1),time,mtl_g_output);
            int jj;

            for ( jj = front; jj < rear; jj++)
            {
            printf("%d, ", buffer_g[jj]);
            }

            }



//MTL_F

    int mtl_f_output = MTL_F(p2, send_interval_F);
    printf("\n");
    printf("\nTimestamp: %d", time);
    printf("\nMTL_F Output %d",mtl_f_output);
    printf("\nP3: %d\n", p2);
    printf("MTL_F interval: %d", send_interval_F );
    printf("\nMTL_F effective buffer size: %d", rear_f - front_f);
    // printf("\nup_times[i+1] %d", up_times[i+1]);


      if (time < up_times[i+1])
            {
             printf("\nNOT ENOUGH INFO. F_WAIT  for Formula ID %d",fids[i+1]);
            }


        if (time >= up_times[i+1])

            {
            printf("\nFinally output for Timestamp=%d and Formula ID %d for the input values of p for evaluation timestamp range (%d to %d) => Output = %d\n", time-up_times[i+1],fids[i+1],(time-(intervals[i+1])+1),time,mtl_f_output);

            int jjj;

            for ( jjj = front_f; jjj < rear_f; jjj++)
            {
            printf("%d, ", buffer_f[jjj]);
            }

            }


    }  //while ends here


        fclose(fp);




 return 0;
}


// Function definitions here.


int MTL_G(int p, int interval) {
    int i;

    buffer_g[rear] = p;
    rear = (rear + 1) % BUFFER_SIZE;

    if ((rear + BUFFER_SIZE - front) % BUFFER_SIZE > interval) {
        front = (front + 1) % BUFFER_SIZE;
    }

    if ((rear + BUFFER_SIZE - front) % BUFFER_SIZE == interval) {
        for (i = front; i != rear; i = (i + 1) % BUFFER_SIZE) {
            if (buffer_g[i] == 0) {
                return 0;
            }
        }

        return 1;
    } else {
        return -1;
    }
}



int MTL_F(int p, int interval) {
    int i;

    buffer_f[rear_f] = p;
    rear_f = (rear_f + 1) % BUFFER_SIZE;

    if ((rear_f + BUFFER_SIZE - front_f) % BUFFER_SIZE > interval) {
        front_f = (front_f + 1) % BUFFER_SIZE;
    }

    if ((rear_f + BUFFER_SIZE - front_f) % BUFFER_SIZE == interval) {
        int exists_one = 0;
        for (i = front_f; i != rear_f; i = (i + 1) % BUFFER_SIZE) {
            if (buffer_f[i] == 1) {
                exists_one = 1;
                break;
            }
        }

        if (exists_one) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

