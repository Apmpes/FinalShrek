#pragma once
#include "EFieldForce.h"
#include "ElectricClasses.h"
#include "ArrowThings.h"
#include <random>
#include "EFieldGrid.h"
#include "ElineField.h"
#include "inspectCalculator1.h"

using namespace std;
namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;

	/// <summary>
	/// Summary for FormAPMFinal
	/// </summary>
	public ref class FormAPMFinal : public System::Windows::Forms::Form
	{
	public:
		FormAPMFinal(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//panel
			this->TopMost = false;
			wallGamePanel1->Visible = false;
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized; //opens full screen
			ElectricSandboxPanel->Resize += gcnew System::EventHandler(this, &FormAPMFinal::PositionButtons); //so home screen buttons are centered
			PositionButtons(nullptr, nullptr);
			System::Reflection::PropertyInfo^ p1 =
				wallGamePanel1->GetType()->GetProperty("DoubleBuffered", //otherwise it was flickering, double buffering apparently is not acessible easily, had to do this
					System::Reflection::BindingFlags::Instance |
					System::Reflection::BindingFlags::NonPublic);
			p1->SetValue(wallGamePanel1, true, nullptr);

			System::Reflection::PropertyInfo^ p2 =
				ElectricSandboxPanel->GetType()->GetProperty("DoubleBuffered", //Same for sandbox
					System::Reflection::BindingFlags::Instance |
					System::Reflection::BindingFlags::NonPublic);
			p2->SetValue(ElectricSandboxPanel, true, nullptr);

			screenWidth = ElectricSandboxPanel->Width;
			screenHeight = ElectricSandboxPanel->Height;

			gameWidth = wallGamePanel1->Width; //set width and height of game panel
			gameHeight = wallGamePanel1->Height;


			wallGamePanel1->MouseClick += gcnew MouseEventHandler(this, &FormAPMFinal::MClick);
			wallGamePanel1->MouseDown += gcnew MouseEventHandler(this, &FormAPMFinal::Mdown);
			wallGamePanel1->MouseUp += gcnew MouseEventHandler(this, &FormAPMFinal::Mup);
			wallGamePanel1->MouseMove += gcnew MouseEventHandler(this, &FormAPMFinal::OnMouseMove);

			ElectricSandboxPanel->MouseClick += gcnew MouseEventHandler(this, &FormAPMFinal::MClick);
			ElectricSandboxPanel->MouseDown += gcnew MouseEventHandler(this, &FormAPMFinal::Mdown);
			ElectricSandboxPanel->MouseUp += gcnew MouseEventHandler(this, &FormAPMFinal::Mup);
			ElectricSandboxPanel->MouseMove += gcnew MouseEventHandler(this, &FormAPMFinal::OnMouseMove);

			//timer and rendering
			this->DoubleBuffered = true; //better rendering, otherwise it looks trash
			timer = gcnew Timer(); // simulation
			timer->Interval = 15;
			timer->Tick += gcnew EventHandler(this, &FormAPMFinal::Tick);
			timer->Stop();

			//Charges
			Q = new vector<Charge>();
			//Test Charge
			testqField = new Vec2D(0, 0);
			startX = screenWidth / 10; //the -100 + testD/2 + 10 is to align iwth the center of the station
			startY = screenHeight / 2;
			x = startX;
			y = startY;

			sandStartX = screenWidth - 150 + testD / 2 + 5; //the -100 + testD/2 + 10 is to align iwth the center of the station
			sandStartY = screenHeight * 0.45 + testD / 2 + 5;
			xs = sandStartX;
			ys = sandStartY;
			testCharge = new Charge(initialQ, 1, testD, Vec2D(x, y), Vec2D(vx, vy), -2);
			sandboxTestCharge = new Charge(initialQ, 1, testD, Vec2D(xs, ys), Vec2D(vx, vy), -2);

			//Max number of Q and mag
			maxQN = 30;
			maxMag = 10;
			//Walls
			wallSegHeight = 100;
			wallWidth = 30;
			wallN = 4;
			Walls = new walls(wallWidth, wallSegHeight, wallN);

			//Efield
			fieldColl = (screenWidth / (EFMaxSize + 20)); //the + 20 is so that you do have arrows at the edge of screen
			fieldRow = (screenHeight / (EFMaxSize + 20));
			EField = new EFieldGrid(fieldRow, fieldColl);

			//E line field
			eLine = new vector<Eline>();
			qCenters = new vector<Vec2D>(Q->size());
			EfatQ = new vector<Vec2D>(Q->size());
			lineFieldRow = screenHeight / (maxLength - maxLength / 5);
			lineFieldColl = screenWidth / (maxLength - maxLength / 15);
			lineHspacing = screenWidth / lineFieldColl;
			lineVspacing = screenHeight / lineFieldRow;
			//color dialog
			LinesColorDialog->FullOpen = true; // shows full spectrum immediately
			LinesColorDialog->Color = selectedBaseColor;


			//Induced mag field
			mag = new vector<inducMag>();

			//So that the works in sandbox pannels work, they werent for some reason
			this->SandboxAddChargeB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxAddChargeB_Click);
			this->SandboxDeleteChargesB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxDeleteChargesB_Click);
			this->SandboxResetChargesB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxResetChargesB_Click);
			this->SandboxReversePolB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxReversePolB_Click);
			this->SandboxNoneB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxNoneB_Click);
			this->SandboxSimulateCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::SandboxSimulateCB_CheckedChanged);
			this->SandboxInspectCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::SandboxInspectCB_CheckedChanged);
			this->SandboxVecFieldCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::SandboxVecFieldCB_CheckedChanged);
			this->SandboxDragCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::SandboxDragCB_CheckedChanged);

			//cover checkbox inmage panel things
			Cover = gcnew System::Windows::Forms::Panel();
			Cover->BackColor = System::Drawing::Color::FromArgb(0, 0, 30);
			Cover->Size = System::Drawing::Size(150, 50);
			Cover->Location = System::Drawing::Point(screenWidth -150, 1);
			Cover->Anchor =static_cast<System::Windows::Forms::AnchorStyles>(System::Windows::Forms::AnchorStyles::Top |System::Windows::Forms::AnchorStyles::Right);
			homePanel->Controls->Add(Cover);

			Cover->BringToFront(); //just to cover sa part of the image
			
		}
	private: System::Windows::Forms::Timer^ timer;


		   //Charges
		   double vx = 0;
		   double vy = 0;
		   vector<Charge>* Q;
		   int initialQ = 5;
		   float Qd = 40.0;
		   int sandQd = 80;
		   int maxQN = 30;
		   int Qleft;
		   int QUsed;
		   int easyQused; //so that you can change between difficulties and the charges you have used in that diffculty are stored.
		   int medQused;
		   int hardQused;
		   int prevQUsed = 0;
		   int easyPrevQused = 0;
		   int medPrevQused = 0;
		   int hardPrevQused = 0;
		   int maxQleft;
		   bool drawQandM = true;
		   int editIndex;
		   bool edit;
		   //game mag field
		   int maxMag = 10;
		   int Magleft;
		   int MagUsed;
		   int easyMagUsed;
		   int medMagUsed;
		   int hardMagUsed;
		   int prevMagUsed = 0;
		   int easyPrevMagUsed = 0;
		   int medPrevMagUsed = 0;
		   int hardPrevMagUsed = 0;

		   //Test Charges
		   Charge* testCharge;
		   Charge* sandboxTestCharge;
		   float testD = 20.0;
		   double startX;
		   double startY;
		   double x;
		   double y;
		   double sandStartX;
		   double sandStartY;
		   double xs;
		   double ys;
		   bool dragTestQ = false;
		   bool activeTestQ = false;
		   //Force (adding it here to be able to update the force in paint (for arrow)
		   Vec2D* testqField;
		   bool sandboxDrawTestQBox = true;
		   bool editingTestQ = false;

		   //Screen
		   int screenHeight;
		   int screenWidth;
		   int gameWidth;
		   int gameHeight;

		   //test charge arrows scales for visual purposes
		   double qVarMaxSize = 200;
		   double qVarScale = 0.6;
		   double qFarMaxSize = 200;
		   double qFarScale = 1.2e0 * 2.2247;
		   double gameqFarScale = 1e7;
		   double qfMinSize = 1;
		   //modes and mouse
		   enum class Mode {
			   none,
			   addCharge,
			   deleteCharges,
			   drag,
			   reversePolarity,
			   induceMag,
		   };
		   Mode mode;
		   Mode prevMode; //for when releasing drag you can still what you were doing
		   bool drag = true;
		   bool dragQ = false;
		   bool dragMag = false;
		   bool sandDrag = true;
		   double mouseX;
		   double mouseY;
		   int dragIndex;



		   //simulation checkbox
		   bool play = false;
		   bool simulateAllQ = false;
		   //level
		   int level = 1;
		   int easyLevel = 1;
		   int medLevel = 1;
		   int hardLevel = 1;
		   int easyHighScore = 1;
		   int hardHighScore = 1;
		   int mediumHighScore = 1;

		   //Wall coords
		   vector<double>* wx;
		   vector<vector<double>>* wy;
		   //Difficulty
		   enum class Diff {
			   easy,
			   medium,
			   hard,
		   };
		   Diff diff;

		   //Wall related variables
		   walls* Walls;
		   int wallN;
		   double wallSegHeight;
		   int wallWidth;
		   bool doWalls = false;
		   bool collision = false;

		   //Arrow EField
		   double EFMaxSize = 35;
		   double minSize = 5;
		   double EFScale = 1.2e0 * 2.2247;
		   int fieldColl;
		   int fieldRow;
		   bool doEField = false;
		   bool sandDoEField = false;
		   EFieldGrid* EField;

		   //E field lines variables
		   int tickCount = 50;
		   int maxTick = 100;
		   bool doEfieldLines = false;
		   bool drawEfieldLines = false;
		   int maxLife = 100;
		   float maxLength = 100;
		   float minLength = 5;
		   int baseSpeed = 2;
		   vector<Eline>* eLine;
		   vector<Vec2D>* qCenters;
		   vector<Vec2D>* EfatQ;
		   int genCycle = 1;
		   int fieldLgenerated = 3; //per generation cycle
		   int lineFieldColl;
		   int lineFieldRow;
		   int lineHspacing;
		   int lineVspacing;
		   //Color Dialog
		   Color selectedBaseColor = Color::FromArgb(201, 50, 146); // default blue

		   enum class gen {
			   RandomField,
			   GridGen,
			   RandomCharge,
			   RadialGen,
		   };
		   gen generation;
		   enum class col {
			   standardFromE,
			   colorChoice,
		   };
		   col colorSel = col::standardFromE;

		   // induced magnetic field
		   vector<inducMag>* mag;
		   float magDia = 80.0;
		   double dbdt = 10;
		   int in = -1;
		   int out = 1;

		   //inspect
		   bool inspect = false;

		   //Simulate in Sandbox
		   enum class sandboxSim {
			   none,
			   simulateTestQ,
			   simulateQ,
			   simulateALL,
		   };
		   sandboxSim sim = sandboxSim::none;


	private: System::Windows::Forms::CheckBox^ checkBox1;

	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::CheckBox^ checkBox2;
	private: System::Windows::Forms::CheckBox^ checkBox3;
	private: System::Windows::Forms::CheckBox^ checkBox4;
	private: System::Windows::Forms::Label^ chargesLeftLabel;
	private: System::Windows::Forms::Button^ ResetCharges;
	private: System::Windows::Forms::Button^ TempGoal;
	private: System::Windows::Forms::CheckBox^ EVectorField;
	private: System::Windows::Forms::Button^ ReversePolarity;
	private: System::Windows::Forms::Button^ ResetLevel;
	private: System::Windows::Forms::Panel^ wallGamePanel1;
	private: System::Windows::Forms::Button^ playWallGame;
	private: System::Windows::Forms::Panel^ homePanel;
	private: System::Windows::Forms::Button^ ExitWallGame;
	private: System::Windows::Forms::Label^ Level;
	private: System::Windows::Forms::CheckBox^ dragCheckBox;
	private: System::Windows::Forms::Button^ noneButton;
	private: System::Windows::Forms::Button^ ElectricSandbox;
	private: System::Windows::Forms::Panel^ ElectricSandboxPanel;
	private: System::Windows::Forms::CheckBox^ SandboxVecFieldCB;
	private: System::Windows::Forms::CheckBox^ SandboxSimulateCB;
	private: System::Windows::Forms::CheckBox^ SandboxInspectCB;
	private: System::Windows::Forms::CheckBox^ SandboxDragCB;
	private: System::Windows::Forms::Button^ SandboxNoneB;
	private: System::Windows::Forms::Button^ SandboxReversePolB;
	private: System::Windows::Forms::Button^ SandboxResetChargesB;
	private: System::Windows::Forms::Button^ SandboxDeleteChargesB;
	private: System::Windows::Forms::Button^ SandboxAddChargeB;
	private: System::Windows::Forms::Button^ ExitESandboxB;
	private: System::Windows::Forms::CheckBox^ EFieldLinesCB;
	private: System::Windows::Forms::Button^ SandboxInduceMagB;
	private: System::Windows::Forms::Label^ DebugLable;
	private: System::Windows::Forms::ComboBox^ EFieldLinesGen;
	private: System::Windows::Forms::CheckBox^ DrawQM;
	private: System::Windows::Forms::CheckBox^ TestChargeActivate;
	private: System::Windows::Forms::Label^ InspectLable;
	private: System::Windows::Forms::Label^ ActivateTesQWarning;
	private: System::Windows::Forms::ComboBox^ SimulateComboBox;
	private: System::Windows::Forms::Label^ HighScore;
	private: System::Windows::Forms::Button^ GameInducMagB;
	private: System::Windows::Forms::Label^ MagnetsLeftLable;
	private: System::Windows::Forms::ComboBox^ VecFieldColor;
	private: System::Windows::Forms::Button^ ELinesColorSelBn;
	private: System::Windows::Forms::ColorDialog^ LinesColorDialog;
	private: System::Windows::Forms::Panel^ panelColorPreview;
	private: System::Windows::Forms::Label^ EColorLabel;
	private: System::Windows::Forms::CheckBox^ HideAndShowCB;
	private: System::Windows::Forms::Button^ ResetDifficultyB;
	private: System::Windows::Forms::Panel^ editChargePanel;
	private: System::Windows::Forms::Label^ MassLable;

	private: System::Windows::Forms::Label^ ChargeLable;
	private: System::Windows::Forms::NumericUpDown^ ChargeNumericUpDown;
	private: System::Windows::Forms::Label^ ChargeNumberLable;
	private: System::Windows::Forms::NumericUpDown^ MassNumericUpDown;

	private: System::Windows::Forms::CheckBox^ EditChargesCB;
	private: System::Windows::Forms::Button^ ExitEditChargePanelB;
	private: System::Windows::Forms::Label^ YPosLable;
	private: System::Windows::Forms::Label^ XposLable;
	private: System::Windows::Forms::TextBox^ YPosTextBox;

	private: System::Windows::Forms::TextBox^ XPosTextBox;
	private: System::Windows::Forms::Button^ ApplyPosB;
	private: System::Windows::Forms::Label^ SimulateWarningLable;
	private: System::Windows::Forms::Panel^ EditTestChargePanel;
	private: System::Windows::Forms::Label^ TestQYPosLable;
	private: System::Windows::Forms::Label^ TestQXposLable;
	private: System::Windows::Forms::Label^ TestQChargeLable;
	private: System::Windows::Forms::Label^ TestQMassLable;
	private: System::Windows::Forms::Label^ TestChargeEditLable;
	private: System::Windows::Forms::Button^ ApplyPosTestQB;

	private: System::Windows::Forms::NumericUpDown^ TestQMassNumericUpDown;
	private: System::Windows::Forms::NumericUpDown^ TestQChargeNumericUpDown;
	private: System::Windows::Forms::TextBox^ TestQYPosTextbox;
	private: System::Windows::Forms::TextBox^ TestQXPosTextbox;
	private: System::Windows::Forms::Button^ TesQXB;
	 private: System::Windows::Forms::Panel^ WonGamePanel;
