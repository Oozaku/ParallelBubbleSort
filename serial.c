#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void bubble(int *A, int n){
    int ini = 0, end = n - 1, i, sorted = 0;
    while ((sorted != 1) && (ini < end)){
        i = ini;
        sorted = 1;
        while (i < end){
            if (A[i] > A[i+1]){
                sorted = 0;
                int aux = A[i];
                A[i] = A[i+1];
                A[i+1] = aux;
            }
            i++;
        }
        end--;
    }
}

int main(){
    int n, *A;
    scanf("%d",&n);
    A = (int*) malloc(n*sizeof(int));
    int i;
    for (i=0;i<n;i++){
        scanf("%d",&A[i]);
    }
    double start = omp_get_wtime();
    bubble(A,n);
    double end = omp_get_wtime();
    for (i=0;i<n;i++)
        printf("%d, ", A[i]);
    printf("\ntime spent = %lf\n",end - start);
    return 0;
}
