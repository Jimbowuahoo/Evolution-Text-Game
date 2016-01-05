#pragma once
#include "MyDirectX.h"

extern string Story_Message;
extern int globalTime;
extern double globalTimeD;

bool Calc_Seconds(int seconds);

class Time
{
private:
	double Seconds_Count = 0.0;
	double Game_Time = 0.0;
public:
	Time() {}
	void Calc_Game_Time();
	float Get_Seconds();
	double Get_Game_Time();
};

class Actions
{
private:
	int id_;
	int x_;
	int y_;
	int time_;
	string status_;
	string name_;
	int modifier_;
	int eventLevel_;
	double cooldown_ = 0;
public:
	void Set_Parameters(int id, int x, int y, int time, string status, string name, int modifier, int eventLevel);
	void Set_Status(string status) { status_ = status; }
	void Change_Event_Level(int i) { eventLevel_ += i; }
	void Set_Cooldown(double t) { cooldown_ = t; }

	int getCooldown() { return cooldown_; }
	int getId() { return id_; }
	int getX() { return x_; }
	int getY() { return y_; }
	int getTime() { return time_; }
	int getEventLvl() { return eventLevel_; }
	string getStatus() { return status_; }
	string getName() { return name_; }
	int getModifier() { return modifier_; }
};

class Stats
{
private:
	int id_;
	int amount_;
	string name_;
public:
	int getID() { return id_; }
	int getAmount(){ return amount_; }
	string getName() { return name_; }

	void Set_Parameters(int id, string name, int amount);

	void Display_Stats(LPD3DXFONT Message_font);
	void changeAmount(int modifier) { amount_ += modifier; }
};

class Message
{
private:
	int id_;
	int eventLevel_;
	int itemAmount_;
	int reqId_;
	int reqEventLevel_;
	int reqAmount_;
	int timeWait_;
	string message_ = " ";
public:
	Message() {}
	void Set_Parameters(int id, int eventLevel, int itemAmount, int reqId, int reqEventLevel, int reqAmount, int timeWait, string message);
	int getId() { return id_; }
	int getEventLvl() { return eventLevel_; }
	int getItemAmount() { return itemAmount_; }
	int getReqId() { return reqId_; }
	int getReqEventLvl() { return reqEventLevel_; }
	int getReqAmount() { return reqAmount_; }
	string getMessage() { return message_; }
	bool checkReq(Actions action[], Stats stats[]);
};

void Button_Click(Actions actions[], long x, long y, Stats stat[], Message message[]);
void loadLayout(string filename[]);
void loadMessages(string filename[]);
void loadStats(string filename[]);
void initActions(Actions action[]);
void initMessage(Message message[]);
void initStats(Stats stats[]);
