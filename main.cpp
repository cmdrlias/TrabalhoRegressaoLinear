#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <fstream>
#include <iomanip>

#define MAX_SIZE	100

using namespace std;

// ----- variáveis globais ----- \\

int i, j, pont = 0;
float **mat_xy;
ofstream outFile;

//---------------------------------- método para preenchimento da matriz mat_xy -------------------------------------------\\

void MatrizPontos(int pont) {
    
    mat_xy = (float**)malloc(MAX_SIZE * sizeof(float*));

    for (i = 0; i < MAX_SIZE; i++){
        mat_xy[i] = (float*) malloc(MAX_SIZE * sizeof(float));
        for (j = 0; j < MAX_SIZE; j++){
             mat_xy[i][j] = 0;
        }
    }
    
    printf("\nPreencha a matriz com os valores de X e Y\n\n");
    
    for (i = 0; i < 2; i++) {
		
        for (j = 0; j < pont; j++) {		
            if (i == 0)	{
        	printf("X%d: ", j+1);
		scanf("%f", &mat_xy[i][j]);
            } else if (i == 1) {
            	printf("Y%d: ", j+1);
                scanf("%f", &mat_xy[i][j]);
            }
			
	}
        printf("\n\n");
    }		
}

//------------------------------------- método para calculo do valor de U ----------------------------------------------\\

float U(float& res, float& xy, float& x, float& y) {

    float u = 0;
    for(i = 0; i < pont; i++) {
        u += res + (xy * mat_xy[0][i]);
    }
    
    return u;
	
}

float U_lote(float& xy, float& x, float vetor[]) {
    
    float u;
    for(i = 0; i < pont; i++) {
        u += vetor[i] + (xy * mat_xy[0][i]);
    }
    
    return u;
}

//------------------------------------ método para calculo do valor de XY ----------------------------------------------\\

float XY(float& x, float& x1, float& y, float& y1) {

    float xy;
    xy = (y1 - y)/(x1 - x);

    return xy;
	
}

float XY_lote(float& x, float& x1, float& y, float& y1, float& passo) {
    
    float xy;
    xy = (y1 - (y + passo)) / (x1 - (x + passo));
    
    return xy;
}

//----------------------------- função para cálculo da qualidade de ajuste D(manual) ------------------------------------\\

float D(int& pont, float& u, float& xy, float& y) {

    float ux, di, tot_D, total = 0;
    float *vet_d;
	
    vet_d = (float *) malloc (pont * sizeof(float));
		
    printf("\nu(x) = %.2f + %.2fx\n\n", u, xy);
	
    printf("i  \t\tXi  \t\tYi  \t\tUi  \t\tDi\n\n");
	
    for(j = 0; j < pont; j++) {	
        ux = y + (xy * mat_xy[0][j]);
        di = mat_xy[1][j] - ux; 
        tot_D = pow(di, 2); 
        total += tot_D; 
		
        printf("%d\t\t%.1f\t\t%.1f\t\t%.2f\t\t%.2f\n", j + 1, mat_xy[0][j], mat_xy[1][j], ux, di);
    }
    
    printf("\nD = %.3f\n\n", total);
        
    return total;	
}

//------------------------------- função para cálculo da qualidade de ajuste D(lote) -----------------------------------\\

float D_lote(float vetor[], float& xy_lote, int& pont, float& passo, float& u) {
    
    float ux, di, tot_D, total, t_passo = 0;
    
    for (i = 0; i < pont; i++) {
        
        ux = vetor[i] + (xy_lote * mat_xy[0][i]);
        di = mat_xy[1][i] - u; 
        tot_D = pow(di, 2); 
        total += tot_D;

        t_passo = t_passo + passo;
        
        outFile << t_passo << "\t" << mat_xy[0][i] << "\t" <<  mat_xy[1][i] << "\t" << ux << "\t" << setprecision(2) << tot_D  << "\r\n" << endl;
    }
    
    outFile << "\r\nD = " << total << "\r\n\r\n" << endl;
    
    return total;
}

