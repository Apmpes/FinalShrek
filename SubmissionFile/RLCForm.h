#pragma once
#include "Simulation.h"
#include "Inputs.h"
#include "Grapher.h"

namespace CppCLRWinFormsProject {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class RLCForm : public System::Windows::Forms::Form
	{
	public:
		RLCForm(void) { InitializeComponent(); }

	protected:
		~RLCForm() { if (components) { delete components; } }

	private:
		System::Windows::Forms::Label^ ResistanceLabel;
		System::Windows::Forms::Label^ InductanceLabel;
		System::Windows::Forms::Label^ CapacitanceLabel;
		System::Windows::Forms::Label^ Title;
		System::Windows::Forms::TextBox^ tbR;
		System::Windows::Forms::TextBox^ tbL;
		System::Windows::Forms::TextBox^ tbC;
		System::Windows::Forms::Button^ btnSimulate;
		System::Windows::Forms::PictureBox^ picGraph;
		System::Windows::Forms::Timer^ simTimer;
		System::ComponentModel::IContainer^ components;

		cli::array<double>^ yPoints;
		int drawIndex;
		System::Drawing::Bitmap^ bmp;
		System::Drawing::Graphics^ g;
		Pen^ graphPen;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;


			this->components = (gcnew System::ComponentModel::Container());
			this->ResistanceLabel = (gcnew System::Windows::Forms::Label());
			this->InductanceLabel = (gcnew System::Windows::Forms::Label());
			this->CapacitanceLabel = (gcnew System::Windows::Forms::Label());
			this->Title = (gcnew System::Windows::Forms::Label());
			this->tbR = (gcnew System::Windows::Forms::TextBox());
			this->tbL = (gcnew System::Windows::Forms::TextBox());
			this->tbC = (gcnew System::Windows::Forms::TextBox());
			this->btnSimulate = (gcnew System::Windows::Forms::Button());
			this->picGraph = (gcnew System::Windows::Forms::PictureBox());
			this->simTimer = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->BeginInit();
			this->SuspendLayout();

			this->ResistanceLabel->AutoSize = true;
			this->ResistanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->ResistanceLabel->Location = System::Drawing::Point(42, 135);
			this->ResistanceLabel->Name = L"ResistanceLabel";
			this->ResistanceLabel->TabIndex = 1;
			this->ResistanceLabel->Text = L"Resistance R (Ohms)";

			this->InductanceLabel->AutoSize = true;
			this->InductanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->InductanceLabel->Location = System::Drawing::Point(42, 173);
			this->InductanceLabel->Name = L"InductanceLabel";
			this->InductanceLabel->TabIndex = 2;
			this->InductanceLabel->Text = L"Inductance L (H)";

			this->CapacitanceLabel->AutoSize = true;
			this->CapacitanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->CapacitanceLabel->Location = System::Drawing::Point(42, 213);
			this->CapacitanceLabel->Name = L"CapacitanceLabel";
			this->CapacitanceLabel->TabIndex = 3;
			this->CapacitanceLabel->Text = L"Capacitance C (F)";

			this->Title->AutoSize = true;
			this->Title->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->Title->Location = System::Drawing::Point(17, 53);
			this->Title->Name = L"Title";
			this->Title->TabIndex = 0;
			this->Title->ForeColor = System::Drawing::Color::FromArgb(120, 250, 180);
			this->Title->Text = L"RLC Circuit Simulator";

			this->tbR->Location = System::Drawing::Point(224, 135);
			this->tbR->Name = L"tbR";
			this->tbR->Size = System::Drawing::Size(100, 22);
			this->tbR->TabIndex = 4;

			this->tbL->Location = System::Drawing::Point(224, 175);
			this->tbL->Name = L"tbL";
			this->tbL->Size = System::Drawing::Size(100, 22);
			this->tbL->TabIndex = 5;

			this->tbC->Location = System::Drawing::Point(224, 215);
			this->tbC->Name = L"tbC";
			this->tbC->Size = System::Drawing::Size(100, 22);
			this->tbC->TabIndex = 6;

			this->btnSimulate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnSimulate->Location = System::Drawing::Point(224, 255);
			this->btnSimulate->Name = L"btnSimulate";
			this->btnSimulate->Size = System::Drawing::Size(100, 45);
			this->btnSimulate->TabIndex = 7;
			this->btnSimulate->Text = L"Simulate";
			this->btnSimulate->Click += gcnew System::EventHandler(this, &RLCForm::btnSimulate_Click);

			this->picGraph->BackColor = System::Drawing::Color::White;
			this->picGraph->Location = System::Drawing::Point(420, 53);
			this->picGraph->Name = L"picGraph";
			this->picGraph->Size = System::Drawing::Size(1000, 615);
			this->picGraph->TabIndex = 8;
			this->picGraph->TabStop = false;

			this->simTimer->Interval = 10;
			this->simTimer->Tick += gcnew System::EventHandler(this, &RLCForm::simTimer_Tick);

			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(230, 244, 255);
			this->ClientSize = System::Drawing::Size(1504, 886);
			this->Name = L"RLCForm";
			this->Text = L"RLC Circuit Simulator";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;

			this->Controls->Add(this->Title);
			this->Controls->Add(this->ResistanceLabel);
			this->Controls->Add(this->InductanceLabel);
			this->Controls->Add(this->CapacitanceLabel);
			this->Controls->Add(this->tbR);
			this->Controls->Add(this->tbL);
			this->Controls->Add(this->tbC);
			this->Controls->Add(this->btnSimulate);
			this->Controls->Add(this->picGraph);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

		void DrawGraph(double R, double L, double C) {
			simTimer->Stop();
			drawIndex = 0;

			int w = picGraph->Width;
			int h = picGraph->Height;

			SimulationResult^ result = RunSimulation(R, L, C, w - 50);

			double yMin = 0, yMax = 1;
			yPoints = ScaleToPixels(result->yPoints, h, yMin, yMax);
			bmp = DrawAxes(w, h, "t (s)", "V(t)", "Total time: " + result->totalTime.ToString("F4") + "s");
			g = Graphics::FromImage(bmp);
			graphPen = gcnew Pen(Color::Pink, 2);

			picGraph->Image = bmp;
			simTimer->Start();
		}

		System::Void btnSimulate_Click(System::Object^ sender, System::EventArgs^ e) {
			double R, L, C;
			if (!TryGetInputs(tbR, tbL, tbC, R, L, C)) return;
			DrawGraph(R, L, C);
		}

		//Draws the graph slowly point by point
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
