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

//Ранги элементов системы с длинной N
//Расм. рерсурсы в системе в порядке их значимости
vector<long double> RangElementsN(const matrix<int>&,const unsigned int);

////////////////////////////////////////////////////////////////

long double Svaznost(const matrix<int>& m)
{
    long double rez=0;
    for(indexer row=0;row<m.size();row++)
        for(indexer cell=0;cell<m[row].size();cell++)
            rez+=m[row][cell];
    rez/=2;
    return rez;
}

long double Izbitochnost(const matrix<int>& m)
{
    long double rez=0;
    indexer N = m.size();
    for(indexer row=0;row<N;row++)
        for(indexer cell=0;cell<N;cell++)
            rez+=m[row][cell];

    rez=(((rez/2)/(N-1))-1);
    return rez;
}

long double Ravnomernost(const matrix<int> & m)
{
    //вершины
    indexer N = m.size();
    //ребра
    long double M=0;

    //расчет количества дуг
    for(indexer row=0;row<N;row++)
        for(indexer cell = row;cell<N;cell++)
            M+=m[row][cell];
    //ро
    long double qsr=4*static_cast<long double>(M*M)/static_cast<long double>(N);

    //вектор с кол-вом дуг исхода(i - точка исхода)
    //инициализация вектора с размерностью N
    vector<int> qi(N);

    for(indexer point=0;point<N;point++)
    {
        qi[point] = 0;

        //проход по столбцам матрицы смежности
        //для опред. точки графа
        //для определения коли-ва дуг исхода
        for(indexer cell=0;cell<N;cell++)
            qi[point] += m[point][cell];
    }

    //среднее квадрватичное отклонение
    long double sko=0;
    for(indexer point =0; point<N;point++)
        sko+=pow((static_cast<long double>(qi[point])),2);
    sko-=qsr;
    return sko;
}

int DiamStruct(const matrix<int>& m)
{
    indexer N = m.size();        //кол-во вершин
    matrix<int> D = minPaths(m); //Получение матрицы минимальных путей

    numeric_limits<int> d;       //структура для доступа к min и max значениям значимых числовых типов
    int diametr = d.min();       //диаметр структуры
    for(indexer pointI=0;pointI<N;pointI++)
        for(indexer pointJ=0;pointJ<N;pointJ++)
            if(diametr<D[pointI][pointJ]) diametr = D[pointI][pointJ];

    return diametr;
}

long double StructCompactOtn(const matrix<int>& m)
{
    long double Q_otn=0;             //Относительная структурная близость

    indexer N = m.size();       //кол-во вершин
    int Q = StructProximity(m); //структурная близость
    int Qmin = N*(N-1);         //Минимальная структурная близость

    Q_otn = (static_cast<long double>(Q)/static_cast<long double>(Qmin))-1;
    return Q_otn;
}

long double StepenCentr(const matrix<int>& m)
{

    indexer N = m.size();               //кол-во вершин
    auto matrixMinPaths = minPaths(m);  //матрица минимальных путей
    int Q = StructProximity(m);         //структурная близость

    vector<long double> z(N);                //хранит степень центральности
    for(indexer i=0;i<N;i++) z[i]=0;    //структурного элемента

    long double indexCentr = 0;              //индекс централизации

    //Оценка централизации каждой точки
    for(indexer pointI=0;pointI<N;pointI++)
    {
        z[pointI]=0;
        for(indexer pointJ=0;pointJ<N;pointJ++)
            if(pointI!=pointJ)
                z[pointI]+=matrixMinPaths[pointI][pointJ];
        z[pointI]=static_cast<long double>(Q)/(2.0*(z[pointI]));
    }

    long double zMax = 0;

    for(indexer point=0;point<N;point++)
        if(zMax<z[point]) zMax = z[point];
    //?
    indexCentr = ((N-1)*(2*zMax-N))/((N-2)*zMax);
    return indexCentr;
}

vector<long double> RangElements3(const matrix<int>& m)
{
    return RangElementsN(m,3);
}

vector<long double> RangElements4(const matrix<int>& m)
{
    return RangElementsN(m,4);
}

////////////////////////////////////////////////////////////////

vector<long double> RangElementsN(const matrix<int>& m,const unsigned int n)
{
    indexer N = m.size();          //кол-во вершин
    auto _matrix = GetSteps(m);    //Получение всех степеней

    vector<long double> elementsRank(N);

    //elementsRank[i]=top/down;

    int down=0;
    for(indexer i=0; i<N;i++)
        for(indexer j=0; j<N;j++)
            down+=_matrix[n][i][j];


    for(indexer i=0;i<N;i++)
    {
        int top =0;
        for(indexer j=0;j<N;j++)
            top+=_matrix[n][i][j];

        elementsRank[i] = static_cast<long double>(top)/down;
    }
    return elementsRank;
}

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

//?
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
