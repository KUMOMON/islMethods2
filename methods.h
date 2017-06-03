#include<vector>

/* About
 * Здесь представлен наобор методов
 * для исследования храрактеристик
 * систем
 */

/* Alert!
 * Методы MxM и MplusM
 * Не переживут использования
 * несоразмерных матриц(векторов векторов)
 * Не рекомендуется испытывать судьбу
 */

namespace islMethods {

using namespace std;

////////////////////////////////////////////////////////////////

template<typename T>
using matrix = vector<vector<T>>;

////////////////////////////////////////////////////////////////

//Возвращает коофициент связности по матрице смежности
long double Svaznost(const matrix<int>&);

//Возвращает коофициент избыточности по матрице смежности
long double Izbitochnost(const matrix<int>&);

//возвращает равномерность распределения связей по матрице смежности
//или недоиспользование структуры при достижении максимальной связности
long double Ravnomernost(const matrix<int>&);

//Вычисляет диаметр структуры (х-ка относиться с структурной компактности)
int DiamStruct(const matrix<int>&);

//возвращает структурную компактность относительную (близость элементов между собой)
long double StructCompactOtn(const matrix<int>&);

//Степень централизации в структуре
long double StepenCentr(const matrix<int>&);

//Ранги элементов системы с длинной 3
//Расм. рерсурсы в системе в порядке их значимости
vector<long double> RangElements3(const matrix<int>&);

//Ранги элементов системы с длинной 4
vector<long double> RangElements4(const matrix<int>&);
}



