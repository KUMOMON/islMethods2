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

  vector<int> OrderElementsOfSystem(const matrix<int>&);

   //судит о кооректности системы по контурам(хотя бы один есть - неисправна | return false)
   bool SystemIsOk(const matrix<int>&);

    //вычисляет порядок всей системы исходя из порядков каждого элемента
   inline int OrderOfSystem(const vector<int>& OrderElementsOfSystem);

   //определяет для каждого документа номер такта после которого он не используется в системе
   vector<int> TactNumberAfterWhichDocumentIsNotUsed(const matrix<int>&,const vector<int>&);

   //определяет для каждого документа число тактов, в течении которых он находился в системе
   vector<int> CountTactsWhichDocumentWasInSystem(const vector<int>& OrderElementsOfSystem, const vector<int>& TactNumberAfterWhichDocumentIsNotUsed);

   //разбиение документов по уровням
   vector<vector<int>> DistributionOfDocumentsByLevels(const vector<int>& OrderElementsOfSystem);
}



