#pragma once
#include "Grapher.h"

namespace CppCLRWinFormsProject {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    ref struct RCResult {
        cli::array<double>^ vPoints;
        double tau;
        double totalTime;
    };

    static RCResult^ RunRCSimulation(double R, double C, double V0, double Vf, int steps) {
        double tau = R * C;
        double totalTime = 5.0 * tau;
        double dt = totalTime / steps;
        cli::array<double>^ v = gcnew cli::array<double>(steps);
        for (int i = 0; i < steps; i++) {
            double t = i * dt;
            v[i] = Vf + (V0 - Vf) * Math::Exp(-t / tau);
        }
        RCResult^ r = gcnew RCResult();
        r->vPoints = v;
        r->tau = tau;
        r->totalTime = totalTime;
        return r;
    }

    public ref class RCForm : public System::Windows::Forms::Form
    {
    public:
        RCForm(void) : hasSimulated(false), lastR(0), lastC(0), lastV0(0), lastVf(0) { InitializeComponent(); }

    protected:
        ~RCForm() { if (components) { delete components; } }

    private:
        System::Windows::Forms::Label^ lblTitle;
        System::Windows::Forms::Label^ lblR;
        System::Windows::Forms::Label^ lblC;
        System::Windows::Forms::Label^ lblV0;
        System::Windows::Forms::Label^ lblVf;
        System::Windows::Forms::TextBox^ tbR;
        System::Windows::Forms::TextBox^ tbC;
        System::Windows::Forms::TextBox^ tbV0;
        System::Windows::Forms::TextBox^ tbVf;
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
        double lastR, lastC, lastV0, lastVf;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->WindowState = System::Windows::Forms::FormWindowState::Maximized;

            this->components = (gcnew System::ComponentModel::Container());
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->lblR = (gcnew System::Windows::Forms::Label());
            this->lblC = (gcnew System::Windows::Forms::Label());
            this->lblV0 = (gcnew System::Windows::Forms::Label());
            this->lblVf = (gcnew System::Windows::Forms::Label());
            this->tbR = (gcnew System::Windows::Forms::TextBox());
            this->tbC = (gcnew System::Windows::Forms::TextBox());
            this->tbV0 = (gcnew System::Windows::Forms::TextBox());
            this->tbVf = (gcnew System::Windows::Forms::TextBox());
            this->btnSimulate = (gcnew System::Windows::Forms::Button());
            this->picGraph = (gcnew System::Windows::Forms::PictureBox());
            this->simTimer = (gcnew System::Windows::Forms::Timer(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->picGraph))->BeginInit();
            this->SuspendLayout();

            this->lblTitle->AutoSize = true;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(0, 180, 120);
            this->lblTitle->Location = System::Drawing::Point(17, 53);
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->TabIndex = 0;
            this->lblTitle->Text = L"RC Charging Simulator";
            this->lblTitle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->lblR->AutoSize = true;
            this->lblR->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->lblR->Location = System::Drawing::Point(42, 135);
            this->lblR->Name = L"lblR";
            this->lblR->TabIndex = 1;
            this->lblR->Text = L"Resistance R (Ohms)";
            this->lblR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->lblC->AutoSize = true;
            this->lblC->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->lblC->Location = System::Drawing::Point(42, 173);
            this->lblC->Name = L"lblC";
            this->lblC->TabIndex = 2;
            this->lblC->Text = L"Capacitance C (F)";
            this->lblC->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->lblV0->AutoSize = true;
            this->lblV0->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->lblV0->Location = System::Drawing::Point(42, 213);
            this->lblV0->Name = L"lblV0";
            this->lblV0->TabIndex = 3;
            this->lblV0->Text = L"Initial voltage V0 (V)";
            this->lblV0->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->lblVf->AutoSize = true;
            this->lblVf->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->lblVf->Location = System::Drawing::Point(42, 253);
            this->lblVf->Name = L"lblVf";
            this->lblVf->TabIndex = 4;
            this->lblVf->Text = L"Supply voltage Vf (V)";
            this->lblVf->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbR->Location = System::Drawing::Point(260, 135);
            this->tbR->Name = L"tbR";
            this->tbR->Size = System::Drawing::Size(100, 22);
            this->tbR->TabIndex = 5;
            this->tbR->Text = L"1000";
            this->tbR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbC->Location = System::Drawing::Point(260, 173);
            this->tbC->Name = L"tbC";
            this->tbC->Size = System::Drawing::Size(100, 22);
            this->tbC->TabIndex = 6;
            this->tbC->Text = L"0.001";
            this->tbC->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbV0->Location = System::Drawing::Point(260, 213);
            this->tbV0->Name = L"tbV0";
            this->tbV0->Size = System::Drawing::Size(100, 22);
            this->tbV0->TabIndex = 7;
            this->tbV0->Text = L"0";
            this->tbV0->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->tbVf->Location = System::Drawing::Point(260, 253);
            this->tbVf->Name = L"tbVf";
            this->tbVf->Size = System::Drawing::Size(100, 22);
            this->tbVf->TabIndex = 8;
            this->tbVf->Text = L"5";
            this->tbVf->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);

