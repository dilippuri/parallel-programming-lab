#include <stdio.h>
FILE *fp;
int main(void){
fp = fopen("output_serial.txt","a+");
int i,j,k,n;
    //n=3;
   // float A[3][3] = { { 1, 1, 1 }, { 1, 2, 2 }, { 1, 2, 3 } }, 
   float L[n][n], U[n][n];//initializing matrices

if(argc !=4){
        printf("Enter the size of matrix (N x N) where N = ");
        scanf("%lu",&matrix_size);

        printf("Enter the version number V = ");
        scanf("%lu",&version);
    }
    else{
        matrix_size=atol(argv[1]);
        version=atol(argv[2]);
    }
    long num_threads=atol(argv[3]);
    if(num_threads<1){
        num_threads=5;

    }
long i, j;
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            if(i<=j )
                A[i][j]=i+1;
            else
                A[i][j]=j+1;

        }
    }

  /*  printf("Enter matrix element:\n");

    for(i=0; i<n; i++)  //matrix input from console
    {
        for(j=0; j<n; j++)
        {
            printf("Enter A[%d][%d] element: ", i,j);
            scanf("%f",&A[i][j]);
        }
    }*/
    //DooLittle's Algo.
        clock_t begin, end;
    double time_spent;
    begin = clock();

    for(j=0; j<n; j++)  //over row element
    {
        for(i=0; i<n; i++)//over column element
        {
            if(i<=j)      //check for diagonal and lower elements
            {
                U[i][j]=A[i][j];
                for(k=0; k<=i-1; k++)
                    U[i][j] = U[i][j] - L[i][k]*U[k][j];
                if(i==j)
                    L[i][j]=1;
                else
                    L[i][j]=0;
            }
            else          //check for upper elements
            {
                L[i][j]=A[i][j];
                for(k=0; k<=j-1; k++)
                    L[i][j]= L[i][j] - L[i][k]*U[k][j];
                L[i][j] = L[i][j] / U[j][j];
                U[i][j]=0;
            }
        }
    }
    end = clock();
    time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;
        fprintf(fp,"%ld     ", matrix_size);
        fprintf(fp,"%ld       ",num_threads);
        fprintf(fp,"%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);


    printf("[L]: \n");
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
            printf("%9.3f",L[i][j]);
        printf("\n");
    }
    printf("\n\n[U]: \n");
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
            printf("%9.3f",U[i][j]);
        printf("\n");
    }

return 0;
}
