#pragma once
#include "Grapher.h"

namespace CppCLRWinFormsProject {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class FourierForm : public System::Windows::Forms::Form
	{
	public:
		FourierForm(void) { InitializeComponent(); }

	protected:
		~FourierForm() { if (components) { delete components; } }

	private:
		System::Windows::Forms::Label^ lblTitle;
		System::Windows::Forms::Label^ lblFormula;
		System::Windows::Forms::Label^ lblWave;
		System::Windows::Forms::Label^ lblTerms;
		System::Windows::Forms::Label^ lblTermsVal;
		System::Windows::Forms::Label^ lblWavefronts;
		System::Windows::Forms::Label^ lblWavefrontsVal;
		System::Windows::Forms::TrackBar^ sliderTerms;
		System::Windows::Forms::TrackBar^ sliderWavefronts;
		System::Windows::Forms::Button^ btnSquare;
		System::Windows::Forms::Button^ btnSawtooth;
		System::Windows::Forms::Button^ btnTriangle;
		System::Windows::Forms::Button^ btnSimulate;
		System::Windows::Forms::PictureBox^ picGraph;
		System::Windows::Forms::Timer^ simTimer;
		System::ComponentModel::IContainer^ components;

		array<double>^ yPoints;
		int                        drawIndex;
		System::Drawing::Bitmap^ bmp;
		System::Drawing::Graphics^ g;
		Pen^ graphPen;

		int selectedWave;
		static const int steps = 950;

