#include "simlib.h"
#include "simlib3D.h"
#include <cmath>

float G = 10;

const double m1 = 10;
const Value3D pos1(-10, 0, 0);

const double m2 = 10;
const Value3D pos2(10, 0, 0);

double get_distance(Value3D pos1, Value3D pos2) {
	return sqrt(pow(pos2.x() - pos1.x(), 2) + pow(pos2.y() - pos1.y(), 2) + pow(pos2.z() - pos1.z(), 2));
}

struct Planet {
	Integrator x, y, z;
	Planet(Value3D pos1, double m2, Value3D pos2) : 
		x(G * ((m2)/pow(get_distance(pos1, pos2), 3)) * (pos2.x()-pos1.x()), 2),
		y(G * ((m2)/pow(get_distance(pos1, pos2), 3)) * (pos2.y()-pos1.y()), 2),
		z(G * ((m2)/pow(get_distance(pos1, pos2), 3)) * (pos2.z()-pos1.z()), 2) {}
};

Planet p1(pos1, m2, pos2);
Planet p2(pos2, m1, pos1);

void sample() {
	Print("1,%f,%g,%g,%g\n", T.Value(), p1.x.Value(), p1.y.Value(), p1.z.Value());
	Print("2,%f,%g,%g,%g\n", T.Value(), p2.x.Value(), p2.y.Value(), p2.z.Value());
}

Sampler S(sample, 0.001);

int main(void) {

  SetOutput("n-body.dat");
  Init(0, 100);
  SetAccuracy(1e-3);
  
  Print("id,time,x,y,z\n");
  Run();
}