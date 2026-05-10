#pragma once

using namespace System;
using namespace System::Windows::Forms;


bool TryGetInputs(TextBox^ tbR, TextBox^ tbL, TextBox^ tbC,
	double% R, double% L, double% C) {
	if (!Double::TryParse(tbR->Text, R) ||
		!Double::TryParse(tbL->Text, L) ||
		!Double::TryParse(tbC->Text, C)) {
		MessageBox::Show("Please enter valid numbers for R, L, and C.");
		return false;
	}

	if (R <= 0 || L <= 0 || C <= 0) {
		MessageBox::Show("R, L, and C must all be positive.");
		return false;
	}

	return true;
}