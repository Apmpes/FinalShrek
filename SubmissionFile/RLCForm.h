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
        RLCForm(void) : hasSimulated(false), lastR(0), lastL(0), lastC(0) { InitializeComponent(); }

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
        int                        drawIndex;
        System::Drawing::Bitmap^ bmp;
        System::Drawing::Graphics^ g;
        Pen^ graphPen;

        bool   hasSimulated;
        double lastR, lastL, lastC;

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

            this->Title->AutoSize = true;
            this->Title->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
            this->Title->Location = System::Drawing::Point(17, 53);
            this->Title->Name = L"Title";
            this->Title->TabIndex = 0;
            this->Title->ForeColor = System::Drawing::Color::FromArgb(120, 250, 180);
            this->Title->Text = L"RLC Circuit Simulator";
            this->Title->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->ResistanceLabel->AutoSize = true;
            this->ResistanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->ResistanceLabel->Location = System::Drawing::Point(42, 135);
            this->ResistanceLabel->Name = L"ResistanceLabel";
            this->ResistanceLabel->TabIndex = 1;
            this->ResistanceLabel->Text = L"Resistance R (Ohms)";
            this->ResistanceLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->InductanceLabel->AutoSize = true;
            this->InductanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->InductanceLabel->Location = System::Drawing::Point(42, 173);
            this->InductanceLabel->Name = L"InductanceLabel";
            this->InductanceLabel->TabIndex = 2;
            this->InductanceLabel->Text = L"Inductance L (H)";
            this->InductanceLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->CapacitanceLabel->AutoSize = true;
            this->CapacitanceLabel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->CapacitanceLabel->Location = System::Drawing::Point(42, 213);
            this->CapacitanceLabel->Name = L"CapacitanceLabel";
            this->CapacitanceLabel->TabIndex = 3;
            this->CapacitanceLabel->Text = L"Capacitance C (F)";
            this->CapacitanceLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbR->Location = System::Drawing::Point(224, 135);
            this->tbR->Name = L"tbR";
            this->tbR->Size = System::Drawing::Size(100, 22);
            this->tbR->TabIndex = 4;
            this->tbR->Text = L"10";
            this->tbR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbL->Location = System::Drawing::Point(224, 175);
            this->tbL->Name = L"tbL";
            this->tbL->Size = System::Drawing::Size(100, 22);
            this->tbL->TabIndex = 5;
            this->tbL->Text = L"0.1";
            this->tbL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbC->Location = System::Drawing::Point(224, 215);
            this->tbC->Name = L"tbC";
            this->tbC->Size = System::Drawing::Size(100, 22);
            this->tbC->TabIndex = 6;
            this->tbC->Text = L"0.001";
            this->tbC->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->btnSimulate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->btnSimulate->Location = System::Drawing::Point(224, 255);
            this->btnSimulate->Name = L"btnSimulate";
            this->btnSimulate->Size = System::Drawing::Size(100, 45);
            this->btnSimulate->TabIndex = 7;
            this->btnSimulate->Text = L"Simulate";
            this->btnSimulate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);
            this->btnSimulate->Click += gcnew System::EventHandler(this, &RLCForm::btnSimulate_Click);

            this->picGraph->BackColor = System::Drawing::Color::White;
            this->picGraph->Location = System::Drawing::Point(420, 53);
            this->picGraph->Size = System::Drawing::Size(1000, 700);
            this->picGraph->Name = L"picGraph";
            this->picGraph->TabIndex = 8;
            this->picGraph->TabStop = false;
            this->picGraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Bottom |
                System::Windows::Forms::AnchorStyles::Left |
                System::Windows::Forms::AnchorStyles::Right);

            this->simTimer->Interval = 10;
            this->simTimer->Tick += gcnew System::EventHandler(this, &RLCForm::simTimer_Tick);

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(230, 244, 255);
            this->ClientSize = System::Drawing::Size(1504, 820);
            this->Name = L"RLCForm";
            this->Text = L"RLC Circuit Simulator";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Resize += gcnew System::EventHandler(this, &RLCForm::RLCForm_Resize);

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

        void DrawGraph() {
            simTimer->Stop();
            drawIndex = 0;

            int w = picGraph->Width;
            int h = picGraph->Height;

            SimulationResult^ result = RunSimulation(lastR, lastL, lastC, w - 50);

            double yMin = 0, yMax = 1;
            yPoints = ScaleToPixels(result->yPoints, h, yMin, yMax);
            bmp = DrawAxes(w, h, "t (s)", "V(t)",
                "Total time: " + result->totalTime.ToString("F4") + "s");
            g = Graphics::FromImage(bmp);
            graphPen = gcnew Pen(Color::Pink, 2);

            picGraph->Image = bmp;
            simTimer->Start();
        }

        System::Void btnSimulate_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!TryGetInputs(tbR, tbL, tbC, lastR, lastL, lastC)) return;
            hasSimulated = true;
            DrawGraph();
        }

        System::Void RLCForm_Resize(System::Object^ sender, System::EventArgs^ e) {
            if (!hasSimulated) return;
            if (picGraph->Width > 0 && picGraph->Height > 0) {
                DrawGraph();
            }
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