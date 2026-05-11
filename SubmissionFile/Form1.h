#pragma once
#include "Form1ANB.h"
#include "FormAPMFinal.h"
#include "Form1_UH.h"

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
        System::Windows::Forms::Label^ blDescriptionANB;
        System::Windows::Forms::Label^ blDescriptionAPM;
        System::Windows::Forms::Label^ blDescriptionUH;
        System::Windows::Forms::Panel^ HelpPagePanel;
        System::Windows::Forms::Label^ FreqQuestTitle;
        System::Windows::Forms::Label^ HelpPageTitle;
        System::Windows::Forms::Label^ ContactTxt;
        System::Windows::Forms::Label^ ContactTitle;
        System::Windows::Forms::Label^ FreqQuestText;
        System::Windows::Forms::Button^ ExitHelpPage;
        System::Windows::Forms::Button^ EnterHelpPage;
        System::Windows::Forms::TableLayoutPanel^ btnPanel;

        System::Void btnPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {}

#pragma region Windows Form Designer generated code
        System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {}

        void InitializeComponent(void)
        {
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->btnPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->btnANB_GraphingSim = (gcnew System::Windows::Forms::Button());
            this->btnAPM_ElectromagSim = (gcnew System::Windows::Forms::Button());
            this->btnUH_MagneticSim = (gcnew System::Windows::Forms::Button());
            this->blDescriptionANB = (gcnew System::Windows::Forms::Label());
            this->blDescriptionAPM = (gcnew System::Windows::Forms::Label());
            this->blDescriptionUH = (gcnew System::Windows::Forms::Label());
            this->HelpPagePanel = (gcnew System::Windows::Forms::Panel());
            this->ExitHelpPage = (gcnew System::Windows::Forms::Button());
            this->ContactTxt = (gcnew System::Windows::Forms::Label());
            this->ContactTitle = (gcnew System::Windows::Forms::Label());
            this->FreqQuestText = (gcnew System::Windows::Forms::Label());
            this->FreqQuestTitle = (gcnew System::Windows::Forms::Label());
            this->HelpPageTitle = (gcnew System::Windows::Forms::Label());
            this->EnterHelpPage = (gcnew System::Windows::Forms::Button());
            this->btnPanel->SuspendLayout();
            this->HelpPagePanel->SuspendLayout();
            this->SuspendLayout();

            this->lblTitle->Dock = System::Windows::Forms::DockStyle::Top;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 26, System::Drawing::FontStyle::Bold));
            this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(0, 210, 190);
            this->lblTitle->Location = System::Drawing::Point(0, 0);
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->Padding = System::Windows::Forms::Padding(20, 20, 0, 0);
            this->lblTitle->Size = System::Drawing::Size(1002, 80);
            this->lblTitle->TabIndex = 0;
            this->lblTitle->Text = L"Choose a Programme to Start";

            this->btnPanel->BackColor = System::Drawing::Color::Transparent;
            this->btnPanel->ColumnCount = 3;
            this->btnPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33F)));
            this->btnPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33F)));
            this->btnPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33F)));
            this->btnPanel->Controls->Add(this->btnANB_GraphingSim, 0, 0);
            this->btnPanel->Controls->Add(this->btnAPM_ElectromagSim, 1, 0);
            this->btnPanel->Controls->Add(this->btnUH_MagneticSim, 2, 0);
            this->btnPanel->Controls->Add(this->blDescriptionANB, 0, 1);
            this->btnPanel->Controls->Add(this->blDescriptionAPM, 1, 1);
            this->btnPanel->Controls->Add(this->blDescriptionUH, 2, 1);
            this->btnPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnPanel->Location = System::Drawing::Point(0, 80);
            this->btnPanel->Name = L"btnPanel";
            this->btnPanel->Padding = System::Windows::Forms::Padding(40, 30, 40, 40);
            this->btnPanel->RowCount = 2;
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 65)));
            this->btnPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 35)));
            this->btnPanel->Size = System::Drawing::Size(1002, 492);
            this->btnPanel->TabIndex = 2;
            this->btnPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::btnPanel_Paint);

            this->btnANB_GraphingSim->BackColor = System::Drawing::Color::FromArgb(0, 140, 200);
            this->btnANB_GraphingSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnANB_GraphingSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnANB_GraphingSim->FlatAppearance->BorderSize = 0;
            this->btnANB_GraphingSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnANB_GraphingSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnANB_GraphingSim->ForeColor = System::Drawing::Color::White;
            this->btnANB_GraphingSim->Margin = System::Windows::Forms::Padding(10);
            this->btnANB_GraphingSim->MinimumSize = System::Drawing::Size(0, 200);
            this->btnANB_GraphingSim->Name = L"btnANB_GraphingSim";
            this->btnANB_GraphingSim->TabIndex = 3;
            this->btnANB_GraphingSim->Text = L"Graphing\nSimulator";
            this->btnANB_GraphingSim->UseVisualStyleBackColor = false;
            this->btnANB_GraphingSim->Click += gcnew System::EventHandler(this, &Form1::btnANB_GraphingSim_Click);

            this->btnAPM_ElectromagSim->BackColor = System::Drawing::Color::FromArgb(160, 60, 200);
            this->btnAPM_ElectromagSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnAPM_ElectromagSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnAPM_ElectromagSim->FlatAppearance->BorderSize = 0;
            this->btnAPM_ElectromagSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnAPM_ElectromagSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnAPM_ElectromagSim->ForeColor = System::Drawing::Color::White;
            this->btnAPM_ElectromagSim->Margin = System::Windows::Forms::Padding(10);
            this->btnAPM_ElectromagSim->MinimumSize = System::Drawing::Size(0, 200);
            this->btnAPM_ElectromagSim->Name = L"btnAPM_ElectromagSim";
            this->btnAPM_ElectromagSim->TabIndex = 4;
            this->btnAPM_ElectromagSim->Text = L"Electromagnetism\nSimulator";
            this->btnAPM_ElectromagSim->UseVisualStyleBackColor = false;
            this->btnAPM_ElectromagSim->Click += gcnew System::EventHandler(this, &Form1::btnAPM_ElectromagSim_Click);

            this->btnUH_MagneticSim->BackColor = System::Drawing::Color::FromArgb(0, 180, 120);
            this->btnUH_MagneticSim->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnUH_MagneticSim->Dock = System::Windows::Forms::DockStyle::Fill;
            this->btnUH_MagneticSim->FlatAppearance->BorderSize = 0;
            this->btnUH_MagneticSim->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnUH_MagneticSim->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
            this->btnUH_MagneticSim->ForeColor = System::Drawing::Color::White;
            this->btnUH_MagneticSim->Margin = System::Windows::Forms::Padding(10);
            this->btnUH_MagneticSim->MinimumSize = System::Drawing::Size(0, 200);
            this->btnUH_MagneticSim->Name = L"btnUH_MagneticSim";
            this->btnUH_MagneticSim->TabIndex = 5;
            this->btnUH_MagneticSim->Text = L"Magnetic\nSimulator";
            this->btnUH_MagneticSim->UseVisualStyleBackColor = false;
            this->btnUH_MagneticSim->Click += gcnew System::EventHandler(this, &Form1::btnUH_MagneticSim_Click);

            this->blDescriptionANB->BackColor = System::Drawing::Color::FromArgb(0, 30, 50);
            this->blDescriptionANB->Dock = System::Windows::Forms::DockStyle::Fill;
            this->blDescriptionANB->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->blDescriptionANB->ForeColor = System::Drawing::Color::FromArgb(0, 140, 200);
            this->blDescriptionANB->Margin = System::Windows::Forms::Padding(10, 8, 10, 10);
            this->blDescriptionANB->Name = L"blDescriptionANB";
            this->blDescriptionANB->TabIndex = 6;
            this->blDescriptionANB->Text = L"Enter your description\nof the graphing simulator here.";
            this->blDescriptionANB->TextAlign = System::Drawing::ContentAlignment::TopCenter;

            this->blDescriptionAPM->BackColor = System::Drawing::Color::FromArgb(30, 10, 50);
            this->blDescriptionAPM->Dock = System::Windows::Forms::DockStyle::Fill;
            this->blDescriptionAPM->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->blDescriptionAPM->ForeColor = System::Drawing::Color::FromArgb(160, 60, 200);
            this->blDescriptionAPM->Margin = System::Windows::Forms::Padding(10, 8, 10, 10);
            this->blDescriptionAPM->Name = L"blDescriptionAPM";
            this->blDescriptionAPM->TabIndex = 7;
            this->blDescriptionAPM->Text = L"Enter your description\nof the electromagnetism simulator here.";
            this->blDescriptionAPM->TextAlign = System::Drawing::ContentAlignment::TopCenter;

            this->blDescriptionUH->BackColor = System::Drawing::Color::FromArgb(0, 30, 20);
            this->blDescriptionUH->Dock = System::Windows::Forms::DockStyle::Fill;
            this->blDescriptionUH->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
            this->blDescriptionUH->ForeColor = System::Drawing::Color::FromArgb(0, 180, 120);
            this->blDescriptionUH->Margin = System::Windows::Forms::Padding(10, 8, 10, 10);
            this->blDescriptionUH->Name = L"blDescriptionUH";
            this->blDescriptionUH->TabIndex = 8;
            this->blDescriptionUH->Text = L"Enter your description\nof the magnetic simulator here.";
            this->blDescriptionUH->TextAlign = System::Drawing::ContentAlignment::TopCenter;

            this->HelpPagePanel->Controls->Add(this->ExitHelpPage);
            this->HelpPagePanel->Controls->Add(this->ContactTxt);
            this->HelpPagePanel->Controls->Add(this->ContactTitle);
            this->HelpPagePanel->Controls->Add(this->FreqQuestText);
            this->HelpPagePanel->Controls->Add(this->FreqQuestTitle);
            this->HelpPagePanel->Controls->Add(this->HelpPageTitle);
            this->HelpPagePanel->BackColor = System::Drawing::Color::FromArgb(28, 28, 45);
            this->HelpPagePanel->Dock = System::Windows::Forms::DockStyle::Right;
            this->HelpPagePanel->Width = 312;
            this->HelpPagePanel->Name = L"HelpPagePanel";
            this->HelpPagePanel->TabIndex = 3;
            this->HelpPagePanel->Visible = false;

            this->ExitHelpPage->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Right);
            this->ExitHelpPage->BackColor = System::Drawing::Color::FromArgb(255, 80, 80);
            this->ExitHelpPage->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->ExitHelpPage->FlatAppearance->BorderSize = 0;
            this->ExitHelpPage->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7.8F, System::Drawing::FontStyle::Bold));
            this->ExitHelpPage->ForeColor = System::Drawing::Color::White;
            this->ExitHelpPage->Location = System::Drawing::Point(268, 12);
            this->ExitHelpPage->Name = L"ExitHelpPage";
            this->ExitHelpPage->Size = System::Drawing::Size(33, 23);
            this->ExitHelpPage->TabIndex = 5;
            this->ExitHelpPage->Text = L"X";
            this->ExitHelpPage->UseVisualStyleBackColor = false;
            this->ExitHelpPage->Click += gcnew System::EventHandler(this, &Form1::ExitHelpPage_Click);

            this->HelpPageTitle->AutoSize = true;
            this->HelpPageTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13.8F, System::Drawing::FontStyle::Bold));
            this->HelpPageTitle->ForeColor = System::Drawing::Color::White;
            this->HelpPageTitle->Location = System::Drawing::Point(20, 28);
            this->HelpPageTitle->Name = L"HelpPageTitle";
            this->HelpPageTitle->TabIndex = 0;
            this->HelpPageTitle->Text = L"Help Page";

            this->FreqQuestTitle->AutoSize = true;
            this->FreqQuestTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
            this->FreqQuestTitle->ForeColor = System::Drawing::Color::White;
            this->FreqQuestTitle->Location = System::Drawing::Point(20, 82);
            this->FreqQuestTitle->Name = L"FreqQuestTitle";
            this->FreqQuestTitle->TabIndex = 1;
            this->FreqQuestTitle->Text = L"Frequently Asked Questions";

            this->FreqQuestText->AutoSize = true;
            this->FreqQuestText->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7.8F));
            this->FreqQuestText->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
            this->FreqQuestText->Location = System::Drawing::Point(20, 120);
            this->FreqQuestText->Name = L"FreqQuestText";
            this->FreqQuestText->TabIndex = 2;
            this->FreqQuestText->Text =
                L"What programming language was this app made in?\n"
                L"Ans: C++ with CLR/CLI\n\n"
                L"For the electromagnetism simulator:\n"
                L"what equations is it based on?\n"
                L"Ans: Maxwell's equations and Coulomb's Laws\n\n";

            this->ContactTitle->AutoSize = true;
            this->ContactTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13.8F, System::Drawing::FontStyle::Bold));
            this->ContactTitle->ForeColor = System::Drawing::Color::White;
            this->ContactTitle->Location = System::Drawing::Point(20, 289);
            this->ContactTitle->Name = L"ContactTitle";
            this->ContactTitle->TabIndex = 3;
            this->ContactTitle->Text = L"Contact";

            this->ContactTxt->AutoSize = true;
            this->ContactTxt->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7.8F));
            this->ContactTxt->ForeColor = System::Drawing::Color::FromArgb(200, 200, 200);
            this->ContactTxt->Location = System::Drawing::Point(20, 330);
            this->ContactTxt->Name = L"ContactTxt";
            this->ContactTxt->TabIndex = 4;
            this->ContactTxt->Text =
                L"For any issues or questions\n"
                L"please contact your group members.\n\n"
                L"Add your contact details here.\n\n"
                L"Alex Naylor: Graphing Simulator\nA.Naylor-Beggs1-25@student.lboro.ac.uk\n\n"
                L"Andres Perez: Electromagnetism Simulator\na.perez-maillard1-24@student.lboro.ac.uk\n\n"
                L"Uzair Hafesji: Magnetism Simulator \nU.Hafesji1-25@student.lboro.ac.uk";

            this->EnterHelpPage->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
                System::Windows::Forms::AnchorStyles::Top |
                System::Windows::Forms::AnchorStyles::Right);
            this->EnterHelpPage->BackColor = System::Drawing::Color::DarkGray;
            this->EnterHelpPage->Font = (gcnew System::Drawing::Font(L"Segoe UI", 7.8F, System::Drawing::FontStyle::Bold));
            this->EnterHelpPage->Location = System::Drawing::Point(915, 13);
            this->EnterHelpPage->Name = L"EnterHelpPage";
            this->EnterHelpPage->Size = System::Drawing::Size(75, 23);
            this->EnterHelpPage->TabIndex = 4;
            this->EnterHelpPage->TabStop = false;
            this->EnterHelpPage->Text = L"Help";
            this->EnterHelpPage->UseVisualStyleBackColor = false;
            this->EnterHelpPage->Click += gcnew System::EventHandler(this, &Form1::EnterHelpPage_Click);

            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::FromArgb(18, 18, 30);
            this->ClientSize = System::Drawing::Size(1002, 572);
            this->Controls->Add(this->EnterHelpPage);
            this->Controls->Add(this->HelpPagePanel);
            this->Controls->Add(this->btnPanel);
            this->Controls->Add(this->lblTitle);
            this->MinimumSize = System::Drawing::Size(700, 400);
            this->Name = L"Form1";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
            this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
            this->btnPanel->ResumeLayout(false);
            this->HelpPagePanel->ResumeLayout(false);
            this->HelpPagePanel->PerformLayout();
            this->ResumeLayout(false);
        }
#pragma endregion

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
        System::Void ExitHelpPage_Click(System::Object^ sender, System::EventArgs^ e) {
            HelpPagePanel->Visible = false;
        }
        System::Void EnterHelpPage_Click(System::Object^ sender, System::EventArgs^ e) {
            HelpPagePanel->Visible = true;
        }
    };
}