            this->btnSimulate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->btnSimulate->Location = System::Drawing::Point(260, 295);
            this->btnSimulate->Name = L"btnSimulate";
            this->btnSimulate->Size = System::Drawing::Size(100, 45);
            this->btnSimulate->TabIndex = 9;
            this->btnSimulate->Text = L"Simulate";
            this->btnSimulate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Left);
            this->btnSimulate->Click += gcnew System::EventHandler(this, &RCForm::btnSimulate_Click);

            this->picGraph->BackColor = System::Drawing::Color::White;
            this->picGraph->Location = System::Drawing::Point(440, 53);
            this->picGraph->Size = System::Drawing::Size(1000, 700);
            this->picGraph->Name = L"picGraph";
            this->picGraph->TabIndex = 10;
            this->picGraph->TabStop = false;
            this->picGraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Bottom |
                System::Windows::Forms::AnchorStyles::Left |
                System::Windows::Forms::AnchorStyles::Right);

            this->simTimer->Interval = 10;
            this->simTimer->Tick += gcnew System::EventHandler(this, &RCForm::simTimer_Tick);

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(230, 250, 240);
            this->ClientSize = System::Drawing::Size(1504, 820);
            this->Name = L"RCForm";
            this->Text = L"RC Charging Simulator";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Resize += gcnew System::EventHandler(this, &RCForm::RCForm_Resize);

            this->Controls->Add(this->lblTitle);
            this->Controls->Add(this->lblR);
            this->Controls->Add(this->lblC);
            this->Controls->Add(this->lblV0);
            this->Controls->Add(this->lblVf);
            this->Controls->Add(this->tbR);
            this->Controls->Add(this->tbC);
            this->Controls->Add(this->tbV0);
            this->Controls->Add(this->tbVf);
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

            RCResult^ result = RunRCSimulation(lastR, lastC, lastV0, lastVf, w - 50);

            double lo = Math::Min(lastV0, lastVf);
            double hi = Math::Max(lastV0, lastVf);
            double rng = (hi - lo < 1e-9) ? 1.0 : (hi - lo);
            double yMin = lo - 0.1 * rng;
            double yMax = hi + 0.1 * rng;

            yPoints = ScaleToPixels(result->vPoints, h, yMin, yMax);
            bmp = DrawAxes(w, h, "t (s)", "V(t)",
                "Time constant = " + result->tau.ToString("F4") +
                "s   |   Total: " + result->totalTime.ToString("F4") + "s");
            g = Graphics::FromImage(bmp);
            graphPen = gcnew Pen(System::Drawing::Color::FromArgb(0, 160, 100), 2);

            picGraph->Image = bmp;
            simTimer->Start();
        }

        System::Void btnSimulate_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!Double::TryParse(tbR->Text, lastR) || lastR <= 0 ||
                !Double::TryParse(tbC->Text, lastC) || lastC <= 0 ||
                !Double::TryParse(tbV0->Text, lastV0) ||
                !Double::TryParse(tbVf->Text, lastVf)) {
                MessageBox::Show("Please enter valid numbers for all fields.\nR and C must be positive.");
                return;
            }
            hasSimulated = true;
            DrawGraph();
        }

        System::Void RCForm_Resize(System::Object^ sender, System::EventArgs^ e) {
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