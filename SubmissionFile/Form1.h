#pragma once
#include "Form1ANB.h"
#include "FormAPMFinal.h"
#include "Form1_UH.h"
//Includes each members forms to launch from homepage


namespace CppCLRWinFormsProject {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void) { InitializeComponent(); }

    protected:
        ~Form1() { if (components) { delete components; } }

    private:
        System::ComponentModel::IContainer^ components;
        System::Windows::Forms::Button^ btnAPM_ElectromagSim;
        System::Windows::Forms::Button^ btnANB_GraphingSim;
        System::Windows::Forms::Button^ btnUH_MagneticSim;
        System::Windows::Forms::Label^ lblTitle;
        System::Windows::Forms::TableLayoutPanel^ btnPanel;

#pragma region Windows Form Designer generated code
        System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {}

        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->btnPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->btnANB_GraphingSim = (gcnew System::Windows::Forms::Button());
            this->btnAPM_ElectromagSim = (gcnew System::Windows::Forms::Button());
            this->btnUH_MagneticSim = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();

            // Title
            this->lblTitle->Dock = System::Windows::Forms::DockStyle::Top;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 26, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(0, 210, 190);
            this->lblTitle->Height = 80;
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->Padding = System::Windows::Forms::Padding(20, 20, 0, 0);
            this->lblTitle->Text = L"Choose a Programme to Start";
            this->lblTitle->TabIndex = 0;

           
            // Button scaler
            this->btnPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnPanel->ColumnCount = 3;
            this->btnPanel->RowCount = 1;
            this->btnPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33f));
            this->btnPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33f));
            this->btnPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33f));
            this->btnPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100.0f));
            this->btnPanel->Padding = System::Windows::Forms::Padding(40, 30, 40, 60);
            this->btnPanel->BackColor = System::Drawing::Color::Transparent;
            this->btnPanel->Name = L"btnPanel";
            this->btnPanel->TabIndex = 2;

            // Alex's Button
            this->btnANB_GraphingSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnANB_GraphingSim->Margin = System::Windows::Forms::Padding(10);
            this->btnANB_GraphingSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnANB_GraphingSim->FlatAppearance->BorderSize = 0;
            this->btnANB_GraphingSim->BackColor = System::Drawing::Color::FromArgb(0, 140, 200);
            this->btnANB_GraphingSim->ForeColor = System::Drawing::Color::White;
            this->btnANB_GraphingSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnANB_GraphingSim->Text = L"Graphing\nSimulator";
            this->btnANB_GraphingSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnANB_GraphingSim->Name = L"btnANB_GraphingSim";
            this->btnANB_GraphingSim->TabIndex = 3;
            this->btnANB_GraphingSim->Click += gcnew System::EventHandler(this, &Form1::btnANB_GraphingSim_Click);

            //Andres button
            this->btnAPM_ElectromagSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnAPM_ElectromagSim->Margin = System::Windows::Forms::Padding(10);
            this->btnAPM_ElectromagSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnAPM_ElectromagSim->FlatAppearance->BorderSize = 0;
            this->btnAPM_ElectromagSim->BackColor = System::Drawing::Color::FromArgb(160, 60, 200);
            this->btnAPM_ElectromagSim->ForeColor = System::Drawing::Color::White;
            this->btnAPM_ElectromagSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnAPM_ElectromagSim->Text = L"Electromagnetism\nSimulator";
            this->btnAPM_ElectromagSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnAPM_ElectromagSim->Name = L"btnAPM_ElectromagSim";
            this->btnAPM_ElectromagSim->TabIndex = 4;
            this->btnAPM_ElectromagSim->Click += gcnew System::EventHandler(this, &Form1::btnAPM_ElectromagSim_Click);


            //Uzairs Button
            this->btnUH_MagneticSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnUH_MagneticSim->Margin = System::Windows::Forms::Padding(10);
            this->btnUH_MagneticSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnUH_MagneticSim->FlatAppearance->BorderSize = 0;
            this->btnUH_MagneticSim->BackColor = System::Drawing::Color::FromArgb(0, 180, 120);
            this->btnUH_MagneticSim->ForeColor = System::Drawing::Color::White;
            this->btnUH_MagneticSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnUH_MagneticSim->Text = L"Magnetic\nSimulator";
            this->btnUH_MagneticSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnUH_MagneticSim->Name = L"btnUH_MagneticSim";
            this->btnUH_MagneticSim->TabIndex = 5;
            this->btnUH_MagneticSim->Click += gcnew System::EventHandler(this, &Form1::btnUH_MagneticSim_Click);

            this->btnPanel->Controls->Add(this->btnANB_GraphingSim, 0, 0);
            this->btnPanel->Controls->Add(this->btnAPM_ElectromagSim, 1, 0);
            this->btnPanel->Controls->Add(this->btnUH_MagneticSim, 2, 0);

            //Form1 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(18, 18, 30);
            this->ClientSize = System::Drawing::Size(900, 500);
            this->MinimumSize = System::Drawing::Size(700, 400);
            this->Controls->Add(this->btnPanel);
            this->Controls->Add(this->lblTitle);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;
            this->Name = L"Form1";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->ResumeLayout(false);
            this->PerformLayout();
        }
#pragma endregion


        //Buttons to launch each individual function
        System::Void btnANB_GraphingSim_Click(System::Object^ sender, System::EventArgs^ e) {
            Form1ANB^ f = gcnew Form1ANB();
            f->Show();
        }
        System::Void btnUH_MagneticSim_Click(System::Object^ sender, System::EventArgs^ e) {
            Form1_UH^ f = gcnew Form1_UH();
            f->Show();
        }
        System::Void btnAPM_ElectromagSim_Click(System::Object^ sender, System::EventArgs^ e) {
            FormAPMFinal^ f = gcnew FormAPMFinal();
            f->Show();
        }
    };
}