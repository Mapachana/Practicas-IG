// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....

   Tupla3f ver1, ver2, ver3, aux1, aux2, normal;
   for(unsigned int i = 0; i<triangulos.size(); i++){
      ver1 = vertices[triangulos[i](0)];
      ver2 = vertices[triangulos[i](1)];
      ver3 = vertices[triangulos[i](2)];

      aux1 = ver2-ver1;
      aux2 = ver3-ver1;

      normal = aux1.cross(aux2);
      if(normal.lengthSq() > 0)
         nor_tri.push_back(normal.normalized());
      else
         nor_tri.push_back({0.0, 0.0, 0.0});  
   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......

   // Calculamos las normales de cada triangulo
   calcularNormalesTriangulos();

   // Inicializamos vector
   for (unsigned int i = 0; i < vertices.size(); ++i){
      nor_ver.push_back({0.0, 0.0, 0.0});
   }

   // A cada vertice de un triangulo le sumamos la normal de dicho triangulo. No hace falta dividir porque se normaliza
   for (unsigned int i = 0; i < triangulos.size(); ++i){
      nor_ver[triangulos[i](0)] = nor_ver[triangulos[i](0)] + nor_tri[i];
      nor_ver[triangulos[i](1)] = nor_ver[triangulos[i](1)] + nor_tri[i];
      nor_ver[triangulos[i](2)] = nor_ver[triangulos[i](2)] + nor_tri[i];
   }

   // Guardamos la normal normalizada de cada verice si no es nula
   for (unsigned int i = 0; i < vertices.size(); ++i){
      if (nor_ver[i].lengthSq() > 0)
         nor_ver[i] = nor_ver[i].normalized();
   }

}




// ----------------------------------------------------------------------------


void MallaInd::visualizarGL( ContextoVis & cv )
{

   using namespace std ;
   assert( cv.cauce_act != nullptr );

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETAR: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
   //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
   //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
   //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos 
   //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.


   if (array_verts == nullptr){
      array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
   

      array_verts->fijarIndices(GL_UNSIGNED_INT, 3*triangulos.size(), triangulos.data());

      if (col_ver.size() != 0)
         array_verts->fijarColores(GL_FLOAT, 3, col_ver.data());
      if (cc_tt_ver.size() != 0)
         array_verts->fijarCoordText(GL_FLOAT, 2, cc_tt_ver.data());
      if (nor_ver.size() != 0)
         array_verts->fijarNormales(GL_FLOAT, nor_ver.data());

   }


   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
   // (en cualquier caso hay que pasar como parámetro el tipo de primitiva adecuada a una malla de triángulos).
   // .....

   if (cv.modo_envio == ModosEnvio::inmediato_begin_end)
      array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
   else if (cv.modo_envio == ModosEnvio::inmediato_drawelements)
      array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
   else if (cv.modo_envio == ModosEnvio::diferido_vao)
      array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);
   else
      assert(false);
   




   // restaurar el color previamente fijado
   glColor4fv( color_previo );

   if ( cv.visualizando_normales)
      visualizarNormales();
}


void MallaInd::visualizarNormales()
{
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  
   if ( array_verts_normales == nullptr )
   {  
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {  
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      array_verts_normales = new ArrayVertices( GL_FLOAT, 3, segmentos_normales.size(), segmentos_normales.data() );
   }

   array_verts_normales->visualizarGL_MI_DAE( GL_LINES );
   CError();
}






// *****************************************************************************


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY(nombre_arch, vertices, triangulos);


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();



}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   calcularNormales();
}
// -----------------------------------------------------------------------------------------------


// ****************************************************************************
// Clase Tetraedro

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices" )
{

   // FIXME repasar vertices y triangulos

   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, 2.0 }, // 1
         { 1.732, 0.0, 1.0 }, // 2
         { 0.58, 1.64, 1.0 } // 3
      } ;

   triangulos =
      {  {0,1,2},
         {0,1,3},
         {0,2,3},
         {1,2,3}
      } ;

   calcularNormales();

}
// -----------------------------------------------------------------------------------------------

// ****************************************************************************
// Clase CuboColores

CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices colores" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   float R,G,B;
	for(unsigned i = 0; i<vertices.size(); i++){
		R = vertices[i](X)==-1 ? 0.0 : 1.0;
		G = vertices[i](Y)==-1 ? 0.0 : 1.0;
		B = vertices[i](Z)==-1 ? 0.0 : 1.0;
		col_ver.push_back({R,G,B});
	}

   calcularNormales();

}
// -----------------------------------------------------------------------------------------------

Cubo24 :: Cubo24(){
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

	 { -1.0, -1.0, -1.0 }, // 0 +8
         { -1.0, -1.0, +1.0 }, // 1 +8
         { -1.0, +1.0, -1.0 }, // 2 +8
         { -1.0, +1.0, +1.0 }, // 3 +8
         { +1.0, -1.0, -1.0 }, // 4 +8
         { +1.0, -1.0, +1.0 }, // 5 +8
         { +1.0, +1.0, -1.0 }, // 6 +8
         { +1.0, +1.0, +1.0 }, // 7 +8

	 { -1.0, -1.0, -1.0 }, // 0 +16
         { -1.0, -1.0, +1.0 }, // 1 +16
         { -1.0, +1.0, -1.0 }, // 2 +16 
         { -1.0, +1.0, +1.0 }, // 3 +16
         { +1.0, -1.0, -1.0 }, // 4 +16
         { +1.0, -1.0, +1.0 }, // 5 +16
         { +1.0, +1.0, -1.0 }, // 6 +16
         { +1.0, +1.0, +1.0 } // 7 +16
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)
         {8,13,9}, {8,12,13}, // Y-
         {10,11,15}, {10,15,14}, // Y+ (+2)
         {16,22,20}, {16,18,22}, // Z-
         {17,21,23}, {17,23,19}  // Z+ (+1)
      } ;

   cc_tt_ver=
     {{0,1}, // 0
      {1,1}, // 1
      {0,0}, // 2
      {1,0}, // 3
      {1,1}, // 4
      {0,1}, // 5
      {1,0}, // 6
      {0,0}, // 7

      {0,0}, // 0 +8
      {1,0}, // 1 +8
      {1,0}, // 2 +8
      {0,0}, // 3 +8
      {0,1}, // 4 +8
      {1,1}, // 5 +8
      {1,1}, // 6 +8
      {0,1}, // 7 +8

      {1,1}, // 0 +16
      {0,1}, // 1 +16
      {1,0}, // 2 +16 
      {0,0}, // 3 +16
      {0,1}, // 4 +16
      {1,1}, // 5 +16
      {0,0}, // 6 +16
      {1,0} // 7 +16
     };

   calcularNormales();
}

NodoCubo :: NodoCubo(){
   ponerNombre("Cubo 24 vertices");
   
   Textura * tex = new Textura("../recursos/imgs/window-icon.jpg");
   
   agregar( new Material(tex, 0.2, 0.4, 0.4, 20) );
   agregar(new Cubo24());


}

