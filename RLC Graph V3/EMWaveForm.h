#pragma once
#pragma once
#include "Grapher.h"

namespace CppCLRWinFormsProject {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	ref struct EMWaveResult {
		array<double>^ ePoints;
		double totalTime;
		double eMin;
		double eMax;
	};

	static EMWaveResult^ RunEMWaveSimulation(double A, double f,
		double alpha, double phi_deg,
		int steps) {
		double phi = phi_deg * Math::PI / 180.0;
		double period = (f > 0) ? 1.0 / f : 1.0;
		double totalTime = 4.0 * period;
		double dt = totalTime / steps;

		array<double>^ e = gcnew array<double>(steps);
		double eMin = Double::MaxValue;
		double eMax = Double::MinValue;

		for (int i = 0; i < steps; i++) {
			double t = i * dt;
			double val = A * Math::Exp(-alpha * t) * Math::Sin(2.0 * Math::PI * f * t + phi);
			e[i] = val;
			if (val < eMin) eMin = val;
			if (val > eMax) eMax = val;
		}
		if (Math::Abs(eMax - eMin) < 1e-12) { eMin = -1; eMax = 1; }

		EMWaveResult^ r = gcnew EMWaveResult();
		r->ePoints = e;
		r->totalTime = totalTime;
		r->eMin = eMin;
		r->eMax = eMax;
		return r;
	}

