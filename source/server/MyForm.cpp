#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThread]

void main(array<String^>^ args)
{
	Server server;

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	sensorHoldingsExerciseServer::MyForm form(&server);
	Application::Run(%form);
}

