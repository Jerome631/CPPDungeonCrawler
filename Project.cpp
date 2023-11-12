#include<iostream.h>
#include<fstream.h>
#include<process.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<iomanip.h>
#include<stdio.h>

typedef int(*Mass)();

/////////////////////////////////////////////////////////////////////////////
//			Global Variables

long int gold=100;   int Story=0;   int turn=1; char used[20]; int gdmg;
char itemtag[8][20];//current Items> 7
char skilltag[9][20];//current Skills> 8
char eqtag[8][20];//current Equips> 7
char chgname[20]="???";

//////////////////////////////////////////////////////////////////////////////
//			Global Switches

int p1sw=1;   int p2sw=0;   int Inbattle=0;
int Inshop=0;

//////////////////////////////////////////////////////////////////////////////
// 			Prototypes

int game();     void allchk();   void intro();  void statbar();
int statusmenu();  int Debug();  void Help();   int inventory();
int skillmenu();   int Skillset();   int Equipmenu();   int Itemset();
int Equipset();    int Intro();    int Story1();   int Story2();   
int Story3();    int Story4();	int Story5();	int Story6();
void Gameover();	int mainmenu();       void filecreate();


///////////////////////////////////////////////////////////////////////////////
//			Structures

class OPINION
{protected:
char opclass;

public:
void chooseopclass()
{cout<<"\n did you enjoy the game ??";
cin>>opclass;
}

};

struct Equipment
{  char name[20]; int amt;   int prc;
   int hp;    int mhp;   int mp;   int mmp;
   int atk;   int def;   int mat;  int mdf;
   int spd;   int luck;  int type;
};//types> 1-weapon, 2-body, 3-accessory


Equipment eqitems[]={	{"None", 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},            //0
						{"Sturdy Stick", 1, 100, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 1},   //1
						{"Cutlass", 0, 500, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 1},    //2
						{"Cursed Blade", 0, 6000, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0, 1},//3
						{"Robes",1 ,100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2},  //4
						{"Chainmail",0 ,400, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2},     //5
						{"Bandana",1 ,100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3},         //6
						{"Leather Bandana",0 ,200, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3}  //7

					};//List of Equipment
						int totalequips=7;

struct eqtypes
{  	Equipment weapon;	Equipment armour;
	Equipment accessory;
};//List of Equipment Types

struct charst
{
	long lv;	long hp;     long mhp; 	 long mp;  	 long mmp;
	long atk;  	long def;    long mat;   long mdf; 	 long spd;
	long luck;  long exp;	 char name[20];	  char weak[20];
	eqtypes equips;
};//All parameters for character stats

charst p1={1,450,450,15,15,16,16,16,16,32,32,0,"???"};
charst modp1={1,56,56,8,8,5,3,3,2,2,5};
charst p1base={1,450,450,15,15,16,16,16,16,32,32,0,"Player1"};
charst null={0,0,0,0,0,0,0,0,0,0,0,0,"Null"};
charst mon[]={ 	{0,0,0,0,0,0,0,0,0,0,0,0,"Null"},//0
				{1,100,100,0,0,12,4,8,8,10,10,4,"Slime","Freeze"},//1
				{1,140,140,0,0,14,6,9,9,12,12,8,"Bat","Wind"},//2
				{1,350,350,60,60,30,20,18,18,24,24,26,"Wisp"},//3
				{1,800,800,0,0,50,25,35,35,40,40,87,"Skeleton","Light"},//4
				{1,900,900,150,150,60,32,40,40,45,45,110,"Imp","Wind"},//5
				{1,1100,1100,0,0,80,50,40,40,60,60,260,"CockatrFreeze","Wind"},//6
				{1,1200,1200,0,0,85,85,50,50,60,60,450,"Chimera","Water"}//7
			 };//List of Monsters

struct cons
{ 	char name[20];	int amt;	 int use;	 int type;
	char descr[50];	int prc;
};//Structure of Consumable Items
//type> 0=HP,1=MP,2=Both

cons recovery[]={ 	{"Null",0,0,0,"Nothing"},//0
					{"Potion",0,100,0,"Recovers HP +100",10}, //1
					{"Moonshine",0,100,0,"Recovers HP +1000",100},//2
					{"X Solution",0,9999,0,"Recovers HP all",2000},//3
					{"Nectar",0,25,1,"Recovers MP +25",10},//4
					{"Turbo Nectar",0,100,1,"Recovers MP +100",100},//5
					{"Dry Nectar",0,9999,1,"Recovers MP all",2000},//6
					{"Elixir",0,9999,2,"Recovers HP and MP all",5000}//7
				};//List of Recovery Items
					int totalitems=7;

struct skills
{ 	char name[20]; 	char descr[100]; 	int req;	int av;	int occ;
	int mpcost;
};//Structure of Skills

skills skill[]={ 	{"Null","Nothing",0,1,0},
					{"Swing","Deals two times the attack of the user\n\tConsumes 10% of HP",0,1,1,0},
					{"Heal","Restores HP to one ally.\n\tConsumes MP",4,0,2,4},
					{"Inferno","Deals fire damage to one enemy.\n\tConsumes MP",2,0,1,5},//3
					{"Freeze","Deals ice damage to one enemy.\n\tConsumes MP",6,0,1,4},//4
					{"Thunder","Deals lightning damage to one enemy.\n\tConsumes MP",6,0,1,5},//5
					{"Water","Deals water damage to one enemy.\n\tConsumes MP",6,0,1,6},//6
					{"Wind","Deals wind damage to one enemy.\n\tConsumes MP",10,0,1,6},//7
					{"Mage","Deals magic damage to one enemy.\n\tConsumes MP",10,0,1,10}//8
				};//List of Skills
					int totalskills=8;

