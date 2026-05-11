#pragma once
#include "RLCForm.h"
#include "RCForm.h"
#include "EMWaveForm.h"
#include "FourierForm.h"

namespace CppCLRWinFormsProject {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1ANB : public System::Windows::Forms::Form
    {
    public:
        Form1ANB(void) { InitializeComponent(); }

    protected:
        ~Form1ANB() { if (components) { delete components; } }

    private:
        System::ComponentModel::IContainer^ components;
        System::Windows::Forms::Label^ lblTitle;
        System::Windows::Forms::Button^ btnRLC;
        System::Windows::Forms::Button^ btnRC;
        System::Windows::Forms::Button^ btnEM;
        System::Windows::Forms::Button^ btnFourier;
        System::Windows::Forms::TableLayoutPanel^ btnPanel;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->WindowState = System::Windows::Forms::FormWindowState::Maximized;

            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->btnPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->btnRLC = (gcnew System::Windows::Forms::Button());
            this->btnRC = (gcnew System::Windows::Forms::Button());
            this->btnEM = (gcnew System::Windows::Forms::Button());
            this->btnFourier = (gcnew System::Windows::Forms::Button());
            this->btnPanel->SuspendLayout();
            this->SuspendLayout();

            this->lblTitle->Dock = System::Windows::Forms::DockStyle::Top;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 28, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(0, 200, 180);
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->Padding = System::Windows::Forms::Padding(30, 30, 0, 0);
            this->lblTitle->Size = System::Drawing::Size(820, 100);
            this->lblTitle->TabIndex = 1;
            this->lblTitle->Text = L"Electromagnetic Simulator";

            this->btnPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnPanel->ColumnCount = 1;
            this->btnPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 50.0F)));
            this->btnPanel->RowCount = 4;
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25.0F)));
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25.0F)));
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25.0F)));
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 25.0F)));
            this->btnPanel->Controls->Add(this->btnRLC, 0, 0);
            this->btnPanel->Controls->Add(this->btnRC, 0, 1);
            this->btnPanel->Controls->Add(this->btnEM, 0, 2);
            this->btnPanel->Controls->Add(this->btnFourier, 0, 3);
            this->btnPanel->BackColor = System::Drawing::Color::Transparent;
            this->btnPanel->Padding = System::Windows::Forms::Padding(80, 20, 80, 40);
            this->btnPanel->Name = L"btnPanel";
            this->btnPanel->TabIndex = 2;

            this->btnRLC->BackColor = System::Drawing::Color::FromArgb(0, 140, 200);
            this->btnRLC->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnRLC->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnRLC->FlatAppearance->BorderSize = 0;
            this->btnRLC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnRLC->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnRLC->ForeColor = System::Drawing::Color::White;
            this->btnRLC->Margin = System::Windows::Forms::Padding(0, 8, 0, 8);
            this->btnRLC->Name = L"btnRLC";
            this->btnRLC->TabIndex = 3;
            this->btnRLC->Text = L"RLC Circuit";
            this->btnRLC->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->btnRLC->Padding = System::Windows::Forms::Padding(24, 0, 0, 0);
            this->btnRLC->UseVisualStyleBackColor = false;
            this->btnRLC->Click += gcnew System::EventHandler(this, &Form1ANB::btnRLC_Click);

            this->btnRC->BackColor = System::Drawing::Color::FromArgb(0, 180, 120);
            this->btnRC->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnRC->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnRC->FlatAppearance->BorderSize = 0;
            this->btnRC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnRC->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnRC->ForeColor = System::Drawing::Color::White;
            this->btnRC->Margin = System::Windows::Forms::Padding(0, 8, 0, 8);
            this->btnRC->Name = L"btnRC";
            this->btnRC->TabIndex = 4;
            this->btnRC->Text = L"RC Charging";
            this->btnRC->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->btnRC->Padding = System::Windows::Forms::Padding(24, 0, 0, 0);
            this->btnRC->UseVisualStyleBackColor = false;
            this->btnRC->Click += gcnew System::EventHandler(this, &Form1ANB::btnRC_Click);

            this->btnEM->BackColor = System::Drawing::Color::FromArgb(160, 60, 200);
            this->btnEM->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnEM->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnEM->FlatAppearance->BorderSize = 0;
            this->btnEM->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnEM->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnEM->ForeColor = System::Drawing::Color::White;
            this->btnEM->Margin = System::Windows::Forms::Padding(0, 8, 0, 8);
            this->btnEM->Name = L"btnEM";
            this->btnEM->TabIndex = 5;
            this->btnEM->Text = L"EM Wave";
            this->btnEM->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->btnEM->Padding = System::Windows::Forms::Padding(24, 0, 0, 0);
            this->btnEM->UseVisualStyleBackColor = false;
            this->btnEM->Click += gcnew System::EventHandler(this, &Form1ANB::btnEM_Click);

            this->btnFourier->BackColor = System::Drawing::Color::FromArgb(200, 160, 0);
            this->btnFourier->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnFourier->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnFourier->FlatAppearance->BorderSize = 0;
            this->btnFourier->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnFourier->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnFourier->ForeColor = System::Drawing::Color::Black;
            this->btnFourier->Margin = System::Windows::Forms::Padding(0, 8, 0, 8);
            this->btnFourier->Name = L"btnFourier";
            this->btnFourier->TabIndex = 6;
            this->btnFourier->Text = L"Fourier Series";
            this->btnFourier->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            this->btnFourier->Padding = System::Windows::Forms::Padding(24, 0, 0, 0);
            this->btnFourier->UseVisualStyleBackColor = false;
            this->btnFourier->Click += gcnew System::EventHandler(this, &Form1ANB::btnFourier_Click);

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(18, 18, 30);
            this->ClientSize = System::Drawing::Size(820, 620);
            this->Controls->Add(this->btnPanel);
            this->Controls->Add(this->lblTitle);
            this->Cursor = System::Windows::Forms::Cursors::Default;
            this->Name = L"Form1ANB";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"EM Simulator Suite";
            this->btnPanel->ResumeLayout(false);
            this->ResumeLayout(false);
        }
#pragma endregion

        System::Void btnRLC_Click(System::Object^ sender, System::EventArgs^ e) {
            RLCForm^ f = gcnew RLCForm();
            f->Show();
        }
        System::Void btnRC_Click(System::Object^ sender, System::EventArgs^ e) {
            RCForm^ f = gcnew RCForm();
            f->Show();
        }
        System::Void btnEM_Click(System::Object^ sender, System::EventArgs^ e) {
            EMWaveForm^ f = gcnew EMWaveForm();
            f->Show();
        }
        System::Void btnFourier_Click(System::Object^ sender, System::EventArgs^ e) {
            FourierForm^ f = gcnew FourierForm();
            f->Show();
        }
    };
}

//include this   to launch mine in project:

// #include "Form1ANB.h"

//System::Void btnAlex_Click(System::Object^ sender, System::EventArgs^ e) {
//	Form1ANB^ f = gcnew Form1ANB();
//	f->Show();
//}

//Include Uzairs
//#include "Form1_UH.h"
//System::Void btnUzair_Click(System::Object^ sender, System::EventArgs^ e) {
//	Form1_UH^ f = gcnew Form1_UH();
//	f->Show();

