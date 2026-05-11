#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

// Graph parameters
ref struct GraphConfig {
	String^ xLabel;
	String^ yLabel;
	String^ title;
	Color   lineColour;
	int     pointsPerTick;  // Graph drawing speed
};

//Axes
System::Drawing::Bitmap^ DrawAxes(int w, int h, String^ xLabel, String^ yLabel, String^ title) {
	Bitmap^ bmp = gcnew Bitmap(w, h);
	Graphics^ g = Graphics::FromImage(bmp);
	g->Clear(Color::White);

	Pen^ axisPen = gcnew Pen(Color::Black, 2);
	g->DrawLine(axisPen, 40, 10, 40, h - 30);               // y axis
	g->DrawLine(axisPen, 40, (h * 4) / 5, w - 10, (h * 4) / 5); // x axis

	System::Drawing::Font^ font = gcnew System::Drawing::Font("Segoe UI", 8);
	System::Drawing::Font^ titleFont = gcnew System::Drawing::Font("Segoe UI", 10, FontStyle::Bold);

	g->DrawString(yLabel, font, Brushes::Black, 2, 10);
	g->DrawString(xLabel, font, Brushes::Black, w - 35, (h * 4) / 5 - 15);
	g->DrawString(title, titleFont, Brushes::Black, 40, h - 25);

	return bmp;
}

//Scaling function
cli::array<double>^ ScaleToPixels(cli::array<double>^ values, int h, double yMin, double yMax) {
	int            steps = values->Length;
	cli::array<double>^ pixels = gcnew cli::array<double>(steps);
	int            yAxis = (h * 4) / 5;
	int            yTop = 20;

	for (int i = 0; i < steps; i++) {
		double normalised = (values[i] - yMin) / (yMax - yMin);
		double y = yAxis - normalised * (yAxis - yTop);
		y = Math::Max((double)yTop, Math::Min((double)(h - 10), y));
		pixels[i] = y;
	}
	return pixels;
}