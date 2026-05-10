#include "pch.h"

using namespace System;

#include "Form1ANB.h"

using namespace System::Windows::Forms;

[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinFormsProject::Form1ANB());
	return 0;
}