	public ref class EMWaveForm : public System::Windows::Forms::Form
	{
	public:
		EMWaveForm(void) { InitializeComponent(); }

	protected:
		~EMWaveForm() { if (components) { delete components; } }

	private:
		System::Windows::Forms::Label^ lblTitle;
		System::Windows::Forms::Label^ lblA;
		System::Windows::Forms::Label^ lblF;
		System::Windows::Forms::Label^ lblAlpha;
		System::Windows::Forms::Label^ lblPhi;
		System::Windows::Forms::TextBox^ tbA;
		System::Windows::Forms::TextBox^ tbF;
		System::Windows::Forms::TextBox^ tbAlpha;
		System::Windows::Forms::TextBox^ tbPhi;
		System::Windows::Forms::Button^ btnSimulate;
		System::Windows::Forms::PictureBox^ picGraph;
		System::Windows::Forms::Timer^ simTimer;
		System::ComponentModel::IContainer^ components;

		array<double>^ yPoints;
		int                        drawIndex;
		System::Drawing::Bitmap^ bmp;
		System::Drawing::Graphics^ g;
		Pen^ graphPen;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblA = (gcnew System::Windows::Forms::Label());
			this->lblF = (gcnew System::Windows::Forms::Label());
			this->lblAlpha = (gcnew System::Windows::Forms::Label());
			this->lblPhi = (gcnew System::Windows::Forms::Label());
			this->tbA = (gcnew System::Windows::Forms::TextBox());
			this->tbF = (gcnew System::Windows::Forms::TextBox());
			this->tbAlpha = (gcnew System::Windows::Forms::TextBox());
			this->tbPhi = (gcnew System::Windows::Forms::TextBox());
			this->btnSimulate = (gcnew System::Windows::Forms::Button());
			this->picGraph = (gcnew System::Windows::Forms::PictureBox());
			this->simTimer = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->BeginInit();
			this->SuspendLayout();

			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(160, 60, 200);
			this->lblTitle->Location = System::Drawing::Point(17, 40);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->TabIndex = 0;
			this->lblTitle->Text = L"EM Wave Simulator";

			this->lblA->AutoSize = true;
			this->lblA->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblA->Location = System::Drawing::Point(42, 135);
			this->lblA->Name = L"lblA";
			this->lblA->TabIndex = 2;
			this->lblA->Text = L"Amplitude A (V/m)";

			this->lblF->AutoSize = true;
			this->lblF->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblF->Location = System::Drawing::Point(42, 173);
			this->lblF->Name = L"lblF";
			this->lblF->TabIndex = 3;
			this->lblF->Text = L"Frequency f (Hz)";

			this->lblAlpha->AutoSize = true;
			this->lblAlpha->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblAlpha->Location = System::Drawing::Point(42, 213);
			this->lblAlpha->Name = L"lblAlpha";
			this->lblAlpha->TabIndex = 4;
			this->lblAlpha->Text = L"Attenuation a (Np/s)";

			this->lblPhi->AutoSize = true;
			this->lblPhi->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblPhi->Location = System::Drawing::Point(42, 253);
			this->lblPhi->Name = L"lblPhi";
			this->lblPhi->TabIndex = 5;
			this->lblPhi->Text = L"Phase offset phi (deg)";

			this->tbA->Location = System::Drawing::Point(240, 135);
			this->tbA->Name = L"tbA";
			this->tbA->Size = System::Drawing::Size(100, 22);
			this->tbA->TabIndex = 6;
			this->tbA->Text = L"1";

			this->tbF->Location = System::Drawing::Point(240, 173);
			this->tbF->Name = L"tbF";
			this->tbF->Size = System::Drawing::Size(100, 22);
			this->tbF->TabIndex = 7;
			this->tbF->Text = L"10";

			this->tbAlpha->Location = System::Drawing::Point(240, 213);
			this->tbAlpha->Name = L"tbAlpha";
			this->tbAlpha->Size = System::Drawing::Size(100, 22);
			this->tbAlpha->TabIndex = 8;
			this->tbAlpha->Text = L"1";

			this->tbPhi->Location = System::Drawing::Point(240, 253);
			this->tbPhi->Name = L"tbPhi";
			this->tbPhi->Size = System::Drawing::Size(100, 22);
			this->tbPhi->TabIndex = 9;
			this->tbPhi->Text = L"0";

			this->btnSimulate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnSimulate->Location = System::Drawing::Point(240, 295);
			this->btnSimulate->Name = L"btnSimulate";
			this->btnSimulate->Size = System::Drawing::Size(100, 45);
			this->btnSimulate->TabIndex = 10;
			this->btnSimulate->Text = L"Simulate";
			this->btnSimulate->Click += gcnew System::EventHandler(this, &EMWaveForm::btnSimulate_Click);

			this->picGraph->BackColor = System::Drawing::Color::White;
			this->picGraph->Location = System::Drawing::Point(420, 53);
			this->picGraph->Name = L"picGraph";
			this->picGraph->Size = System::Drawing::Size(1000, 615);
			this->picGraph->TabIndex = 11;
			this->picGraph->TabStop = false;

			this->simTimer->Interval = 10;
			this->simTimer->Tick += gcnew System::EventHandler(this, &EMWaveForm::simTimer_Tick);

			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(240, 232, 255);
			this->ClientSize = System::Drawing::Size(1504, 720);
			this->Name = L"EMWaveForm";
			this->Text = L"EM Wave Simulator";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;

			this->Controls->Add(this->lblTitle);
			this->Controls->Add(this->lblA);
			this->Controls->Add(this->lblF);
			this->Controls->Add(this->lblAlpha);
			this->Controls->Add(this->lblPhi);
			this->Controls->Add(this->tbA);
			this->Controls->Add(this->tbF);
			this->Controls->Add(this->tbAlpha);
			this->Controls->Add(this->tbPhi);
			this->Controls->Add(this->btnSimulate);
			this->Controls->Add(this->picGraph);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

		void DrawGraph() {
			double A, f, alpha, phi;
			if (!Double::TryParse(tbA->Text, A) || A <= 0 ||
				!Double::TryParse(tbF->Text, f) || f <= 0 ||
				!Double::TryParse(tbAlpha->Text, alpha) || alpha < 0 ||
				!Double::TryParse(tbPhi->Text, phi)) {
				MessageBox::Show("Please enter valid numbers.\nA and f must be positive. Alpha must be >= 0.");
				return;
			}

			simTimer->Stop();
			drawIndex = 0;

			int w = picGraph->Width;
			int h = picGraph->Height;

			EMWaveResult^ result = RunEMWaveSimulation(A, f, alpha, phi, w - 50);

			double rng = result->eMax - result->eMin;
			double yMin = result->eMin - 0.05 * rng;
			double yMax = result->eMax + 0.05 * rng;

			yPoints = ScaleToPixels(result->ePoints, h, yMin, yMax);
			bmp = DrawAxes(w, h, "t (s)", "E(t) V/m",
				"f = " + f.ToString("F2") +
				" Hz   |   a = " + alpha.ToString("F2") +
				" Np/s   |   Total: " + result->totalTime.ToString("F4") + "s");
			g = Graphics::FromImage(bmp);
			graphPen = gcnew Pen(System::Drawing::Color::FromArgb(140, 40, 200), 2);

			picGraph->Image = bmp;
			simTimer->Start();
		}

		System::Void btnSimulate_Click(System::Object^ sender, System::EventArgs^ e) {
			DrawGraph();
		}

		System::Void simTimer_Tick(System::Object^ sender, System::EventArgs^ e) {
			if (drawIndex >= yPoints->Length - 1) { simTimer->Stop(); return; }
			for (int i = 0; i < 2 && drawIndex < yPoints->Length - 1; i++) {
				g->DrawLine(graphPen,
					(float)(40 + drawIndex), (float)yPoints[drawIndex],
					(float)(40 + drawIndex + 1), (float)yPoints[drawIndex + 1]);
				drawIndex++;
			}
			picGraph->Refresh();
		}
	};
}
