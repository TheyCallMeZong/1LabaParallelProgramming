#include <iostream>
#include <math.h>
#include <vector>
#include "pthread.h"
#pragma comment(lib, "pthreadVCE2.lib")

using namespace std;

int a;
int b;
int thread_number;
int n;
double *result;
double h;

void set_h(){
    h = (b - a) / 1.0 / n;
}

struct data{
    int index;
};

double f(double x)
{
    return(10 - x);
}

void* potok(void * data_param){
    int j = ((data*)data_param)->index;
    double S = 0;
    double F, xi;
    int start = j * (n / thread_number);
    int finish = j * (n / thread_number) + (n / thread_number);
    for (int i = start; i < finish; i++)
    {
        xi = a + i * h;
        F = f(xi);
        S += F * h;
    }
    result[j] = S;
}

void integralpram()
{
    double S = 0;
    result = new double[thread_number];
    pthread_t threads[thread_number];
    data data_t[thread_number];
    set_h();
    for (int i = 0; i < thread_number; i++) {
        data_t[i].index = i;
    }
    for (int i = 0; i < thread_number; i++) {
        pthread_create(&threads[i], NULL, potok, &data_t[i]);
    }
    for (int i = 0; i < thread_number; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < thread_number; ++i) {
        S += result[i];
    }

    cout << "result - " << S << endl;
}

int main()
{
    setlocale(LC_ALL, "ru");
    cout << "Введите нижний предел интегрирования" << endl;
    cin >> a;
    cout << "Введите верхний предел интегрирования" << endl;
    cin >> b;
    cout << "Введите кол-во шагов" << endl;
    cin >> n;
    cout << "Введите кол-во потоков" << endl;
    cin >> thread_number;

    auto begin = std::chrono::steady_clock::now();
    integralpram();
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    cout << "The time: " << elapsed_ms.count() << " ms";
}