//////////////////////////////////////////////////////////////////////////////////////////
//			Skill Function Prototypes

int Swing(charst &a,charst &b);
int Attack(charst &a,charst &b);
int Heal(charst& a,charst& b= null);
int Inferno(charst& a,charst& b);
int Freeze(charst& a,charst& b);
int Thunder(charst& a,charst& b);
int Water(charst& a,charst& b);
int Wind(charst& a,charst& b);
int Mage(charst& a,charst& b);
///////////////////////////////////////////////////////////////////////////////


Mass play[]=
{   Intro,
	Story1,
	Story2,
	Story3,
	Story4,
	Story5,
	Story6
};//Array of Story Functions

///////////////////////////////////////////////////////////////////////////
//			Compiler Main

int main()
{ 	textbackground(0); 	clrscr();
	p1.equips.weapon=eqitems[1];
	p1.equips.armour=eqitems[0];
	p1.equips.accessory=eqitems[0];
	allchk();
	clrscr();
	Help();
	for(int z=0;z<=6;z++)
	{ 	play [z]();
		clrscr();
		cout<< "Loading";
		for(int t=0;t<=400;t++)
		{     	
			cout<<".";
		}
	}
	return 0;
	
}


//////////////////////////////////////////////////////////////////////////////////////////
//		        Other Functions

void nameinput(charst& player);
void battleprs(charst a);//, charst b= null);
void randomdmg(int &dmg,int var=0);

///////////////////////////////////////////////////////////////////////////////////////////
//			Story Functions

int Story1()
{	clrscr();
	int k_;
	statbar();
	cout<<"\n\n\tEverything is blurry...";
	k_=getch();
	cout<<"\n\n\tYou see a fence in front of you,";
  	k_=getch();
	cout<<"\n\n\tYou try to remember why you are here...";
	k_=getch();
	cout<<"\n\n\tYou try to remember your name...";
	k_=getch();
	cout<<"\n\n\tA name comes up in your mind...";
	k_=getch();
	cout<<"\n\t"<<chgname;
	k_=getch();
	strcpy(p1.name,chgname);
        cout<<"\n\n\tYou find a sturdy stick and arm yourself...";
	k_=getch();
	cout<<"\n\n\tYou decide to go to the fence...";
	k_=getch();
	return 0;
}

int Story2()
{	int k_,r=0;
	while( r==0)
	{ 	clrscr();
		statbar();
		cout<<"\n  Villager : Come help us,the village is under attack.\n";
		cout<<"\n\t";
		cout<<"| Choose";
		cout<<"\n\t";
		cout<<"| 1.Choose to help";
		cout<<"\n\t";
		cout<<"| 2.Choose not to"<<endl;
		cin>>k_;
		cout<<endl;
		if(k_==1)
		{  	cout<<"\n\n  Villager : Lets go we've gotta hurry!";
			k_=getch();
			r=1;
		}
		else if(k_==2)
		{	cout<<"\n\n  Villager :  Oh!, you have got to help us.\n   Please.";
			cout<<"\n    "<<p1.name<<" realizes there is no other way to further the plot";
			k_=getch();
		}
	};
	statbar();
	cout<<"\n\tYou are in the village of Codal. You see chaos everywhere,";
	cout<<"\n\tpeople running here and there, crying out for help.";
	k_=getch();
	cout<<"\n\n\tSuddenly.....";
	k_=getch();
	battleprs(mon[1]);
	statbar();
	cout<<"\n  Villager : Wow you are strong.";
	k_=getch();
	cout<<"\n\n\t     This just started two days ago.";
	k_=getch();
	cout<<"\n\n\t     Usually the slimes do not attack the villages.";
	k_=getch();
	cout<<"\n\n\t     Something's off.";
	k_=getch();
	battleprs(mon[1]);
	statbar();
	cout<<"\n  Villager : Lets meet up with the Elder.";
	k_=getch();
	return 0;
}

int Story3()
{ 	int k_,r=0;
	clrscr();
	statbar();
	cout<<"\n  Village Elder : Welcome, Young man!"<<endl;
	k_=getch();
	cout<<"\nI heard that you defeated two slimes easily,"<<endl;
	k_=getch();
	cout<<"\nWithout any experience in battle it is not possible."<<endl;
	k_=getch();
	cout<<"\nTell me where do you come from ?"<<endl;
	k_=getch();
	cout<<"\n  "<<p1.name<<" : Um...";
	while( r==0)
		{
			cout<<"\n\t";
			cout<<"| Choose:";
			cout<<"\n\t";
			cout<<"| 1.Tell him you forgot.";
			cout<<"\n\t";
			cout<<"| 2.Tell him you will not.";
			cout<<endl;
			k_=getch();
			if((getch())=='2')
			{	cout<<"  "<<p1.name<<" : Sorry, But I can't tell...";
				r=1;
			}
			else
			{ 	cout<<"  "<<p1.name<<" : I don't remember...";
				r=1;
			}
		};
		k_=getch();
		cout<<"\n\n  Village Elder : Oh! No problem then,"<<endl;
		k_=getch();
		cout<<"Ok! Lets comeback to the issue...";
		k_=getch();
		return 0;
}

