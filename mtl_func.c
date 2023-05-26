
/*
			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                                                MTL_RAW

			+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Function: This program Evalauted MTL operators with time reference from 'NOW' - the current time and continues to evalaute the results for every time unit.


// Developed by P Thirumeni -203085002, PhD, student IIT Dharwad India.

// Under the guidance of PhD. Supervisors, Dr.Rajshekar.K (IIT-Dh), Dr.C.M.Ananda (CSIR-NAL)

//   P THIRUMENI  date :10.05.23

// Functions : G,F,U,R,H,P,S,X,Y

Test G and F in MODE_1

csv1.txt files containts the APs. with time stamp
mtl.txt contains the MTL formula. (simple/single formulas)

both csv1.txt and mtl.txt to be kept in the same directory of the exe file.

/**************************************************************************************************************************************************************/






#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 10000  //
#define BUFFER_2_SIZE 200
#define BUFFER_SIZE_U 1000 // for until
#define BUFFER_SIZE_R 1000 // for release

char line[100];
int num_formulas = 7; // change this to the number of formulas in your file
char* formula_line;
int time, p1, p2, p3, p4, p5;
int fids[BUFFER_2_SIZE], low_times[BUFFER_2_SIZE], up_times[BUFFER_2_SIZE], intervals[BUFFER_2_SIZE];
char letters[BUFFER_2_SIZE];

int buffer_g[BUFFER_SIZE] = {0};
int buffer_f[BUFFER_SIZE] = {0};
int front = 0, rear = 0;
int front_f = 0, rear_f = 0;
int fid;

int buffer_u_p1[BUFFER_SIZE_U];
int buffer_u_p2[BUFFER_SIZE_U];
int buffer_u_timestamp[BUFFER_SIZE_U];
int rear_u_p1 = -1;
int rear_u_p2 = -1;
int U_satisfied_flag =0;
int U_satisfied_ts =0;

int q_full=0;
int flag_q =0;


int buffer_r_p1[BUFFER_SIZE_R];
int buffer_r_p2[BUFFER_SIZE_R];
int buffer_r_timestamp[BUFFER_SIZE_R];
int rear_r_p1 = -1;
int rear_r_p2 = -1;
int R_satisfied_flag =0;
int R_satisfied_ts =0;

int q_full_r=0;
int flag_q_r =0;
int alt_m_r;


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

/*
 printf("\n++++++++++++++++++++++++++++++++++++++++++++++++");
//MTL_G
    int mtl_g_output = MTL_G(p2, send_interval);
    printf("\n");
    printf("\nTimestamp: %d", time);
    printf("\nGlobal.\n");
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
 printf("\n++++++++++++++++++++++++++++++++++++++++++++++++");
    int mtl_f_output = MTL_F(p2, send_interval_F);
    printf("\n");
    printf("\nTimestamp: %d", time);
    printf("\nFinally.\n");
    printf("\nMTL_F Output %d",mtl_f_output);
    printf("\nP3: %d\n", p2);
    printf("MTL_F interval: %d", send_interval_F );
  //  printf("\nMTL_F effective buffer size: %d", rear_f - front_f);
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

*/
    //MTL_U

    //int MTL_U(int p1, int p2, int start_time,int end_time)

   // printf("\nbefore until func. calling ... rear_u_p1=%d,rear_u_p2=%d\n",rear_u_p1,rear_u_p2);
    int u_lowtime=low_times[4];
    int u_hightime=up_times[4];
    printf("\n");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\nTimestamp: %d", time);
    printf("\nUntil.\n");
    int mtl_u_output = MTL_U(p2,p1,u_lowtime,u_hightime);
    printf("\nUntil-low time %d, high time %d",u_lowtime,u_hightime);
    printf("\nMTL_Until Output %d",mtl_u_output);



     //MTL_R

    //int MTL_R(int p1, int p2, int start_time,int end_time)


    int r_lowtime=low_times[6];
    int r_hightime=up_times[6];
    printf("\n");
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++");
    printf("\nTimestamp: %d", time);
    printf("\nRelease.\n");
    //printf("\np: %d, q %d", p1,p2);
    int mtl_r_output = MTL_R(p1,p2,r_lowtime,r_hightime);
    printf("\nRelease - low time %d, high time %d",r_lowtime,r_hightime);
    printf("\nMTL_Release Output %d",mtl_r_output);
    printf("\n\n");
    printf("\n=================================================================================================================================================");


    }  //while ends here




        fclose(fp);




 return 0;
}


// Function definitions here.



// Globally function


