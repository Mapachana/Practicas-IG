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

   // Cálculo de normales
   vector<Tupla3f> normales_aris;
   Tupla3f normal, aux;

   for(unsigned int i = 0; i < perfil.size()-1; ++i){
      aux = perfil[i+1] - perfil[i];

      normal(0) = aux(1);
      normal(1) = -aux(0);
      normal(2) = 0.0;

      if(normal.lengthSq() > 0)
         normales_aris.push_back(normal.normalized());
      else
         normales_aris.push_back({0.0, 0.0, 0.0});
   }

    for (unsigned int i = 0; i < perfil.size(); ++i){
      nor_ver.push_back({0.0, 0.0, 0.0});
   }

   if (normales_aris[0].lengthSq() > 0)
      nor_ver[0] = normales_aris[0].normalized();
   else
      nor_ver[0] = {0.0, 0.0, 0.0};

   for(unsigned int i = 1; i < perfil.size()-1; ++i){
      nor_ver[i] = normales_aris[i] + normales_aris[i-1];

      if (nor_ver[i].lengthSq() > 0) // Normalizo si no es 0
         nor_ver[i] = nor_ver[i].normalized();
   }

   if (normales_aris[perfil.size()-2].lengthSq() > 0)
      nor_ver[perfil.size()-1] = normales_aris[perfil.size()-2].normalized();
   else
      nor_ver[perfil.size()-1] = {0.0, 0.0, 0.0};

   // Calcular coordenadas de textura
   std::vector<float> d, t;
   float den = 0;

   for(unsigned int i = 0; i < perfil.size()-1; ++i){
      d.push_back(sqrt((perfil[i+1]-perfil[i]).lengthSq()));
      den += d[i];
   }
   t.push_back(0);
   for(unsigned int i = 1; i < perfil.size(); ++i)
      t.push_back(t[i-1]+d[i-1]/den);



   // COMPLETAR: Práctica 2: completar: creación de la malla....

   Tupla3f q, r;
   int k;
   int m = perfil.size();
   float t_x, t_y;
   Tupla2f textura;

   for (unsigned int i = 0; i < num_copias; ++i){
      for(unsigned int j = 0; j < m; ++j){
         //q = MAT_Rotacion((360.0*i)/(num_copias-1), 0.0, 1.0, 0.0) * perfil[j]; // perfil
         //r = MAT_Rotacion((360.0*i)/(num_copias-1), 0.0, 1.0, 0.0) * nor_ver[j]; // normal
         vertices.push_back( MAT_Rotacion((360.0*i)/(num_copias-1), 0.0, 1.0, 0.0) * perfil[j]);
         if (i != 0)
            nor_ver.push_back(MAT_Rotacion((360.0*i)/(num_copias-1), 0.0, 1.0, 0.0) * nor_ver[j]);

         t_x = (float)i/(num_copias-1); // textura
         t_y = 1.0 - t[j];
         textura = {t_x, t_y};
         cc_tt_ver.push_back(textura);

         
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