int Story4()
{ 	int k_,r=0;
	clrscr();
	statbar();
	cout<<"\n  Village Elder : So the problem is,"<<endl;
	k_=getch();
	cout<<"\nFor two days, slimes from the nearby dungeon have started"<<endl;
	cout<<"\nattacking the village."<<endl;
	k_=getch();
	cout<<"\nA group of our men went to the dungeon,"<<endl;
	k_=getch();
	cout<<"\nBut when they returned,"<<endl;
	k_=getch();
	cout<<"\nThey were too scared to answer."<<endl;
	k_=getch();
	clrscr();
	statbar();
	cout<<"\n  Village Elder : I think, some beast is residing in the cave."<<endl;
	k_=getch();
	cout<<"\nAnd so the monsters in the dungeon are coming out."<<endl;
	k_=getch();
	cout<<"\nCan you help us, by defeating it ?"<<endl;
	while( r==0)
		{

			cout<<"\n\t";
			cout<<"| Choose:";
			cout<<"\n\t";
			cout<<"| 1.Tell him you will.";
			cout<<"\n\t";
			cout<<"| 2.Ask him for profit.";
			cout<<endl;
			//(1);
			if((getch())=='2')
			{	cout<<"  "<<p1.name<<" : What will I get out of it ?";
				k_=getch();
				cout<<"\n\n  Village Elder : We don't want your help, Bye !";
				k_=getch();
				Gameover();
				r=1;
			}
			else
			{	cout<<"  "<<p1.name<<" : Sure, Why not !";
				r=1;
			}
		};
		k_=getch();
		cout<<"\n\n  Village Elder : Oh, Thank You. You are a very good person.";
		k_=getch();
		return 0;
}

int Story5()
{	int k_;
	clrscr();
	statbar();
	cout<<"\n  Village Elder : Here, some items.";
	recovery[1].amt+=5;
	eqitems[2].amt+=1;
	eqitems[5].amt+=1;
	eqitems[7].amt+=1;
	cout<<"\n\n\t";
	cout<<"<!> You received 5 <Potions>";
	cout<<"\n\n\t";
	cout<<"<!> You received <Cutlass>";
	cout<<"\n\n\t";
	cout<<"<!> You received <Leather Bandana>";
	cout<<"\n\n\t";
	cout<<"<!> You received <Chainmail>";
	k_=getch();
	cout<<"\n  Village Elder : Use them wisely. Good luck, And be safe.";
	k_=getch();
	game();
	return 0;
}