		static System::Drawing::Color WaveActive() { return System::Drawing::Color::FromArgb(220, 180, 0); }
		static System::Drawing::Color WaveInactive() { return System::Drawing::Color::FromArgb(80, 70, 20); }

#pragma region
		void InitializeComponent(void)
		{
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;

			this->components = (gcnew System::ComponentModel::Container());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblFormula = (gcnew System::Windows::Forms::Label());
			this->lblWave = (gcnew System::Windows::Forms::Label());
			this->lblTerms = (gcnew System::Windows::Forms::Label());
			this->lblTermsVal = (gcnew System::Windows::Forms::Label());
			this->lblWavefronts = (gcnew System::Windows::Forms::Label());
			this->lblWavefrontsVal = (gcnew System::Windows::Forms::Label());
			this->sliderTerms = (gcnew System::Windows::Forms::TrackBar());
			this->sliderWavefronts = (gcnew System::Windows::Forms::TrackBar());
			this->btnSquare = (gcnew System::Windows::Forms::Button());
			this->btnSawtooth = (gcnew System::Windows::Forms::Button());
			this->btnTriangle = (gcnew System::Windows::Forms::Button());
			this->btnSimulate = (gcnew System::Windows::Forms::Button());
			this->picGraph = (gcnew System::Windows::Forms::PictureBox());
			this->simTimer = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->sliderTerms))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->sliderWavefronts))->BeginInit();
			this->SuspendLayout();

			// lblTitle
			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->lblTitle->Location = System::Drawing::Point(17, 40);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->TabIndex = 0;
			this->lblTitle->Text = L"Fourier Series";

			// lblFormula
			this->lblFormula->AutoSize = true;
			this->lblFormula->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Italic));
			this->lblFormula->ForeColor = System::Drawing::Color::FromArgb(160, 130, 0);
			this->lblFormula->Location = System::Drawing::Point(20, 88);
			this->lblFormula->Name = L"lblFormula";
			this->lblFormula->TabIndex = 1;
			this->lblFormula->Text = L"Sum of harmonics approximating a periodic waveform";

			// lblWave
			this->lblWave->AutoSize = true;
			this->lblWave->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblWave->Location = System::Drawing::Point(42, 130);
			this->lblWave->Name = L"lblWave";
			this->lblWave->TabIndex = 2;
			this->lblWave->Text = L"Waveform";

			// btnSquare
			this->btnSquare->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSquare->FlatAppearance->BorderSize = 2;
			this->btnSquare->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->btnSquare->BackColor = WaveActive();
			this->btnSquare->ForeColor = System::Drawing::Color::Black;
			this->btnSquare->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnSquare->Location = System::Drawing::Point(42, 158);
			this->btnSquare->Name = L"btnSquare";
			this->btnSquare->Size = System::Drawing::Size(110, 34);
			this->btnSquare->TabIndex = 3;
			this->btnSquare->Text = L"Square";
			this->btnSquare->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSquare->Click += gcnew System::EventHandler(this, &FourierForm::btnSquare_Click);

			// btnSawtooth
			this->btnSawtooth->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSawtooth->FlatAppearance->BorderSize = 2;
			this->btnSawtooth->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->btnSawtooth->BackColor = WaveInactive();
			this->btnSawtooth->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
			this->btnSawtooth->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnSawtooth->Location = System::Drawing::Point(160, 158);
			this->btnSawtooth->Name = L"btnSawtooth";
			this->btnSawtooth->Size = System::Drawing::Size(110, 34);
			this->btnSawtooth->TabIndex = 4;
			this->btnSawtooth->Text = L"Sawtooth";
			this->btnSawtooth->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSawtooth->Click += gcnew System::EventHandler(this, &FourierForm::btnSawtooth_Click);

			// btnTriangle
			this->btnTriangle->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnTriangle->FlatAppearance->BorderSize = 2;
			this->btnTriangle->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->btnTriangle->BackColor = WaveInactive();
			this->btnTriangle->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
			this->btnTriangle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnTriangle->Location = System::Drawing::Point(278, 158);
			this->btnTriangle->Name = L"btnTriangle";
			this->btnTriangle->Size = System::Drawing::Size(110, 34);
			this->btnTriangle->TabIndex = 5;
			this->btnTriangle->Text = L"Triangle";
			this->btnTriangle->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnTriangle->Click += gcnew System::EventHandler(this, &FourierForm::btnTriangle_Click);

			// lblTerms
			this->lblTerms->AutoSize = true;
			this->lblTerms->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblTerms->Location = System::Drawing::Point(42, 210);
			this->lblTerms->Name = L"lblTerms";
			this->lblTerms->TabIndex = 6;
			this->lblTerms->Text = L"Harmonics (N)";

			// lblTermsVal
			this->lblTermsVal->AutoSize = true;
			this->lblTermsVal->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblTermsVal->ForeColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->lblTermsVal->Location = System::Drawing::Point(360, 210);
			this->lblTermsVal->Name = L"lblTermsVal";
			this->lblTermsVal->TabIndex = 7;
			this->lblTermsVal->Text = L"1";

			// sliderTerms
			this->sliderTerms->Location = System::Drawing::Point(42, 235);
			this->sliderTerms->Name = L"sliderTerms";
			this->sliderTerms->Size = System::Drawing::Size(346, 45);
			this->sliderTerms->TabIndex = 8;
			this->sliderTerms->Minimum = 1;
			this->sliderTerms->Maximum = 50;
			this->sliderTerms->Value = 1;
			this->sliderTerms->TickFrequency = 5;
			this->sliderTerms->BackColor = System::Drawing::Color::FromArgb(255, 248, 210);
			this->sliderTerms->Scroll += gcnew System::EventHandler(this, &FourierForm::sliderTerms_Scroll);

			// lblWavefronts
			this->lblWavefronts->AutoSize = true;
			this->lblWavefronts->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblWavefronts->Location = System::Drawing::Point(42, 295);
			this->lblWavefronts->Name = L"lblWavefronts";
			this->lblWavefronts->TabIndex = 9;
			this->lblWavefronts->Text = L"Wavefronts (periods)";

			// lblWavefrontsVal
			this->lblWavefrontsVal->AutoSize = true;
			this->lblWavefrontsVal->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblWavefrontsVal->ForeColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->lblWavefrontsVal->Location = System::Drawing::Point(360, 295);
			this->lblWavefrontsVal->Name = L"lblWavefrontsVal";
			this->lblWavefrontsVal->TabIndex = 10;
			this->lblWavefrontsVal->Text = L"2";

			// sliderWavefronts
			this->sliderWavefronts->Location = System::Drawing::Point(42, 320);
			this->sliderWavefronts->Name = L"sliderWavefronts";
			this->sliderWavefronts->Size = System::Drawing::Size(346, 45);
			this->sliderWavefronts->TabIndex = 11;
			this->sliderWavefronts->Minimum = 1;
			this->sliderWavefronts->Maximum = 10;
			this->sliderWavefronts->Value = 2;
			this->sliderWavefronts->TickFrequency = 1;
			this->sliderWavefronts->BackColor = System::Drawing::Color::FromArgb(255, 248, 210);
			this->sliderWavefronts->Scroll += gcnew System::EventHandler(this, &FourierForm::sliderWavefronts_Scroll);

			// btnSimulate
			this->btnSimulate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnSimulate->Location = System::Drawing::Point(42, 382);
			this->btnSimulate->Name = L"btnSimulate";
			this->btnSimulate->Size = System::Drawing::Size(100, 45);
			this->btnSimulate->TabIndex = 12;
			this->btnSimulate->Text = L"Simulate";
			this->btnSimulate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSimulate->FlatAppearance->BorderSize = 0;
			this->btnSimulate->BackColor = System::Drawing::Color::FromArgb(220, 180, 0);
			this->btnSimulate->ForeColor = System::Drawing::Color::Black;
			this->btnSimulate->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSimulate->Click += gcnew System::EventHandler(this, &FourierForm::btnSimulate_Click);

			// picGraph
			this->picGraph->BackColor = System::Drawing::Color::White;
			this->picGraph->Location = System::Drawing::Point(420, 53);
			this->picGraph->Name = L"picGraph";
			this->picGraph->Size = System::Drawing::Size(1000, 615);
			this->picGraph->TabIndex = 13;
			this->picGraph->TabStop = false;

			// simTimer
			this->simTimer->Interval = 10;
			this->simTimer->Tick += gcnew System::EventHandler(this, &FourierForm::simTimer_Tick);

			// Form
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(255, 248, 210);
			this->ClientSize = System::Drawing::Size(1504, 720);
			this->Name = L"FourierForm";
			this->Text = L"Fourier Series Simulator";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;

			this->Controls->Add(this->lblTitle);
			this->Controls->Add(this->lblWave);
			this->Controls->Add(this->btnSquare);
			this->Controls->Add(this->btnSawtooth);
			this->Controls->Add(this->btnTriangle);
			this->Controls->Add(this->lblTerms);
			this->Controls->Add(this->lblTermsVal);
			this->Controls->Add(this->sliderTerms);
			this->Controls->Add(this->lblWavefronts);
			this->Controls->Add(this->lblWavefrontsVal);
			this->Controls->Add(this->sliderWavefronts);
			this->Controls->Add(this->btnSimulate);
			this->Controls->Add(this->picGraph);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->sliderTerms))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->sliderWavefronts))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

			selectedWave = 0;
		}
