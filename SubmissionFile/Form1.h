//#include "Form1ANB.h"
#include "FormAPMFinal.h"


#pragma once

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	public:
		Form1(void) { InitializeComponent(); }

	protected:

		~Form1() { if (components) { delete components; } }

	private:
		System::ComponentModel::Container^ components;
		System::Windows::Forms::Button^ btnANBGraphingSim;



#pragma region Windows Form Designer generated code
		/// <summary>
		System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {}
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnANBGraphingSim = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btnANBGraphingSim
			// 
			this->btnANBGraphingSim->Location = System::Drawing::Point(12, 12);
			this->btnANBGraphingSim->Name = L"btnANBGraphingSim";
			this->btnANBGraphingSim->Size = System::Drawing::Size(248, 180);
			this->btnANBGraphingSim->TabIndex = 0;
			this->btnANBGraphingSim->Text = L"btnANBGraphingSim";
			this->btnANBGraphingSim->UseVisualStyleBackColor = true;
			this->btnANBGraphingSim->Click += gcnew System::EventHandler(this, &Form1::btnANBGraphingSim_Click);

			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(596, 503);
			this->Controls->Add(this->btnANBGraphingSim);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->TabIndex = 1;
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion


		System::Void btnANBGraphingSim_Click(System::Object^ sender, System::EventArgs^ e) {
			FormAPMFinal^ f = gcnew FormAPMFinal();
			f->Show();
		}
	};
}
