#include <vector>       //модуль расширяемых массивов
#include <math.h>       //модуль математических функций
#include <array>
#include <algorithm>
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

//Возвращает матрицы максимальных путей из I в J
//matrix<int> maxPaths(const matrix<int>& m);

////////////////////////////////////////////////////////////////

bool SystemIsOk(const matrix<int>& m)
{
    //все степени матрицы
    vector<matrix<int>> stepsM = GetSteps(m);

    //если на главной диагонали любой из стеней из матрицы
    //будет не нулевой элемент - системам не корректна
    for(indexer step=0;step<stepsM.size();step++)
        for(indexer diagIndex = 0;diagIndex<stepsM.size();diagIndex++)
            if(stepsM[step][diagIndex][diagIndex]!=0)
                return false;

    return true;
}

vector<int> OrderElementsOfSystem(const matrix<int>& m)
{  
    indexer N = m.size();
    vector<int>pi(N);
    vector<bool> compliteWork(N);//используется для обхода обработки j-ых определенных элементов

    for(indexer j=0;j<N;j++){//исключение из обработки столбцы изначально имеющие все нули
        int sum = 0;
        for(indexer i=0;i<N;i++)
            if(m[i][j]!=0)
                sum+=m[i][j];

        if(sum==0)
            compliteWork[j] = true;
    }

    vector<matrix<int>> steps = GetSteps(m);

    for(indexer step = 0;step<N-1;step++){//цикл по документам(столбцам) pj
        for(indexer j=0;j<N;j++)
            if(!compliteWork[j]){//если порядок элемента неопределен
                int sumCol = 0;            //сумма элементов j-ого столбца
                int sumColNextStep = 0;    //сумма эл. по столбцу след. степени
                for(indexer i = 0;i<N;i++){//индекс по строкам
                    sumCol+=steps[step][i][j];
                    sumColNextStep+=steps[step+1][i][j];
                }

                if(sumCol>0&&sumColNextStep==0)
                {
                    pi[j]=step+1;
                    compliteWork[j] = true;
                }
            }
    }
    return pi;

}

inline int OrderOfSystem(const vector<int>& OrderElementsOfSystem)
{
    return *max_element(OrderElementsOfSystem.begin(),OrderElementsOfSystem.end());
}

vector<int> TactNumberAfterWhichDocumentIsNotUsed(const matrix<int>& m,const vector<int>& OrderElementsOfSystem)
{
    indexer N =m.size();
    vector<int> tacts(m.size());    //результирующий массив тактов
    matrix<int> mForTmp(m);              //промежутачная таблица для вычисления тактов

    for(indexer j=0;j<N;j++)        //заменяем значения матрицы отличные от нуля
        for(indexer i=0;i<N;i++)    //на порядок для каждого отдельного элемента
            if(mForTmp[i][j]!=0) mForTmp[i][j] = OrderElementsOfSystem[j];

    for(indexer i = 0;i<N;i++)      //найдем максимальный порядок в строке
        for(indexer j=0;j<N;j++)    //это и будет номер такта, после которого
            if(tacts[i]<mForTmp[i][j]) tacts[i] = mForTmp[i][j]; //он не используется

    for(indexer i=0;i<N;i++)
        if(*max_element(m[i].begin(),m[i].end())==0)
            tacts[i] = *max_element(tacts.begin(),tacts.end());

    return tacts;
}

vector<int> CountTactsWhichDocumentWasInSystem(const vector<int>& OrderElementsOfSystem, const vector<int>& TactNumberAfterWhichDocumentIsNotUsed)
{
    indexer N = TactNumberAfterWhichDocumentIsNotUsed.size();
    vector<int> rez(N);

    for(indexer i=0;i<N;i++)
        rez[i] = abs(TactNumberAfterWhichDocumentIsNotUsed[i]-OrderElementsOfSystem[i]);

    return rez;
}

vector<vector<int>> DistributionOfDocumentsByLevels(const vector<int>& OrderElementsOfSystem)
{
    vector<vector<int>> levelsOfDocuments(*max_element(OrderElementsOfSystem.begin(),OrderElementsOfSystem.end())+1);

    for(vector<int> i:levelsOfDocuments)
        i = vector<int>();

    for(indexer i=0;i<OrderElementsOfSystem.size();i++)
        levelsOfDocuments[OrderElementsOfSystem[i]].push_back(i);

    return levelsOfDocuments;
}

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

vector<vector<int>> GetStronglySonnectedComponents()
{

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