int Story6()
{  	int key,k,x=9,rg,k_;
	randomize();
	for(int i=1;i<=15;)
	{
		clrscr();
		statbar();
		cout<<"\t\tLocation: ";
		if(i==10)
		{   cout<<"\n\n\tThe beast which the Elder told you about stands before you";
			k_=getch();
			battleprs(mon[7]);
			filecreate();
			clrscr();
			cout<<"\n\n\n\n\n\n\t\t\t\tThanks For Playing, "<<p1.name;
			k_=getch();
		}
		cout<<"Dungeon Level,"<<i<<endl;
		cout<<"1. Go Ahead"<<endl;
		cout<<"2. Go Back"<<endl;
		cout<<"3. Search"<<endl;
		cout<<"x. Menu"<<endl;
		cout<<"h. Help"<<endl;
		cout<<">";
		dg:;
		key=getch();
		switch(key)
		{	case '1': i++;
					  cout<<"\n\nMoving to Next Floor\n";
					  k_=getch();
					  break;
			case '2': i--;
					  if(i==0)
					  {	Gameover(); }
					  cout<<"\n\nMoving to Previous Floor\n";
					  k_=getch();
					  break;
			case '3': rg=random(100);
					  if(rg%3==0)
					  {	rg=random(7);
						if(rg==0)//gold
						{	int rgold=rand() % ((i+1)*10) +i*10;
							gold+=rgold;
							cout<<"\n\n\t";
							cout<<"<!> You found Gold,"<<rgold;
							k_=getch();
						}
						else if(rg==1)//damage
						{	int rhdmg=rand() % 70;
							cout<<"\n\n\tYou got hurt while looking";
							p1.hp-=rhdmg;
							if(p1.hp<=0)
							{	Gameover(); }
							k_=getch();
						}
						else if(rg==2)//item
						{	int ritem=rand() % 100;
							if(ritem>=1 && ritem<10)
							{	cout<<"\n\n\tNothing";
								k_=getch();
							}
							else if(ritem>=10)// && ritem<=20)
							{   int rr;
								int amount=rand() % 3;
								int rlistitems=random(100);
								if(rlistitems==10)
								rr=2;
								else if(rlistitems==20)
								rr=5;
								else if(rlistitems==30)
								rr=3;
								else if(rlistitems==40)
								rr=6;
								else if(rlistitems==50)
								rr=4;
								else if(rlistitems==60)
								rr=7;
								else
								switch(rlistitems%2)
								{   case 0: rr=1;	break;
									case 1: rr=4;	break;
								}
								if(amount==0)
								{	cout<<"\n\n\tNothing";
									k_=getch();
									break;
								}
								recovery[rr].amt+=amount;
								cout<<"\n\n\t";
								cout<<"<!> You found "<<amount;
								cout<<"<";
								cout<<recovery[rr].name;
								cout<<">";
								k_=getch();
							}
						}
						else
						{	cout<<"\n\n\tNothing";
							k_=getch();
						}
					  }
					  else if(rg%5!=0)
					  {	int renemy=rand() % 7;

						if(renemy==2)
						{	if(i>=2)
							{	battleprs(mon[2]); }
						}
						else if(renemy==3)
						{	if(i>=4)
							{	battleprs(mon[3]); }
						}
						else if(renemy==4)
						{	if(i>=6)
							{	battleprs(mon[4]); }
						}
						else if(renemy==5)
						{	if(i>=9)
							{	battleprs(mon[5]); }
						}
						else if(renemy==6)
						{	if(i>=11)
							{	battleprs(mon[6]); }
						}
						else
						{	if(i>=1)
							{	battleprs(mon[1]); }
						}
					  }
					  else
					  {	cout<<"\n\n\tNothing";
						k_=getch();
					  }
					  break;
			case 'x': mainmenu();
				  goto dg;
					  break;
			case 'h': Help();
					  break;
			default: break;
		}
	}
	return 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//			Functions

void filecreate()
{
	ofstream fout("Play.txt");
		if(!fout)
			{
				cout<<"\nFile could not be opened";
				exit(0);
			}
	fout<<"\n\tName: "<<p1.name;
	fout<<"\n\tLv= "<<p1.lv;
	fout<<"\n\tHP= "<<p1.hp<<"/"<<p1.mhp;
	fout<<"\n\tMP= "<<p1.mp<<"/"<<p1.mmp;
	fout<<"\n\tAttack= "<<p1.atk;
	fout<<"\n\tDefence= "<<p1.def;
	fout<<"\n\tMagic Attack= "<<p1.mat;
	fout<<"\n\tMagic Defence= "<<p1.mdf;
	fout<<"\n\tSpeed= "<<p1.spd;
	fout<<"\n\tLuck= "<<p1.luck;
	fout<<"\n\tExp= "<<p1.exp;
	fout.close();
}

void modify(charst &m,charst &n,charst &o)
{
	m.mhp=o.mhp+((m.lv-1)*n.mhp)+ m.equips.weapon.mhp
	+ m.equips.armour.mhp + m.equips.accessory.mhp;

	m.mmp=o.mmp+((m.lv-1)*n.mmp)+ m.equips.weapon.mmp
	+ m.equips.armour.mmp + m.equips.accessory.mmp;

	m.atk=o.atk+((m.lv-1)*n.atk)+ m.equips.weapon.atk
	+ m.equips.armour.atk + m.equips.accessory.atk;

	m.def=o.def+((m.lv-1)*n.def)+ m.equips.weapon.def
	+ m.equips.armour.def + m.equips.accessory.def;

	m.mat=o.mat+((m.lv-1)*n.mat)+ m.equips.weapon.mat
	+ m.equips.armour.mat + m.equips.accessory.mat;

	m.mdf=o.mdf+((m.lv-1)*n.mdf)+ m.equips.weapon.mdf
	+m.equips.armour.mdf + m.equips.accessory.mdf;

	m.spd=o.spd+((m.lv-1)*n.spd)+ m.equips.weapon.spd
	+ m.equips.armour.spd + m.equips.accessory.spd;

	m.luck=o.luck+((m.lv-1)*n.luck)+ m.equips.weapon.luck
	+ m.equips.armour.luck + m.equips.accessory.luck;
}

void lvchk(charst &x)
{ 	int v=99;

	while(v>=1)
	{ for(int lsum=0,i=v-1;i>=0;i--)
		{  lsum+=i; }
		if(x.exp>=5*lsum)
		{ x.lv=v;
		break;
		}
		v--;
	}
	Skillset();
}

void randomdmg(int &dmg,int var)
{ 	var=var*dmg/100;
	int val=random(var+1);
	int operate=random(100);
	operate-=random(10);
	if(operate%2==0)
	{   dmg+=val; }
	else
	{   dmg-=val; }
}

int Skillset()
{  	int j,numskills=0;
	for(j=1;j<=totalskills;j++)// 3-no. of skills <temp>
	{	if( (skill[j].req)<=p1.lv)
		{   skill[j].av=1;
			numskills++;
			strcpy(skilltag[numskills], skill[j].name);
		}
	}
	return numskills;
};

int Itemset()
{   int j,numitems=0;
	for(j=1;j<=totalitems;j++)// 7-no. of items <temp>
	{   if( recovery[j].amt>0)
		{ 	numitems++;
			strcpy(itemtag[numitems], recovery[j].name);
		}
	}
	return numitems;
};

int Equipset()
{   int j,numitems=0;
    for(j=1;j<=totalequips;j++)// 7-no. of items <temp>
    {   if( eqitems[j].amt>0)
		{	numitems++;
			strcpy(eqtag[numitems], eqitems[j].name);
		}
	}
    return numitems;
};

void Gameover()
{   clrscr();
    int k_;
    filecreate();
	OPINION o;
	o.chooseopclass();
	cout<<"\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t   Gameover";
    k_=getch();
    exit(0);
}

charst display(charst x)
{
	cout<<"\nName: "<<x.name;
	cout<<"\n\tLv= "<<x.lv;
	cout<<"\n\tHP= "<<x.hp<<"/"<<x.mhp;
	cout<<"\n\tMP= "<<x.mp<<"/"<<x.mmp;
	cout<<"\n\tAttack= "<<x.atk;
	cout<<"\n\tDefence= "<<x.def;
	cout<<"\n\tMagic Attack= "<<x.mat;
	cout<<"\n\tMagic Defence= "<<x.mdf;
	cout<<"\n\tSpeed= "<<x.spd;
	cout<<"\n\tLuck= "<<x.luck;
	cout<<"\n\tExp= "<<x.exp;

	return x;
}

void statbar()
{
	clrscr();
	cout<<"--------------------------------------------------------------------------------";
	//(3);
	cout<<"< "<<p1.name<<" >";
	cout<<"    Lvl: "<<p1.lv;
	cout<<"    HP: "<<p1.hp<<"/"<<p1.mhp;
	cout<<"	  MP: "<<p1.mp<<"/"<<p1.mmp;
	//(15);
	cout<<"\n--------------------------------------------------------------------------------";
}

int Intro()
{	int k_;
	char ch;
	clrscr();
	cout<<"\n\n\n\n\n\n\n\n\n\n\n";
	cout<<"*******************************    >New Game **********************************"<<endl;
	cout<<"*******************************    >Exit     **********************************"<<endl;
	cout<<"Press any key to continue or e to exit"<<endl;
	ch=getch();
	do{
		
		clrscr();
		nameinput(p1);
		break;

	}while(ch!='e');

	if(ch=='e')
			{	clrscr();
				cout<<endl<<"\n\n\n\n\n\t\t\t\t    Ok Bye!";
				k_=getch();
				exit(0);
			}
	return 0;
}

void nameinput(charst& player)
{ 	int key,k_;
	int chg=0;
	do{
		key=0;
		chg=0;
		clrscr();
		cout<<"\n\n\tWhat is your name?\n ";
		if((strcmp(chgname,"???"))==0)
		{	chg=1;
			cin>>chgname;
		}
		else
		{	cin>>player.name; }
		cout<<"\nHello ";
		if(chg==0)
		{	cout<<player.name; }
		else
		{	cout<<chgname;}
		cout<<", Are you ready to begin your adventure?";
		cout<<" (y/n)\n> ";
		key=getch();
		if(key=='y')
		{	cout<<"\n\n\tWell as you wish.";
			k_=getch();
			return ;
		}
		strcpy(chgname,"???");
	}while(key!='y');
	k_=getch();
	clrscr();
}

int statusmenu()
{  	clrscr();
	int key=0;
	statbar();
	cout<<">> Menu ";
	cout<<">> Status ";
	cout<<"\n--------------------------------------------------------------------------------";
	display(p1);
	do{ key=getch();
	}while(key!='x');
	return 0;
}

int mainmenu()
{ 	clrscr();
	int k_,r=0;
	do{
	r=0;
	statbar();
	cout<<">> Menu";
	cout<<"\n--------------------------------------------------------------------------------";
	cout<<"\n\t  ----------------- "; cout<<"\t -------------------";
	cout<<"\n\t |    1.Inventory  |"; cout<<"\t| Gold:"<<setw(12)<<gold<<" |";
	cout<<"\n\t |    2.Skills     |"; cout<<"\t -------------------";
	cout<<"\n\t |    3.Equip      |";
	cout<<"\n\t |    4.Status     |";
	cout<<"\n\t |    5.Go Back    |";
	cout<<"\n\t |    6.Exit Game  |";
	cout<<"\n\t  ---------------\n";
	//(h,i);
	cout<<">";
	cin>>k_;

	do{

			if(k_==1)
			{	inventory();
				break;
			}
			else if(k_==2)
			{	skillmenu();
				break;
			}
			else if(k_==3)
			{	Equipmenu();
				break;
			}
			else if(k_==4)
			{	statusmenu();
				break;
			}
			else if(k_==6)
			{	clrscr();
				cout<<"\n\n\n\n\t\t\t\tPress any key...";
				k_=getch();
				exit(0);
			}
			else if(k_==5)
			{	r=10;
				break;
			}

	}while(k_==1||k_==2||k_==3||k_==4||k_==5||k_==6);

	}while(r!=10);
	return 0;
}
void invdisp(cons a);
void invopt(cons &a);
void recprs(charst &c,cons &d);
void skilldisp(skills a);
int inventory()
{
	int n,k_,x,h=9,i=8,j,geti,t,r=0;;
	do{
	r=0;
	geti=i;
	j=Itemset()+ 7;
	clrscr();
	statbar();
	cout<<">> Menu ";
	cout<<">> Items";
	cout<<"\n--------------------------------------------------------------------------------";
	cout<<endl;
	for(n=1;n<=totalitems;n++){ 	 
	cout<<"\n\t"<<n<<"."<<recovery[n].name<<" x"<<recovery[n].amt<<endl; 
				  }
	cout<<">";
	cin>>k_;
	switch(k_)
	{	case 1:if(recovery[1].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[1]);  invopt(recovery[1]); }
				if(Inbattle==1){ turn--; invdisp(recovery[1]);  invopt(recovery[1]); } return 0;}
			else{cout<<"You do not have this item"; }

		case 2:if(recovery[2].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[2]);  invopt(recovery[2]); }
				if(Inbattle==1){ turn--; invdisp(recovery[2]);  invopt(recovery[2]); } return 0;}
			else{cout<<"You do not have this item"; }

		case 3:if(recovery[3].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[3]);  invopt(recovery[3]); }
				if(Inbattle==1){ turn--; invdisp(recovery[3]);  invopt(recovery[3]);} return 0;}
			else{cout<<"You do not have this item"; }
		
		case 4:if(recovery[4].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[4]);  invopt(recovery[4]); }
				if(Inbattle==1){ turn--; invdisp(recovery[4]);  invopt(recovery[4]);} return 0;}
			else{cout<<"You do not have this item"; }

    		case 5:if(recovery[5].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[5]);  invopt(recovery[5]); }
				if(Inbattle==1){ turn--; invdisp(recovery[5]);  invopt(recovery[5]);} return 0;}
			else{cout<<"You do not have this item"; }
		
		case 6:if(recovery[6].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[6]);  invopt(recovery[6]); }
				if(Inbattle==1){ turn--; invdisp(recovery[6]);  invopt(recovery[6]);} return 0;}
			else{cout<<"You do not have this item"; }
					
		case 7:if(recovery[7].amt>0){
				if(Inbattle==0)
				{clrscr();  invdisp(recovery[7]);  invopt(recovery[7]); }
				if(Inbattle==1){ turn--; invdisp(recovery[7]);  invopt(recovery[7]);} return 0;}
			else{cout<<"You do not have this item"; }
		
	}

	r=1;
	break;	

	}while(r==0 || r==1);
	return 0;
}