// ------------------------------------------------------- MAIN --------------------------------------------------------- \\

int main(int argc, char** argv) {
        
    printf("----------- TRABALHO ----- REGRESSÃO LINEAR ----------\n");
	
	float x, x1, y, y1, u, xy, xy_lote, res;
	int opc;
        
	printf("\nDigite a quantidade de pontos desejada: ");
	scanf("%d", &pont);
		
	MatrizPontos(pont);
        
        printf("Tipo de calculo a ser efetuado\n\n\t1. MANUAL\t2.EM LOTE");
        scanf("%d", &opc);
        
	switch(opc) {
            
            //---------------------------------------- cálculo manual -------------------------------------------------\\

            case 1:
                printf("\n---------------- PRIMEIRA TENTATIVA ------------------\n");
		printf("\nDigite os pontos finais e iniciais de X e Y que deseja utilizar:");
		
		printf("\n\nX inicial: ");
		scanf("%f", &x);
		
		printf("X Final: ");
		scanf("%f", &x1);
		
		printf("\nY inicial: ");
		scanf("%f", &y);
		
		printf("Y Final: ");
		scanf("%f", &y1);
                
		res = y + xy * (-x);
                
		xy = XY(x, x1, y, y1);
                u = U(res, xy, x, y);
		
		float val1;
                val1 = D(pont, u, xy, y);
                
                // ----------------------------------------------------------------------------------------------------- \\
                
                printf("\n---------------- SEGUNDA TENTATIVA ------------------\n");
		
		printf("\nDigite os pontos finais e iniciais de X e Y que deseja utilizar:");
		
		printf("\n\nX inicial: ");
		cin >> x;
		
		printf("X Final: ");
		cin >> x1;
		
		printf("\nY inicial: ");
		cin >> y;
		
		printf("Y Final: ");
		cin >> y1;
		
		xy = XY(x, x1, y, y1);
                res = y + xy * (-x);
                
		u = U(res, xy, x, y);
                
		float val2;
                val2 = D(pont, u, xy, y);
                
                if(val1 < val2) {
                    printf("\n\nA primeiera tentativa possui o melhor valor de D: %.3f\n", val1);
                } else {
                    printf("\n\nA segunda tentativa possui o melhor valor de D: %.3f\n", val2);
                }
                break;
            
            //---------------------------------------- cálculo em lote -----------------------------------------------\\

            case 2: 
                float passo = 0;
                
                printf("\nX inicial: ");
                cin >> x;
                printf("X final: ");
                cin >> x1;
                printf("\nY inicial: ");
                cin >> y;
                printf("Y final: ");
                cin >> y1;
                printf("\nPasso: ");
                cin >> passo;

                xy_lote = XY_lote(x, x1, y, y1, passo);

                float *vetor = (float*) malloc (256);
                float *total_d = (float*) malloc (256);
                int l = 0;
                
                //----------------------- Cálculo em lote e armazenamento em arquivo .xls -------------------------------\\
                
                outFile.open("Lote.xls");

                for(float k = 0; k < x1; k++) {
                    for(i = 0; i < pont; i++) {
                        vetor[i] = y + ((xy_lote) * (- x));
                    }
                    float u_lote = U_lote(xy, x, vetor);
                    total_d[l] = D_lote(vetor, xy_lote, pont, passo, u_lote);
                    
                    x += passo;
                    y += passo;
                    l++;
                }
                
                // -------------------------------- definir menor valor de D ------------------------------------------- \\
                
                float menor_d = total_d[0];
                int m = 0;
                while(m < l) {
                    if(total_d[m] < menor_d) 
                        menor_d = total_d[m];
                    m++;
                }
                
                outFile << "\r\nMelhor valor de D: " << setprecision(3) << menor_d << endl;
                outFile.close(); 
                
                printf("Todos os cálculos foram salvos no arquivo .xls");
                
                break;
	}
        printf("\n\n\n\xa9Programa desenvolvido por Larissa Silva e Leonora de Oliveira\n\n");
	return 0;
}

/*
 * 
 * © Larissa Silva
                    *
                    *
                    */