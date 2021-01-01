
#include "malla-ind.h"
#include "malla-revol.h"
#include "matrices-tr.h"
#include "latapeones.h"


SuperiorLata::SuperiorLata(){
    ponerNombre("Lata: Tapa superior");

    agregar(new Material(0.2, 0.2, 0.3, 15));
    agregar(new MallaRevolPLY("../recursos/plys/lata-psup.ply", 40));
}

CuerpoLata::CuerpoLata(const std::string & tex){
    ponerNombre("Lata: Cuerpo");

    Textura * textura = new Textura(tex);

    agregar(new Material(textura, 0.5, 0.5, 0.5, 15));
    agregar(new MallaRevolPLY("../recursos/plys/lata-pcue.ply", 40));
}

InferiorLata::InferiorLata(){
    ponerNombre("Lata: Tapa inferior");

    agregar(new Material(0.2, 0.2, 0.3, 15));
    agregar(new MallaRevolPLY("../recursos/plys/lata-pinf.ply", 40));
}

Lata::Lata(const std::string & textura){
    ponerNombre("Lata");

    agregar(MAT_Escalado(2.0,2.0,2.0));
    agregar(new InferiorLata() );
    agregar(new SuperiorLata());
    agregar(new CuerpoLata(textura));
}

LataCocaCola::LataCocaCola(){
    ponerNombre("Lata Coca Cola");

    agregar(new Lata("../recursos/imgs/lata-coke.jpg"));
}

LataPepsi::LataPepsi(){
    ponerNombre("Lata Pepsi");

    agregar(new Lata("../recursos/imgs/lata-pepsi.jpg"));
}

LataUGR::LataUGR(){
    ponerNombre("Lata UGR");

    agregar(new Lata("../recursos/imgs/window-icon.jpg"));
}

PeonMadera::PeonMadera(){
    ponerNombre("Peon Madera");

     Textura * textura = new Textura("../recursos/imgs/text-madera.jpg");

    agregar( new Material(textura, 0.2, 0.8, 0.2, 15 ) );
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 40));
}

PeonBlanco::PeonBlanco(){
    ponerNombre("Peon Blanco");

    ponerColor({1.0, 1.0, 1.0});

    agregar( new Material(0.2, 0.8, 0.2, 15 ) );
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 40));
}

PeonNegro::PeonNegro(){
    ponerNombre("Peon Negro");

    ponerColor({0.0, 0.0, 0.0});

    agregar( new Material(0.2, 0.8, 0.2, 15 ) );
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply", 40));
}

Peones::Peones(){
    agregar(new PeonMadera());
    agregar(MAT_Traslacion(3.0, 0.0, 0.0));
    agregar(new PeonBlanco());
    agregar(MAT_Traslacion(3.0, 0.0, 0.0));
    agregar(new PeonNegro());
}

LataPeones::LataPeones(){
    agregar(new LataCocaCola());
    agregar(MAT_Traslacion(0.0, 1.5, 3.0));
    agregar(new Peones());
}

Latas::Latas(){
    agregar(new LataCocaCola());
    agregar(MAT_Traslacion(3.0, 0.0, 0.0));
    agregar(new LataPepsi());
    agregar(MAT_Traslacion(3.0, 0.0, 0.0));
    agregar(new LataUGR());
}

VariasLatasPeones::VariasLatasPeones(){
    agregar(new Latas());
    agregar(MAT_Traslacion(0.0, 1.5, 3.0));
    agregar(new Peones());
}