void invdisp(cons i)
{
	cout<<"--------------------------------------------------------------------------------";
	cout<<"\n\tItem: "<<i.name;
	cout<<"\n\tDescription: "<<i.descr;
}

void invopt(cons &i)
{   	int k_,y=0;
	cout<<"\n\t1.Use";
	cout<<"\t\t2.Remove";
	cout<<"\tx.Cancel\n\n\t> ";
	while(y!='1'||'2'||'x')
	{
		y=getch();
		char key=y;
		switch(y)
		{	case '1': cout<<key;
					  recprs(p1,i);
					  k_=getch();
					  return;
			case '2': cout<<key;
					  if(i.amt>0)
					  i.amt-=1;
					  cout<<"\n\tRemoved 1 "<<i.name;
					  k_=getch();
					  return;
			case 'x': cout<<key;
					  return;
		}
	}
	return;
}

int skillmenu()
{
	int x,k_,h=9,i=8,j,geti,r=0;
	do{
	r=0;
	geti=i;
	j=Skillset() + 7;
	clrscr();
	statbar();
	cout<<">> Menu ";
	cout<<">> Skills";
	cout<<"\n--------------------------------------------------------------------------------";
	cout<<endl;
	for(int n=1;n<=totalskills;n++)
	{	if(skill[n].av==1)
		{	cout<<"\t " <<n<<"."<<skill[n].name<<endl; }
	}
	cout<<">";
	cin>>k_;
 			
				if(Inbattle==1)  {
					skilldisp(skill[k_]);
					--turn;
					return 0;}
					 
				if(Inbattle==0)
				{if(strcmp(used,p1.name)!=0){return 1;}
						
					else
					{      	r=1;
						break;
					}
				}

	}while(r==0 || r==1);
	return 0;
}

