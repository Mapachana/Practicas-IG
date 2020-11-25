#ifndef MODELO_JER_H
#define MODELO_JER_H

#include "grafo-escena.h"


//-----------------------------------------------------------------------
// Nodo Raíz del grafo
class CNodoRaiz : public NodoGrafoEscena{
     protected:
        Matriz4f * trasl_salto = nullptr;
        Matriz4f * trasl_nariz = nullptr;
        //Matriz4f * trasl_brazo2 = nullptr;
        Matriz4f * rotacion = nullptr;

        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

    public:
        CNodoRaiz();
        unsigned leerNumParametros() const;

        void fijar_traslsalto(const float nuevatraslsalto);
        void fijar_traslnariz(const float nuevatraslnariz);
        void fijar_rotacion(const float nuevarotacion);
};

//----------------------------------------------------------------------------------
// Cilindro Parametrico
class CilindroParametrico : public NodoGrafoEscena{
    public:
        CilindroParametrico(  const int num_verts_per, const unsigned nperfiles, const float radio, const float altura );
};

//----------------------------------------------------------------------------------
// Esfera paramétrica
class EsferaParametrica : public NodoGrafoEscena{
    public:
        EsferaParametrica(const int num_verts_per, const unsigned nperfiles, const float radio);
};

//----------------------------------------------------------------------------------
// Cono parametrico
class ConoParametrico : public NodoGrafoEscena{
    public:
        ConoParametrico(const int num_verts_per, const unsigned nperfiles, const float radio, const float altura);
};

//---------------------------------------------------------------------
class Boton : public NodoGrafoEscena{
    public:
        Boton();
};
//---------------------------------------------------------------------
class Botones : public NodoGrafoEscena{
    public:
        Botones();
};
//---------------------------------------------------------------------
class Brazo : public NodoGrafoEscena{
    public:
        Brazo();
};
//---------------------------------------------------------------------
class DosBrazos : public NodoGrafoEscena{
    public:
        DosBrazos();
};
//---------------------------------------------------------------------
class Cuerpo : public NodoGrafoEscena{
    public:
        Cuerpo();
};
//---------------------------------------------------------------------
class Ojo : public NodoGrafoEscena{
    public:
        Ojo();
};
//---------------------------------------------------------------------
class DosOjos : public NodoGrafoEscena{
    public:
        DosOjos();
};
//---------------------------------------------------------------------
class Nariz : public NodoGrafoEscena{
    public:
        Nariz(Matriz4f * & traslacion);
};
//---------------------------------------------------------------------
class Copa : public NodoGrafoEscena{
    public:
        Copa();
};
//---------------------------------------------------------------------
class Ala : public NodoGrafoEscena{
    public:
        Ala();
};
//---------------------------------------------------------------------
class Cinta : public NodoGrafoEscena{
    public:
        Cinta();
};
//---------------------------------------------------------------------
class Sombrero : public NodoGrafoEscena{
    public:
        Sombrero();
};
//---------------------------------------------------------------------
class Cabeza : public NodoGrafoEscena{
    public:
        Cabeza(Matriz4f * & traslacion);
};
//---------------------------------------------------------------------
class Final : public NodoGrafoEscena{
    public:
        Final( Matriz4f * & traslacion_salto, Matriz4f * & rotaciontotal, Matriz4f * & traslacion_nariz );
};

#endif
