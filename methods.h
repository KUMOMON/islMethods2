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

  //вычисляет порядок кажого элемента элемента системы
  //означает  номер такта, к которому готовы все документы,
  vector<int> OrderElementsOfSystem(matrix<int> matrix);

   //судит о кооректности системы по контурам (хотя бы один есть - неисправна | return false)
   bool SystemIsOk(const matrix<int>&);

    //вычисляет порядок всей системы исходя из порядков каждого элемента
   inline int OrderOfSystem(vector<int> OrderElementsOfSystem);
}