void skilldisp(skills s)
{   int k_,key=0;
	cout<<"--------------------------------------------------------------------------------";
	cout<<"\n\tSkill: "<<s.name;
	cout<<"\n\tDescription: "<<s.descr;
	if(Inbattle==0)
	{  	if(s.occ!=1)
		{
			cout<<"\tz. Use\tx.Cancel";
			cout<<"> ";
			while(key!='x' || key!='z')
			{  	key=getch();
				if(key=='x')
				{ 	return; }
				else if(key=='z')
				{ 	clrscr();   statbar();
					if(strcmp(s.name,"Heal")==0)
					{	if(p1.mp<skill[2].req)
						{    cout<<"\n\n\n\t\tNot enough MP!";
							 k_=getch();
						}
						else
						{	Heal(p1);}
					}
					break;
				}
			}
		}
		//(1,2);  //1,2
		if(s.occ==1){	while((getch())!='x')
						{	}
					}
					//Leave this bit like this dont remove
		}
		else
		{ 
		cout<<"\tz. Use\tx.Cancel";
		cout<<"> ";
		while(key!='x' || key!='z')
		{ 	key=getch();
			if(key=='x')
			{	return; }
			else if(key=='z')
			{	if(p1.mp<skill[2].req)
				{        cout<<"Not enough MP!";
					 k_=getch();
				}
				else
				{	strcpy(used,s.name);}
				return;
			}
		}
	}
}

