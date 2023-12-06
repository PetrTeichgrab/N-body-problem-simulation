
// druzice3D.cc -- příklad použití knihovny SIMLIB pro spojitou simulaci 3D
//
// model družice pohybující se v soustavě Země-Měsíc
// [upozornění - bez záruky]
//
// (c) 1997-2001  Petr Peringer

#include "simlib.h"
#include "simlib3D.h"

// pojmenování typů:
typedef Value3D Position;
typedef Value3D Speed;
typedef Value3D Force;

// konstanty:
const double gravity_constant = 10; // gravitační konstanta
// const double gravity_constant = 6.67e-11; // gravitační konstanta
// const double m0 = 1000;		// hmotnost družice [kg]
// const double m1 = 5.983e24;	// hmotnost Země [kg]
// const double m2 = 7.374e22;	// hmotnost Měsíce [kg]

// // inicializace:
// const Position p0(36.0e6, 0, 0);	// poloha družice
// const Position p2(384.405e6, 0, 0);	// poloha Měsíce
// const Speed v0(0, 4200, 1600);		// počáteční rychlost družice
// const Speed v2(0, 1022, 0);		// oběžná rychlost Měsíce
// const double YEAR = 3.6e3 * 24 * 365;	// 1 rok v sekundách
// const double MAXTime = 1*YEAR;		// doba simulace

Constant3D Zero(0, 0, 0);	// pomocný objekt

struct Body {
	// hmotnost
    double m;			
	// vstupní síla
    Expression3D inforce;	
	// rychlost
    Integrator3D v;		
	// poloha
    Integrator3D p;		
    
	Body(const double mass, Position p0, Speed v0 = Speed(0, 0, 0)):
        m(mass), inforce(Zero), v(inforce / m, v0), p(v, p0) {}
    
	void SetInput(Input3D i) {
		inforce.SetInput(i);
    }
};

struct System {
	// pole ukazatelů na objekty v systému
    Body *m[10]; 
	// počet objektů -- hmotných bodů
    int n;             
    
	System();
    
	// výpočet těžiště systému
	Position get_center() {
		Position p = Value3D(0, 0, 0);
		double sum = 0;
		for(int i = 0; i < n; i++) {
			p = p + m[i]->m * m[i]->p.Value();
			sum += m[i]->m;
		}
		return   p / sum;
    }
};

System *w;		// svět vznikne až později :-)

// gravitační síla pusobící na hmotný bod p
class GravityForce : public aContiBlock3D {
    Body *p;
    System *w;
  
	public:
	GravityForce(Body * _p, System * _w):p(_p), w(_w) {}
    
	Force Value() {
		Force f(0, 0, 0);	// gravitační síla
		for (int i = 0; i < w->n; i++) {
			Body *m = w->m[i];
			if(m == p)
				continue;
			Value3D distance = m->p.Value() - p->p.Value();
			double d = abs(distance);	// vzdálenost
			f = f + (distance * gravity_constant * p->m * m->m / (d * d * d));
		} 
        return f;
    }
};

// pohyblivé planety
typedef Body Planet;
typedef Body Moon;
typedef Body Satellite;

// vytvoříme digitální svět
System::System(): n(0) {				
    m[n++] = new Planet(100, Position(-10, 0, 0), Speed(10, 0, 0));
    m[n++] = new Planet(100, Position(10, 0, 0), Speed(0, 10, 0));
    m[n++] = new Planet(100, Position(0, 0, 0), Speed(10, 10, 0));
    // m[n++] = new Moon(m2, p2, v2);
    // m[n++] = new Satellite(m0, p0, v0);
    
	// zapneme silové působení
	for (int i = 0; i < n; i++)	{
		m[i]->SetInput(new GravityForce(m[i], this));
	}
}


// vzorkování stavu modelu
extern void Sample();
Sampler s(Sample, 1e-1);
void Sample() {
	// těžiště systému
    // Position t = w->get_center();

	for (int i = 0; i < w->n; i++) {
		// Position p = (w->m[i])->p.Value() - t;
		Position p = (w->m[i])->p.Value();
		Print("%g %d ", Time, i);
    	Print("%g %g %g\n", p.x(), p.y(), p.z());
	}	
}

int main() {

    SetOutput("n-body.dat");
    w = new System;
	
	// inicializace experimentu
    Init(0, 10);	
    
	// vícekroková integrační metoda
	SetMethod("abm4");          
    
	// je nutná vysoká přesnost
	SetAccuracy(1e-3);		
    
	// start simulace
	Run();			
}