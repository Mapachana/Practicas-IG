#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "matrices-tr.h"
#include <cmath>

//-----------------------------------------------------------------------
// C

CNodoRaiz::CNodoRaiz(){
	ponerNombre("Snowy");

    agregar( new Final(trasl_salto, rotacion, trasl_nariz));
}

void CNodoRaiz::actualizarEstadoParametro( const unsigned iParam, const float t_sec ){
    assert(iParam < leerNumParametros());

    switch(iParam){
        case 0:
            fijar_traslsalto(1.5 * sin(t_sec));
            break;
        case 1:
            fijar_rotacion(50.0 * t_sec);
            break;
        case 2:
            fijar_traslnariz(3.0 * (1 + sin(t_sec))/2.0);
    }
}

unsigned CNodoRaiz::leerNumParametros() const{
    return 3;
}

void CNodoRaiz::fijar_traslsalto(const float nuevatraslsalto){
    *trasl_salto = MAT_Traslacion(0.0, nuevatraslsalto, 0.0);
}

void CNodoRaiz::fijar_traslnariz(const float nuevatraslnariz){
    *trasl_nariz = MAT_Traslacion(0.0, 0.0, nuevatraslnariz);
}

void CNodoRaiz::fijar_rotacion(const float nuevarotacion){
    *rotacion = MAT_Rotacion(nuevarotacion, 0.0, 1.0, 0.0);
}



//-----------------------------------------------------------------------
// CilindroParametrico

CilindroParametrico::CilindroParametrico( const int num_verts_per, const unsigned nperfiles, const float radio, const float altura  )
{
    agregar( MAT_Escalado( radio, altura, radio ) );
    agregar( new Cilindro(num_verts_per, nperfiles) );
}

//---------------------------------------------------------------------
// EsferaParametrica

EsferaParametrica::EsferaParametrica(const int num_verts_per, const unsigned nperfiles, const float radio){
    agregar( MAT_Escalado( radio, radio, radio ) );
    agregar( new Esfera(num_verts_per, nperfiles) );
}

//---------------------------------------------------------------------
// ConoParametrico
ConoParametrico::ConoParametrico(const int num_verts_per, const unsigned nperfiles, const float radio, const float altura){
    agregar( MAT_Escalado( radio, altura, radio ) );
    agregar( new Cono(num_verts_per, nperfiles) );
}

//---------------------------------------------------------------------
// Clases varias

Boton::Boton(){
    agregar(MAT_Rotacion(90.0, 1.0, 0.0, 0.0));
    agregar(MAT_Rotacion(90.0, 0.0, 1.0, 0.0));
    agregar(new CilindroParametrico(30, 30, 0.2, 0.2));

    ponerColor({0.0, 0.0, 0.0});
}

Botones::Botones(){
    agregar(MAT_Traslacion(0.0, 0.0, 1.35));
    agregar(new Boton());
    agregar(MAT_Traslacion(0.0, 0.5, -0.1));
    agregar(MAT_Rotacion(330.0, 1.0, 0.0, 0.0));
    agregar(new Boton());
}

Brazo::Brazo(){
    agregar(MAT_Rotacion(70.0, 0.0, 0.0, 1.0));
    agregar( new CilindroParametrico(15, 30, 0.1, 2.5));

    ponerColor({0.65, 0.17, 0.17});
}

DosBrazos::DosBrazos(){
    agregar(MAT_Traslacion(0.0, 0.5, 0.0));
    agregar(new Brazo());
    agregar(MAT_Rotacion(180.0, 0.0, 1.0, 0.0));
    agregar(new Brazo());

}

Cuerpo::Cuerpo(){
    agregar( new EsferaParametrica(30, 30, 1.5) );
    agregar( new DosBrazos());
    agregar( new Botones());
}

Ojo::Ojo(){
    agregar(new EsferaParametrica(30, 30, 0.12));

    ponerColor({0.0, 0.0, 0.0});
}

DosOjos::DosOjos(){
    agregar(MAT_Traslacion(-0.3,0.0,1.0));
    agregar(new Ojo());
    agregar(MAT_Traslacion(0.6,0.0,0.0));
    agregar(new Ojo());

}

Nariz::Nariz(Matriz4f * & traslacion){
    unsigned ind = agregar(MAT_Traslacion(0.0, 0.0, 0.0));

    agregar(MAT_Traslacion(0.0, -0.2, 0.9));
    agregar(MAT_Rotacion(90.0, 1.0, 0.0, 0.0));
    agregar(new ConoParametrico(30, 30, 0.15, 0.8));

    ponerColor({1.0, 0.5, 0.0});

    traslacion = leerPtrMatriz(ind);
}

Copa::Copa(){
    agregar(new CilindroParametrico(30, 30, 0.7, 1.5));

    ponerColor({0.0, 0.0, 0.0});
}

Ala::Ala(){
    agregar(new CilindroParametrico(30, 30, 1.5, 0.2));

    ponerColor({0.0, 0.0, 0.0});
}

Cinta::Cinta(){
    agregar(new CilindroParametrico(30, 30, 0.8, 0.2));

    ponerColor({1.0, 0.0, 0.0});
}

Sombrero::Sombrero(){
    agregar(new Copa());
    agregar(new Ala());
    agregar(MAT_Traslacion(0.0, 0.2, 0.0));
    agregar(new Cinta());
}

Cabeza::Cabeza(Matriz4f * & traslacion){
    agregar( new EsferaParametrica(30, 30, 1.0) );
    agregar(new DosOjos());
    agregar(new Nariz(traslacion));
}

Final::Final( Matriz4f* &traslacion_salto, Matriz4f * & rotaciontotal, Matriz4f * & traslacion_nariz ){
    unsigned ind = agregar( MAT_Traslacion(0.0, 0.0, 0.0) );
    unsigned ind2 = agregar( MAT_Rotacion(0.0, 0.0, 1.0, 0.0));

    agregar( new Cuerpo() );
    agregar( MAT_Traslacion(0.0, 2.0, 0.0) );
    agregar( new Cabeza(traslacion_nariz) );
    agregar( MAT_Traslacion(0.0, 0.5, 0.0));
    agregar ( new Sombrero() );

    traslacion_salto = leerPtrMatriz(ind);
    rotaciontotal = leerPtrMatriz(ind2);

}
