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
		System::Windows::Forms::Label^ lblSubtitle;
		System::Windows::Forms::Button^ btnRLC;
		System::Windows::Forms::Button^ btnRC;
		System::Windows::Forms::Button^ btnEM;
		System::Windows::Forms::Button^ btnFourier;
		System::Windows::Forms::Panel^ pnlStripe;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->btnRLC = (gcnew System::Windows::Forms::Button());
			this->btnRC = (gcnew System::Windows::Forms::Button());
			this->btnEM = (gcnew System::Windows::Forms::Button());
			this->btnFourier = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// lblTitle
			// 
			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 28, System::Drawing::FontStyle::Bold));
			this->lblTitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(200)),
				static_cast<System::Int32>(static_cast<System::Byte>(180)));
			this->lblTitle->Location = System::Drawing::Point(30, 50);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(614, 62);
			this->lblTitle->TabIndex = 1;
			this->lblTitle->Text = L"Electromagnetic simulator.";
			// 
			// btnRLC
			// 
			this->btnRLC->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(140)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnRLC->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnRLC->FlatAppearance->BorderSize = 0;
			this->btnRLC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnRLC->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
			this->btnRLC->ForeColor = System::Drawing::Color::White;
			this->btnRLC->Location = System::Drawing::Point(180, 160);
			this->btnRLC->Name = L"btnRLC";
			this->btnRLC->Padding = System::Windows::Forms::Padding(16, 0, 0, 0);
			this->btnRLC->Size = System::Drawing::Size(460, 75);
			this->btnRLC->TabIndex = 3;
			this->btnRLC->Text = L"RLC Circuit";
			this->btnRLC->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnRLC->UseVisualStyleBackColor = false;
			this->btnRLC->Click += gcnew System::EventHandler(this, &Form1ANB::btnRLC_Click);
			// 
			// btnRC
			// 
			this->btnRC->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(180)),
				static_cast<System::Int32>(static_cast<System::Byte>(120)));
			this->btnRC->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnRC->FlatAppearance->BorderSize = 0;
			this->btnRC->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnRC->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
			this->btnRC->ForeColor = System::Drawing::Color::White;
			this->btnRC->Location = System::Drawing::Point(180, 250);
			this->btnRC->Name = L"btnRC";
			this->btnRC->Padding = System::Windows::Forms::Padding(16, 0, 0, 0);
			this->btnRC->Size = System::Drawing::Size(460, 75);
			this->btnRC->TabIndex = 4;
			this->btnRC->Text = L"RC Charging";
			this->btnRC->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnRC->UseVisualStyleBackColor = false;
			this->btnRC->Click += gcnew System::EventHandler(this, &Form1ANB::btnRC_Click);
			// 
			// btnEM
			// 
			this->btnEM->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(160)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnEM->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEM->FlatAppearance->BorderSize = 0;
			this->btnEM->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEM->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
			this->btnEM->ForeColor = System::Drawing::Color::White;
			this->btnEM->Location = System::Drawing::Point(180, 340);
			this->btnEM->Name = L"btnEM";
			this->btnEM->Padding = System::Windows::Forms::Padding(16, 0, 0, 0);
			this->btnEM->Size = System::Drawing::Size(460, 75);
			this->btnEM->TabIndex = 5;
			this->btnEM->Text = L"EM Wave";
			this->btnEM->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnEM->UseVisualStyleBackColor = false;
			this->btnEM->Click += gcnew System::EventHandler(this, &Form1ANB::btnEM_Click);
			// 
			// btnFourier
			// 
			this->btnFourier->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(200)), static_cast<System::Int32>(static_cast<System::Byte>(160)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->btnFourier->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnFourier->FlatAppearance->BorderSize = 0;
			this->btnFourier->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnFourier->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
			this->btnFourier->ForeColor = System::Drawing::Color::Black;
			this->btnFourier->Location = System::Drawing::Point(180, 430);
			this->btnFourier->Name = L"btnFourier";
			this->btnFourier->Padding = System::Windows::Forms::Padding(16, 0, 0, 0);
			this->btnFourier->Size = System::Drawing::Size(460, 75);
			this->btnFourier->TabIndex = 6;
			this->btnFourier->Text = L"Fourier Series";
			this->btnFourier->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnFourier->UseVisualStyleBackColor = false;
			this->btnFourier->Click += gcnew System::EventHandler(this, &Form1ANB::btnFourier_Click);
			// 
			// Form1ANB
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(18)), static_cast<System::Int32>(static_cast<System::Byte>(18)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->ClientSize = System::Drawing::Size(820, 620);
			this->Controls->Add(this->lblTitle);
			this->Controls->Add(this->btnRLC);
			this->Controls->Add(this->btnRC);
			this->Controls->Add(this->btnEM);
			this->Controls->Add(this->btnFourier);
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"Form1ANB";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"EM Simulator Suite";
			this->ResumeLayout(false);
			this->PerformLayout();

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


//INCLUDE  the folloiwing to launch mine in project:

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

