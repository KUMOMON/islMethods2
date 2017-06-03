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

//определение матрицы
template<typename T>
using matrix = vector<vector<T>>;


matrix<int> GetSomeMatrixByIndex(const unsigned short int);     //выдает тестовую матрицу смежности графа (1-6)
void ShowMatrixToMonitor(const matrix<int>&) noexcept;          //выводит матрицу на экран
void AnalysSystem(const matrix<int>& m);                        //выполняет анализ системы по матрице смежности
matrix<int> LoadMatrixFromFile();                               //загружает матрицу из файла

//выполняет сохранение результатов исследования в файл
void SafeResultInExcellFile(vector<long double>,bool,vector<long double>,vector<long double>);


int main()
{
    //matrix<int> m = GetSomeMatrixByIndex(4);
    matrix<int> m = LoadMatrixFromFile();
    cout<<endl;
    if(m.size()>0)
        AnalysSystem(m);
    else
        cout<<"file not found or matrix not exist";
    system("pause");
    return 0;
}

matrix<int> GetSomeMatrixByIndex(const unsigned short int index)
{
    matrix<int> _matrix;

    switch (index) {

    case 1://Последовательная структура
        _matrix = {{0,1,0,0,0},
                   {1,0,1,0,0},
                   {0,1,0,1,0},
                   {0,0,1,0,1},
                   {0,0,0,1,0}};break;

    case 2://Кольцевая структура
        _matrix = {{0,1,0,0,1},
                   {1,0,1,0,0},
                   {0,1,0,1,0},
                   {0,0,1,0,1},
                   {1,0,0,1,0}};break;

    case 3://Радиальная структура
        _matrix = {{0,1,1,1,1},
                   {1,0,0,0,0},
                   {1,0,0,0,0},
                   {1,0,0,0,0},
                   {1,0,0,0,0}};break;

    case 4://Древовидная структура
        _matrix = {{0,1,1,0,0},
                   {1,0,0,0,0},
                   {1,0,0,1,1},
                   {0,0,1,0,0},
                   {0,0,1,0,0}};break;

    case 5://Полный граф
        _matrix = {{0,1,1,1,1},
                   {1,0,1,1,1},
                   {1,1,0,1,1},
                   {1,1,1,0,1},
                   {1,1,1,1,0}}; break;

    case 6://Несвязный граф
        _matrix = {{0,1,0,0,0},
                   {1,0,0,0,0},
                   {0,0,0,1,1},
                   {0,0,1,0,0},
                   {0,0,1,0,0}};break;
    }
    return _matrix;
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

    unsigned int N = m.size();      //количество вершин

    long double ks = Svaznost(m);        //Коофициент связности
    long double R = Izbitochnost(m);     //Коофициент ихбыточности
    bool ksb = (ks>=N-1);           //показатель связности

    cout<<"Sistema:"<<endl;
    ShowMatrixToMonitor(m);
    cout<<"H-ki sistemy"<<endl;

    cout<<"Kooficient svyaznosti K = "<<ks<<'.'<<endl;
    cout<<"Struktura "<<((ksb)?"svyaznaya":"ne svyaznaya")<<'.'<<endl<<endl;

    cout<<"Kooficient izbytochnosti R = "<<R<<'.'<<endl;
    if(R==0)
        cout<<"Struktura svyazno-minimal'na"<<'.'<<endl;

    if(R<0)
        cout<<"Struktura svyazno-izbytochna"<<'.'<<endl;
    cout<<endl;
    vector<long double> rez1={ks,R};
    if(ksb)
    {
        long double sko = 0;            //Равномерность распределения связей
        long double diamStr = 0;        //Диаметр структуры
        long double structCompact = 0;  //Структурная компактность(относительная)
        long double stepCentr = 0;      //Степень централизации
        vector<long double> rank3;      //ранги элементов длинной 3
        vector<long double> rank4;      //ранги элементов длинной 4

        sko = Ravnomernost(m);
        cout<<"Kvadratnoe otklonenie zadannogo raspredeleniya stepeni vershin"<<endl<<"E^2 = "<<sko<<'.'<<endl;
        cout<<"Struktura "<<((sko==0)?"ravnomernaya":"neravnomernaya")<<'.'<<endl<<endl;

        diamStr = DiamStruct(m);
        structCompact = StructCompactOtn(m);
        cout<<"Diametr struktury (d)= "<<diamStr<<'.'<<endl;
        cout<<"Qotn = "<<structCompact<<'.'<<endl;

        if((structCompact>=0) & (structCompact<=1))
            cout<<"0 <= Qotn <= 1"<<endl;
        if(structCompact==0)
            cout<<"Struktura kompaktnaya."<<endl;
        if(structCompact==1)
            cout<<"Struktura nekompaktnaya."<<endl;
        cout<<endl;

        stepCentr = StepenCentr(m);

        cout<<"Indeks centralizacii b = "<<roundf(stepCentr*10)/10<<'.'<<endl;

        rank3 = RangElements3(m);
        cout<<"Rangi elementov dlya k=3:"<<endl;
        for(unsigned int i = 0;i<rank3.size();i++)
            cout<<setw(4)<<rank3[i]<<'|';
        cout<<endl<<endl;


        rank4 = RangElements4(m);
        cout<<"Rangi elementov dlya k=4:"<<endl;
        for(unsigned int i = 0;i<rank4.size();i++)
            cout<<setw(4)<<rank4[i]<<'|';
        cout<<endl<<endl;
        rez1.push_back(sko);
        rez1.push_back(diamStr);
        rez1.push_back(structCompact);
        rez1.push_back(stepCentr);
        SafeResultInExcellFile(rez1,ksb,rank3,rank4);
    }
   else
        SafeResultInExcellFile(rez1,ksb,vector<long double>(),vector<long double>());

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

void SafeResultInExcellFile(vector<long double> someParams,bool qsb,vector<long double> rank3,vector<long double> rank4)
{
    using namespace libxl;

    //Создание объекта для работы с Excell документом
    Book* book = xlCreateBook();

    //открытие файла
    bool loadResult = book->load("matrixExell.xls");
    if(loadResult)
    {
        if(book->sheetCount()<2)
            book->addSheet("result");

        Sheet* shetResult = book->getSheet(1);
        shetResult->clear();

        shetResult->writeStr(1,1,"Kooficient svyaznosti K = ");
        shetResult->writeNum(2,1,someParams[0]);
        shetResult->writeStr(3,1,((qsb)?"Struktura svyaznaya":"Struktura ne svyaznaya"));
        shetResult->writeStr(4,1,"Kooficient izbytochnosti R = ");
        shetResult->writeNum(5,1,someParams[1]);

        if(someParams[1]==0)
            shetResult->writeStr(6,1,"Struktura svyazno-minimal'na");

        if(someParams[1]<0)
            shetResult->writeStr(6,1,"Struktura svyazno-izbytochna");

        if(qsb)
        {
            shetResult->writeStr(7,1,"Kvadratnoe otklonenie zadannogo raspredeleniya stepeni vershin E^2 = ");
            shetResult->writeNum(8,1,someParams[2]);
            shetResult->writeStr(9,1,"Diametr struktury = ");
            shetResult->writeNum(10,1,someParams[3]);
            shetResult->writeStr(11,1,"Qotn = ");
            shetResult->writeNum(12,1,someParams[4]);

            if((someParams[4]>=0) & (someParams[4]<=1))
                shetResult->writeStr(13,1,"0 <= Qotn <= 1");
            if(someParams[4]==0)
                shetResult->writeStr(14,1,"Struktura kompaktnaya.");
            if(someParams[4]==1)
                shetResult->writeStr(14,1,"Struktura nekompaktnaya.");

            shetResult->writeStr(15,1,"Indeks centralizacii b = ");
            shetResult->writeNum(16,1,someParams[5]);
            shetResult->writeStr(17,1,"Rangi elementov dlya k=3:");
            for(unsigned int i=0;i<rank3.size();i++)
                shetResult->writeNum(18,i+1,rank3[i]);
            shetResult->writeStr(19,1,"Rangi elementov dlya k=4:");
            for(unsigned int i=0;i<rank3.size();i++)
                shetResult->writeNum(20,i+1,rank4[i]);
        }
    }
    else
        cout<<book->errorMessage();
    book->save("matrixExell.xls");
    book->release();
}
