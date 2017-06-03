#include <vector>       //модуль расширяемых массивов
#include <math.h>       //модуль математических функций
#include <c++/limits>   //для извлечения границ типов

namespace islMethods {

using namespace std;

////////////////////////////////////////////////////////////////

template<typename T>
using matrix = vector<vector<T>>;

using indexer = unsigned int;

//Возвращает все степени матрицы смежности
vector<matrix<int>> GetSteps(const matrix<int>&);

//структурная близость Q
int StructProximity(const matrix<int>&);

//Умножает одну матрицу на другую
matrix<int> MxM(const matrix<int>&,const matrix<int>&);

//складывает одну матрицу с другой
matrix<int> MplusM(const matrix<int>&, const matrix<int>&);

//Возвращает матрицы минимальных путей из I в J
matrix<int> minPaths(const matrix<int>& m);


////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////


int StructProximity(const matrix<int>& m)
{
    //кол-во вершин
    indexer N = m.size();

    //Создание копии матрицы
    matrix<int> D = minPaths(m);

    int Q = 0;              //структурная близость
    for(indexer pointI=0;pointI<N;pointI++)
        for(indexer pointJ=0;pointJ<N;pointJ++)
            if(pointI!=pointJ) Q+=D[pointI][pointJ];

    return Q;
}

//Матрица минимальных путей графа
matrix<int> minPaths(const matrix<int>& m)
{
    //кол-во вершин
    indexer N = m.size();
    //Создание копии матрицы
    matrix<int> D(m);

    //Получение всех степеней этой матрицы
    vector<matrix<int>> PN = GetSteps(m);

    //Формирование матрицы минимальных путей
    for(indexer stepen=0;stepen<N;stepen++)
        for(indexer row=0;row<N;row++)
            for(indexer coll=0;coll<N;coll++)
                if((D[row][coll]==0)&(PN[stepen][row][coll]>0)&(row!=coll))
                    D[row][coll] = stepen+1;
    return D;
}

matrix<int> MplusM(const matrix<int> & a, const matrix<int> & b)
{
    matrix<int> rez;
    indexer N = a.size();


    rez.reserve(N);

    for(indexer row=0;row<N;row++)
    {
        rez[row] = vector<int>(N);
        for(indexer cell=0;cell<N;cell++)
            rez[row][cell]=a[row][cell] + b[row][cell];
    }
    return rez;
}

matrix<int> MxM(const matrix<int> & a, const matrix<int> & b)
{
    indexer N = a.size();
    matrix<int> rez(N);

    for(indexer i = 0;i<rez.size();i++)
    {
        rez[i] = vector<int>(N);
        for(indexer j = 0;j<rez[i].size();j++)
            rez[i][j]=0;
    }

    //Спасибо MSDN
    for(indexer row=0;row<N;row++)
        for(indexer cell=0;cell<N;cell++)
            for(indexer inner = 0; inner<N;inner++)
                rez[row][cell]+=a[row][inner] * b[inner][cell];

    return rez;
}

vector<matrix<int>> GetSteps(const matrix<int>& m)
{
    indexer N = m.size();

    vector<matrix<int>> rez;
    matrix<int> _step(m);

    for(indexer step = 0;step<N;step++)
    {
        rez.push_back(_step);
        _step = MxM(m,_step);
    }

    return rez;
}

}
