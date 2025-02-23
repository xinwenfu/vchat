#include "VChatGUI.h"
#include "ServerManager.h"
#include <stdio.h>
#include <strsafe.h>

using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]
void main(array<String^>^ args) {
	Application::SetCompatibleTextRenderingDefault(false);
	Application::EnableVisualStyles();
	VChatGUI::VChatGUI frm;
	Application::Run(% frm);
}