private: System::Windows::Forms::Label^ NextLvlExplainLable;
private: System::Windows::Forms::Label^ WellDoneLable;
private: System::Windows::Forms::Button^ NextLevelB;
private: System::Windows::Forms::Button^ RewatchB;
	   private: System::Windows::Forms::Label^ ElectricSimTitle;
		   System::Windows::Forms::Panel^ Cover;


	private: System::Windows::Forms::Button^ button1;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

		~FormAPMFinal()
		{
			delete Q;
			delete testCharge;
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;

			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->chargesLeftLabel = (gcnew System::Windows::Forms::Label());
			this->ResetCharges = (gcnew System::Windows::Forms::Button());
			this->TempGoal = (gcnew System::Windows::Forms::Button());
			this->EVectorField = (gcnew System::Windows::Forms::CheckBox());
			this->ReversePolarity = (gcnew System::Windows::Forms::Button());
			this->ResetLevel = (gcnew System::Windows::Forms::Button());
			this->wallGamePanel1 = (gcnew System::Windows::Forms::Panel());
			this->WonGamePanel = (gcnew System::Windows::Forms::Panel());
			this->NextLvlExplainLable = (gcnew System::Windows::Forms::Label());
			this->WellDoneLable = (gcnew System::Windows::Forms::Label());
			this->NextLevelB = (gcnew System::Windows::Forms::Button());
			this->RewatchB = (gcnew System::Windows::Forms::Button());
			this->ResetDifficultyB = (gcnew System::Windows::Forms::Button());
			this->MagnetsLeftLable = (gcnew System::Windows::Forms::Label());
			this->GameInducMagB = (gcnew System::Windows::Forms::Button());
			this->HighScore = (gcnew System::Windows::Forms::Label());
			this->noneButton = (gcnew System::Windows::Forms::Button());
			this->dragCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->Level = (gcnew System::Windows::Forms::Label());
			this->ExitWallGame = (gcnew System::Windows::Forms::Button());
			this->playWallGame = (gcnew System::Windows::Forms::Button());
			this->homePanel = (gcnew System::Windows::Forms::Panel());
			this->ElectricSandbox = (gcnew System::Windows::Forms::Button());
			this->ElectricSandboxPanel = (gcnew System::Windows::Forms::Panel());
			this->ElectricSimTitle = (gcnew System::Windows::Forms::Label());
			this->EditTestChargePanel = (gcnew System::Windows::Forms::Panel());
			this->TesQXB = (gcnew System::Windows::Forms::Button());
			this->ApplyPosTestQB = (gcnew System::Windows::Forms::Button());
			this->TestQMassNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->TestQChargeNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->TestQYPosTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TestQXPosTextbox = (gcnew System::Windows::Forms::TextBox());
			this->TestQYPosLable = (gcnew System::Windows::Forms::Label());
			this->TestQXposLable = (gcnew System::Windows::Forms::Label());
			this->TestQChargeLable = (gcnew System::Windows::Forms::Label());
			this->TestQMassLable = (gcnew System::Windows::Forms::Label());
			this->TestChargeEditLable = (gcnew System::Windows::Forms::Label());
			this->SimulateWarningLable = (gcnew System::Windows::Forms::Label());
			this->EditChargesCB = (gcnew System::Windows::Forms::CheckBox());
			this->editChargePanel = (gcnew System::Windows::Forms::Panel());
			this->ApplyPosB = (gcnew System::Windows::Forms::Button());
			this->YPosLable = (gcnew System::Windows::Forms::Label());
			this->XposLable = (gcnew System::Windows::Forms::Label());
			this->YPosTextBox = (gcnew System::Windows::Forms::TextBox());
			this->XPosTextBox = (gcnew System::Windows::Forms::TextBox());
			this->ExitEditChargePanelB = (gcnew System::Windows::Forms::Button());
			this->MassNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->MassLable = (gcnew System::Windows::Forms::Label());
			this->ChargeLable = (gcnew System::Windows::Forms::Label());
			this->ChargeNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ChargeNumberLable = (gcnew System::Windows::Forms::Label());
			this->HideAndShowCB = (gcnew System::Windows::Forms::CheckBox());
			this->EColorLabel = (gcnew System::Windows::Forms::Label());
			this->panelColorPreview = (gcnew System::Windows::Forms::Panel());
			this->ELinesColorSelBn = (gcnew System::Windows::Forms::Button());
			this->VecFieldColor = (gcnew System::Windows::Forms::ComboBox());
			this->SimulateComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ActivateTesQWarning = (gcnew System::Windows::Forms::Label());
			this->InspectLable = (gcnew System::Windows::Forms::Label());
			this->TestChargeActivate = (gcnew System::Windows::Forms::CheckBox());
			this->DrawQM = (gcnew System::Windows::Forms::CheckBox());
			this->EFieldLinesGen = (gcnew System::Windows::Forms::ComboBox());
			this->DebugLable = (gcnew System::Windows::Forms::Label());
			this->SandboxInduceMagB = (gcnew System::Windows::Forms::Button());
			this->EFieldLinesCB = (gcnew System::Windows::Forms::CheckBox());
			this->ExitESandboxB = (gcnew System::Windows::Forms::Button());
			this->SandboxVecFieldCB = (gcnew System::Windows::Forms::CheckBox());
			this->SandboxSimulateCB = (gcnew System::Windows::Forms::CheckBox());
			this->SandboxInspectCB = (gcnew System::Windows::Forms::CheckBox());
			this->SandboxDragCB = (gcnew System::Windows::Forms::CheckBox());
			this->SandboxNoneB = (gcnew System::Windows::Forms::Button());
			this->SandboxReversePolB = (gcnew System::Windows::Forms::Button());
			this->SandboxResetChargesB = (gcnew System::Windows::Forms::Button());
			this->SandboxDeleteChargesB = (gcnew System::Windows::Forms::Button());
			this->SandboxAddChargeB = (gcnew System::Windows::Forms::Button());
			this->LinesColorDialog = (gcnew System::Windows::Forms::ColorDialog());
			this->wallGamePanel1->SuspendLayout();
			this->homePanel->SuspendLayout();
			this->ElectricSandboxPanel->SuspendLayout();
			this->EditTestChargePanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TestQMassNumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TestQChargeNumericUpDown))->BeginInit();
			this->editChargePanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MassNumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ChargeNumericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// checkBox1
			// 
			this->checkBox1->BackColor = System::Drawing::Color::DarkGreen;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox1->ForeColor = System::Drawing::Color::LightGray;
			this->checkBox1->Location = System::Drawing::Point(20, 88);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(60, 30);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"Play";
			this->checkBox1->UseVisualStyleBackColor = false;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::checkBox1_CheckedChanged);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::ForestGreen;
			this->button1->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(13, 841);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(113, 48);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Add Charge";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FormAPMFinal::button1_MouseClick);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::Maroon;
			this->button3->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(151, 935);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(109, 48);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Delete Charges";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &FormAPMFinal::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::DarkSlateGray;
			this->button4->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(139, 116);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(94, 34);
			this->button4->TabIndex = 4;
			this->button4->Text = L"Do walls";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &FormAPMFinal::button4_Click);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->BackColor = System::Drawing::Color::Indigo;
			this->checkBox2->Checked = true;
			this->checkBox2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox2->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox2->Location = System::Drawing::Point(31, 161);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(57, 19);
			this->checkBox2->TabIndex = 5;
			this->checkBox2->Text = L"Easy";
			this->checkBox2->UseVisualStyleBackColor = false;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::checkBox2_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->BackColor = System::Drawing::Color::Indigo;
			this->checkBox3->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox3->Location = System::Drawing::Point(31, 184);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(82, 19);
			this->checkBox3->TabIndex = 6;
			this->checkBox3->Text = L"Medium";
			this->checkBox3->UseVisualStyleBackColor = false;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::checkBox3_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->BackColor = System::Drawing::Color::Indigo;
			this->checkBox4->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->checkBox4->Location = System::Drawing::Point(31, 207);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(60, 19);
			this->checkBox4->TabIndex = 7;
			this->checkBox4->Text = L"Hard";
			this->checkBox4->UseVisualStyleBackColor = false;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::checkBox4_CheckedChanged);
			// 
			// chargesLeftLabel
			// 
			this->chargesLeftLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->chargesLeftLabel->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 10.5F, System::Drawing::FontStyle::Bold));
			this->chargesLeftLabel->Location = System::Drawing::Point(13, 805);
			this->chargesLeftLabel->Name = L"chargesLeftLabel";
			this->chargesLeftLabel->Size = System::Drawing::Size(138, 20);
			this->chargesLeftLabel->TabIndex = 8;
			this->chargesLeftLabel->Text = L"Charges Left";
			// 
			// ResetCharges
			// 
			this->ResetCharges->BackColor = System::Drawing::Color::Maroon;
			this->ResetCharges->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ResetCharges->Location = System::Drawing::Point(283, 935);
			this->ResetCharges->Name = L"ResetCharges";
			this->ResetCharges->Size = System::Drawing::Size(109, 48);
			this->ResetCharges->TabIndex = 9;
			this->ResetCharges->Text = L"Reset Charges";
			this->ResetCharges->UseVisualStyleBackColor = false;
			this->ResetCharges->Click += gcnew System::EventHandler(this, &FormAPMFinal::ResetCharges_Click);
			// 
			// TempGoal
			// 
			this->TempGoal->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->TempGoal->Location = System::Drawing::Point(1659, 569);
			this->TempGoal->Name = L"TempGoal";
			this->TempGoal->Size = System::Drawing::Size(94, 69);
			this->TempGoal->TabIndex = 10;
			this->TempGoal->Text = L"Temporary Goal";
			this->TempGoal->UseVisualStyleBackColor = false;
			this->TempGoal->Click += gcnew System::EventHandler(this, &FormAPMFinal::TempGoal_Click);
			this->TempGoal->Visible = false;
			// 
			// EVectorField
			// 
			this->EVectorField->AutoSize = true;
			this->EVectorField->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EVectorField->Location = System::Drawing::Point(20, 126);
			this->EVectorField->Name = L"EVectorField";
			this->EVectorField->Size = System::Drawing::Size(111, 19);
			this->EVectorField->TabIndex = 11;
			this->EVectorField->Text = L"Vector Field";
			this->EVectorField->UseVisualStyleBackColor = true;
			this->EVectorField->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::EVectorField_CheckedChanged);
			
			// 
			// ReversePolarity
			// 
			this->ReversePolarity->BackColor = System::Drawing::Color::DarkMagenta;
			this->ReversePolarity->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ReversePolarity->Location = System::Drawing::Point(151, 841);
			this->ReversePolarity->Name = L"ReversePolarity";
			this->ReversePolarity->Size = System::Drawing::Size(113, 48);
			this->ReversePolarity->TabIndex = 12;
			this->ReversePolarity->Text = L"Reverse Polarity";
			this->ReversePolarity->UseVisualStyleBackColor = false;
			this->ReversePolarity->Click += gcnew System::EventHandler(this, &FormAPMFinal::ReversePolarity_Click);
			// 
			// ResetLevel
			// 
			this->ResetLevel->BackColor = System::Drawing::Color::DarkSlateGray;
			this->ResetLevel->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ResetLevel->Location = System::Drawing::Point(22, 284);
			this->ResetLevel->Name = L"ResetLevel";
			this->ResetLevel->Size = System::Drawing::Size(82, 52);
			this->ResetLevel->TabIndex = 13;
			this->ResetLevel->Text = L"Reset";
			this->ResetLevel->UseVisualStyleBackColor = false;
			this->ResetLevel->Click += gcnew System::EventHandler(this, &FormAPMFinal::ResetLevel_Click);
			// 
			// wallGamePanel1
			// 
			this->wallGamePanel1->Controls->Add(this->WonGamePanel);
			this->wallGamePanel1->Controls->Add(this->ResetDifficultyB);
			this->wallGamePanel1->Controls->Add(this->MagnetsLeftLable);
			this->wallGamePanel1->Controls->Add(this->GameInducMagB);
			this->wallGamePanel1->Controls->Add(this->HighScore);
			this->wallGamePanel1->Controls->Add(this->noneButton);
			this->wallGamePanel1->Controls->Add(this->dragCheckBox);
			this->wallGamePanel1->Controls->Add(this->Level);
			this->wallGamePanel1->Controls->Add(this->ExitWallGame);
			this->wallGamePanel1->Controls->Add(this->ResetLevel);
			this->wallGamePanel1->Controls->Add(this->ReversePolarity);
			this->wallGamePanel1->Controls->Add(this->checkBox1);
			this->wallGamePanel1->Controls->Add(this->ResetCharges);
			this->wallGamePanel1->Controls->Add(this->button4);
			this->wallGamePanel1->Controls->Add(this->button3);
			this->wallGamePanel1->Controls->Add(this->chargesLeftLabel);
			this->wallGamePanel1->Controls->Add(this->checkBox2);
			this->wallGamePanel1->Controls->Add(this->button1);
			this->wallGamePanel1->Controls->Add(this->TempGoal);
			this->wallGamePanel1->Controls->Add(this->EVectorField);
			this->wallGamePanel1->Controls->Add(this->checkBox3);
			this->wallGamePanel1->Controls->Add(this->checkBox4);
			this->wallGamePanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->wallGamePanel1->ForeColor = System::Drawing::Color::LightGray;
			this->wallGamePanel1->Location = System::Drawing::Point(0, 0);
			this->wallGamePanel1->Name = L"wallGamePanel1";
			this->wallGamePanel1->Size = System::Drawing::Size(1836, 1000);
			this->wallGamePanel1->TabIndex = 14;
			this->wallGamePanel1->Visible = false;
			this->wallGamePanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormAPMFinal::wallGamePanel1_Paint);
			// 
		// WonGamePanel
		// 
			this->WonGamePanel->BackColor = System::Drawing::Color::Linen;
			this->WonGamePanel->Controls->Add(this->NextLvlExplainLable);
			this->WonGamePanel->Controls->Add(this->WellDoneLable);
			this->WonGamePanel->Controls->Add(this->NextLevelB);
			this->WonGamePanel->Controls->Add(this->RewatchB);
			this->WonGamePanel->Location = System::Drawing::Point(931, 347);
			this->WonGamePanel->Name = L"WonGamePanel";
			this->WonGamePanel->Size = System::Drawing::Size(287, 154);
			this->WonGamePanel->TabIndex = 22;
			this->WonGamePanel->Visible = false;
			// 
			// NextLvlExplainLable
			// 
			this->NextLvlExplainLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->NextLvlExplainLable->ForeColor = System::Drawing::Color::Black;
			this->NextLvlExplainLable->Location = System::Drawing::Point(18, 47);
			this->NextLvlExplainLable->Name = L"NextLvlExplainLable";
			this->NextLvlExplainLable->Size = System::Drawing::Size(248, 49);
			this->NextLvlExplainLable->TabIndex = 3;
			this->NextLvlExplainLable->Text = L"You have reached the goal. Do you want to go rewatch your master play or go to th"
				L"e next level\?";
			// 
			// WellDoneLable
			// 
			this->WellDoneLable->AutoSize = true;
			this->WellDoneLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 20.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->WellDoneLable->ForeColor = System::Drawing::Color::Black;
			this->WellDoneLable->Location = System::Drawing::Point(65, 10);
			this->WellDoneLable->Name = L"WellDoneLable";
			this->WellDoneLable->Size = System::Drawing::Size(169, 32);
			this->WellDoneLable->TabIndex = 2;
			this->WellDoneLable->Text = L"Well Done!";
			// 
			// NextLevelB
			// 
			this->NextLevelB->BackColor = System::Drawing::Color::Ivory;
			this->NextLevelB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->NextLevelB->ForeColor = System::Drawing::Color::Black;
			this->NextLevelB->Location = System::Drawing::Point(173, 99);
			this->NextLevelB->Name = L"NextLevelB";
			this->NextLevelB->Size = System::Drawing::Size(75, 46);
			this->NextLevelB->TabIndex = 1;
			this->NextLevelB->Text = L"Next Level";
			this->NextLevelB->UseVisualStyleBackColor = false;
			this->NextLevelB->Click += gcnew System::EventHandler(this, &FormAPMFinal::NextLevelB_Click);
			// 
		// RewatchB
		// 
			this->RewatchB->BackColor = System::Drawing::Color::Ivory;
			this->RewatchB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->RewatchB->ForeColor = System::Drawing::Color::Black;
			this->RewatchB->Location = System::Drawing::Point(38, 101);
			this->RewatchB->Name = L"RewatchB";
			this->RewatchB->Size = System::Drawing::Size(75, 44);
			this->RewatchB->TabIndex = 0;
			this->RewatchB->Text = L"Rewatch";
			this->RewatchB->UseVisualStyleBackColor = false;
			this->RewatchB->Click += gcnew System::EventHandler(this, &FormAPMFinal::RewatchB_Click);
			// 
			// ResetDifficultyB
			// 
			this->ResetDifficultyB->BackColor = System::Drawing::Color::SaddleBrown;
			this->ResetDifficultyB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ResetDifficultyB->Location = System::Drawing::Point(22, 232);
			this->ResetDifficultyB->Name = L"ResetDifficultyB";
			this->ResetDifficultyB->Size = System::Drawing::Size(80, 44);
			this->ResetDifficultyB->TabIndex = 21;
			this->ResetDifficultyB->Text = L"Reset Dificulty";
			this->ResetDifficultyB->UseVisualStyleBackColor = false;
			this->ResetDifficultyB->Click += gcnew System::EventHandler(this, &FormAPMFinal::ResetDifficultyB_Click);
			// 
			// MagnetsLeftLable
			// 
			this->MagnetsLeftLable->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->MagnetsLeftLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MagnetsLeftLable->Location = System::Drawing::Point(10, 896);
			this->MagnetsLeftLable->Name = L"MagnetsLeftLable";
			this->MagnetsLeftLable->Size = System::Drawing::Size(139, 23);
			this->MagnetsLeftLable->TabIndex = 20;
			this->MagnetsLeftLable->Text = L"Magnets Left: ";
			// 
			// GameInducMagB
			// 
			this->GameInducMagB->BackColor = System::Drawing::Color::MidnightBlue;
			this->GameInducMagB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->GameInducMagB->Location = System::Drawing::Point(13, 922);
			this->GameInducMagB->Name = L"GameInducMagB";
			this->GameInducMagB->Size = System::Drawing::Size(113, 75);
			this->GameInducMagB->TabIndex = 19;
			this->GameInducMagB->Text = L"Induce A Changing Magnetic Field";
			this->GameInducMagB->UseVisualStyleBackColor = false;
			this->GameInducMagB->Click += gcnew System::EventHandler(this, &FormAPMFinal::GameInducMagB_Click);
			// 
			// HighScore
			// 
			this->HighScore->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->HighScore->Location = System::Drawing::Point(148, 63);
			this->HighScore->Name = L"HighScore";
			this->HighScore->Size = System::Drawing::Size(224, 23);
			this->HighScore->TabIndex = 18;
			this->HighScore->Text = L"Easy High Score: 1";
			// 
			// noneButton
			// 
			this->noneButton->BackColor = System::Drawing::Color::OldLace;
			this->noneButton->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->noneButton->ForeColor = System::Drawing::Color::MidnightBlue;
			this->noneButton->Location = System::Drawing::Point(283, 841);
			this->noneButton->Name = L"noneButton";
			this->noneButton->Size = System::Drawing::Size(75, 48);
			this->noneButton->TabIndex = 17;
			this->noneButton->Text = L"None";
			this->noneButton->UseVisualStyleBackColor = false;
			this->noneButton->Click += gcnew System::EventHandler(this, &FormAPMFinal::noneButton_Click);
			// 
			// dragCheckBox
			// 
			this->dragCheckBox->AutoSize = true;
			this->dragCheckBox->BackColor = System::Drawing::Color::SaddleBrown;
			this->dragCheckBox->Checked = true;
			this->dragCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->dragCheckBox->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dragCheckBox->Location = System::Drawing::Point(13, 724);
			this->dragCheckBox->Name = L"dragCheckBox";
			this->dragCheckBox->Size = System::Drawing::Size(118, 19);
			this->dragCheckBox->TabIndex = 16;
			this->dragCheckBox->Text = L"Drag Charges";
			this->dragCheckBox->UseVisualStyleBackColor = false;
			this->dragCheckBox->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::dragCheckBox_CheckedChanged);
			// 
			// Level
			// 
			this->Level->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Level->Location = System::Drawing::Point(148, 88);
			this->Level->Name = L"Level";
			this->Level->Size = System::Drawing::Size(96, 16);
			this->Level->TabIndex = 15;
			this->Level->Text = L"Level : 1";
			// 
			// ExitWallGame
			// 
			this->ExitWallGame->BackColor = System::Drawing::Color::DarkRed;
			this->ExitWallGame->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ExitWallGame->Location = System::Drawing::Point(22, 21);
			this->ExitWallGame->Name = L"ExitWallGame";
			this->ExitWallGame->Size = System::Drawing::Size(94, 42);
			this->ExitWallGame->TabIndex = 14;
			this->ExitWallGame->Text = L"Exit Game";
			this->ExitWallGame->UseVisualStyleBackColor = false;
			this->ExitWallGame->Click += gcnew System::EventHandler(this, &FormAPMFinal::ExitWallGame_Click);
			// 
			// playWallGame
			// 
			this->playWallGame->BackColor = System::Drawing::Color::DarkSlateGray;
			this->playWallGame->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->playWallGame->ForeColor = System::Drawing::Color::LightGray;
			this->playWallGame->Location = System::Drawing::Point(650, 465);
			this->playWallGame->Name = L"playWallGame";
			this->playWallGame->Size = System::Drawing::Size(163, 126);
			this->playWallGame->TabIndex = 15;
			this->playWallGame->Text = L"Play Electromag Run";
			this->playWallGame->UseVisualStyleBackColor = false;
			this->playWallGame->Click += gcnew System::EventHandler(this, &FormAPMFinal::playWalllGame_Click);
			// 
			// homePanel
			// 
			this->homePanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->homePanel->Controls->Add(this->ElectricSandbox);
			this->homePanel->Controls->Add(this->playWallGame);
			this->homePanel->Controls->Add(this->ElectricSimTitle);
			this->homePanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->homePanel->Location = System::Drawing::Point(0, 0);
			this->homePanel->Name = L"homePanel";
			this->homePanel->Size = System::Drawing::Size(1836, 1000);
			this->homePanel->TabIndex = 16;
			// 
			// ElectricSimTitle
			// 
			this->ElectricSimTitle->AutoSize = true;
			this->ElectricSimTitle->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 27.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ElectricSimTitle->ForeColor = System::Drawing::SystemColors::Control;
			this->ElectricSimTitle->Location = System::Drawing::Point(this->ClientSize.Width/2, 88);
			this->ElectricSimTitle->Name = L"ElectricSimTitle";
			this->ElectricSimTitle->Size = System::Drawing::Size(573, 43);
			this->ElectricSimTitle->TabIndex = 22;
			this->ElectricSimTitle->Text = L"Electromagnetism Simulator";
			// 
			// ElectricSandbox
			// 
			this->ElectricSandbox->BackColor = System::Drawing::Color::DarkSlateGray;
			this->ElectricSandbox->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ElectricSandbox->ForeColor = System::Drawing::Color::LightGray;
			this->ElectricSandbox->Location = System::Drawing::Point(this->ClientSize.Width/2, 465);
			this->ElectricSandbox->Name = L"ElectricSandbox";
			this->ElectricSandbox->Size = System::Drawing::Size(177, 126);
			this->ElectricSandbox->TabIndex = 16;
			this->ElectricSandbox->Text = L"Electric Sandbox";
			this->ElectricSandbox->UseVisualStyleBackColor = false;
			this->ElectricSandbox->Click += gcnew System::EventHandler(this, &FormAPMFinal::ElectricSandbox_Click);
			// 
			// ElectricSandboxPanel
			// 
			this->ElectricSandboxPanel->Controls->Add(this->EditTestChargePanel);
			this->ElectricSandboxPanel->Controls->Add(this->SimulateWarningLable);
			this->ElectricSandboxPanel->Controls->Add(this->EditChargesCB);
			this->ElectricSandboxPanel->Controls->Add(this->editChargePanel);
			this->ElectricSandboxPanel->Controls->Add(this->HideAndShowCB);
			this->ElectricSandboxPanel->Controls->Add(this->EColorLabel);
			this->ElectricSandboxPanel->Controls->Add(this->panelColorPreview);
			this->ElectricSandboxPanel->Controls->Add(this->ELinesColorSelBn);
			this->ElectricSandboxPanel->Controls->Add(this->VecFieldColor);
			this->ElectricSandboxPanel->Controls->Add(this->SimulateComboBox);
			this->ElectricSandboxPanel->Controls->Add(this->ActivateTesQWarning);
			this->ElectricSandboxPanel->Controls->Add(this->InspectLable);
			this->ElectricSandboxPanel->Controls->Add(this->TestChargeActivate);
			this->ElectricSandboxPanel->Controls->Add(this->DrawQM);
			this->ElectricSandboxPanel->Controls->Add(this->EFieldLinesGen);
			this->ElectricSandboxPanel->Controls->Add(this->DebugLable);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxInduceMagB);
			this->ElectricSandboxPanel->Controls->Add(this->EFieldLinesCB);
			this->ElectricSandboxPanel->Controls->Add(this->ExitESandboxB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxVecFieldCB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxSimulateCB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxInspectCB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxDragCB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxNoneB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxReversePolB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxResetChargesB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxDeleteChargesB);
			this->ElectricSandboxPanel->Controls->Add(this->SandboxAddChargeB);
			this->ElectricSandboxPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->ElectricSandboxPanel->ForeColor = System::Drawing::Color::LightGray;
			this->ElectricSandboxPanel->Location = System::Drawing::Point(0, 0);
			this->ElectricSandboxPanel->Name = L"ElectricSandboxPanel";
			this->ElectricSandboxPanel->Size = System::Drawing::Size(1836, 1000);
			this->ElectricSandboxPanel->TabIndex = 18;
			this->ElectricSandboxPanel->Visible = false;
			this->ElectricSandboxPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormAPMFinal::ElectricSandboxPanel_Paint);
			// 
			// EditTestChargePanel
			// 
			this->EditTestChargePanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->EditTestChargePanel->Controls->Add(this->TesQXB);
			this->EditTestChargePanel->Controls->Add(this->ApplyPosTestQB);
			this->EditTestChargePanel->Controls->Add(this->TestQMassNumericUpDown);
			this->EditTestChargePanel->Controls->Add(this->TestQChargeNumericUpDown);
			this->EditTestChargePanel->Controls->Add(this->TestQYPosTextbox);
			this->EditTestChargePanel->Controls->Add(this->TestQXPosTextbox);
			this->EditTestChargePanel->Controls->Add(this->TestQYPosLable);
			this->EditTestChargePanel->Controls->Add(this->TestQXposLable);
			this->EditTestChargePanel->Controls->Add(this->TestQChargeLable);
			this->EditTestChargePanel->Controls->Add(this->TestQMassLable);
			this->EditTestChargePanel->Controls->Add(this->TestChargeEditLable);
			this->EditTestChargePanel->Location = System::Drawing::Point(416, 354);
			this->EditTestChargePanel->Name = L"EditTestChargePanel";
			this->EditTestChargePanel->Size = System::Drawing::Size(149, 169);
			this->EditTestChargePanel->TabIndex = 27;
			this->EditTestChargePanel->Visible = false;
			// 
			// TesQXB
			// 
			this->TesQXB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->TesQXB->BackColor = System::Drawing::Color::RosyBrown;
			this->TesQXB->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->TesQXB->Location = System::Drawing::Point(124, 3);
			this->TesQXB->Name = L"TesQXB";
			this->TesQXB->Size = System::Drawing::Size(19, 18);
			this->TesQXB->TabIndex = 10;
			this->TesQXB->Text = L"X";
			this->TesQXB->UseVisualStyleBackColor = false;
			this->TesQXB->Click += gcnew System::EventHandler(this, &FormAPMFinal::TesQXB_Click);
			// 
			// ApplyPosTestQB
			// 
			this->ApplyPosTestQB->BackColor = System::Drawing::Color::OliveDrab;
			this->ApplyPosTestQB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ApplyPosTestQB->Location = System::Drawing::Point(23, 134);
			this->ApplyPosTestQB->Name = L"ApplyPosTestQB";
			this->ApplyPosTestQB->Size = System::Drawing::Size(106, 23);
			this->ApplyPosTestQB->TabIndex = 9;
			this->ApplyPosTestQB->Text = L"Apply Position";
			this->ApplyPosTestQB->UseVisualStyleBackColor = false;
			this->ApplyPosTestQB->Click += gcnew System::EventHandler(this, &FormAPMFinal::ApplyPosTestQB_Click);
			// 
			// TestQMassNumericUpDown
			// 
			this->TestQMassNumericUpDown->DecimalPlaces = 1;
			this->TestQMassNumericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			this->TestQMassNumericUpDown->Location = System::Drawing::Point(83, 58);
			this->TestQMassNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->TestQMassNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			this->TestQMassNumericUpDown->Name = L"TestQMassNumericUpDown";
			this->TestQMassNumericUpDown->Size = System::Drawing::Size(49, 20);
			this->TestQMassNumericUpDown->TabIndex = 8;
			this->TestQMassNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->TestQMassNumericUpDown->ValueChanged += gcnew System::EventHandler(this, &FormAPMFinal::TestQMassNumericUpDown_ValueChanged);
			// 
			// TestQChargeNumericUpDown
			// 
			this->TestQChargeNumericUpDown->DecimalPlaces = 1;
			this->TestQChargeNumericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->TestQChargeNumericUpDown->Location = System::Drawing::Point(12, 58);
			this->TestQChargeNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->TestQChargeNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->TestQChargeNumericUpDown->Name = L"TestQChargeNumericUpDown";
			this->TestQChargeNumericUpDown->Size = System::Drawing::Size(50, 20);
			this->TestQChargeNumericUpDown->TabIndex = 7;
			this->TestQChargeNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->TestQChargeNumericUpDown->ValueChanged += gcnew System::EventHandler(this, &FormAPMFinal::TestQChargeNumericUpDown_ValueChanged);
			// 
			// TestQYPosTextbox
			// 
			this->TestQYPosTextbox->Location = System::Drawing::Point(93, 108);
			this->TestQYPosTextbox->Name = L"TestQYPosTextbox";
			this->TestQYPosTextbox->Size = System::Drawing::Size(43, 20);
			this->TestQYPosTextbox->TabIndex = 6;
			// 
			// TestQXPosTextbox
			// 
			this->TestQXPosTextbox->Location = System::Drawing::Point(16, 108);
			this->TestQXPosTextbox->Name = L"TestQXPosTextbox";
			this->TestQXPosTextbox->Size = System::Drawing::Size(46, 20);
			this->TestQXPosTextbox->TabIndex = 5;
			// 
			// TestQYPosLable
			// 
			this->TestQYPosLable->AutoSize = true;
			this->TestQYPosLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestQYPosLable->ForeColor = System::Drawing::Color::Black;
			this->TestQYPosLable->Location = System::Drawing::Point(80, 86);
			this->TestQYPosLable->Name = L"TestQYPosLable";
			this->TestQYPosLable->Size = System::Drawing::Size(67, 14);
			this->TestQYPosLable->TabIndex = 4;
			this->TestQYPosLable->Text = L"Y Position";
			// 
			// TestQXposLable
			// 
			this->TestQXposLable->AutoSize = true;
			this->TestQXposLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestQXposLable->ForeColor = System::Drawing::Color::Black;
			this->TestQXposLable->Location = System::Drawing::Point(8, 86);
			this->TestQXposLable->Name = L"TestQXposLable";
			this->TestQXposLable->Size = System::Drawing::Size(66, 14);
			this->TestQXposLable->TabIndex = 3;
			this->TestQXposLable->Text = L"X Position";
			// 
			// TestQChargeLable
			// 
			this->TestQChargeLable->AutoSize = true;
			this->TestQChargeLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestQChargeLable->ForeColor = System::Drawing::Color::Black;
			this->TestQChargeLable->Location = System::Drawing::Point(8, 41);
			this->TestQChargeLable->Name = L"TestQChargeLable";
			this->TestQChargeLable->Size = System::Drawing::Size(48, 14);
			this->TestQChargeLable->TabIndex = 2;
			this->TestQChargeLable->Text = L"Charge";
			// 
			// TestQMassLable
			// 
			this->TestQMassLable->AutoSize = true;
			this->TestQMassLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestQMassLable->ForeColor = System::Drawing::Color::Black;
			this->TestQMassLable->Location = System::Drawing::Point(84, 41);
			this->TestQMassLable->Name = L"TestQMassLable";
			this->TestQMassLable->Size = System::Drawing::Size(39, 14);
			this->TestQMassLable->TabIndex = 1;
			this->TestQMassLable->Text = L"Mass ";
			// 
			// TestChargeEditLable
			// 
			this->TestChargeEditLable->AutoSize = true;
			this->TestChargeEditLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestChargeEditLable->ForeColor = System::Drawing::Color::Black;
			this->TestChargeEditLable->Location = System::Drawing::Point(28, 8);
			this->TestChargeEditLable->Name = L"TestChargeEditLable";
			this->TestChargeEditLable->Size = System::Drawing::Size(90, 15);
			this->TestChargeEditLable->TabIndex = 0;
			this->TestChargeEditLable->Text = L"Test Charge";
			// 
			// SimulateWarningLable
			// 
			this->SimulateWarningLable->AutoSize = true;
			this->SimulateWarningLable->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->SimulateWarningLable->ForeColor = System::Drawing::Color::IndianRed;
			this->SimulateWarningLable->Location = System::Drawing::Point(1398, 88);
			this->SimulateWarningLable->Name = L"SimulateWarningLable";
			this->SimulateWarningLable->Size = System::Drawing::Size(222, 13);
			this->SimulateWarningLable->TabIndex = 26;
			this->SimulateWarningLable->Text = L"SELECT WHAT TO SIMULATE FIRST";
			this->SimulateWarningLable->Visible = false;
			// 
			// EditChargesCB
			// 
			this->EditChargesCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->EditChargesCB->AutoSize = true;
			this->EditChargesCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EditChargesCB->Location = System::Drawing::Point(1624, 523);
			this->EditChargesCB->Name = L"EditChargesCB";
			this->EditChargesCB->Size = System::Drawing::Size(113, 19);
			this->EditChargesCB->TabIndex = 25;
			this->EditChargesCB->Text = L"Edit Charges";
			this->EditChargesCB->UseVisualStyleBackColor = true;
			this->EditChargesCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::EditChargesCB_CheckedChanged);
			// 
			// editChargePanel
			// 
			this->editChargePanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(224)), static_cast<System::Int32>(static_cast<System::Byte>(224)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->editChargePanel->Controls->Add(this->ApplyPosB);
			this->editChargePanel->Controls->Add(this->YPosLable);
			this->editChargePanel->Controls->Add(this->XposLable);
			this->editChargePanel->Controls->Add(this->YPosTextBox);
			this->editChargePanel->Controls->Add(this->XPosTextBox);
			this->editChargePanel->Controls->Add(this->ExitEditChargePanelB);
			this->editChargePanel->Controls->Add(this->MassNumericUpDown);
			this->editChargePanel->Controls->Add(this->MassLable);
			this->editChargePanel->Controls->Add(this->ChargeLable);
			this->editChargePanel->Controls->Add(this->ChargeNumericUpDown);
			this->editChargePanel->Controls->Add(this->ChargeNumberLable);
			this->editChargePanel->Location = System::Drawing::Point(806, 207);
			this->editChargePanel->Name = L"editChargePanel";
			this->editChargePanel->Size = System::Drawing::Size(148, 179);
			this->editChargePanel->TabIndex = 24;
			this->editChargePanel->Visible = false;
			// 
			// ApplyPosB
			// 
			this->ApplyPosB->BackColor = System::Drawing::Color::OliveDrab;
			this->ApplyPosB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ApplyPosB->Location = System::Drawing::Point(32, 145);
			this->ApplyPosB->Name = L"ApplyPosB";
			this->ApplyPosB->Size = System::Drawing::Size(102, 23);
			this->ApplyPosB->TabIndex = 10;
			this->ApplyPosB->Text = L"Apply Position";
			this->ApplyPosB->UseVisualStyleBackColor = false;
			this->ApplyPosB->Click += gcnew System::EventHandler(this, &FormAPMFinal::ApplyPosB_Click);
			// 
			// YPosLable
			// 
			this->YPosLable->AutoSize = true;
			this->YPosLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->YPosLable->ForeColor = System::Drawing::Color::Black;
			this->YPosLable->Location = System::Drawing::Point(78, 97);
			this->YPosLable->Name = L"YPosLable";
			this->YPosLable->Size = System::Drawing::Size(67, 14);
			this->YPosLable->TabIndex = 9;
			this->YPosLable->Text = L"Y Position";
			// 
			// XposLable
			// 
			this->XposLable->AutoSize = true;
			this->XposLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->XposLable->ForeColor = System::Drawing::Color::Black;
			this->XposLable->Location = System::Drawing::Point(3, 97);
			this->XposLable->Name = L"XposLable";
			this->XposLable->Size = System::Drawing::Size(66, 14);
			this->XposLable->TabIndex = 8;
			this->XposLable->Text = L"X Position";
			// 
			// YPosTextBox
			// 
			this->YPosTextBox->Location = System::Drawing::Point(89, 114);
			this->YPosTextBox->Name = L"YPosTextBox";
			this->YPosTextBox->Size = System::Drawing::Size(46, 20);
			this->YPosTextBox->TabIndex = 7;
			// 
			// XPosTextBox
			// 
			this->XPosTextBox->Location = System::Drawing::Point(13, 114);
			this->XPosTextBox->Name = L"XPosTextBox";
			this->XPosTextBox->Size = System::Drawing::Size(46, 20);
			this->XPosTextBox->TabIndex = 6;
			// 
			// ExitEditChargePanelB
			// 
			this->ExitEditChargePanelB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->ExitEditChargePanelB->BackColor = System::Drawing::Color::RosyBrown;
			this->ExitEditChargePanelB->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->ExitEditChargePanelB->Location = System::Drawing::Point(125, 3);
			this->ExitEditChargePanelB->Name = L"ExitEditChargePanelB";
			this->ExitEditChargePanelB->Size = System::Drawing::Size(19, 18);
			this->ExitEditChargePanelB->TabIndex = 5;
			this->ExitEditChargePanelB->Text = L"X";
			this->ExitEditChargePanelB->UseVisualStyleBackColor = false;
			this->ExitEditChargePanelB->Click += gcnew System::EventHandler(this, &FormAPMFinal::ExitEditChargePanelB_Click);
			// 
			// MassNumericUpDown
			// 
			this->MassNumericUpDown->DecimalPlaces = 1;
			this->MassNumericUpDown->Enabled = false;
			this->MassNumericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			this->MassNumericUpDown->Location = System::Drawing::Point(89, 58);
			this->MassNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->MassNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			this->MassNumericUpDown->Name = L"MassNumericUpDown";
			this->MassNumericUpDown->Size = System::Drawing::Size(50, 20);
			this->MassNumericUpDown->TabIndex = 4;
			this->MassNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->MassNumericUpDown->ValueChanged += gcnew System::EventHandler(this, &FormAPMFinal::MassNumericUpDown_ValueChanged);
			// 
			// MassLable
			// 
			this->MassLable->AutoSize = true;
			this->MassLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MassLable->ForeColor = System::Drawing::Color::Black;
			this->MassLable->Location = System::Drawing::Point(86, 41);
			this->MassLable->Name = L"MassLable";
			this->MassLable->Size = System::Drawing::Size(35, 14);
			this->MassLable->TabIndex = 3;
			this->MassLable->Text = L"Mass";
			// 
			// ChargeLable
			// 
			this->ChargeLable->AutoSize = true;
			this->ChargeLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ChargeLable->ForeColor = System::Drawing::Color::Black;
			this->ChargeLable->Location = System::Drawing::Point(10, 41);
			this->ChargeLable->Name = L"ChargeLable";
			this->ChargeLable->Size = System::Drawing::Size(48, 14);
			this->ChargeLable->TabIndex = 2;
			this->ChargeLable->Text = L"Charge";
			// 
			// ChargeNumericUpDown
			// 
			this->ChargeNumericUpDown->DecimalPlaces = 1;
			this->ChargeNumericUpDown->Enabled = false;
			this->ChargeNumericUpDown->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->ChargeNumericUpDown->Location = System::Drawing::Point(13, 58);
			this->ChargeNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->ChargeNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->ChargeNumericUpDown->Name = L"ChargeNumericUpDown";
			this->ChargeNumericUpDown->Size = System::Drawing::Size(50, 20);
			this->ChargeNumericUpDown->TabIndex = 1;
			this->ChargeNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->ChargeNumericUpDown->ValueChanged += gcnew System::EventHandler(this, &FormAPMFinal::ChargeNumericUpDown_ValueChanged);
			// 
			// ChargeNumberLable
			// 
			this->ChargeNumberLable->AutoSize = true;
			this->ChargeNumberLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ChargeNumberLable->ForeColor = System::Drawing::Color::Black;
			this->ChargeNumberLable->Location = System::Drawing::Point(44, 13);
			this->ChargeNumberLable->Name = L"ChargeNumberLable";
			this->ChargeNumberLable->Size = System::Drawing::Size(68, 15);
			this->ChargeNumberLable->TabIndex = 0;
			this->ChargeNumberLable->Text = L"Charge 1";
			// 
			// HideAndShowCB
			// 
			this->HideAndShowCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->HideAndShowCB->AutoSize = true;
			this->HideAndShowCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->HideAndShowCB->Location = System::Drawing::Point(1700, 10);
			this->HideAndShowCB->Name = L"HideAndShowCB";
			this->HideAndShowCB->Size = System::Drawing::Size(124, 18);
			this->HideAndShowCB->TabIndex = 23;
			this->HideAndShowCB->Text = L"HIDE CONTROLS";
			this->HideAndShowCB->UseVisualStyleBackColor = true;
			this->HideAndShowCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::HideAndShowCB_CheckedChanged);
			// 
			// EColorLabel
			// 
			this->EColorLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->EColorLabel->AutoSize = true;
			this->EColorLabel->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EColorLabel->Location = System::Drawing::Point(1768, 234);
			this->EColorLabel->Name = L"EColorLabel";
			this->EColorLabel->Size = System::Drawing::Size(42, 14);
			this->EColorLabel->TabIndex = 22;
			this->EColorLabel->Text = L"Color:";
			this->EColorLabel->Visible = false;
			// 
			// panelColorPreview
			// 
			this->panelColorPreview->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->panelColorPreview->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->panelColorPreview->Location = System::Drawing::Point(1775, 261);
			this->panelColorPreview->Name = L"panelColorPreview";
			this->panelColorPreview->Size = System::Drawing::Size(29, 20);
			this->panelColorPreview->TabIndex = 21;
			this->panelColorPreview->Visible = false;
			// 
			// ELinesColorSelBn
			// 
			this->ELinesColorSelBn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->ELinesColorSelBn->BackColor = System::Drawing::Color::DarkOrange;
			this->ELinesColorSelBn->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ELinesColorSelBn->ForeColor = System::Drawing::Color::Black;
			this->ELinesColorSelBn->Location = System::Drawing::Point(1631, 234);
			this->ELinesColorSelBn->Name = L"ELinesColorSelBn";
			this->ELinesColorSelBn->Size = System::Drawing::Size(107, 47);
			this->ELinesColorSelBn->TabIndex = 20;
			this->ELinesColorSelBn->Text = L"Choose Lines Color";
			this->ELinesColorSelBn->UseVisualStyleBackColor = false;
			this->ELinesColorSelBn->Visible = false;
			this->ELinesColorSelBn->Click += gcnew System::EventHandler(this, &FormAPMFinal::ELinesColorSelBn_Click);
			// 
			// VecFieldColor
			// 
			this->VecFieldColor->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->VecFieldColor->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->VecFieldColor->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->VecFieldColor->ForeColor = System::Drawing::SystemColors::Info;
			this->VecFieldColor->FormattingEnabled = true;
			this->VecFieldColor->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Physics Intensity", L"Color Choice" });
			this->VecFieldColor->Location = System::Drawing::Point(1631, 194);
			this->VecFieldColor->Name = L"VecFieldColor";
			this->VecFieldColor->Size = System::Drawing::Size(179, 23);
			this->VecFieldColor->TabIndex = 19;
			this->VecFieldColor->Text = L"Line Field Color";
			this->VecFieldColor->SelectedIndexChanged += gcnew System::EventHandler(this, &FormAPMFinal::VecFieldColor_SelectedIndexChanged);
			// 
			// SimulateComboBox
			// 
			this->SimulateComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SimulateComboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->SimulateComboBox->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SimulateComboBox->ForeColor = System::Drawing::SystemColors::Info;
			this->SimulateComboBox->FormattingEnabled = true;
			this->SimulateComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				L"Simulate ALL", L"Simulate Test Charge",
					L"Simulate Charges"
			});
			this->SimulateComboBox->Location = System::Drawing::Point(1631, 81);
			this->SimulateComboBox->Name = L"SimulateComboBox";
			this->SimulateComboBox->Size = System::Drawing::Size(173, 23);
			this->SimulateComboBox->TabIndex = 18;
			this->SimulateComboBox->Text = L"Simulate Options";
			this->SimulateComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &FormAPMFinal::SimulateComboBox_SelectedIndexChanged);
			// 
			// ActivateTesQWarning
			// 
			this->ActivateTesQWarning->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->ActivateTesQWarning->AutoSize = true;
			this->ActivateTesQWarning->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ActivateTesQWarning->ForeColor = System::Drawing::Color::IndianRed;
			this->ActivateTesQWarning->Location = System::Drawing::Point(1393, 407);
			this->ActivateTesQWarning->Name = L"ActivateTesQWarning";
			this->ActivateTesQWarning->Size = System::Drawing::Size(227, 13);
			this->ActivateTesQWarning->TabIndex = 17;
			this->ActivateTesQWarning->Text = L"ACTIVATE THE TEST CHARGE FIRST";
			this->ActivateTesQWarning->Visible = false;
			// 
			// InspectLable
			// 
			this->InspectLable->BackColor = System::Drawing::Color::AliceBlue;
			this->InspectLable->Font = (gcnew System::Drawing::Font(L"Mongolian Baiti", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InspectLable->ForeColor = System::Drawing::Color::Black;
			this->InspectLable->Location = System::Drawing::Point(1360, 448);
			this->InspectLable->Name = L"InspectLable";
			this->InspectLable->Size = System::Drawing::Size(225, 230);
			this->InspectLable->TabIndex = 16;
			this->InspectLable->Text = L"Inspect Lable";
			this->InspectLable->Visible = false;
			// 
			// TestChargeActivate
			// 
			this->TestChargeActivate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->TestChargeActivate->AutoSize = true;
			this->TestChargeActivate->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TestChargeActivate->Location = System::Drawing::Point(1627, 405);
			this->TestChargeActivate->Name = L"TestChargeActivate";
			this->TestChargeActivate->Size = System::Drawing::Size(171, 19);
			this->TestChargeActivate->TabIndex = 15;
			this->TestChargeActivate->Text = L"Activate Test Charge";
			this->TestChargeActivate->UseVisualStyleBackColor = true;
			this->TestChargeActivate->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::TestChargeActivate_CheckedChanged);
			// 
			// DrawQM
			// 
			this->DrawQM->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->DrawQM->AutoSize = true;
			this->DrawQM->Checked = true;
			this->DrawQM->CheckState = System::Windows::Forms::CheckState::Checked;
			this->DrawQM->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DrawQM->Location = System::Drawing::Point(1627, 633);
			this->DrawQM->Name = L"DrawQM";
			this->DrawQM->Size = System::Drawing::Size(118, 19);
			this->DrawQM->TabIndex = 14;
			this->DrawQM->Text = L"Draw Objects";
			this->DrawQM->UseVisualStyleBackColor = true;
			this->DrawQM->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::DrawQM_CheckedChanged);
			// 
			// EFieldLinesGen
			// 
			this->EFieldLinesGen->AllowDrop = true;
			this->EFieldLinesGen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->EFieldLinesGen->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->EFieldLinesGen->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EFieldLinesGen->ForeColor = System::Drawing::Color::WhiteSmoke;
			this->EFieldLinesGen->FormattingEnabled = true;
			this->EFieldLinesGen->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"Random Field", L"Grid Generation", L"Random from Charge",
					L"Radial Generation"
			});
			this->EFieldLinesGen->Location = System::Drawing::Point(1631, 156);
			this->EFieldLinesGen->Name = L"EFieldLinesGen";
			this->EFieldLinesGen->Size = System::Drawing::Size(179, 23);
			this->EFieldLinesGen->TabIndex = 13;
			this->EFieldLinesGen->Text = L"Field Line Generation";
			this->EFieldLinesGen->SelectedIndexChanged += gcnew System::EventHandler(this, &FormAPMFinal::EFieldLinesGen_SelectedIndexChanged);
			// 
			// DebugLable
			// 
			this->DebugLable->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->DebugLable->AutoSize = true;
			this->DebugLable->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->DebugLable->Location = System::Drawing::Point(1628, 284);
			this->DebugLable->Name = L"DebugLable";
			this->DebugLable->Size = System::Drawing::Size(110, 15);
			this->DebugLable->TabIndex = 12;
			this->DebugLable->Text = L"Lines generated:";
			this->DebugLable->Visible = false;
			// 
			// SandboxInduceMagB
			// 
			this->SandboxInduceMagB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxInduceMagB->BackColor = System::Drawing::Color::DarkBlue;
			this->SandboxInduceMagB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxInduceMagB->Location = System::Drawing::Point(1536, 773);
			this->SandboxInduceMagB->Name = L"SandboxInduceMagB";
			this->SandboxInduceMagB->Size = System::Drawing::Size(120, 61);
			this->SandboxInduceMagB->TabIndex = 11;
			this->SandboxInduceMagB->Text = L"Induce a Changing Magnetic Field";
			this->SandboxInduceMagB->UseVisualStyleBackColor = false;
			this->SandboxInduceMagB->Click += gcnew System::EventHandler(this, &FormAPMFinal::SandboxInduceMagB_Click);
			// 
			// EFieldLinesCB
			// 
			this->EFieldLinesCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->EFieldLinesCB->AutoSize = true;
			this->EFieldLinesCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EFieldLinesCB->Location = System::Drawing::Point(1631, 131);
			this->EFieldLinesCB->Name = L"EFieldLinesCB";
			this->EFieldLinesCB->Size = System::Drawing::Size(114, 19);
			this->EFieldLinesCB->TabIndex = 10;
			this->EFieldLinesCB->Text = L"E Field Lines";
			this->EFieldLinesCB->UseVisualStyleBackColor = true;
			this->EFieldLinesCB->CheckedChanged += gcnew System::EventHandler(this, &FormAPMFinal::EFieldLinesCB_CheckedChanged);
			// 
			// ExitESandboxB
			// 
			this->ExitESandboxB->BackColor = System::Drawing::Color::Maroon;
			this->ExitESandboxB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ExitESandboxB->Location = System::Drawing::Point(29, 21);
			this->ExitESandboxB->Name = L"ExitESandboxB";
			this->ExitESandboxB->Size = System::Drawing::Size(122, 59);
			this->ExitESandboxB->TabIndex = 9;
			this->ExitESandboxB->Text = L"Exit Electric Sandbox";
			this->ExitESandboxB->UseVisualStyleBackColor = false;
			this->ExitESandboxB->Click += gcnew System::EventHandler(this, &FormAPMFinal::ExitESandboxB_Click);
			// 
			// SandboxVecFieldCB
			// 
			this->SandboxVecFieldCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxVecFieldCB->AutoSize = true;
			this->SandboxVecFieldCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxVecFieldCB->Location = System::Drawing::Point(1627, 595);
			this->SandboxVecFieldCB->Name = L"SandboxVecFieldCB";
			this->SandboxVecFieldCB->Size = System::Drawing::Size(111, 19);
			this->SandboxVecFieldCB->TabIndex = 8;
			this->SandboxVecFieldCB->Text = L"Vector Field";
			this->SandboxVecFieldCB->UseVisualStyleBackColor = true;
			// 
			// SandboxSimulateCB
			// 
			this->SandboxSimulateCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxSimulateCB->AutoSize = true;
			this->SandboxSimulateCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxSimulateCB->Location = System::Drawing::Point(1631, 56);
			this->SandboxSimulateCB->Name = L"SandboxSimulateCB";
			this->SandboxSimulateCB->Size = System::Drawing::Size(87, 19);
			this->SandboxSimulateCB->TabIndex = 7;
			this->SandboxSimulateCB->Text = L"Simulate";
			this->SandboxSimulateCB->UseVisualStyleBackColor = true;
			// 
			// SandboxInspectCB
			// 
			this->SandboxInspectCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxInspectCB->AutoSize = true;
			this->SandboxInspectCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxInspectCB->Location = System::Drawing::Point(1627, 497);
			this->SandboxInspectCB->Name = L"SandboxInspectCB";
			this->SandboxInspectCB->Size = System::Drawing::Size(77, 19);
			this->SandboxInspectCB->TabIndex = 6;
			this->SandboxInspectCB->Text = L"Inspect";
			this->SandboxInspectCB->UseVisualStyleBackColor = true;
			// 
			// SandboxDragCB
			// 
			this->SandboxDragCB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxDragCB->AutoSize = true;
			this->SandboxDragCB->Checked = true;
			this->SandboxDragCB->CheckState = System::Windows::Forms::CheckState::Checked;
			this->SandboxDragCB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxDragCB->Location = System::Drawing::Point(1627, 707);
			this->SandboxDragCB->Name = L"SandboxDragCB";
			this->SandboxDragCB->Size = System::Drawing::Size(118, 19);
			this->SandboxDragCB->TabIndex = 5;
			this->SandboxDragCB->Text = L"Drag Charges";
			this->SandboxDragCB->UseVisualStyleBackColor = true;
			// 
			// SandboxNoneB
			// 
			this->SandboxNoneB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxNoneB->BackColor = System::Drawing::Color::OldLace;
			this->SandboxNoneB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxNoneB->ForeColor = System::Drawing::Color::Black;
			this->SandboxNoneB->Location = System::Drawing::Point(1396, 886);
			this->SandboxNoneB->Name = L"SandboxNoneB";
			this->SandboxNoneB->Size = System::Drawing::Size(120, 61);
			this->SandboxNoneB->TabIndex = 4;
			this->SandboxNoneB->Text = L"None";
			this->SandboxNoneB->UseVisualStyleBackColor = false;
			// 
			// SandboxReversePolB
			// 
			this->SandboxReversePolB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxReversePolB->BackColor = System::Drawing::Color::Indigo;
			this->SandboxReversePolB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxReversePolB->Location = System::Drawing::Point(1678, 846);
			this->SandboxReversePolB->Name = L"SandboxReversePolB";
			this->SandboxReversePolB->Size = System::Drawing::Size(120, 63);
			this->SandboxReversePolB->TabIndex = 3;
			this->SandboxReversePolB->Text = L"Reverse Polarity";
			this->SandboxReversePolB->UseVisualStyleBackColor = false;
			// 
			// SandboxResetChargesB
			// 
			this->SandboxResetChargesB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxResetChargesB->BackColor = System::Drawing::Color::Maroon;
			this->SandboxResetChargesB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxResetChargesB->Location = System::Drawing::Point(1678, 922);
			this->SandboxResetChargesB->Name = L"SandboxResetChargesB";
			this->SandboxResetChargesB->Size = System::Drawing::Size(120, 66);
			this->SandboxResetChargesB->TabIndex = 2;
			this->SandboxResetChargesB->Text = L"Reset Charges";
			this->SandboxResetChargesB->UseVisualStyleBackColor = false;
			// 
			// SandboxDeleteChargesB
			// 
			this->SandboxDeleteChargesB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxDeleteChargesB->BackColor = System::Drawing::Color::Maroon;
			this->SandboxDeleteChargesB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxDeleteChargesB->Location = System::Drawing::Point(1536, 922);
			this->SandboxDeleteChargesB->Name = L"SandboxDeleteChargesB";
			this->SandboxDeleteChargesB->Size = System::Drawing::Size(120, 63);
			this->SandboxDeleteChargesB->TabIndex = 1;
			this->SandboxDeleteChargesB->Text = L"Delete Charges";
			this->SandboxDeleteChargesB->UseVisualStyleBackColor = false;
			// 
			// SandboxAddChargeB
			// 
			this->SandboxAddChargeB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->SandboxAddChargeB->BackColor = System::Drawing::Color::DarkGreen;
			this->SandboxAddChargeB->Font = (gcnew System::Drawing::Font(L"Lucida Fax", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SandboxAddChargeB->ForeColor = System::Drawing::Color::LightGray;
			this->SandboxAddChargeB->Location = System::Drawing::Point(1536, 846);
			this->SandboxAddChargeB->Name = L"SandboxAddChargeB";
			this->SandboxAddChargeB->Size = System::Drawing::Size(120, 63);
			this->SandboxAddChargeB->TabIndex = 0;
			this->SandboxAddChargeB->Text = L"Add Charges";
			this->SandboxAddChargeB->UseVisualStyleBackColor = false;
			// 
			// LinesColorDialog
			// 
			this->LinesColorDialog->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			// 
			// FormAPMFinal
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)));
			System::String^ imagePath =	System::IO::Path::Combine(System::Windows::Forms::Application::StartupPath,	"Assets\\ElectricSandboxPP.png");

			this->homePanel->BackgroundImage =System::Drawing::Image::FromFile(imagePath);

			this->homePanel->BackgroundImageLayout =
				System::Windows::Forms::ImageLayout::Stretch;
			this->homePanel->BackgroundImageLayout =
				System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1836, 1000);
			this->Controls->Add(this->ElectricSandboxPanel);
			this->Controls->Add(this->homePanel);
			this->Controls->Add(this->wallGamePanel1);
			this->Name = L"FormAPMFinal";
			this->Text = L"FormAPMFinal";
			this->TopMost = true;
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormAPMFinal::FormAPMFinal_Paint);
			this->wallGamePanel1->ResumeLayout(false);
			this->wallGamePanel1->PerformLayout();
			this->WonGamePanel->ResumeLayout(false);
			this->homePanel->ResumeLayout(false);
			this->ElectricSandboxPanel->ResumeLayout(false);
			this->ElectricSandboxPanel->PerformLayout();
			this->EditTestChargePanel->ResumeLayout(false);
			this->EditTestChargePanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TestQMassNumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TestQChargeNumericUpDown))->EndInit();
			this->editChargePanel->ResumeLayout(false);
			this->editChargePanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->MassNumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ChargeNumericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
	
