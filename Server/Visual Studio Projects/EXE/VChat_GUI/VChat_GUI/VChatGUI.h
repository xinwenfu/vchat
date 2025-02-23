#pragma once
#include <iostream>
#include <process.h>
#include <exception>
#include <msclr/marshal_cppstd.h>
#include "ServerManager.h"

// Define Buffer Size
#define BUFFSIZE 4096

namespace VChatGUI {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for VChatGUI
	/// </summary>
	public ref class VChatGUI : public System::Windows::Forms::Form
	{
		 
	public:
		VChatGUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			srv_prompt = gcnew System::String("VChat Server: ");
			gui_prompt = gcnew System::String("VChat GUI ");
			u_port = gcnew System::String("NULL");
			this->serv_h = (server_manage::ServerManager*)new server_manage::ServerManager("9999");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~VChatGUI()
		{
			if (components)
			{
				delete components;
			}
			
			// If the server is running we need to kill it
			if (this->serv_h != nullptr && this->serv_h->get_isStarted() == 1) 
				this->serv_h->killServer();
			
			// Clean up allocated resources
			delete this->serv_h;
			this->serv_h = nullptr; // Not necsissary but paranoia is good sometimes. In this case unlikely.
		}

	private: server_manage::ServerManager *serv_h; // Pointer to obj because it is unmanaged
	private: System::Threading::Thread^ t_handle; // Managaed Thread Object
	private: System::Threading::Mutex^ t_mutex; // Managaed Mutex Object Object
	private: System::Windows::Forms::Button^ Start_Button;
	private: System::String^ srv_prompt;
	private: System::String^ gui_prompt;
	private: System::String^ u_port;


	private: System::Windows::Forms::TextBox^ VChatOut;
	private: System::Windows::Forms::Button^ Stop_Button;
	private: System::Windows::Forms::Label^ InputPortLabel;
	private: System::Windows::Forms::TextBox^ VChatPort;
	private: System::Windows::Forms::Label^ ExeLabel;

	private: System::Windows::Forms::TextBox^ VChatPath;

	private: System::Windows::Forms::Label^ PortLabel;


	private: System::Windows::Forms::TextBox^ StatusBox;
	private: System::Windows::Forms::TextBox^ PortBox;
	private: System::Windows::Forms::Button^ button1;










	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Start_Button = (gcnew System::Windows::Forms::Button());
			this->VChatOut = (gcnew System::Windows::Forms::TextBox());
			this->Stop_Button = (gcnew System::Windows::Forms::Button());
			this->InputPortLabel = (gcnew System::Windows::Forms::Label());
			this->VChatPort = (gcnew System::Windows::Forms::TextBox());
			this->ExeLabel = (gcnew System::Windows::Forms::Label());
			this->VChatPath = (gcnew System::Windows::Forms::TextBox());
			this->PortLabel = (gcnew System::Windows::Forms::Label());
			this->StatusBox = (gcnew System::Windows::Forms::TextBox());
			this->PortBox = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// Start_Button
			// 
			this->Start_Button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->Start_Button->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Start_Button->BackColor = System::Drawing::Color::Blue;
			this->Start_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Start_Button->ForeColor = System::Drawing::Color::White;
			this->Start_Button->Location = System::Drawing::Point(831, 462);
			this->Start_Button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Start_Button->Name = L"Start_Button";
			this->Start_Button->Size = System::Drawing::Size(157, 46);
			this->Start_Button->TabIndex = 0;
			this->Start_Button->Text = L"Start Server";
			this->Start_Button->UseVisualStyleBackColor = false;
			this->Start_Button->Click += gcnew System::EventHandler(this, &VChatGUI::Start_Button_Click);
			// 
			// VChatOut
			// 
			this->VChatOut->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->VChatOut->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->VChatOut->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->VChatOut->ForeColor = System::Drawing::Color::ForestGreen;
			this->VChatOut->Location = System::Drawing::Point(24, 11);
			this->VChatOut->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->VChatOut->Multiline = true;
			this->VChatOut->Name = L"VChatOut";
			this->VChatOut->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->VChatOut->Size = System::Drawing::Size(1294, 416);
			this->VChatOut->TabIndex = 2;
			// 
			// Stop_Button
			// 
			this->Stop_Button->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->Stop_Button->BackColor = System::Drawing::Color::Blue;
			this->Stop_Button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Stop_Button->ForeColor = System::Drawing::Color::White;
			this->Stop_Button->Location = System::Drawing::Point(995, 464);
			this->Stop_Button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Stop_Button->Name = L"Stop_Button";
			this->Stop_Button->Size = System::Drawing::Size(150, 42);
			this->Stop_Button->TabIndex = 4;
			this->Stop_Button->Text = L"Stop Server";
			this->Stop_Button->UseVisualStyleBackColor = false;
			this->Stop_Button->Click += gcnew System::EventHandler(this, &VChatGUI::Stop_Button_Click);
			// 
			// InputPortLabel
			// 
			this->InputPortLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->InputPortLabel->AutoSize = true;
			this->InputPortLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InputPortLabel->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->InputPortLabel->Location = System::Drawing::Point(371, 471);
			this->InputPortLabel->Name = L"InputPortLabel";
			this->InputPortLabel->Size = System::Drawing::Size(133, 29);
			this->InputPortLabel->TabIndex = 5;
			this->InputPortLabel->Text = L"VChat Port:";
			// 
			// VChatPort
			// 
			this->VChatPort->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->VChatPort->BackColor = System::Drawing::SystemColors::ScrollBar;
			this->VChatPort->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->VChatPort->Location = System::Drawing::Point(504, 468);
			this->VChatPort->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->VChatPort->MaxLength = 6;
			this->VChatPort->Name = L"VChatPort";
			this->VChatPort->Size = System::Drawing::Size(101, 34);
			this->VChatPort->TabIndex = 9;
			this->VChatPort->Text = L"9999";
			// 
			// ExeLabel
			// 
			this->ExeLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->ExeLabel->AutoSize = true;
			this->ExeLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ExeLabel->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->ExeLabel->Location = System::Drawing::Point(31, 471);
			this->ExeLabel->Name = L"ExeLabel";
			this->ExeLabel->Size = System::Drawing::Size(182, 29);
			this->ExeLabel->TabIndex = 7;
			this->ExeLabel->Text = L"VChat.exe Path:";
			// 
			// VChatPath
			// 
			this->VChatPath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->VChatPath->BackColor = System::Drawing::SystemColors::ScrollBar;
			this->VChatPath->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->VChatPath->Location = System::Drawing::Point(212, 468);
			this->VChatPath->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->VChatPath->Name = L"VChatPath";
			this->VChatPath->Size = System::Drawing::Size(152, 34);
			this->VChatPath->TabIndex = 8;
			this->VChatPath->Text = L"VChat.exe";
			// 
			// PortLabel
			// 
			this->PortLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->PortLabel->AutoSize = true;
			this->PortLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PortLabel->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->PortLabel->Location = System::Drawing::Point(615, 471);
			this->PortLabel->Name = L"PortLabel";
			this->PortLabel->Size = System::Drawing::Size(132, 29);
			this->PortLabel->TabIndex = 11;
			this->PortLabel->Text = L"Used Port: ";
			// 
			// StatusBox
			// 
			this->StatusBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->StatusBox->BackColor = System::Drawing::Color::Red;
			this->StatusBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->StatusBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20));
			this->StatusBox->Location = System::Drawing::Point(1152, 466);
			this->StatusBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->StatusBox->Name = L"StatusBox";
			this->StatusBox->Size = System::Drawing::Size(72, 38);
			this->StatusBox->TabIndex = 12;
			// 
			// PortBox
			// 
			this->PortBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->PortBox->BackColor = System::Drawing::SystemColors::ScrollBar;
			this->PortBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->PortBox->Location = System::Drawing::Point(745, 468);
			this->PortBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->PortBox->Name = L"PortBox";
			this->PortBox->Size = System::Drawing::Size(69, 34);
			this->PortBox->TabIndex = 13;
			this->PortBox->Text = L"NULL";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Blue;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->ForeColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(1243, 465);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(72, 41);
			this->button1->TabIndex = 14;
			this->button1->Text = L"Exit";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &VChatGUI::button1_Click);
			// 
			// VChatGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->ClientSize = System::Drawing::Size(1352, 564);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->PortBox);
			this->Controls->Add(this->StatusBox);
			this->Controls->Add(this->PortLabel);
			this->Controls->Add(this->VChatPath);
			this->Controls->Add(this->ExeLabel);
			this->Controls->Add(this->VChatPort);
			this->Controls->Add(this->InputPortLabel);
			this->Controls->Add(this->Stop_Button);
			this->Controls->Add(this->VChatOut);
			this->Controls->Add(this->Start_Button);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Location = System::Drawing::Point(1510, 1510);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->MaximizeBox = false;
			this->Name = L"VChatGUI";
			this->Text = L"VChatGUI";
			this->Load += gcnew System::EventHandler(this, &VChatGUI::VChatGUI_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	/*
	* Background thread function that will be started when the server 
	* starts. This function will call the serverRead function from the 
	* server manager object (blocking) if this is successful we will
	* output the content to the message block.
	*
	* @param p: Not used.
	*
	* @return: None
	*/
	public: System::Void Server_Lisener(Object^ p) {
		/*
		std::cout << "Test\n";
		*/

		char buff[BUFFSIZE];
		
		// Catch any exceptions thrown by killing the processes we are 
		// trying to read from
		try {
			while (1) {
				if (this->serv_h->serverRead(buff, BUFFSIZE) == -1) {
					/*
					std::cout << GetLastError() << "\n";
					*/
					break;
				}
				System::String^ converted = gcnew System::String(buff);
				Write_Text_Box_Block(srv_prompt, converted);
				delete converted;
			}
		}
		// System::Threading::ThreadInterruptedException^
		catch (...) {
			// If we catch an exception we probably should exit (Nothing to clean up)
			return;
		}
	}
	
	/*
	* This functions uses a mutex with an indefinate block. This will write to the 
	* VChat Output textbox.
	*
	* @param textblock: This is a block of text we will output to the screen (Should 
	*                   terminated in `\r\n`
	* 
	* @return: None
	*/
	private: System::Void Write_Text_Box_Block(System::String^ prepend, System::String^ textBlock) {
		t_mutex->WaitOne();
		this->VChatOut->AppendText(prepend + textBlock);
		t_mutex->ReleaseMutex();
		return;
	}

	/*
	* This functions uses a mutex with an timed block. This will write to the 
	* VChat Output textbox.
	*
	* @param textblock: This is a block of text we will output to the screen (Should 
	*                   terminated in `\r\n`
	* 
	* @return: None
	*/
	private: System::Void Write_Text_Box_Timed(System::String^ prepend, System::String^ textBlock) {
		bool chk = 0;
		try {
			chk = t_mutex->WaitOne(1000);
			this->VChatOut->AppendText(prepend + textBlock);
			if (chk == 1)
				t_mutex->ReleaseMutex();
		} 
		catch (...) { }
		return;
	}

	/* Handler for start button being clicked */
	private: System::Void Start_Button_Click(System::Object^ sender, System::EventArgs^ e) {
			char* end;
			std::string portVal;

			// Get Current Date
			DateTime localDate = DateTime::Now;
			
			// If the server pointer has been allocated, and the server is started 
			if (serv_h != nullptr && this->serv_h->get_isStarted())
				return;

			// Validate port value and only update if numeric.
			portVal = msclr::interop::marshal_as<std::string>(VChatPort->Text);
			if (portVal != this->serv_h->get_serverPort() &&
				strtol(portVal.c_str(), &end, 10) != 0 && *end == '\0') {
				this->serv_h->set_port(portVal);
			}

			// To prevent any conditions where the thread is killed in the middle of a mutex operation, we create a new one once we start a server 
			this->t_mutex = gcnew System::Threading::Mutex();


			// Notify User we are starting the server
			this->Write_Text_Box_Block(gui_prompt + localDate.ToString() + ": ", "Starting Server\r\n");


			if (this->serv_h->initPipes() == -1) {
				this->Write_Text_Box_Block(gui_prompt + localDate.ToString() + ": ", "Failed to Create Unnamed Pipes\r\n");
				return;
			}

			if (this->serv_h->CreateServerProcess(msclr::interop::marshal_as<std::string>(VChatPath->Text)) == -1) {
				this->Write_Text_Box_Block(gui_prompt + localDate.ToString() + ": ", "Failed to Create Child Process\r\n");
				return;
			}

			// Start Listener
			this->t_handle = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &VChatGUI::Server_Lisener));
			this->t_handle->Start(10);

			// Set Started Tracker
			this->serv_h->set_isStarted(1);
			
			// Update Status Box Color
			this->StatusBox->BackColor = System::Drawing::Color::Green;

			// Update Port Used, this string is garbage collected
			this->PortBox->Text = gcnew System::String(this->serv_h->get_serverPort().c_str());
			return;
	}

	/* Handler for stop button being clicked */
	private: System::Void Stop_Button_Click(System::Object^ sender, System::EventArgs^ e) {
			// Only allow killing the server if it has been started. 
			if (!this->serv_h->get_isStarted())
				return;
			
			// Get Current Date
			DateTime localDate = DateTime::Now;
			
			// Kill Server
			this->t_handle->Interrupt(); // Abort since there is not much cleanup we do. This is illadvised.
			this->serv_h->killServer();
			this->serv_h->set_isStarted(0);


			// Display that we are killing/killed the server 
			this->Write_Text_Box_Timed(gui_prompt + localDate.ToString() + ": ","Killing Server\r\n");

			// Remove mutex
			delete this->t_mutex;

			// Update Status Box Color
			this->StatusBox->BackColor = System::Drawing::Color::Red;

			// Update Port Used to NULL (Use a string stored in class to prevent repeated deletions and creations)
			this->PortBox->Text = u_port;
			return;

		}

private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	this->Close();  // Closes the current form
}
private: System::Void VChatGUI_Load(System::Object^ sender, System::EventArgs^ e) {
}
}; // Class End
} // Namespace end
