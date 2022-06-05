#include <stdio.h>
#include <math.h>
#define SIZE 12
#define E_CONSTANT 2.71828192
#define PI 3.14159265

/*
notação que eu adotei para as variáveis y_(classe)_f_(coluna)_(informação) 
y_a_f_a_sum é a soma das linhas da classe a (0), coluna a.
*/
int naive_bayes(double df[SIZE][3], double classe_a[SIZE][3], double classe_b[SIZE][3], double x1, double x2);
double gaussian_probability(double x, double media, double desvio_padrao);

int main(){
    double classe_a[SIZE][3], classe_b[SIZE][3];
    //a última coluna é a etiqueta da classe
    double data[SIZE][3] = {
        {2 ,15, 0},
        {40, 2, 1},
        {40.2, 5, 1},
        {1, 20, 0},
        {2, 15, 0},
        {43, 3, 1},
        {39.98, 0.3, 1},
        {41.02, 0.05, 1},
        {38.777, 5, 1},
        {5, 9, 0},
        {6, 10, 0},
        {3.39, 9.78, 0}
    };

    double x_1_predict = 41.0;
    double x_2_predict = 1.0;
    int previsao = naive_bayes(data, classe_a, classe_b, x_1_predict, x_2_predict);
    printf("classe prevista: %d", previsao);
    return 0;
}

int naive_bayes(double df[SIZE][3], double classe_a[SIZE][3], double classe_b[SIZE][3], double x1, double x2){

    int class_a_count = 0, class_b_count = 0;
    double y_a_f_a_sum = 0;
    double y_a_f_b_sum = 0;

    double y_b_f_a_sum = 0;
    double y_b_f_b_sum = 0;

    for(int c = 0; c<SIZE; c++){
        if(df[c][2]==1){
            classe_b[class_b_count][0] = df[c][0];
            classe_b[class_b_count][1] = df[c][1];
            class_b_count++;

            y_b_f_a_sum += df[c][0];
            y_b_f_b_sum += df[c][1];




        } else if(df[c][2]==0){
            classe_a[class_a_count][0] = df[c][0];
            classe_a[class_a_count][1] = df[c][1];
            class_a_count++;

            y_a_f_a_sum += df[c][0];
            y_a_f_b_sum += df[c][1];

        }
    }
    //media
    double y_a_f_a_media = y_a_f_a_sum/(double)class_a_count;
    double y_a_f_b_media = y_a_f_b_sum/(double)class_a_count;

    double y_b_f_a_media = y_b_f_a_sum/(double)class_b_count;
    double y_b_f_b_media = y_b_f_b_sum/(double)class_b_count;

    
    double y_a_f_a_dev = 0;
    double y_a_f_b_dev = 0;

    double y_b_f_a_dev = 0;
    double y_b_f_b_dev = 0;

    for(int c=0; c<class_a_count; c++){
        y_a_f_a_dev += pow(classe_a[c][0]-y_a_f_a_media, 2);
        y_a_f_b_dev += pow(classe_a[c][1]-y_a_f_b_media, 2);

    }
    for(int c=0; c<class_b_count; c++){
        y_b_f_a_dev += pow(classe_b[c][0]-y_b_f_a_media, 2);
        y_b_f_b_dev += pow(classe_b[c][1]-y_b_f_b_media, 2);

    }
    
    
    //desvio padrão
    double y_a_f_a_desvio_padrao = sqrt(y_a_f_a_dev/((double)class_a_count-1 ));
    double y_a_f_b_desvio_padrao= sqrt(y_a_f_b_dev/((double)class_a_count-1 ));

    double y_b_f_a_desvio_padrao = sqrt(y_b_f_a_dev/((double)class_b_count-1 ));
    double y_b_f_b_desvio_padrao = sqrt(y_b_f_b_dev/((double)class_b_count-1 ));

    double a_probabilidade = (double)class_a_count/(double)SIZE;
    double b_probabilidade = (double)class_b_count/(double)SIZE;

    
    a_probabilidade *= gaussian_probability(x1, y_a_f_a_media, y_a_f_a_desvio_padrao) * gaussian_probability(x2, y_a_f_b_media, y_a_f_b_desvio_padrao);
    b_probabilidade *= gaussian_probability(x1, y_b_f_a_media, y_b_f_a_desvio_padrao) * gaussian_probability(x2, y_b_f_b_media, y_b_f_b_desvio_padrao);

  
    return b_probabilidade>a_probabilidade;

}

double gaussian_probability(double x, double media, double desvio_padrao){
    double x_exponent = pow(E_CONSTANT, -pow(x-media, 2)/(2*pow(desvio_padrao, 2)));
    return (1/ (sqrt(2*PI)*desvio_padrao))*x_exponent;
}