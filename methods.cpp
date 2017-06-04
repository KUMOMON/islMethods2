#include <vector>       //модуль расширяемых массивов
#include <math.h>       //модуль математических функций
#include <array>        //?
#include <algorithm>    //для поиска максимального элемента в векторе
#include <c++/limits>   //для извлечения границ типов
#include <set>          //для хранения множества документов в системе
#include <map>          //для хранения пар документ - его прорадители
#include <string>

namespace islMethods {

using namespace std;

////////////////////////////////////////////////////////////////

template<typename T>
using matrix = vector<vector<T>>;

using indexer = unsigned int;

//Возвращает все степени матрицы смежности
vector<matrix<int>> GetSteps(const matrix<int>&);

//Умножает одну матрицу на другую
matrix<int> MxM(const matrix<int>&,const matrix<int>&);

//складывает одну матрицу с другой
matrix<int> MplusM(const matrix<int>&, const matrix<int>&);

//Возвращает матрицы минимальных путей из I в J
matrix<int> minPaths(const matrix<int>& m);

//удаляет изолированные элементы графа
matrix<int>DeleteIsolatedVertices(const matrix<int>& m, string& logMessage);

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

vector<vector<int>> GetStronglyConnectedComponents(const matrix<int>& m)
{
    matrix<int> pats=minPaths(m);       //для определения существояания пути в графе
    indexer N = pats.size();
    vector<vector<int>> strngConnComp;  //вектор связных компонент
    set<int> numOfElemGraph;            //множество вершин

    //заполняем множество
    for(indexer i = 0;i<N;i++)
        numOfElemGraph.insert(i);

    indexer i=0;//начинаем поиск с 0 элемента
    do//ищем сильно связные компоненты
    {
        i = *numOfElemGraph.begin();

        vector<bool>R(N); //достижимость вершин из i | ->
        vector<bool>Q(N); //достижимость вершины i из других | <-
        vector<bool>V(N); //сильносвязный граф

        for(indexer k = 0;k<N;k++)
            if(i!=k)
            {
                R[k]=(pats[i][k]!=0);
                Q[k]=(pats[k][i]!=0);
            }

        for(indexer k = 0;k<N;k++)
            V[k]=R[k]&Q[k];

        vector<int> tmp(0);     //запоминаем получившуюся компоненту в tmp
        numOfElemGraph.erase(i);
        tmp.push_back(i);
        for(int k = 0;k<N;k++)
            if(V[k])
            {
                numOfElemGraph.erase(k);
                tmp.push_back(k);
            }
        if(tmp.size()>1)
        strngConnComp.push_back(tmp);

    }
    while (!numOfElemGraph.empty());


    return strngConnComp;
}

vector<int> GetInitialVertices(const matrix<int>& m)
{
    indexer N = m.size();
    vector<int> rez;
    for(int i=0;i<N;i++)
    {
        int sumCol=0;
        for(int j=0;j<N;j++)
            sumCol+=m[j][i];

        if(sumCol==0)
            rez.push_back(i);
    }
    return rez;
}

vector<int> GetEndVertices(const matrix<int>& m)
{
    indexer N = m.size();
    vector<int> rez;
    for(int i=0;i<N;i++)
    {
        int sumLine=0;
        for(int j=0;j<N;j++)
            sumLine+=m[i][j];
        if(sumLine==0)
            rez.push_back(i);
    }
    return rez;
}

matrix<int> GetAllPaths(const matrix<int>& m)
{
    indexer N = m.size();
    matrix<int> rez = m;

    for(vector<int> i:rez)
        i=vector<int>();

    vector<matrix<int>>step = GetSteps(m);

    for(indexer stepI=1;stepI<N;stepI++)
        for(indexer i=0;i<N;i++)
            for(indexer j = 0; j<N;j++)
                if(step[stepI][i][j]!=0)
                    rez[i][j]+=step[stepI][i][j];
    return rez;
}

map<int,vector<int>> GetNumbersDocumentsInvolvedInCreation(const matrix<int>& m)
{
    indexer N = m.size();
    matrix<int> allPtahs =GetAllPaths(m);
    vector<int> initVertices = GetInitialVertices(m);
    vector<bool> requiredDocDrop(m.size());
    map<int,vector<int>> pairsDocChildDocParents;

    for(int num:initVertices)
        requiredDocDrop[num] = true;

    for(int j=0;j<N;j++)
    {
        if(!requiredDocDrop[j])
        {
            vector<int> parentsDoc;
            for(int i=0;i<N;i++)
                if(allPtahs[i][j]!=0)
                    parentsDoc.push_back(i);

            if(parentsDoc.size()>0)
            {
                pairsDocChildDocParents.insert(pair<int,vector<int>>(j,parentsDoc));
            }
        }
    }

    return pairsDocChildDocParents;
}

map<int,vector<int>> GetChildDocuments(const matrix<int>& m)
{
    indexer N = m.size();
    matrix<int> allPtahs =GetAllPaths(m);
    vector<int> endVertices = GetEndVertices(m);
    vector<bool> requiredDocDrop(m.size());
    map<int,vector<int>> pairsDocParentDocsChild;

    for(int num:endVertices)
        requiredDocDrop[num] = true;

    for(int i=0;i<N;i++)
    {
        if(!requiredDocDrop[i])
        {
            vector<int> childDocs;
            for(int j=0;j<N;j++)
                if(allPtahs[i][j]!=0)
                    childDocs.push_back(j);

            if(childDocs.size()>0)
            {
                pairsDocParentDocsChild.insert(pair<int,vector<int>>(i,childDocs));
            }
        }
    }

    return pairsDocParentDocsChild;
}

matrix<int> OptimizationGraph(const matrix<int>& m, string& logMessages)
{
    logMessages = "";
    return DeleteIsolatedVertices(m,logMessages);
}

/////////////////ВСПОМОГАТЕЛЬНЫЕ_МЕТОДЫ_ДЛЯ_РАБОТЫ_С_МАТРИЦАМИ_ГРАФОВ///////////////////

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

/////////////////ВСПОМОГАТЕЛЬНЫЕ_МЕТОДЫ_ДЛЯ_ОПТИМИЗАЦИИ_ГРАФОВ/////////////////////////

matrix<int>DeleteIsolatedVertices(const matrix<int>& m, string& logMessage)
{
    int N = m.size();
    set<int> toDelete;//список вершин на удаление

    for(int i=0;i<N;i++)
    {
        int sumValsLine =0;
        int sumValsCol =0;
        for(int j=0;j<N;j++)
        {
            sumValsLine+=m[i][j];
            sumValsCol+=m[j][i];
        }

        if(sumValsCol==0&&sumValsLine==0)
            toDelete.insert(i);
    }
    logMessage+="\nNedostijimye vershiny ";
    logMessage+=((toDelete.empty())?"otsutstvuyut.":":");

    if(toDelete.empty())
        return m;

    logMessage+='\n';
    for(auto iter = toDelete.begin();iter!=toDelete.end();++iter)
    {
        int num = *iter;
        logMessage+=" "+std::to_string(num);

    }
    logMessage+='\n';


    //создание матрицы под оптимизированный граф
    matrix<int> optimalMatrix;

    for(int i=0;i<N;i++)
        if(toDelete.find(i)==toDelete.end())
        {
            vector<int>tmp;
            for(int j=0;j<N;j++)
                if(toDelete.find(j)==toDelete.end())
                    tmp.push_back(m[i][j]);

            if(tmp.size()>0)
                optimalMatrix.push_back(tmp);
        }

    logMessage+="Izolirovannye tochki isklyucheny. \n";
    return optimalMatrix;
}

}