#pragma endregion
			private: System::Void PositionButtons(System::Object^ sender, System::EventArgs^ e) {
				int panelW = ElectricSandboxPanel->ClientSize.Width;
				playWallGame->Location = System::Drawing::Point((int)(panelW * 0.42) - playWallGame->Width / 2, 465);
				ElectricSandbox->Location = System::Drawing::Point((int)(panelW * 0.58),	465	);
				ElectricSimTitle->Location = System::Drawing::Point((int)(panelW * 0.37), 43);
			}
		void ChargesUsed() {
			// // the prevQUsed is so that when you move to next level you still used those	
			if (diff == Diff::easy) { easyQused = easyPrevQused + Q->size(); QUsed = easyQused; }
			if (diff == Diff::medium) { medQused = medPrevQused + Q->size(); QUsed = medQused; }
			if (diff == Diff::hard) { hardQused = hardPrevQused + Q->size(); QUsed = hardQused; }
		}
		void QleftLableUpdate() {
			ChargesUsed();
			Qleft = maxQN - QUsed;
			chargesLeftLabel->Text = "Charges left : " + Qleft.ToString();
		}
		void MagnetsUsed() {
			//same as with the chatges
			if (diff == Diff::easy) { easyMagUsed = easyPrevMagUsed + mag->size(); MagUsed = easyMagUsed; }
			if (diff == Diff::medium) { medMagUsed = medPrevMagUsed + mag->size(); MagUsed = medMagUsed; }
			if (diff == Diff::hard) { hardMagUsed = hardPrevMagUsed + mag->size(); MagUsed = hardMagUsed; }
		}
		void MagLeftLableUpdate() {
			MagnetsUsed();
			Magleft = maxMag - MagUsed;
			MagnetsLeftLable->Text = "Magnets Left : " + Magleft.ToString();
		}
		void deleteQ() {
			for (int i = Q->size() - 1; i >= 0; --i) {
				Q->erase(Q->begin() + i);
			}
		}
		void inspectLable() {
			Vec2D gradMag = toMathCoords(computeGradMag(*Q, *mag, xs, ys));
			InspectLable->Text = "All non-angle quantities are 10^-4 smaller than SI values\n\n"+
				"Position = (" + sandboxTestCharge->getX().ToString() + ", " + switchCoords(sandboxTestCharge->getY(), ElectricSandboxPanel->Height).ToString() + ")\n\n" +
				"Electric Field Magnitude = " + (Emag(totEat(*Q, *mag, xs, ys)) * 5).ToString("F2") + "\n" +
				"Force Experienced= " + testQF(*sandboxTestCharge, *Q, *mag, sandboxTestCharge->getX(), sandboxTestCharge->getY()).ToString("F2") + "\n" +
				"Direction of E and F = " + EdirInDeg(toMathCoords(totEat(*Q, *mag, xs, ys))).ToString("F2") + "° \n\n" +
				"Curl E = " + computeCurl(*Q, *mag, xs, ys).ToString("F5") + "\n\n" +
				"Div E = " + computeDivergence(*Q, *mag, xs, ys).ToString("F2") + "\n\n" +
				"Grad |E| x = " + gradMag.getX().ToString("F2") + "\nGrad |E| y = " + gradMag.getY().ToString("F2") + "\n" +
				"Grad |E| dir = " + gradEmagDir(gradMag).ToString("F2") + "°";
			InspectLable->Location = Point(xs + testD / 2, ys + testD / 2);
		}

		void wallCollisionCheck() {
			for (int ix = 0; ix < Walls->getWx().size(); ++ix) {
				for (int iy = 0; iy < Walls->getWy()[ix].size(); ++iy) {
					if ((((x - testD / 2 <= Walls->getWx()[ix] + wallWidth) && (x + testD / 2 >= Walls->getWx()[ix] + wallWidth)) || ((x + testD / 2 >= Walls->getWx()[ix]) && (x - testD / 2 <= Walls->getWx()[ix]))) && (((y + testD / 2 >= Walls->getWy()[ix][iy]) && (y - testD / 2 <= Walls->getWy()[ix][iy] + wallSegHeight)))) {
						collision = true;
						return;
					}
				}
			}
		}
		void restartLevel() { //reset all relevant variables and stop simulation
			checkBox1->Checked = false;
			x = startX;
			y = startY;
			testCharge->setPos(Vec2D(x, y));
			collision = false;
			testCharge->setVx(0);
			testCharge->setVy(0);
			if (doEField || sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }

		}
		void lostLevel() { //go back to start pos, reset colision variable, and set v to 0
			checkBox1->Checked = false;
			x = startX;
			y = startY;
			testCharge->setPos(Vec2D(x, y));
			collision = false;
			testCharge->setVx(0);
			testCharge->setVy(0);
		}
		void goalReached() {
			// update the relevant dificult's variables so that the Q and mags used are stored for that level
			if (diff == Diff::easy) {
				easyPrevQused = easyQused; //update relevant charges, level and mag left
				prevQUsed = easyQused;
				easyLevel += 1;
				level = easyLevel;
				easyPrevMagUsed = easyMagUsed;
				prevMagUsed = easyMagUsed;

			}
			if (diff == Diff::medium) {
				medPrevQused = medQused;
				prevQUsed = medQused;
				medLevel += 1;
				level = medLevel;
				medPrevMagUsed = medMagUsed;
				prevMagUsed = medMagUsed;
			}
			if (diff == Diff::hard) {
				hardPrevQused = hardQused;
				prevQUsed = hardQused;
				hardLevel += 1;
				level = hardLevel;
				hardPrevMagUsed = hardMagUsed;
				prevMagUsed = hardMagUsed;
			}
			Walls->generate(wallGamePanel1->Width, wallGamePanel1->Height);
			restartLevel();

			levelUpdate();
			highScoreUpdate();
		}
		void levelUpdate() {
	
			Level->Text = "Level : " + level.ToString();
			;
		}
		void highScoreUpdate() { //depending on the dificulty, have that high score shown
			if (diff == Diff::easy) {
				HighScore->Text = "Easy High Score : " + easyHighScore.ToString(); // display the highscore of relevant diff
				if (easyLevel >= easyHighScore) {
					easyHighScore = easyLevel; //if high score needs change then update
					HighScore->Text = "Easy High Score : " + easyHighScore.ToString();
				}
			}
			if (diff == Diff::medium) {
				HighScore->Text = "Medium High Score : " + mediumHighScore.ToString();
				if (medLevel >= mediumHighScore) {
					mediumHighScore = medLevel;
					HighScore->Text = "Medium High Score : " + mediumHighScore.ToString();
				}
			}
			if (diff == Diff::hard) {
				HighScore->Text = "Hard High Score : " + hardHighScore.ToString();
				if (hardLevel >= hardHighScore) {
					hardHighScore = hardLevel;
					HighScore->Text = "Hard High Score : " + hardHighScore.ToString();
				}
			}

		}
		void applyDifficulty() {
			if (diff == Diff::easy) Walls->setHoleSize(5);
			else if (diff == Diff::medium) Walls->setHoleSize(3);
			else if (diff == Diff::hard) Walls->setHoleSize(1);
		}
		void gameScreenCollision(Charge& testQ, int screenWidth, int screenHeight) {

			if (testQ.getY() >= screenHeight - testQ.getRad()) testQ.setY(screenHeight - testQ.getRad());
			if (testQ.getX() >= screenWidth - testQ.getRad()) {
				WonGamePanel->Visible = true;
				checkBox1->Checked = false;
			};

			if (testQ.getX() <= testQ.getRad()) testQ.setX(testQ.getRad());
			if (testQ.getY() <= testQ.getRad()) testQ.setY(testQ.getRad());

		}
		void drawGame(Graphics^ g) {
			g->SmoothingMode = SmoothingMode::AntiAlias; //it improves visuals a lot
			Pen^ pen = gcnew Pen(Color::Black, 1);
			Pen^ dragPen = gcnew Pen(Color::Gray, 1);

			Pen^ vArrowPen = gcnew Pen(Color::DarkTurquoise, 2);
			Pen^ fArrowPen = gcnew Pen(FieldColor(*testqField), 2);

			int width = 30;
			Rectangle gradientRect(wallGamePanel1->Width - width - 0, 0, width, wallGamePanel1->Height); //without having a slightly larger width for rectangle, there was a green outline in the gradient
			Rectangle rect(wallGamePanel1->Width - width, 0, width, wallGamePanel1->Height);
			Brush^ targetBrush = gcnew  LinearGradientBrush(gradientRect, Color::FromArgb(0, 0, 30), Color::FromArgb(180, 100, 255, 50), LinearGradientMode::Horizontal);
			//end goal wall
			g->FillRectangle(targetBrush, rect);

			//test charge draw
			testCharge->fill(g);
			//E field draw (before stationary charges to hide arrows over them)
			if (doEField && (Q->size() > 0 || mag->size() > 0)) {
				EField->draw(g, EFMaxSize, minSize, EFScale);
			}
			//All stationary charges
			for (int i = 0; i < Q->size(); ++i) {
				double cx = (*Q)[i].getX();
				double cy = (*Q)[i].getY();
				(*Q)[i].fill(g);
			}
			//All induced Mag fields
			for (int i = 0; i < mag->size(); ++i) {
				(*mag)[i].draw(g);
			}

			//Draw force arrow if there is a force acting on test charge. THESE CONDITIONS CAN BE CHANGED TO A BUTTON CLIKC (to say if you want thea arrows to show or not)
			if (testqField->getX() != 0 || testqField->getY() != 0) {
				DrawArrow(g, x - testD / 2, y - testD / 2, totFDir(*testqField), testD, arSize(*testqField, qFarMaxSize, minSize, qFarScale), fArrowPen);
			}
			//Draw vel arrow if the test charge is moving
			if (testCharge->getVx() != 0 || testCharge->getVy() != 0) {
				DrawArrow(g, x - testD / 2, y - testD / 2, testVDir(*testCharge), testD, arSize(testCharge->getV(), qVarMaxSize, minSize, qVarScale), vArrowPen);
			}

			//Draw a circle following the mouse
			if (mode == Mode::addCharge) {
				if (wallGamePanel1->Visible) g->DrawEllipse(dragPen, (float)mouseX - Qd / 2, (float)mouseY - Qd / 2, Qd, Qd);
			}
			//draw magnetic field following mouse
			if (mode == Mode::induceMag) {
				if (wallGamePanel1->Visible) g->DrawEllipse(dragPen, (float)mouseX - (float)magDia / 2, (float)mouseY - magDia / 2, (float)magDia, (float)magDia);
				drawXinO(g, dragPen, mouseX, mouseY, magDia);
			}
			//walls
			if (doWalls) {
				Walls->draw(g);
			}

		}

		void drawSandbox(Graphics^ g) {
			g->SmoothingMode = SmoothingMode::AntiAlias;
			Pen^ pen = gcnew Pen(Color::Black, 1);
			Pen^ dragPen = gcnew Pen(Color::Gray, 2);
			Pen^ shapePen = gcnew Pen(Color::White, 3);
			Brush^ testQBrush = gcnew SolidBrush(Color::SeaGreen);

			Pen^ vArrowPen = gcnew Pen(Color::FromArgb(0, 255, 0), 1);
			Pen^ fArrowPen = gcnew Pen(Color::FromArgb(255, 0, 0), 1);
			
			//E field draw (before stationary charges to hide arrows over them)
			if (sandDoEField && (Q->size() > 0 || mag->size() > 0)) {
				EField->draw(g, EFMaxSize, minSize, EFScale);
			}
			//All stationary charges
			if (drawQandM) {
				for (int i = 0; i < Q->size(); ++i) {
					double cx = (*Q)[i].getX();
					double cy = (*Q)[i].getY();
					(*Q)[i].fill(g);
				}
				//All induced Mag fields
				for (int i = 0; i < mag->size(); ++i) {
					(*mag)[i].draw(g);
				}
			}
			//test charge station and draw
			if (sandboxDrawTestQBox) {
				g->DrawRectangle(shapePen, ElectricSandboxPanel->Width - 150, ElectricSandboxPanel->Height * 0.43, testCharge->getDiameter() + 10, testCharge->getDiameter() + 10);
				if (!activeTestQ) g->FillEllipse(testQBrush, ElectricSandboxPanel->Width - 150 + testD / 2 - 5, ElectricSandboxPanel->Height * 0.43 + testD / 2 - 5, testD, testD);
			}
			if (activeTestQ) {
				sandboxTestCharge->fill(g);
				//Draw force arrow if there is a force acting on test charge. THESE CONDITIONS CAN BE CHANGED TO A BUTTON CLIKC (to say if you want thea arrows to show or not)
				if (testqField->getX() != 0 || testqField->getY() != 0) {

					DrawArrow(g, xs - testD / 2, ys - testD / 2, totFDir(*testqField), testD, arSize(*testqField, qFarMaxSize, qfMinSize, qFarScale), fArrowPen);
				}
				//Draw vel arrow if the test charge is moving
				if (sandboxTestCharge->getVx() != 0 || sandboxTestCharge->getVy() != 0) {
					DrawArrow(g, xs - testD / 2, ys - testD / 2, testVDir(*sandboxTestCharge), testD, arSize(sandboxTestCharge->getV(), qVarMaxSize, qfMinSize, qVarScale), vArrowPen);
				}
			}



			//Draw a circle following the mouse
			if (mode == Mode::addCharge) {
				if (wallGamePanel1->Visible) g->DrawEllipse(dragPen, (float)mouseX - Qd / 2, (float)mouseY - Qd / 2, Qd, Qd);
				else if (ElectricSandboxPanel->Visible) g->DrawEllipse(dragPen, (float)mouseX - (float)sandQd / 2, (float)mouseY - sandQd / 2, (float)sandQd, (float)sandQd);
			}
			if (mode == Mode::induceMag) {
				if (ElectricSandboxPanel->Visible)  g->DrawEllipse(dragPen, (float)mouseX - (float)magDia / 2, (float)mouseY - magDia / 2, (float)magDia, (float)magDia);
				drawXinO(g, dragPen, mouseX, mouseY, magDia);
			}

			if (doEfieldLines) {
				if (colorSel == col::colorChoice) { drawElinesChoice(g, *eLine, *Q, *mag, selectedBaseColor); }
				if (colorSel == col::standardFromE) { drawElinesStandard(g, *eLine, *Q, *mag); }
			}
		}
		bool negQPresent(vector<Charge> Q) {
			for (int i = 0; i < Q.size(); ++i) {
				if (Q[i].getQ() < 0) { return true; }
				else continue;
			}
			return false;
		}

		void ShowPanel(Panel^ target) { //easy way to swithc panels
			homePanel->Visible = false;
			wallGamePanel1->Visible = false;
			ElectricSandboxPanel->Visible = false;

			target->Visible = true;
			target->BringToFront();
		}

	private: System::Void FormAPMFinal_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}

	private: void Tick(Object^ sender, EventArgs^ e) {
		double dt = 0.05;
		if (wallGamePanel1->Visible) {
			if (level > 1) button4->Visible = false; //only able to change walls on level1
			if (level == 1) button4->Visible = true;
			QleftLableUpdate();
			MagLeftLableUpdate();
			if (Q->size() > 0 || mag->size() > 0) { *testqField = Vec2D(totEat(*Q, *mag, testCharge->getX(), testCharge->getY())); }
			else *testqField = Vec2D(0, 0);

			if (play) {
				simulateTestQCord(*Q, *mag, *testCharge, dt);
				x = testCharge->getX();
				y = testCharge->getY();
				gameScreenCollision(*testCharge, wallGamePanel1->Width, wallGamePanel1->Height);
				wallCollisionCheck();
				if (collision) { lostLevel(); }
				dragQ = false;
				button4->Visible = false;
				
			}

		}
		if (ElectricSandboxPanel->Visible && activeTestQ) { *testqField = Vec2D(totEat(*Q, *mag, sandboxTestCharge->getX(), sandboxTestCharge->getY())); }
		else if (!wallGamePanel1->Visible && !ElectricSandbox->Visible)*testqField = Vec2D(0, 0);
		//drag
		if (mode == Mode::drag) {
			if (sandDrag || drag) {
				if (dragQ) { (*Q)[dragIndex].setPos(Vec2D(mouseX, mouseY)); }
				if (dragMag) { (*mag)[dragIndex].setPos(Vec2D(mouseX, mouseY)); }
				if (dragTestQ) {
					xs = mouseX; //for arrow purposes
					ys = mouseY;
					sandboxTestCharge->setPos(Vec2D(mouseX, mouseY));
				}
				//also drag the edit charges panel with charge and update it
				if (edit) {
					if (EditTestChargePanel->Visible) {
						EditTestChargePanel->Location = Point(sandboxTestCharge->getX() + sandboxTestCharge->getRad(), sandboxTestCharge->getY()); // fill in the relevant values for the numeric up downs and texboxes
						TestQXPosTextbox->Text = sandboxTestCharge->getX().ToString();
						TestQYPosTextbox->Text = switchCoords(sandboxTestCharge->getY(), ElectricSandboxPanel->Height).ToString();
					}
					if (editChargePanel->Visible) {
						editChargePanel->Location = Point((*Q)[editIndex].getX() + (*Q)[editIndex].getRad(), (*Q)[editIndex].getY());

						XPosTextBox->Text = (*Q)[editIndex].getX().ToString();
						YPosTextBox->Text = switchCoords((*Q)[editIndex].getY(), ElectricSandboxPanel->Height).ToString();
					}
				}
			}
		}
		//simulation of charges
		if (simulateAllQ) {
			if (sim == sandboxSim::simulateQ) {
				simulateAllQcord(*Q, *mag, dt);
				allQscreenCollision(*Q, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height);
				if (sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
			}
			if (sim == sandboxSim::simulateTestQ) {
				if (!activeTestQ) ActivateTesQWarning->Visible = true; //only simulate testQ if its activated
				else {
					ActivateTesQWarning->Visible = false;
					simulateTestQCord(*Q, *mag, *sandboxTestCharge, dt);
					testQScreenCollision(*sandboxTestCharge, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height);
					xs = sandboxTestCharge->getX();
					ys = sandboxTestCharge->getY();
					if (sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); } //always do update the e field when stuff is happening, seen throught the code
				}
			}
			if (sim == sandboxSim::simulateALL) {
				simulateAllQcord(*Q, *mag, dt);
				allQscreenCollision(*Q, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height);

				if (activeTestQ) { //only update if activated
					xs = sandboxTestCharge->getX();
					ys = sandboxTestCharge->getY();
					simulateTestQCord(*Q, *mag, *sandboxTestCharge, dt);
					testQScreenCollision(*sandboxTestCharge, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height);
				}
				if (sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
			}
		}
		if (sim == sandboxSim::simulateTestQ && !simulateAllQ || (sim != sandboxSim::simulateTestQ && ((inspect && activeTestQ) || (!inspect)))) ActivateTesQWarning->Visible = false;
		//E Field lines generation
		if (doEfieldLines) {
			if (generation == gen::RadialGen) {
				++tickCount;
				if (tickCount >= maxTick) {
					*qCenters = getQCenters(*Q);
					*EfatQ = getEatQ(*Q);
					generateLines2(*eLine, *qCenters, fieldLgenerated, maxLife, maxLength, minLength, *Q);
					tickCount = 0;
					DebugLable->Text = "lines alive = " + eLine->size().ToString();
					if (negQPresent(*Q)) { lineAtEdgeGen(*eLine, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height, lineHspacing, lineVspacing, maxLife, minLength); }
					drawEfieldLines = true;
				}
			}
			if (generation == gen::RandomCharge) {
				++tickCount;
				if (tickCount >= maxTick) {
					*qCenters = getQCenters(*Q);
					*EfatQ = getEatQ(*Q);
					++genCycle;
					RotgenerateLines2(*eLine, *qCenters, *EfatQ, fieldLgenerated, maxLife, maxLength, minLength, *Q, genCycle);
					tickCount = 0;
	
					drawEfieldLines = true;
				}
			}
			if (generation == gen::GridGen) {
				++tickCount;
				if (tickCount >= maxTick) {
					niceFieldGen(*eLine, *Q, *mag, lineFieldColl, lineFieldRow, lineHspacing, lineVspacing, maxLife, minLength, genCycle);
					++genCycle;
					tickCount = 0;
					if (genCycle >= 5) genCycle = 1; //restart cycle
					drawEfieldLines = true;
				}
			}
			if (generation == gen::RandomField) {
				randFieldGen(*eLine, *Q, *mag, fieldLgenerated, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height, maxLife);
				drawEfieldLines = true;
			}

		}

		//E field lines update
		if (drawEfieldLines) {
			DebugLable->Text = "lines alive = " + eLine->size().ToString();
			fieldLineUpdateGridVersion(*eLine, *Q, *mag, sandQd, baseSpeed, dt, maxLength, minLength, maxLife, dragQ, dragMag);
			updateLineLife(*eLine, *Q, *mag, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height);
		}
		// Arrow field update
		if ((doEField || sandDoEField) && mode == Mode::drag) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
		if (ElectricSandboxPanel->Visible)
			ElectricSandboxPanel->Invalidate();
		else if (wallGamePanel1->Visible)
			wallGamePanel1->Invalidate();
		//inspect
		if (inspect && activeTestQ) {
			ActivateTesQWarning->Visible = false;
			InspectLable->Visible = true;
			inspectLable();
		}
		else {
			InspectLable->Visible = false;
		}
	}
		   //play checkbox
	private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		play = checkBox1->Checked;
		if (checkBox1->Checked == true) {
			button4->Visible = false; // walls
			button1->Visible = false; //ad charges
			button3->Visible = false; //delete charges
			checkBox2->Visible = false; //easy
			checkBox3->Visible = false;// med
			checkBox4->Visible = false;//hard
			ResetCharges->Visible = false;
			ReversePolarity->Visible = false;
			chargesLeftLabel->Visible = false;
			dragCheckBox->Visible = false;
			drag = false; //not allowed to drag when in play
			mode = Mode::none;
			GameInducMagB->Visible = false;
			MagnetsLeftLable->Visible = false;
			noneButton->Visible = false;
		}
		else {

			button4->Visible = true; // walls
			button1->Visible = true; //ad charges
			button3->Visible = true; //delete charges
			checkBox2->Visible = true; //easy
			checkBox3->Visible = true;// med
			checkBox4->Visible = true;//hard
			ResetCharges->Visible = true;
			ReversePolarity->Visible = true;
			chargesLeftLabel->Visible = true;
			dragCheckBox->Visible = true;
			mode = prevMode;
			drag = dragCheckBox->Checked; //back to whatever setting you had
			GameInducMagB->Visible = true;
			MagnetsLeftLable->Visible = true;
			noneButton->Visible = true;

		}

	}
		   void OnMouseMove(Object^ sender, MouseEventArgs^ e) {
			   mouseX = e->X;
			   mouseY = e->Y;
		   }

		   void MClick(Object^ sender, MouseEventArgs^ e) {
			   double mx = e->X;
			   double my = e->Y;

			   if (edit) {
				   for (int i = Q->size() - 1; i >= 0; --i) {
					   if (inCircle((*Q)[i].getX(), (*Q)[i].getY(), (*Q)[i].getRad(), mx, my) && mode != Mode::deleteCharges) { //do not change index to charge that is being deleted
						   //edit normal charges
						   editIndex = i; //to use it elsewere
						   editChargePanel->Location = Point((*Q)[i].getX() + (*Q)[i].getRad(), (*Q)[i].getY()); //set loc
						   ChargeNumberLable->Text = "Charge " + (editIndex + 1).ToString();
						   XPosTextBox->Text = (*Q)[i].getX().ToString();// fill in the valeus with those of the charge
						   YPosTextBox->Text = switchCoords((*Q)[i].getY(), ElectricSandboxPanel->Height).ToString();
						   ChargeNumericUpDown->Value = (Decimal)(*Q)[i].getQ();
						   MassNumericUpDown->Value = (Decimal)(*Q)[i].getM();
						   editChargePanel->Visible = true;
						   ChargeNumericUpDown->Enabled = true; //do not edit test charge values, only pos
						   MassNumericUpDown->Enabled = true;


					   }
				   }//eddit test charrge
				   if (inCircle(sandboxTestCharge->getX(), sandboxTestCharge->getY(), sandboxTestCharge->getRad(), mx, my)) {
					   
					   EditTestChargePanel->Visible = true;
					   EditTestChargePanel->Location = Point(sandboxTestCharge->getX() + sandboxTestCharge->getRad(), sandboxTestCharge->getY()); //set loc
					   TestQXPosTextbox->Text = sandboxTestCharge->getX().ToString();// fill in the valeus with those of the charge
					   TestQYPosTextbox->Text = switchCoords(sandboxTestCharge->getY(), ElectricSandboxPanel->Height).ToString();
					   TestQChargeNumericUpDown->Value = (Decimal)sandboxTestCharge->getQ();
					   TestQMassNumericUpDown->Value = (Decimal)sandboxTestCharge->getM();


				   }
			   }
			   if (mode == Mode::addCharge) {
				   if (ElectricSandboxPanel->Visible) { Q->push_back(Charge(initialQ, 1, sandQd, Vec2D(mx, my), Vec2D(0, 0), Q->size() - 1)); } // simply push back a charge with index appropiate for vector
				   else if (wallGamePanel1->Visible && Qleft > 0) { Q->push_back(Charge(initialQ, 1, Qd, Vec2D(mx, my), Vec2D(0, 0), Q->size() - 1)); }
				   if (doEField || sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
			   }
			   if (mode == Mode::induceMag) {
				   if (ElectricSandboxPanel->Visible) { mag->push_back(inducMag(dbdt, Vec2D(mouseX, mouseY), magDia, in, mag->size() - 1)); } //simply push back a mag
				   if (wallGamePanel1->Visible && Magleft > 0) {
					   mag->push_back(inducMag(dbdt, Vec2D(mouseX, mouseY), magDia, in, mag->size() - 1));
				   }
				   if (doEField || sandDoEField) EField->updateEfield(*Q, *mag);
			   }
			   if (mode == Mode::deleteCharges) {
				   //for charges
				   for (int i = Q->size() - 1; i >= 0; --i) {
					   if (inCircle((*Q)[i].getX(), (*Q)[i].getY(), (*Q)[i].getRad(), mx, my)) { //Check if mouse is in diameter of that charge
						   Q->erase(Q->begin() + i);
						   if (doEField || sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }


						   if (editIndex == i) {//so if you deleted that charge you were working with, it desapears (otherwise if edit values crahses)
							   editChargePanel->Visible = false;
							   editIndex = 0; //reset editINdex, otherwise might crash if accessing index outside of scope
						   }
						   else if (editIndex > i) {
							   editIndex--; //since when deleting the indices go one to the left
						   }
						   break;

					   }

				   }
				   //for magnets
				   for (int i = mag->size() - 1; i >= 0; --i) {
					   if (inCircle((*mag)[i].getX(), (*mag)[i].getY(), (*mag)[i].getRad(), mx, my)) {
						   mag->erase(mag->begin() + i);
						   if (doEField || sandDoEField) { EField->updateEfield(*Q, *mag); }
					   }
				   }
			   }


			   if (wallGamePanel1->Visible) wallGamePanel1->Invalidate();
			   if (ElectricSandboxPanel->Visible) ElectricSandboxPanel->Invalidate();
		   }

		   void Mdown(Object^ sender, MouseEventArgs^ e) {
			   double mx = e->X;
			   double my = e->Y;
			   //first revrse polarity then drag cuz otherwise you just drag since mode gets set to drag
			   if (mode == Mode::reversePolarity) {
				   for (int i = Q->size() - 1; i >= 0; --i) {
					   if (inCircle((*Q)[i].getX(), (*Q)[i].getY(), (*Q)[i].getRad(), mx, my)) { //if mose on top of charge
						   (*Q)[i].setQ((*Q)[i].getQ() * -1); //multiplying the charge's q by -1
					   }
					   if (doEField || sandDoEField) EField->updateEfield(*Q, *mag);
				   }
				   for (int i = mag->size() - 1; i >= 0; --i) {
					   if (inCircle((*mag)[i].getX(), (*mag)[i].getY(), (*mag)[i].getRad(), mx, my)) { // if mouse on top of mag
						   (*mag)[i].inverse();
					   }
					   if (doEField || sandDoEField) EField->updateEfield(*Q, *mag);
				   }
			   }
			   if (drag || sandDrag) {
				   if (prevMode == Mode::deleteCharges) return;
				   for (int i = Q->size() - 1; i >= 0; --i) { //drag Charges
					   if (inCircle((*Q)[i].getX(), (*Q)[i].getY(), (*Q)[i].getRad(), mx, my)) { //if mouse on top of charge
						   mode = Mode::drag; //now actually drag
						   dragIndex = i; //to know which charge to drag
						   dragQ = true;
						   dragMag = false; // so that you dont also drag that index of mag
						   dragTestQ = false;
					   }
				   }//drag mags
				   for (int i = mag->size() - 1; i >= 0; --i) {
					   if (inCircle((*mag)[i].getX(), (*mag)[i].getY(), (*mag)[i].getRad(), mx, my)) {
						   mode = Mode::drag; //now actually drag
						   dragIndex = i; //to know which charge to drag
						   dragMag = true;
						   dragQ = false;
						   dragTestQ = false;
					   }
				   }//drag test Q
				   if (inCircle(sandboxTestCharge->getX(), sandboxTestCharge->getY(), sandboxTestCharge->getRad(), mx, my) && activeTestQ) {
					   mode = Mode::drag; //now actually drag
					   dragIndex = -1; //so that you dont drag another charge accidently, testcharge index doesnt matter
					   dragMag = false;
					   dragQ = false;
					   dragTestQ = true;
					   sandboxTestCharge->setVx(0);
					   sandboxTestCharge->setVy(0); //reset the velocity if dragged
				   }
			   }


		   }
		   void Mup(Object^ sender, MouseEventArgs^ e) {
			   if (mode == Mode::drag && play == false) mode = prevMode; //to stop draging charge when releasing mouse and back to what you where doing. Also, the play = false is so that you cant do anything while play simulation is on (otherwise if you click on charge mode = prevMode)
			   dragQ = false;
			   dragMag = false;
		   }
		   //add charges button
	private: System::Void button1_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) { //Add acharges button
		mode = Mode::addCharge;
		prevMode = Mode::addCharge;
	}
		   //delete charges button
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) { //Delete Charges button
		mode = Mode::deleteCharges;
		prevMode = Mode::deleteCharges;
	}
		   //do walls button
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) { //Walls
		// FIRST set difficulty
		applyDifficulty();
		Walls->generate(wallGamePanel1->Width, wallGamePanel1->Height);
		doWalls = true;
	}
		   //diffs
	private: System::Void checkBox2_CheckedChanged(System::Object^ sender, System::EventArgs^ e) { //easy
		if (checkBox2->Checked) {
			checkBox3->Checked = false; //only one dificulty at a time
			checkBox4->Checked = false;
			diff = Diff::easy;
			level = easyLevel;
			levelUpdate();
			highScoreUpdate(); // change the high score to relevant dificulty
			applyDifficulty();
			Walls->generate(gameWidth, gameHeight); //re draw walls when chanigng dificulty
			doWalls = true;
		}
	} //easy

	private: System::Void checkBox3_CheckedChanged(System::Object^ sender, System::EventArgs^ e) { //medium
		if (checkBox3->Checked) {
			checkBox2->Checked = false;
			checkBox4->Checked = false;
			diff = Diff::medium;
			level = medLevel;
			levelUpdate();
			highScoreUpdate();
			
			applyDifficulty();
			Walls->generate(gameWidth, gameHeight);
			doWalls = true;
		}
	} //med
	private: System::Void checkBox4_CheckedChanged(System::Object^ sender, System::EventArgs^ e) { //hard
		if (checkBox4->Checked) {
			checkBox2->Checked = false;
			checkBox3->Checked = false;
			diff = Diff::hard;
			level = hardLevel;
			levelUpdate();
			highScoreUpdate();
			applyDifficulty();
			Walls->generate(gameWidth, gameHeight);
			doWalls = true;
		}
	}
	private: System::Void ResetDifficultyB_Click(System::Object^ sender, System::EventArgs^ e) {
		//reset Charges and mags used (hence left), and the level.
		if (diff == Diff::easy) {
			easyQused = 0;
			easyPrevQused = 0;
			prevQUsed = 0;
			easyLevel = 1;
			level = 1;
			easyMagUsed = 0;
			easyPrevMagUsed = 0;
			MagUsed = 0;
		}
		if (diff == Diff::medium) {
			medQused = 0;
			medPrevQused = 0;
			prevQUsed = 0;
			medLevel = 1;
			level = 1;
			medMagUsed = 0;
			medPrevMagUsed = 0;
			MagUsed = 0;
		}
		if (diff == Diff::hard) {
			hardQused = 0;
			hardPrevQused = 0;
			prevQUsed = 0;
			hardLevel = 1;
			level = 1;
			hardMagUsed = 0;
			hardPrevMagUsed = 0;
			MagUsed = 0;
		}
		levelUpdate();
		
		Walls->generate(wallGamePanel1->Width, wallGamePanel1->Height);
	}
	private: System::Void ResetCharges_Click(System::Object^ sender, System::EventArgs^ e) {
		deleteQ();
		deleteMag(*mag);
		if (doEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
	}
	private: System::Void TempGoal_Click(System::Object^ sender, System::EventArgs^ e) {
		goalReached();
	}
	private: System::Void EVectorField_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		doEField = EVectorField->Checked;
		if (EVectorField->Checked) { EField->generate(*Q, *mag, wallGamePanel1->Width, wallGamePanel1->Height); }
		if (EVectorField->Checked == false) { EField->deleteEF(); }
	}
	private: System::Void ReversePolarity_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::reversePolarity;
		prevMode = Mode::reversePolarity;
	}
	private: System::Void ResetLevel_Click(System::Object^ sender, System::EventArgs^ e) {
		restartLevel();
	}
	private: System::Void playWalllGame_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPanel(wallGamePanel1);
		timer->Start();
		mode = Mode::none;
		prevMode = Mode::none;
		// FIRST set difficulty
		applyDifficulty();
		Walls->generate(wallGamePanel1->Width, wallGamePanel1->Height);
		doWalls = true;
		EVectorField->Checked = false;
		doEField = false;
		dragCheckBox->Checked = true;
		drag = true;
		sandDrag = false;
	}
	private: System::Void wallGamePanel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;
		drawGame(g);
	}
	private: System::Void ExitWallGame_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPanel(homePanel);
		level = 1;
		Qleft = maxQN;
		Magleft = maxMag;
		prevQUsed = 0;
		dragCheckBox->Checked = false;
		deleteQ();
		deleteMag(*mag);
		levelUpdate();
	}
	private: System::Void dragCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		drag = dragCheckBox->Checked;
	}
	private: System::Void GameInducMagB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::induceMag;
		prevMode = Mode::induceMag;
	}
	private: System::Void noneButton_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::none;
		prevMode = Mode::none;
	}
		   //Sandbox----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	private: System::Void ElectricSandbox_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPanel(ElectricSandboxPanel);
		mode = Mode::none;
		prevMode = Mode::none;
		timer->Start();
		drag = false;
		sandDrag = true;
	}
	private: System::Void SandboxAddChargeB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::addCharge;
		prevMode = Mode::addCharge;
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void SandboxDeleteChargesB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::deleteCharges;
		prevMode = Mode::deleteCharges;
		ElectricSandboxPanel->Invalidate();
	}

	private: System::Void SandboxReversePolB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::reversePolarity;
		prevMode = Mode::reversePolarity;
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void SandboxResetChargesB_Click(System::Object^ sender, System::EventArgs^ e) {
		deleteQ();
		deleteMag(*mag);
		if (sandDoEField) { EField->updateEfield(*Q, *mag); *qCenters = getQCenters(*Q); }
		SandboxSimulateCB->Checked = false; //it was crashing if while you simulate you reset charges, and then add a charge
		sandStartX = ElectricSandboxPanel->Width - 150 + testD / 2 + 5; //update startx and y if the panel width and height changed
		sandStartY = ElectricSandboxPanel->Height * 0.45 + testD / 2 + 5;
		sandboxTestCharge->setPos(Vec2D(sandStartX, sandStartY)); //reset testQ as well
		xs = sandStartX;
		ys = sandStartY;
		sandboxTestCharge->setVx(0);
		sandboxTestCharge->setVy(0);
		editChargePanel->Visible = false;
	}
	private: System::Void SandboxNoneB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::none;
		prevMode = Mode::none;
		ElectricSandboxPanel->Invalidate(); //it was not invalidating for some reaoson
	}
	private: System::Void SandboxSimulateCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		simulateAllQ = SandboxSimulateCB->Checked;
		if (simulateAllQ && sim == sandboxSim::none) {
			SimulateWarningLable->Visible = true;
		}
		else SimulateWarningLable->Visible = false;
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void SandboxDragCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		sandDrag = SandboxDragCB->Checked;
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void SandboxInspectCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		inspect = SandboxInspectCB->Checked;
		if (!activeTestQ) {
			ActivateTesQWarning->Visible = true;
		}
		if (!inspect) { ActivateTesQWarning->Visible = false; }
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void SandboxVecFieldCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		sandDoEField = SandboxVecFieldCB->Checked;
		if (SandboxVecFieldCB->Checked) { EField->generate(*Q, *mag, ElectricSandboxPanel->Width, ElectricSandboxPanel->Height); }
		if (SandboxVecFieldCB->Checked == false) { EField->deleteEF(); }
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void ElectricSandboxPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		Graphics^ g = e->Graphics;
		drawSandbox(g);

	}
	private: System::Void ExitESandboxB_Click(System::Object^ sender, System::EventArgs^ e) {
		ShowPanel(homePanel);

		SandboxDragCB->Checked = false;
		EFieldLinesCB->Checked = false;
		SandboxVecFieldCB->Checked = false;
		deleteQ();
		deleteMag(*mag);
		mode = Mode::none;
		prevMode = Mode::none;
		sandDoEField = false;
	}
	private: System::Void EFieldLinesCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		doEfieldLines = EFieldLinesCB->Checked;
		DebugLable->Visible = EFieldLinesCB->Checked;
		if (EFieldLinesCB->Checked == false) {
			eLine->clear();
		}
		
	}
	private: System::Void SandboxInduceMagB_Click(System::Object^ sender, System::EventArgs^ e) {
		mode = Mode::induceMag;
		prevMode = Mode::induceMag;
	}
	private: System::Void EFieldLinesGen_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if ("Random Field" == EFieldLinesGen->SelectedItem->ToString()) { // setting relevant life, generation mode, ticks, and lines generated per relevant tick
			generation = gen::RandomField;
			maxLife = 100;
			fieldLgenerated = 3;
			
		}
		if ("Grid Generation" == EFieldLinesGen->SelectedItem->ToString()) {
			generation = gen::GridGen;
			maxLife = 100;
			tickCount = 20;
			maxTick = 50;
		
		}
		if ("Random from Charge" == EFieldLinesGen->SelectedItem->ToString()) {
			generation = gen::RandomCharge;
			maxLife = 10000;
			tickCount = 0;
			maxTick = 5;
			fieldLgenerated = 3;
			
		}
		if ("Radial Generation" == EFieldLinesGen->SelectedItem->ToString()) {
			generation = gen::RadialGen;
			maxLife = 10000;
			tickCount = 50;
			maxTick = 100;
			fieldLgenerated = 50;
			
		}
	}
	private: System::Void DrawQM_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		drawQandM = DrawQM->Checked;
	}
	private: System::Void TestChargeActivate_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		activeTestQ = TestChargeActivate->Checked;
		if (!activeTestQ) {

			sandboxTestCharge->setPos(Vec2D(sandStartX, sandStartY));
			xs = sandStartX;
			ys = sandStartY;
			sandboxTestCharge->setVx(0); //so that when reactivated it resets its vel
			sandboxTestCharge->setVy(0);
		}
		if (TestChargeActivate->Checked) { //so that if you activated it mid simulation id starts with v = 0 not with what it acumulated silently
			sandStartX = ElectricSandboxPanel->Width - 150 + testD / 2 + 5; // so that if in different screen sizes (computers), it rescales appropiately
			sandStartY = ElectricSandboxPanel->Height * 0.43 + testD / 2 + 5; //these are the same operations that are in drawSandbox but with a + since Q cords are from its center not the top left corner
			sandboxTestCharge->setPos(Vec2D(sandStartX, sandStartY));
			xs = sandStartX;
			ys = sandStartY;
			sandboxTestCharge->setVx(0); //so that when reactivated it resets its vel
			sandboxTestCharge->setVy(0);
		}

	}
	private: System::Void SimulateComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if ("Simulate Test Charge" == SimulateComboBox->Text) {
			sim = sandboxSim::simulateTestQ;
		}
		if ("Simulate Charges" == SimulateComboBox->Text) {

			sim = sandboxSim::simulateQ;
		}
		if ("Simulate ALL" == SimulateComboBox->Text) {

			sim = sandboxSim::simulateALL;
		}
	}

	private: System::Void ELinesColorSelBn_Click(System::Object^ sender, System::EventArgs^ e) {
		LinesColorDialog->Color = selectedBaseColor;

		if (LinesColorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			selectedBaseColor = LinesColorDialog->Color;
			panelColorPreview->BackColor = selectedBaseColor;


			//redraw
			this->Invalidate();
		}
	}
	private: System::Void VecFieldColor_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if ("Physics Intensity" == VecFieldColor->Text) {
			colorSel = col::standardFromE;
			ELinesColorSelBn->Visible = false;
			EColorLabel->Visible = false;
			panelColorPreview->Visible = false; //only show these when the color selection is chosen
		}
		if ("Color Choice" == VecFieldColor->Text) {
			colorSel = col::colorChoice;
			ELinesColorSelBn->Visible = true;
			EColorLabel->Visible = true;
			panelColorPreview->BackColor = selectedBaseColor;
			panelColorPreview->Visible = true;
		}
	}
	private: System::Void HideAndShowB_Click(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void HideAndShowCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		bool hide = HideAndShowCB->Checked;

		if (hide) {
			//TestQ box
			sandboxDrawTestQBox = false;
			//ELines
			ELinesColorSelBn->Visible = false;
			EColorLabel->Visible = false;
			panelColorPreview->Visible = false;
			EFieldLinesCB->Visible = false;
			EFieldLinesGen->Visible = false;
			VecFieldColor->Visible = false;
			DebugLable->Visible = false;
			//Simulate
			SimulateComboBox->Visible = false;
			SandboxSimulateCB->Visible = false;
			//TestCharge
			TestChargeActivate->Visible = false;
			InspectLable->Visible = false; //maybeno
			ActivateTesQWarning->Visible = false; //maybe no
			SandboxInspectCB->Visible = false;
			//Vector field
			SandboxVecFieldCB->Visible = false;
			//Draw and drag Objects
			DrawQM->Visible = false;
			SandboxDragCB->Visible = false;
			//Controls
			SandboxInduceMagB->Visible = false;
			SandboxAddChargeB->Visible = false;
			SandboxReversePolB->Visible = false;
			SandboxResetChargesB->Visible = false;
			SandboxDeleteChargesB->Visible = false;
			SandboxNoneB->Visible = false;
			//Exit
			ExitESandboxB->Visible = false;
			EditChargesCB->Visible = false;
		}
		else if (!hide) {
			//TestQ box
			sandboxDrawTestQBox = true;
			//ELines
			if (colorSel == col::colorChoice) {
				ELinesColorSelBn->Visible = true;
				EColorLabel->Visible = true;
				panelColorPreview->Visible = true;
			}
			EFieldLinesCB->Visible = true;
			EFieldLinesGen->Visible = true;
			VecFieldColor->Visible = true;
			DebugLable->Visible = EFieldLinesCB->Checked;
			//Simulate
			SimulateComboBox->Visible = true;
			SandboxSimulateCB->Visible = true;
			//TestCharge
			TestChargeActivate->Visible = true;
			//	InspectLable->Visible = true; //maybeno
			//	ActivateTesQWarning->Visible = true; //maybe no
			SandboxInspectCB->Visible = true;
			//Vector field
			SandboxVecFieldCB->Visible = true;
			//Draw and drag Objects
			DrawQM->Visible = true;
			SandboxDragCB->Visible = true;
			//Controls
			SandboxInduceMagB->Visible = true;
			SandboxAddChargeB->Visible = true;
			SandboxReversePolB->Visible = true;
			SandboxResetChargesB->Visible = true;
			SandboxDeleteChargesB->Visible = true;
			SandboxNoneB->Visible = true;
			EditChargesCB->Visible = true;
			//Exit
			ExitESandboxB->Visible = true;
		}

	}

	private: System::Void ChargeNumericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		(*Q)[editIndex].setQ((double)ChargeNumericUpDown->Value); //setting the value of the num up down to the Charge
		(*Q)[editIndex].diameterFromCharge();
		if (sandDoEField) EField->updateEfield(*Q, *mag); //update the arrow field if drawn
		ElectricSandboxPanel->Invalidate(); //update screen
	}
	private: System::Void MassNumericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		(*Q)[editIndex].setM((double)MassNumericUpDown->Value);
		if (sandDoEField) EField->updateEfield(*Q, *mag);
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void EditChargesCB_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		edit = EditChargesCB->Checked;
		if (EditChargesCB->Checked == false) {
			editChargePanel->Visible = false;
			EditTestChargePanel->Visible = false;
		}

	}

	private: System::Void ExitEditChargePanelB_Click(System::Object^ sender, System::EventArgs^ e) {
		editChargePanel->Visible = false;
	}
	private: System::Void ApplyPosB_Click(System::Object^ sender, System::EventArgs^ e) {
		double setx, sety;

		bool okX = Double::TryParse(XPosTextBox->Text, setx); //this function returns true or false if the left variable can be converted to the one on the right and if true it sets the vallue of the left to the right
		bool okY = Double::TryParse(YPosTextBox->Text, sety);

		if (okX && okY) {
			//so that if the user inserts a value outside the screen, it gets clamped
			if (setx > ElectricSandboxPanel->Width) setx = ElectricSandboxPanel->Width;
			if (setx < 0) setx = 0;
			if (sety > ElectricSandboxPanel->Height) sety = ElectricSandboxPanel->Height;
			if (sety < 0) sety = 0;

			(*Q)[editIndex].setPos(Vec2D(setx, switchCoords(sety, ElectricSandboxPanel->Height)));
			editChargePanel->Location = Point((*Q)[editIndex].getX() + (*Q)[editIndex].getRad(), (*Q)[editIndex].getY()); //Move panel with Q


			if (sandDoEField) EField->updateEfield(*Q, *mag);
			ElectricSandboxPanel->Invalidate();
		}
		else {
			MessageBox::Show("Invalid coordinates.");
		}
	}
		   //Test Charge edit Panel
	private: System::Void TesQXB_Click(System::Object^ sender, System::EventArgs^ e) {
		EditTestChargePanel->Visible = false;
	}

	private: System::Void ApplyPosTestQB_Click(System::Object^ sender, System::EventArgs^ e) { //same logic as this botton for charges edit panel
		double setx, sety;

		bool okX = Double::TryParse(TestQXPosTextbox->Text, setx);
		bool okY = Double::TryParse(TestQYPosTextbox->Text, sety);

		if (okX && okY) {

			if (setx > ElectricSandboxPanel->Width) setx = ElectricSandboxPanel->Width;
			if (setx < 0) setx = 0;
			if (sety > ElectricSandboxPanel->Height) sety = ElectricSandboxPanel->Height;
			if (sety < 0) sety = 0;
			sety = switchCoords(sety, ElectricSandboxPanel->Height);
			sandboxTestCharge->setPos(Vec2D(setx, sety));
			xs = setx;//so arrow moves too
			ys = sety;
			EditTestChargePanel->Location = Point(sandboxTestCharge->getX() + sandboxTestCharge->getRad(), sandboxTestCharge->getY());
		}
		else {
			MessageBox::Show("Invalid coordinates.");
		}
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void TestQChargeNumericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		sandboxTestCharge->setQ((double)TestQChargeNumericUpDown->Value);
		if (sandDoEField) EField->updateEfield(*Q, *mag);
		ElectricSandboxPanel->Invalidate();
	}
	private: System::Void TestQMassNumericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		sandboxTestCharge->setM((double)TestQMassNumericUpDown->Value);
		if (sandDoEField) EField->updateEfield(*Q, *mag);
		ElectricSandboxPanel->Invalidate();
	}

 private: System::Void RewatchB_Click(System::Object^ sender, System::EventArgs^ e) {
			   restartLevel();
			   WonGamePanel->Visible = false;
		   }
private: System::Void NextLevelB_Click(System::Object^ sender, System::EventArgs^ e) {
	goalReached();
	WonGamePanel->Visible = false;
}
	};
}



