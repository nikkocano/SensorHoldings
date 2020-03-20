#pragma once

#include "Server.hpp"

#include <string>

namespace sensorHoldingsExerciseServer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(Server* server)
			: mServer(server)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void updateMsgBox(const std::string& msg)
		{
			if (!msg.empty())
			{
				this->msgBox->Text = gcnew String(msg.c_str());
			}
			else
			{
				this->msgBox->Text = gcnew String("WHAT THE HELL HAPPENED!");
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ startButton;
	protected:

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ msgBox;
	private: System::Windows::Forms::Button^ stopButton;
	private: System::Windows::Forms::Button^ saveButton;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		Server* mServer;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->startButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->msgBox = (gcnew System::Windows::Forms::Label());
			this->stopButton = (gcnew System::Windows::Forms::Button());
			this->saveButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// startButton
			// 
			this->startButton->Location = System::Drawing::Point(144, 124);
			this->startButton->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->startButton->Name = L"startButton";
			this->startButton->Size = System::Drawing::Size(112, 35);
			this->startButton->TabIndex = 0;
			this->startButton->Text = L"Start";
			this->startButton->UseVisualStyleBackColor = true;
			this->startButton->Click += gcnew System::EventHandler(this, &MyForm::startButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(118, 23);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(369, 32);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Webcam Stream Recorder";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::msgBox_Click);
			// 
			// msgBox
			// 
			this->msgBox->Location = System::Drawing::Point(39, 290);
			this->msgBox->Name = L"msgBox";
			this->msgBox->Size = System::Drawing::Size(489, 28);
			this->msgBox->TabIndex = 2;
			this->msgBox->Text = L"Message";
			this->msgBox->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->msgBox->Click += gcnew System::EventHandler(this, &MyForm::msgBox_Click);
			// 
			// stopButton
			// 
			this->stopButton->Location = System::Drawing::Point(324, 124);
			this->stopButton->Name = L"stopButton";
			this->stopButton->Size = System::Drawing::Size(112, 35);
			this->stopButton->TabIndex = 3;
			this->stopButton->Text = L"Stop";
			this->stopButton->UseVisualStyleBackColor = true;
			this->stopButton->Click += gcnew System::EventHandler(this, &MyForm::stopButton_Click);
			// 
			// saveButton
			// 
			this->saveButton->Location = System::Drawing::Point(233, 196);
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(112, 35);
			this->saveButton->TabIndex = 4;
			this->saveButton->Text = L"Save";
			this->saveButton->UseVisualStyleBackColor = true;
			this->saveButton->Click += gcnew System::EventHandler(this, &MyForm::saveButton_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(582, 327);
			this->Controls->Add(this->saveButton);
			this->Controls->Add(this->stopButton);
			this->Controls->Add(this->msgBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->startButton);
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void startButton_Click(System::Object^ sender, System::EventArgs^ e) {
		const std::string statusMsg = mServer->start();
		updateMsgBox("Start streaming...");
		//updateMsgBox(statusMsg); // bugged by 2 recv calls
	}
	private: System::Void stopButton_Click(System::Object^ sender, System::EventArgs^ e) {
		const std::string statusMsg = mServer->stop();
		updateMsgBox("Stop streaming...");
		//updateMsgBox(statusMsg); // bugged by 2 recv calls
	}
	private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e) {
		const std::string statusMsg = mServer->save();
		updateMsgBox(statusMsg);
	}
	private: System::Void msgBox_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
