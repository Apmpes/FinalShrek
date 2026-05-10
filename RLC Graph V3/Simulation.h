#pragma once
#pragma once
#include <cmath>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

// Holds the result of a simulation
ref struct SimulationResult {
	array<double>^ yPoints;
	double totalTime;
};

SimulationResult^ RunSimulation(double R, double L, double C, int steps) {
	double omega = 1.0 / Math::Sqrt(L * C);
	double totalTime = 10.0 * (1.0 / omega) * (1.0 + R / (2.0 * Math::Sqrt(L / C)));
	double dt = totalTime / steps;

	double V = 1.0, I = 0.0;

	array<double>^ vPoints = gcnew array<double>(steps);
	for (int i = 0; i < steps; i++) {
		double dI = (V - I * R) / L * dt;
		double dV = (-I / C) * dt;
		I += dI;
		V += dV;
		vPoints[i] = V;
	}

	SimulationResult^ result = gcnew SimulationResult();
	result->yPoints = vPoints;
	result->totalTime = totalTime;
	return result;
}