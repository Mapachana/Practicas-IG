// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 2: completar: creación de la malla....

   Tupla3f q;
   int k;
   int m = perfil.size();

   for (unsigned int i = 0; i < num_copias; ++i){
      for(unsigned int j = 0; j < m; ++j){
         q = MAT_Rotacion((360.0*i)/(num_copias-1), 0.0, 1.0, 0.0) * perfil[j];
         vertices.push_back(q);
      }
   }

   for (unsigned int i = 0; i < num_copias-1; ++i){
      for(unsigned int j = 0; j < m-1; ++j){
         k = i*m+j;
         triangulos.push_back({k, k+m, k+m+1});
         triangulos.push_back({k, k+m+1, k+1});
      }
   }





}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................

   vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);


}

Cilindro::Cilindro
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
)
{
   ponerNombre( std::string("Cilindro") );

   vector<Tupla3f> perfil;
   float y;

   perfil.push_back({0.0, 0.0, 0.0});

   for (int i = 0; i < num_verts_per-2; ++i){
      y = (float) i/(num_verts_per-3);
      perfil.push_back({1.0, y, 0.0});
   }

   perfil.push_back({0.0, 1.0, 0.0});

   inicializar(perfil, nperfiles);
}

Cono::Cono
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
)
{
   ponerNombre( std::string("Cono") );

   vector<Tupla3f> perfil;

   perfil.push_back({0.0, 0.0, 0.0});

   for (int i = 0; i < num_verts_per-2; ++i){
      perfil.push_back({1-(float)i/(num_verts_per-2), (float)i/(num_verts_per-2), 0.0});
   }

   perfil.push_back({0.0, 1.0, 0.0});

   inicializar(perfil, nperfiles);
}

Esfera::Esfera
(
   const int num_verts_per, // número de vértices del perfil original (m)
   const unsigned nperfiles // número de perfiles (n)
)
{
   ponerNombre( std::string("Esfera") );

   vector<Tupla3f> perfil;
   Tupla3f q;

   perfil.push_back({0.0, -1.0, 0.0});

   for (int i = 0; i < num_verts_per-2; ++i){
      q = MAT_Rotacion((180.0*i)/(num_verts_per-2), 0.0, 0.0, 1.0) * perfil[0];
      perfil.push_back(q);
   }
   perfil.push_back({0.0, 1.0, 0.0});

   inicializar(perfil, nperfiles);
}