int MTL_G(int p, int interval) {
    int i;

    buffer_g[rear] = p;
    rear = (rear + 1) % BUFFER_SIZE;

    if ( ((rear + BUFFER_SIZE - front) % BUFFER_SIZE) > interval) {
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


// Finally funtion

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



// Until function.
//#define BUFFER_SIZE interval



// Until function.
//#define BUFFER_SIZE interval



int MTL_U(int p1, int p2, int start_time,int end_time)


{

   int interval =(end_time-start_time)+1;
   printf("\nInterval = %d",interval);
   printf("\nCurrent value of p:%d, q:%d", p2,p1);

   rear_u_p1 = (rear_u_p1 + 1) % interval;
   buffer_u_p1[rear_u_p1] = p1;

   rear_u_p2 = (rear_u_p2 + 1) % interval;
   buffer_u_p2[rear_u_p2] = p2;

   buffer_u_timestamp[rear_u_p2] = time;


   // printf("\n U_satisfied_flag =%d, q_full=%d,",U_satisfied_flag,q_full);

 //   printf("\nrear_u_p1=%d,rear_u_p2=%d, interval = %d",rear_u_p1,rear_u_p2,interval);

    if ((rear_u_p1 == (interval-1)) && (rear_u_p2 == (interval-1)) && (flag_q == 0) )

    {
       printf("\n*****************************************");
       printf("\n\n First time Until_Que_full\n\n");
       printf("\n*****************************************");
       q_full =1;
       flag_q =1;
    }
    else if (q_full == 0)

    {
            printf ("\n[][][][][][][][]WAIT to fill.....Until_Que,q_full =%d ",q_full);

    }

         if (q_full == 1)

         {

        // compare p1 p2 for until condition here for every time stamp once q filled..........

        U_satisfied_flag=0;

   //     printf("\n Before - U_satisfied_flag =%d, q_full=%d,",U_satisfied_flag,q_full);

        //printf("\nUntil evaluation range , Row#1:Time stamp,Row#2: p1,Row#3: p2");
         printf("\n\nTime stamp:");
         for (int m1=0; m1<=interval-1 ;m1++)

         {
         printf("%d,",buffer_u_timestamp[m1]);
         }

         printf("\n         p:");

          for (int m11=0; m11<=interval-1 ;m11++)

         {
         printf(" %d,",buffer_u_p1[m11]);
         }

          printf("\n         q:");

          for (int m111=0; m111<=interval-1 ;m111++)

         {
         printf(" %d,",buffer_u_p2[m111]);
         }

         printf("\n");




         // Find the lowest timestamp index in the buffer for iteration
    int lowestIndex,highestIndex,currentIndex;
        highestIndex= rear_u_p2;
        lowestIndex = ((highestIndex+1)%interval);
        currentIndex = lowestIndex;
        int co=0;

        printf("Until evaluation for the time range from =%d to =%d ", buffer_u_timestamp[lowestIndex],buffer_u_timestamp[highestIndex]);

        while (1 == 1)

            {


          //  printf("\n Until current index =%d ", currentIndex);

        //    printf("\n count BEFORE INCREMENT...= %d,", co);


            if ((buffer_u_p1[currentIndex] == 1) &&  (buffer_u_p2[currentIndex] == 1) && (U_satisfied_flag ==0))
            {

          //  printf ("\nInside p 1 q1 result 1");
            U_satisfied_ts = (buffer_u_timestamp[currentIndex]);
            U_satisfied_flag =1;
            printf ("\nUntil SATISFIED at %d,", U_satisfied_ts);
            printf ("\nUntil satisfied since p=1 and q=1");
            printf ("\nResult : Until at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_u_timestamp[highestIndex]-end_time), buffer_u_timestamp[lowestIndex],buffer_u_timestamp[highestIndex]);
            printf ("\nUNTIL_OUTPUT = 1");
            return 1;
            }

            else if ((buffer_u_p1[currentIndex] == 0) &&  (buffer_u_p2[currentIndex] == 1) && (U_satisfied_flag ==0))
            {
        //    printf ("\nInside p 0 q1 result 1 ");
            U_satisfied_ts = (buffer_u_timestamp[currentIndex]);
            U_satisfied_flag =1;
            printf ("\nUntil SATISFIED at %d,", U_satisfied_ts);
            printf ("\nUntil satisfied since p=0 and q=1");
            printf ("\nResult : Until at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_u_timestamp[highestIndex]-end_time), buffer_u_timestamp[lowestIndex],buffer_u_timestamp[highestIndex]);
            printf ("\nUNTIL_OUTPUT = 1");
            return 1;
            }

            else if ((buffer_u_p1[currentIndex] == 0) &&  (buffer_u_p2[currentIndex] == 0) && (U_satisfied_flag ==0))
            {
        //    printf ("\nInside p 0 q 0 result 0");
            U_satisfied_ts = (buffer_u_timestamp[currentIndex]);
            U_satisfied_flag =0;
            printf ("\nUntil FAILED at %d,", U_satisfied_ts);
            printf ("\nUntil failed since p=0 and q=0 at time stamp %d ",U_satisfied_ts);
            printf ("\nResult : Until at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_u_timestamp[highestIndex]-end_time), buffer_u_timestamp[lowestIndex],buffer_u_timestamp[highestIndex]);
            printf ("\nUNTIL_OUTPUT = 0");
            return 0;
            }

            else if ((buffer_u_p1[currentIndex] == 1) &&  (buffer_u_p2[currentIndex] == 0) && (U_satisfied_flag ==0))
            {

        //    printf ("\nInside p 1 q 0 result -1");
            U_satisfied_ts = (buffer_u_timestamp[currentIndex]);
            U_satisfied_flag =0;
            printf ("\nUntil MAYBE..at time stamp...%d,", U_satisfied_ts);
            //return -1;
                if (U_satisfied_ts == buffer_u_timestamp[highestIndex])

                {
            U_satisfied_ts = (buffer_u_timestamp[currentIndex]);
            U_satisfied_flag =0;
            printf ("\n current time stamp = %d, highest time stamp = %d ",U_satisfied_ts,buffer_u_timestamp[highestIndex]);
            printf ("\nUntil FAILED at %d,", U_satisfied_ts);
            printf ("\nUntil failed since p=1 and q=0 for the entire given time range");
            printf ("\nResult : Until at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_u_timestamp[highestIndex]-end_time), buffer_u_timestamp[lowestIndex],buffer_u_timestamp[highestIndex]);
            printf ("\nUNTIL_OUTPUT = 0");
            return 0;
                }
            }

            else if (U_satisfied_flag ==1)
            {
            printf ("\nUntil Already satisfied...");
            return -1;
            }
            ////////////////iteration technic here...
            currentIndex = (currentIndex + 1) % interval; // update current index to point to next higher time stamp with wraparound.
        //    printf ("\n AFTER updated CURRENT INDEX currentIndex = %d....",currentIndex);
            if (currentIndex == (highestIndex + 1) % interval) ////// exits when index is lowvalue_index-1 with modulo
           {
        //    printf ("\nInside break condition acheived, current index %d and high index %d",currentIndex,highestIndex);
            break; // BREAK WHILE LOOP
            }
            co=co+1;
        //     printf ("\n count AFTER INCREMENT %d..",co);

              }//while loop ends.


             } // q_full if loop ends.


          // return -1;

            } //Until function ends



//Release




int MTL_R(int p1, int p2, int start_time,int end_time)


{



   int interval =(end_time-start_time)+1;
   printf("\nSTART TIME  %d, END TIME %d Interval = %d",start_time,end_time,interval);
   printf("\nInterval = %d",interval);
   printf("\nCurrent value of p:%d, q:%d", p2,p1);

   rear_r_p1 = (rear_r_p1 + 1) % interval;
   buffer_r_p1[rear_r_p1] = p1;

   rear_r_p2 = (rear_r_p2 + 1) % interval;
   buffer_r_p2[rear_r_p2] = p2;

   buffer_r_timestamp[rear_r_p2] = time;


   // printf("\n R_satisfied_flag =%d, q_full_r=%d,",R_satisfied_flag,q_full_r);

    printf("\nrear_r_p1=%d,rear_r_p2=%d, interval = %d, flag_q_r=%d",rear_r_p1,rear_r_p2,  interval,flag_q_r);

    if ((rear_r_p1 == (interval-1)) && (rear_r_p2 == (interval-1)) && (flag_q_r == 0) )

    {
       printf("\n*****************************************");
       printf("\n\n First time Release_Que_full\n\n");
       printf("\n*****************************************");

       q_full_r =1;
       flag_q_r =1;
    }
    else if (q_full_r == 0)

    {
            printf ("\n    [][][][][][][][] WAIT to fill.....Release_Que,q_full_r =%d ",q_full_r);

    }

         if (q_full_r == 1)

         {

        // compare p1 p2 for Release condition here for every time stamp once q filled..........

        R_satisfied_flag=0;

   //     printf("\n Before - R_satisfied_flag =%d, q_full_r=%d,",R_satisfied_flag,q_full_r);

        //printf("\nRelease evaluation range , Row#1:Time stamp,Row#2: p1,Row#3: p2");
         printf("\n\nTime stamp:");
         for (int m1=0; m1<=interval-1 ;m1++)

         {
         printf("%d,",buffer_r_timestamp[m1]);
         }

         printf("\n         p:");

          for (int m11=0; m11<=interval-1 ;m11++)

         {
         printf(" %d,",buffer_r_p1[m11]);
         }

          printf("\n         q:");

          for (int m111=0; m111<=interval-1 ;m111++)

         {
         printf(" %d,",buffer_r_p2[m111]);
         }

         printf("\n");




         // Find the lowest timestamp index in the buffer for iteration
    	int lowestIndex,highestIndex,currentIndex;
        highestIndex= rear_r_p2;
        lowestIndex = ((highestIndex+1)%interval);
        currentIndex = lowestIndex;
        int co=0;

        printf("Release evaluation for the time range from =%d to =%d ", buffer_r_timestamp[lowestIndex],buffer_r_timestamp[highestIndex]);

        while (1 == 1)

            {


          //  printf("\n Release current index =%d ", currentIndex);

         //    printf("\n count BEFORE INCREMENT...= %d,", co);


            if ((buffer_r_p1[currentIndex] == 1) &&  (buffer_r_p2[currentIndex] == 1) && (R_satisfied_flag ==0))// p=1 q=1
            {

         //   printf ("\nInside p 1 q1 result 1");
            R_satisfied_ts = (buffer_r_timestamp[currentIndex]);
            R_satisfied_flag =1;
            printf ("\nRelease SATISFIED at %d,", R_satisfied_ts);
            printf ("\nRelease satisfied since p=1 and q=1");
            printf ("\nResult : Release at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_r_timestamp[highestIndex]-end_time), buffer_r_timestamp[lowestIndex],buffer_r_timestamp[highestIndex]);
            printf ("\nRelease_OUTPUT = 1");
            return 1;
            }

            else if ((buffer_r_p1[currentIndex] == 1) &&  (buffer_r_p2[currentIndex] == 0) && (R_satisfied_flag ==0)) //p=1 q=0
            {
         //   printf ("\nInside p1 q0 result 0 ");
            R_satisfied_ts = (buffer_r_timestamp[currentIndex]);
            R_satisfied_flag =0;
            printf ("\nRelease FAILED at %d,", R_satisfied_ts);
            printf ("\nRelease FAILED since p=1 and q=0 i.e. p holds but q does not hold at time stamp %d ",R_satisfied_ts);
            printf ("\nResult : Release at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_r_timestamp[highestIndex]-end_time), buffer_r_timestamp[lowestIndex],buffer_r_timestamp[highestIndex]);
            printf ("\nRelease_OUTPUT = 0");
            return 0;
            }

            else if ((buffer_r_p1[currentIndex] == 0) &&  (buffer_r_p2[currentIndex] == 0) && (R_satisfied_flag ==0))
            {
        //    printf ("\nInside p 0 q 0 result 0");
            R_satisfied_ts = (buffer_r_timestamp[currentIndex]);
            R_satisfied_flag =0;
            printf ("\nRelease FAILED at %d,", R_satisfied_ts);
            printf ("\nRelease failed since p=0 and q=0 at time stamp %d ",R_satisfied_ts);
            printf ("\nResult : Release at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_r_timestamp[highestIndex]-end_time), buffer_r_timestamp[lowestIndex],buffer_r_timestamp[highestIndex]);
            printf ("\nRelease_OUTPUT = 0");
            return 0;
            }

            else if ((buffer_r_p1[currentIndex] == 0) &&  (buffer_r_p2[currentIndex] == 1) && (R_satisfied_flag ==0)) // p0 q1
            {

       //     printf ("\nInside p0 q1 result 0");
            R_satisfied_ts = (buffer_r_timestamp[currentIndex]);
            R_satisfied_flag =0;
            printf ("\nRelease MAYBE..at time stamp...%d,", R_satisfied_ts);
            //return -1;
                if (R_satisfied_ts == buffer_r_timestamp[highestIndex])

                {
            R_satisfied_ts = (buffer_r_timestamp[currentIndex]);
            R_satisfied_flag =1;
            printf ("\nRelease SATISFIED at %d,", R_satisfied_ts);
            printf ("\nRelease satisfied since p=0 and q=1 throughout the given time range");
            printf ("\nResult : Release at TIME STAMP = %d, for the evaluation range start_time = %d  to  end_time = %d",(buffer_r_timestamp[highestIndex]-end_time), buffer_r_timestamp[lowestIndex],buffer_r_timestamp[highestIndex]);
            printf ("\nRelease_OUTPUT = 1");
            return 1;
                }
            }

            else if (R_satisfied_flag ==1)
            {
            printf ("\nRelease Already satisfied...");
            return -1;
            }
            ////////////////iteration technic here...
            currentIndex = (currentIndex + 1) % interval; // update current index to point to next higher time stamp with wraparound.
        //    printf ("\n AFTER updated CURRENT INDEX currentIndex = %d....",currentIndex);
            if (currentIndex == (highestIndex + 1) % interval) ////// exits when index is lowvalue_index-1 with modulo
           {
          //   printf ("\nInside break condition acheived, current index %d and high index %d",currentIndex,highestIndex);
            break; // BREAK WHILE LOOP
            }
            co=co+1;
          //   printf ("\n count AFTER INCREMENT %d..",co);

              }//while loop ends.


             } // q_full_r if loop ends.


        //   return -1;

            } //Release function ends