#pragma endregion
		//Waveform selector 
		void SetWaveButtons(int wave) {
			selectedWave = wave;
			btnSquare->BackColor = WaveInactive();
			btnSquare->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
			btnSawtooth->BackColor = WaveInactive();
			btnSawtooth->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
			btnTriangle->BackColor = WaveInactive();
			btnTriangle->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
			System::Windows::Forms::Button^ active =
				(wave == 0) ? btnSquare : (wave == 1) ? btnSawtooth : btnTriangle;
			active->BackColor = WaveActive();
			active->ForeColor = System::Drawing::Color::Black;
		}

		void btnSquare_Click(System::Object^ sender, System::EventArgs^ e) { SetWaveButtons(0); }
		void btnSawtooth_Click(System::Object^ sender, System::EventArgs^ e) { SetWaveButtons(1); }
		void btnTriangle_Click(System::Object^ sender, System::EventArgs^ e) { SetWaveButtons(2); }


		void sliderTerms_Scroll(System::Object^ sender, System::EventArgs^ e) {
			lblTermsVal->Text = sliderTerms->Value.ToString();
		}
		void sliderWavefronts_Scroll(System::Object^ sender, System::EventArgs^ e) {
			lblWavefrontsVal->Text = sliderWavefronts->Value.ToString();
		}

		//Fourier series calculations
		array<double>^ ComputeSeries(int N, int periods) {
			array<double>^ vals = gcnew array<double>(steps);
			double pi = Math::PI;

			for (int i = 0; i < steps; i++) {
				double x = (2.0 * pi * periods * i) / steps;
				double sum = 0.0;

				if (selectedWave == 0) {
					// Square
					for (int k = 1; k <= N; k += 2)
						sum += Math::Sin(k * x) / k;
					sum *= (4.0 / pi);
				}
				else if (selectedWave == 1) {
					// Sawtooth
					for (int k = 1; k <= N; k++)
						sum += (Math::Pow(-1.0, k + 1) * Math::Sin(k * x)) / k;
					sum *= (2.0 / pi);
				}
				else {
					// Triangle
					for (int k = 1; k <= N; k += 2) {
						double sign = Math::Pow(-1.0, (k - 1) / 2.0);
						sum += sign * Math::Sin(k * x) / (k * k);
					}
					sum *= (8.0 / (pi * pi));
				}

				vals[i] = sum;
			}
			return vals;
		}

		void DrawGraph() {
			simTimer->Stop();
			drawIndex = 0;

			int N = sliderTerms->Value;
			int periods = sliderWavefronts->Value;
			int w = picGraph->Width;
			int h = picGraph->Height;

			array<double>^ raw = ComputeSeries(N, periods);
			yPoints = ScaleToPixels(raw, h, -1.2, 1.2);

			String^ waveNames = (selectedWave == 0) ? "Square" :
				(selectedWave == 1) ? "Sawtooth" : "Triangle";
			bmp = DrawAxes(w, h, "t", "A",
				waveNames + " wave  |  N = " + N.ToString() + " harmonics  |  Periods = " + periods.ToString());
			g = Graphics::FromImage(bmp);
			graphPen = gcnew Pen(System::Drawing::Color::FromArgb(200, 160, 0), 2);

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
