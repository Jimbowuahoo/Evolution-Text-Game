#include "MyDirectX.h"
#include "Fmod.h"
#include "MyClasses.h"

enum GAME_STATE
{
	START_MENU = 0,
	OPTIONS_MENU = 1,
	CHARACTER_MENU = 2,
	WORLD = 3,
	COMBAT = 4
};
GAME_STATE Game_State = START_MENU;

//number of lines in certain files
int Num_Layout_lines = 0;
int Num_Message_lines = 0;
int Num_Resource_lines = 0;

//initialize the arrays that will need to hold the file information
string Layout_Array[50][10];
string Message_Array[1000][20];
string Stat_Array[50][5];

//store each line of a file into a array to be further parsed later on
string Layout_Lines[50];
string Message_Lines[1000];
string Resource_Lines[50];

string test;

//values for user input
int Key_Wait = 0;
long MouseX;
long MouseY;

//class definitions
Message Game_Message;
Time Game_time;
Actions action[50];
Message message[1000];
Stats stats[50];

//sprite definitions
SPRITE actButton;

//texture definitions
LPDIRECT3DTEXTURE9 imgButton;

//screensize
const string APPTITLE = "Evolution";
const int SCREENW = 1024;
const int SCREENH = 768;

//define font
LPD3DXFONT Message_font;

//Definitions for the Fmod sound system
FMOD::System		*fmodsystem;
FMOD::Sound			*music, *effect1;
FMOD::Channel		*channel = 0;
unsigned int		version;

//game initialization function
bool Game_Init(HWND window)
{
	FMOD_RESULT			result;
	result = FMOD::System_Create(&fmodsystem);
	result = fmodsystem ->init(50, FMOD_INIT_NORMAL, 0);
	result = fmodsystem->createSound("assets/swish.wav", FMOD_DEFAULT, 0, &effect1);
	result = fmodsystem->createStream("assets/Music.wav", FMOD_DEFAULT, 0, &music);
	
	//Initializing direct3D
	if (!Direct3D_Init(window, SCREENW, SCREENH, FALSE))
	{
		MessageBox(0, "Error initializing Direct3D", "ERROR", 0);
		return false;
	}

	//This is the Input system from the directx SDK
	if (!DirectInput_Init(window))
	{
		MessageBox(0, "Error initializing DirectInput", "ERROR", 0);
		return false;
	}

	//a string that helps in counting the lines in each file
	string line;

	//open files to read from
	std::fstream Layout_File("Layout.txt");
	std::fstream Message_File("Messages.txt");
	std::fstream Resource_File("Resources.txt");

	//store each line of file into array
	if (Layout_File.is_open()) //if the file is open
	{
		while (!Layout_File.eof()) //while the end of file is NOT reached
		{
			getline(Layout_File, line); //get one line from the file
			Layout_Lines[Num_Layout_lines] = line;
			Num_Layout_lines++;
		}
		Layout_File.close(); //closing the file
	}
	if (Message_File.is_open()) //if the file is open
	{
		while (!Message_File.eof()) //while the end of file is NOT reached
		{
			getline(Message_File, line); //get one line from the file
			Message_Lines[Num_Message_lines] = line;
			Num_Message_lines++;
		}
		Message_File.close(); //closing the file
	}
	if (Resource_File.is_open()) //if the file is open
	{
		while (!Resource_File.eof()) //while the end of file is NOT reached
		{
			getline(Resource_File, line); //get one line from the file
			Resource_Lines[Num_Resource_lines] = line;
			Num_Resource_lines++;
		}
		Resource_File.close(); //closing the file
	}
	
	//load the image of the button
	imgButton = LoadTexture("assets/button.png");
	if (!imgButton) return false;

	//set the parameters of the button
	actButton.width = 75;
	actButton.height = 30;

	//load the the Line by Line arrays into the 2D arrays
	loadLayout(Layout_Lines);
	loadMessages(Message_Lines);
	loadStats(Resource_Lines);

	//initialize objects
	initActions(action);
	initMessage(message);

	Message_font = MakeFont("Times New Roman", 18);

	//pointer to backbuffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	return true;
}


void Game_Run(HWND window)
{	
	FMOD_RESULT			result;
	//make sure the Direct3D device is valid
	if (!d3ddev)return;

	//update the input devices
	DirectInput_Update();

	if (Key_Down(DIK_SPACE) == 128 && Key_Wait == 0)
	{
		Update_Message("New Message");
		Key_Wait = 1;
	}
	if (Key_Down(DIK_SPACE) == 0 && Key_Wait == 1)
	{
		Key_Wait = 0;
	}

	//mouse position
	MouseX = Get_Mouse_Pos(window).x+3;
	MouseY = Get_Mouse_Pos(window).y;

	Game_time.Calc_Game_Time();
	Button_Click(action, MouseX, MouseY, stats, message);
	
	//clear the scene
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene())
	{
		
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		Sprite_Transform_Draw(imgButton, 0, 0, actButton.width, actButton.height, actButton.frame, actButton.columns, actButton.rotation, actButton.scalingX, actButton.scalingY, (0,0,0));

		std::ostringstream oss;
		oss << Key_Down(DIK_SPACE)<< " || " << action[0].getEventLvl() << " || "<< action[1].getEventLvl()<< " || " << Game_time.Get_Game_Time()<< " || " << MouseX << ", " << MouseY << "||" <<action[0].getStatus() << "||";
		FontPrint(Message_font, 0, 0, oss.str());
		
		Display_Message(Message_font);

		spriteobj->End();
		
		for (int i = 0; i < 5; i++)
		{
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			if (action[i].getStatus() == "Active")
			{
				Sprite_Transform_Draw(imgButton, action[i].getX(), action[i].getY(), actButton.width, actButton.height, actButton.frame, actButton.columns, actButton.rotation, actButton.scalingX, actButton.scalingY, actButton.color);

				std::ostringstream *oss1 = new ostringstream[Num_Layout_lines];
				oss1[i] << action[i].getName();
				FontPrint(Message_font, 10, 5, oss1[i].str());
			}
			//stop drawing
			spriteobj->End();
		}
		//stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//escape key exits (doesn't work as direct input is currently down)
	if (Key_Down(DIK_ESCAPE))
		gameover = true;
	
	//controller Backbutton also exits (doesn't work as direct input is currently down)
	if (controllers[0].wButtons&XINPUT_GAMEPAD_BACK)
		gameover = true;
}

void Game_End()
{
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}