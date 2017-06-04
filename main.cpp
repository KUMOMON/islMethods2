#include <iostream>
#include <methods.h>
#include <iomanip>
#include <memory>
#include "libxl.h"
#include <vector>
#include <math.h>

/*
 * K-связность
 * R-повышение общ. числа связей в системе над min необходимым
 * e^2 - неравномерность связей
 * Qotn -
 */

using namespace std;

///////////////////////////////////////ПСЕВДОНИМЫ/////////////////////////////////////////////////////////////////////
//определение матрицы
template<typename T>
using matrix = vector<vector<T>>;

///////////////////////////////////////ОБЪЯВЛЕНИЯ//////////////////////////////////////////////////////////////////////

matrix<int> GetTestGraph(const int numberTestGraph);            //выдает тестовую матрицу смежности графа (0-4)
void ShowMatrixToMonitor(const matrix<int>&) noexcept;          //выводит матрицу на экран
void AnalysSystem(const matrix<int>& m);                        //выполняет анализ системы по матрице смежности
matrix<int> LoadMatrixFromFile();                               //загружает матрицу из файла

//выполняет сохранение результатов исследования в файл
void SafeResultInExcellFile(vector<long double>,bool,vector<long double>,vector<long double>);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    matrix<int> m = GetTestGraph(4);
    //matrix<int> m = LoadMatrixFromFile();
    cout<<endl;
    if(m.size()>0)
        AnalysSystem(m);
    else
        cout<<"file not found or matrix not exist";
    system("pause");
    return 0;
}

void ShowMatrixToMonitor(const matrix<int>& a) noexcept
{
    cout<<endl;
    for(unsigned int row =0;row<a.size();row++)
    {
        for(unsigned int cell =0;cell<a[row].size();cell++)
            cout<<setw(3)<<a[row][cell];
        cout<<endl;
    }

}

void AnalysSystem(const matrix<int>& m)
{
    using namespace islMethods;

    //проверяем наличие контуров (если есть - это ошибка информационного графа)
    bool systemIsOk = SystemIsOk(m);

    if(systemIsOk)
    {
        cout<<"V grafe otsutstvuyut kontura"<<endl;
        vector<int> pi = OrderElementsOfSystem(m);
        vector<int> TctNumAftDocNotUsed = TactNumberAfterWhichDocumentIsNotUsed(m,pi);
        vector<int> CntTctDocWasInSystem = CountTactsWhichDocumentWasInSystem(pi, TctNumAftDocNotUsed);

        cout<<setw(4)<<"pi,"<<setw(3)<<"ti,"<<setw(4)<<"t2"<<endl;
        for(unsigned int i = 0;i<pi.size();i++)
            cout<<setw(3)<<pi[i]<<setw(4)<<TctNumAftDocNotUsed[i]<<setw(4)<<CntTctDocWasInSystem[i]<<endl;

        vector<vector<int>>lvlsDocs = DistributionOfDocumentsByLevels(pi);
        cout<<"Razbienie po urovnyam:"<<endl;
        for(unsigned int i=0; i<lvlsDocs.size();i++)
        {
            cout<<i<<':';
            for(unsigned int j =0;j<lvlsDocs[i].size();j++)
                cout<<' '<<lvlsDocs[i][j];
            cout<<endl;
        }
    }
    else {       
        cout<<"Harakteristiki nel'zya poluchit'"<<endl
            <<"t.k. v grafe prisutstvuyut kontury"<<endl;

        vector<vector<int>> StrngConnComp
                =GetStronglyConnectedComponents(m);
        cout<<"Сильные компоненты:"<<endl;

        for(vector<int> v:StrngConnComp)
            for(int num:v)
                cout<<setw(3)<<num;
        cout<<endl;

    }
}

matrix<int> LoadMatrixFromFile()
{
    using namespace libxl;

    vector<vector<int>>_matrix;

    //Создание объекта для работы с Excell документом
    Book* book = xlCreateBook();

    //открытие файла
    book->load("matrixExell.xls");
    if(book)
    {
        //обращение к первой вкладке
        Sheet* sheet = book->getSheet(0);

        //Если такая вкладка существует в книге(документе)
        if(sheet)
        {
            //проходим по строкам документа
            for(int row = sheet->firstRow();row<sheet->lastRow(); ++row)
            {
                //добавление строки в матрицу
                _matrix.push_back(vector<int>());

                //проходим по столбцам строки и считываем числа в матрицу
                for(int col = sheet->firstCol(); col<sheet->lastRow();++col)
                    _matrix[row-1].push_back(sheet->readNum(row,col));
            }
        }
    }
    else
        cout<<book->errorMessage();

    //закрываем документ
    book->release();
    return _matrix;

}

//Возвращает тестовые графы:
//без контуров(0),с контурами(1),с изолированными выршинами(2),с дублированными связями(3)
//с контурами, изолированными вершинами и дублированными связями(4)
matrix<int> GetTestGraph(int numberTestGraph)
{
    if(numberTestGraph<0 || numberTestGraph>4)
        numberTestGraph = 0;

    matrix<int> m;

    switch (numberTestGraph) {
    case 0:
        m = {
            {0,1,1,0,0,0,0,0,0,0},
            {0,0,0,1,1,0,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0},
            {0,0,0,0,0,0,0,1,1,0},
            {0,0,0,0,0,0,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0}
        };
        break;

    case 1:
        m = {
            {0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,1,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0},
            {0,0,1,1,0,1,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1},
            {0,0,0,0,0,0,0,1,0,0},
            {0,0,0,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0}
        };
        break;

    case 2:
        m={
            {0,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,1,1,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0}
        };
        break;
    case 3:
        m={
            {0,1,0,0,2,0,0,0,0,0},
            {0,0,1,0,0,0,0,1,0,0},
            {0,0,0,0,0,1,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,2},
            {0,0,0,0,0,1,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0},
        };
        break;
    case 4:
        m={
            {0,1,0,1,0,0,0,0,0,0},
            {0,0,0,0,1,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0},
            {0,0,0,0,0,0,0,2,0,0},
            {0,0,0,1,0,0,0,0,1,0},
            {0,0,0,0,0,1,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
        };
        break;
    }

    return m;
}