int Equipmenu()
{   int x,k_,r=0;
do{
r=0;
clrscr();
statbar();
cout<<">> Menu ";
cout<<">> Equip";
cout<<"\n--------------------------------------------------------------------------------"<<endl;
cout<<"\t1.Weapon:   < "<<p1.equips.weapon.name<<" >"<<endl;
cout<<"\t2.Armour:   < "<<p1.equips.armour.name<<" >"<<endl;
cout<<"\t3.Accesory: < "<<p1.equips.accessory.name<<" >"<<endl;
cout<<"Select the category"<<endl;
mn:;
cin>>k_; 
if(k_==1){

	for(int n=1;n<=totalequips;n++){
		if(eqitems[n].type==1)
			{cout<<"\n\t "<<n<<"."<<eqitems[n].name<<" x"<<eqitems[n].amt;}
				       }
	cout<<"\nSelct your weapon"<<endl;
	cout<<">";
	cin>>x;
	if(eqitems[x].amt>0){
	p1.equips.weapon=eqitems[x];
	eqitems[x].amt-=1;
	cout<<"You have equipped "<<eqitems[x].name<<endl;}
	else{cout<<"You do not have this item";}
	k_=getch();
	 }
else if(k_==2){
	for(int n=1;n<=totalequips;n++){
		if(eqitems[n].type==2)
			{cout<<"\n\t "<<n<<"."<<eqitems[n].name<<" x"<<eqitems[n].amt;}
				       }
	cout<<"\nSelct your armour"<<endl;
	cout<<">";
	cin>>x;
	if(eqitems[x].amt>0){
	p1.equips.armour=eqitems[x];
	eqitems[x].amt-=1;
	cout<<"You have equipped "<<eqitems[x].name<<endl;}
	else{cout<<"You do not have this item";}
	k_=getch();
	      }
else if(k_==3){
	for(int n=1;n<=totalequips;n++){
		if(eqitems[n].type==3)
			{cout<<"\n\t "<<n<<"."<<eqitems[n].name<<" x"<<eqitems[n].amt;}
				       }
	cout<<"\nSelct your weapon"<<endl;
	cout<<">";
	cin>>x;
	if(eqitems[x].amt>0){
	p1.equips.accessory=eqitems[x];
	eqitems[x].amt-=1;
	cout<<"You have equipped "<<eqitems[x].name<<endl;}
	else{cout<<"You do not have this item";}
	k_=getch();
	      }

else{ goto mn;}
if(Inbattle==1)
		{	if(strcmp(used,p1.name)!=0)
			{ return 0;}
			turn--;
			return 1;
		}
else{ r=1;}
if(r==1){break;}

allchk();
Equipset();

}while(r==1);

return 0;
}

void recprs(charst &x,cons &y)
{  cout<<endl;
   if(y.amt<=0)
   { 	cout<<"\n\tYou do not have enough "<<y.name<<"s.";
	cout<<endl;
   }
   else if(y.type==0)
   { 	if(x.hp==x.mhp)
	{	cout<<"\n\tHP is already full.";}
	else
	{
		x.hp+=y.use;
		if(x.hp>x.mhp)
		x.hp=x.mhp;
		cout<<"\n\n\tYou used a "<<y.name<<".";
		cout<<"\n\tCurrent HP= "<<x.hp<<"/"<<x.mhp;
		y.amt--;
	}
   }
   else if(y.type==1)
   { 	if(x.mp==x.mmp)
	{	cout<<"\n\tMP is already full.";}
	else
	{
		x.mp+=y.use;
		if(x.mp>x.mmp)
		x.mp=x.mmp;
		cout<<"\n\n\tYou used a "<<y.name<<".";
		cout<<"\n\tCurrent MP= "<<x.mp<<"/"<<x.mmp;
		y.amt--;
	}
   }
   else if(y.type==2)
   {	if(x.hp==x.mhp&&x.mp==x.mmp)
	{ 	cout<<"\n\tYou are in the best condition.";}
	else
	{
		x.hp+=y.use;
		x.mp+=y.use;
		if(x.hp>x.mhp)
		x.hp=x.mhp;
		if(x.mp>x.mmp)
		x.mp=x.mmp;
		cout<<"\tYou used a "<<y.name<<".";
		cout<<"\n\tCurrent HP= "<<x.hp<<"/"<<x.mhp;
		cout<<"\n\tCurrent MP= "<<x.mp<<"/"<<x.mmp;
		y.amt--;
	}
   }

}

void allchk()
{	lvchk(p1);
	modify(p1,modp1,p1base);
}

///////////////////////////////////////////////////////////////////////////////////////////
//			Battle Related

void battleopt(charst &enemy,int l);
void battledraw(charst a,charst b);
int chkdeath(charst& enemy);

void battleprs(charst b)
{
	int k_,l=11;
	charst *opp[2];
	Inbattle=1;
	cout<<"\n\n";
	cout<<"********************************************************************************";
	cout<<"\n\t\t";
	cout<<"<!> ";
	cout<<b.name;
	cout<<" appeared!";
	//(1);
	k_=getch();
	//(0);
	turn=1;
	while(b.hp!=0)
	{
		battledraw(p1,b);
		if(turn%2==0)
		{
			Attack(b,p1);
			chkdeath(b);
	}
	else
	{ 	strcpy(used,p1.name);
		battleopt(b,l);
		if(strcmp(used,skill[1].name)==0)
		{  Swing(p1,b);}
		else if(strcmp(used,skill[2].name)==0)
		{  Heal(p1,b);}
		else if(strcmp(used,skill[3].name)==0)
		{  Inferno(p1,b);}
		else if(strcmp(used,skill[4].name)==0)
		{  Freeze(p1,b);}
		else if(strcmp(used,skill[5].name)==0)
		{  Thunder(p1,b);}
		else if(strcmp(used,skill[6].name)==0)
		{  Water(p1,b);}
		else if(strcmp(used,skill[7].name)==0)
		{  Wind(p1,b);}
		else if(strcmp(used,skill[8].name)==0)
		{  Mage(p1,b);}
	}
	if((chkdeath(b))==0)
	{ 	p1.exp +=b.exp;
		cout<<"Gained Exp +"<<b.exp;
		break; }
	l=11;
	turn++;
	};
	//(1);
	k_=getch();
	//(0);
	int olv=p1.lv;
	allchk();
	if(p1.lv>olv)
	{	cout<<"\nYou leveled up.\t\tCurrent Lv: "<<p1.lv;
		k_=getch();
	}
	Inbattle=0;

};

