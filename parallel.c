#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void swap(int *A, int i, int j){
    int aux = A[i];
    A[i] = A[j];
    A[j] = aux;
}

void bubble(int *A, int n){
    int ini = 0, end = n - 1, now = 0;
    int **check = (int**)malloc((n+1)*sizeof(int*));
    int k;
    for (k=0;k<(n+1);k++)
        check[k] = (int*)malloc(n*sizeof(int));
#   pragma omp parallel default(none) shared(A,n,ini,end,now,check)
    {
#       pragma omp single nowait
        {
            while (ini < end){
                int state = now, i, end2 = end;
#               pragma omp task firstprivate(state,end2) private(i)
                {
                    i = ini;
                    while (i < end2){
                        if ((state == 0) && (i == 0)){
                            int j = i;
#                           pragma omp task depend(out: check[state][j]) firstprivate(j)
                            {
                                check[state][j] = 1;
                                if (A[j] > A[j+1]){
                                    swap(A,j,j+1);
                                }
                            }
                        }
                        else if ((state == 0) && (i > 0)){
                            int j = i;
#                           pragma omp task depend(in: check[state][j-1]) \
                            depend(out: check[state][j]) firstprivate(j)
                            {
                                check[state][j] = 1;
                                if (A[j] > A[j+1]){
                                    swap(A,j,j+1);
                                }
                            }
                        }
                        else if ((state != 0) && (i == 0)){
                            int j = i;
#                           pragma omp task depend(in: check[state-1][j+1]) \
                            depend(out: check[state][j]) firstprivate(j)
                            {
                                check[state][j] = 1;
                                if (A[j] > A[j+1]){
                                    swap(A,j,j+1);
                                }
                            }
                        }
                        else if ((state != 0) && (i > 0)) {
                            int j = i;
#                           pragma omp task depend(in: check[state-1][j+1]) \
                            depend(out: check[state][j]) firstprivate(j)
                            {
                                check[state][j] = 1;
                                if (A[j] > A[j+1]){
                                    swap(A,j,j+1);
                                }
                            }
                        }
                        i++;
                    }
                }
                end--;
                now++;
            }
        }
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
