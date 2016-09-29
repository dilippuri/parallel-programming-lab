#include<stdio.h>

int main(void){

    int i,j,k,n;
    n=3;
    float A[3][3] = { { 1, 2, 3 }, { 3, 4, 5 }, { 5, 6, 7 } }, L[n][n], U[n][n];//initializing matrices

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