void battleopt(charst &enemy,int l)
{
	int k_;
	cout<<"\n--------------------------------------------------------------------------------";
	cout<<"\n   1.Attack\t\n";
	cout<<"   2.Skill\t\n";
	cout<<"   3.Item\t\n";
	cout<<"   4.Wait\t"<<endl;
	cout<<">";
	re:;
	cin>>k_;
	do{
		if(k_==1)
		{ 	Attack(p1,enemy);
			break; }
		else if(k_==2)
		{	skillmenu();
			break; }
		else if(k_==3)
		{ 	inventory();
			break; }
		else if(k_==4)
		{ return; }

	}while(k_==1||k_==2||k_==3||k_==4);
	int r=l; r++;//Waste
};

void battledraw(charst a,charst b)
{
	if(a.hp<=0){ a.hp=0; }
	if(b.hp<=0){ b.hp=0; }
	if(b.hp!=0){ clrscr(); }

	cout<<"\n"<<"< "<<b.name<<" >";   
	cout<<"  HP="<<b.hp<<"/"<<b.mhp;
	cout<<"  MP="<<b.mp<<"/"<<b.mmp;
	cout<<"\n\n\n ";
	cout<<"\n"<<"< "<<a.name<<" >";   
	cout<<"  HP="<<a.hp<<"/"<<a.mhp;
	cout<<"  MP="<<a.mp<<"/"<<a.mmp;
	
}


int chkdeath(charst &enemy)
{   int k_; 
    if(p1.hp<=0)
   {	battledraw(p1,enemy);
	cout<<"\n"<<p1.name<<" fainted.";
	k_=getch();
	Gameover();
   }
   if(enemy.hp<=0)
   {  	battledraw(p1,enemy);
	cout<<"\n"<<enemy.name<<" fainted.";
	return 0;
   }
   return 1;
}

//////////////////////////////////////////////////////////////////////////////////////
//			Skills

int Attack(charst &a,charst &b)
{	int dmg,k_;
	dmg= (a.atk * 4) - (b.def * 2);
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	b.hp-=dmg;
	cout<<"\n"<<a.name<<" dealt "<<dmg<<" damage.";
	if(b.hp<=0){ b.hp=0;}
	k_=getch();
	return 0;
}

int Swing(charst &a,charst &b)//skill 1
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 2 * ((a.atk * 4) - (b.def * 2));
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	b.hp-=dmg;
	a.hp-=(a.mhp/10);
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[1].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Heal(charst& a,charst& b)//skill 2
{   	int heal,k_;
	if(a.mp<=0){ a.mp=0;}
	if(a.hp>=a.mhp)
	{ 	cout<<"HP is already Full";
		a.mp+=skill[2].req;
		return 0;
	}
	heal= 200+ a.mat;
	randomdmg(heal,20);
	a.hp+=heal;
	if(a.hp>=a.mhp){ a.hp=a.mhp;}
	a.mp-=skill[2].mpcost;
	if(Inbattle==1)
	{
		clrscr();
		battledraw(a,b);
	}
	cout<<"\n"<<a.name<<" used <"<<skill[2].name<<">  Recovered "<<heal<<" HP.";
	k_=getch();
	return 0;
};

int Inferno(charst& a,charst& b)//skill 3
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 100 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Inferno")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[3].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[3].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Freeze(charst& a,charst& b)//skill 4
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 150 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Freeze")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[4].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[4].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Thunder(charst& a,charst& b)//skill 5
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 200 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Thunder")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[5].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[5].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Water(charst& a,charst& b)//skill 6
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 300 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Water")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[6].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[6].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Wind(charst& a,charst& b)//skill 7
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 300 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Wind")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[7].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[7].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}

int Mage(charst& a,charst& b)//skill 8
{   	int dmg,k_;
	if(a.mp<=0){ a.mp=0;}
	dmg= 400 + (a.mat * 2) - (b.mdf * 2) ;
	randomdmg(dmg,20);
	if(dmg<=0){ dmg=0;}
	if( strcmp(b.weak,"Light")==0 ){ dmg*=2;}
	b.hp-=dmg;
	a.mp-=skill[8].mpcost;
	clrscr();
	battledraw(a,b);
	cout<<"\n"<<a.name<<" used <"<<skill[8].name<<"> and dealt "<<dmg<<" damage.";
	k_=getch();
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////
//			Process Main

int game()
{ 	clrscr();
	int k_,key,brk=0;
	do{
		statbar();
		cout<<"\n    'h' to open HELP";
		cout<<"\n    'e' to go to Quit";
		cout<<"\n    'x' to open Menu";
		cout<<"\n    'z' to progress";
		key=getch();
		switch (key)
		{	case 'x': mainmenu();
				break;
			case 'e' : clrscr();
				  cout<<"\n\n\n\n\t\t\t\tPress any key...";
				  k_=getch();
				  exit(0);
			case 'h': Help();
				  break;
			case 'z' : brk=1;
				  break;
		}
		if(brk==1){ break;}
	}while(key!='x' ||key!='h' ||key!='e' ||key!='z' ||key!='c');
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//				HELP

void Help()
{	clrscr();
	int k_;
	cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Help>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	cout<<"\n\n\tPress 'x' to open Menu or Cancel.\n";
	cout<<"\tPress 'z' or 'Enter' to Accept.\n";
	cout<<"\tWhile Searching in the dungeon press the Search button(3) again\n";
	cout<<"\tand again until you are notified of something.\n";
	cout<<"\n\tPlayer stats are stored in the text file 'Play'";
	k_=getch();
}

////////////////////////////////////////////////////////////////////////////////////////