#include "MyClasses.h"
#include <fstream>

int M_UP = 0;
string Status;

//string that hold all the messages
string Story_Message = "It is Dark";

//add another string to the message log
string Update_Message(string Message)
{
	Story_Message = Message + "\n" + Story_Message;
	return Story_Message;
}

//Display message
void Display_Message(LPD3DXFONT Message_font)
{
	std::ostringstream Message_Stream;
	Message_Stream << Story_Message;
	FontPrint(Message_font, 25, 20, Message_Stream.str());
}

int checkNumEvents(int id)
{
	int counter = 0;
	for (int i = 0; i < Num_Message_lines; i++)
	{
		if (stoi(Message_Array[i][0]) == id)
		{
			counter++;
		}
	}
	return counter;
}

//register single button clicks
void Button_Click(Actions actions[], long x, long y, Stats stat[], Message message[])
{
	for (int i = 0; i < 50; i++)
	{
		if (Mouse_Button(0) == 0x80 && x > actions[i].getX() && x < actions[i].getX() + 75 && y > actions[i].getY() && y < actions[i].getY() + 30 && M_UP == 0 && actions[i].getStatus() == "Active")
		{
			M_UP = 1;
			stat[i].changeAmount(actions[i].getModifier());
			int j = 0;
			while (j != Num_Message_lines)
			{
				if (message[j].getId() == actions[i].getId())
				{
					if (message[j].checkReq(actions, stat) == true)
						break;
				}
				j++;
			}
			actions[i].Set_Cooldown(globalTimeD);
			actions[i].Set_Status("Cooldown");
			
		}
		if (Mouse_Button(0) != 0x80 && M_UP != 0)
		{
			M_UP = 0;
		}
		if (actions[i].getCooldown() + actions[i].getTime() < globalTimeD && actions[i].getStatus() == "Cooldown")
		{
			actions[i].Set_Status("Active");
		}
		
	}
}

//load the layout and message file lines into individual parts of an array
void loadLayout(string filename[])//id|status|name|x|y|time|Modifier|Event Level
{
	for (int i = 0; i < Num_Layout_lines; i++)
	{
		string str = filename[i];
		string word;
		stringstream stream(str);
		int k = 0;
		while (getline(stream, word, '|'))
		{
			Layout_Array[i][k] = word;
			k++;
		}
	}
}
void loadMessages(string filename[])//id|Event Level|quantity|id of prerequisite|Event level of prerequisite|quantity of prerequisite|How much time to wait|Message displayed
{
	for (int i = 0; i < Num_Message_lines; i++)
	{
		string str = filename[i];
		string word;
		stringstream stream(str);
		int k = 0;
		while (getline(stream, word, '|'))
		{
			Message_Array[i][k] = word;
			k++;
		}
	}
}
void loadStats(string filename[])
{
	for (int i = 0; i < Num_Resource_lines; i++)
	{
		string str = filename[i];
		string word;
		stringstream stream(str);
		int k = 0;
		while (getline(stream, word, '|'))
		{
			Stat_Array[i][k] = word;
			k++;
		}
	}
}

//initialize the action and message classes with the values in the array given
void initActions(Actions actions[])
{
	for (int i = 0; i < Num_Layout_lines; i++)
	{
		actions[i].Set_Parameters(stoi(Layout_Array[i][0]), stoi(Layout_Array[i][3]), stoi(Layout_Array[i][4]), stoi(Layout_Array[i][5]), Layout_Array[i][1], Layout_Array[i][2], stoi(Layout_Array[i][6]), stoi(Layout_Array[i][7]));
	}
}
void initMessage(Message message[])
{
	for (int i = 0; i < Num_Message_lines; i++)
	{
		message[i].Set_Parameters(stoi(Message_Array[i][0]), stoi(Message_Array[i][1]), stoi(Message_Array[i][2]), stoi(Message_Array[i][3]), stoi(Message_Array[i][4]), stoi(Message_Array[i][5]), stoi(Message_Array[i][6]), Message_Array[i][7]);
	}
}
void initStats(Stats stat[])
{
	for (int i = 0; i < Num_Resource_lines; i++)
	{
		stat[i].Set_Parameters(stoi(Stat_Array[i][0]), Stat_Array[i][1], stoi(Stat_Array[i][2]));
	}
}

void Stats::Display_Stats(LPD3DXFONT Message_font)
{
	std::ostringstream Message_Stream;
	for (int i = 0; i < 50; i++)
	{
		Status = Status + "/n" + getName() + ":";
		Message_Stream << Status << getAmount();
	}
	
	FontPrint(Message_font, 300, 300, Message_Stream.str());
}

//set the values of the classese
void Actions::Set_Parameters(int ID, int X, int Y, int Time, string Status, string Name, int Modifier, int eventLevel)
{
	id_ = ID;
	x_ = X;
	y_ = Y;
	time_ = Time;
	status_ = Status;
	name_ = Name;
	modifier_ = Modifier;
	eventLevel_ = eventLevel;
}
void Message::Set_Parameters(int id, int eventLevel, int itemAmount, int reqId, int reqEventLevel, int reqAmount, int timeWait, string message)
{
	id_ = id;
	eventLevel_ = eventLevel;
	itemAmount_ = itemAmount;
	reqId_ = reqId;
	reqEventLevel_ = reqEventLevel;
	reqAmount_ = reqAmount;
	timeWait_ = timeWait;
	message_ = message;
}
void Stats::Set_Parameters(int id, string name, int amount)
{
	id_ = id;
	name_ = name;
	amount_ = amount;
}


//check if the values meet the requirements for the next event
bool Message::checkReq(Actions action[], Stats stats[])
{
	//integer to find the location of certain id's in the array
	int idPos = -1;
	int reqIdPos = -1;
	int maxEvent = checkNumEvents(id_);

	//search the array for these id's
	for (int i = 0; i < Num_Layout_lines; i++)
	{
		if (id_ == action[i].getId())
		{
			idPos = i;
		}
		if (reqId_ == action[i].getId())
		{
			reqIdPos = i;
		}
	}
	if (idPos == -1 || reqIdPos == -1)
		return false;
	//if requirements are met, return true
	if (action[reqIdPos].getEventLvl() == reqEventLevel_) //&& stats[idPos].getAmount() >= itemAmount_ && stats[reqIdPos].getAmount() >= reqAmount_)
	{
		action[idPos].Change_Event_Level(1);
		Update_Message(message_);
		return true;
	}
	else if (action[reqIdPos].getEventLvl() == reqEventLevel_) //&& stats[idPos].getAmount() >= itemAmount_ && stats[reqIdPos].getAmount() >= reqAmount_)
	{
		Update_Message(message_);
		return true;
	}
	else if (action[idPos].getEventLvl() == maxEvent && action[reqIdPos].getEventLvl() == reqEventLevel_ + 1) //&& stats[idPos].getAmount() >= itemAmount_ && stats[reqIdPos].getAmount() >= reqAmount_)
	{
		action[idPos].Change_Event_Level(-1);
		Update_Message(message_);
		action[idPos].Change_Event_Level(1);
		return true;
	}
	return false;
}
