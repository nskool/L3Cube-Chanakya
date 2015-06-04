#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>

using namespace std;

class HTTP{
	vector <string> ips;
	vector <string> ips_1;


public:
	void Display_Info();
	void Analyse(char*);

};

bool date_change(int current,int past){
	if(current==past)
		return false;
	else
		return true;

}
void HTTP::Display_Info(){
cout<<"***********************************"<<endl;
cout<<"*The format of the weblog is \'NCSA Combine logfile format\'"<<endl;
cout<<"*Meaning of the analytical terms: "<<endl;
cout<<"*Hit: Any request to the server, which is logged."<<endl;
cout<<"*Number of Sites: Count of unique client ip addresses "<<endl;
cout<<"*Pages: HTML documents, that are exclusive of web content such as images, audio or video content"<<endl;
cout<<"*Number of files: The files that were successfully returned to the client"<<endl;
cout<<"*Total number of bytes: Size of data being exchanged"<<endl;
}


void HTTP::Analyse(char *filepath){
	ifstream file(filepath);
//	string s;
	int total_hits=0;
	int counter=0;
	int files=0;
	int databytes=0,twibuzz=0,recipewithme=0,kinneryandrajan=0;
	int hits_206=0,hits_200=0,hits_404=0,hits_304=0,hits_500=0,hits_302=0;
	int max_hits=0,max_files=0,max_sites=0,max_kb_files=0;//per day
	int count_hits=0,count_files=0,count_sites=0,count_kb_files=0;//per day
	int date_current=0,date_past=14,num_of_days=0;
	bool date_chg=false;

	string s,temp;
	int flag=0,flag_1=0;
while(!file.eof()){
	file>>s;

	for(unsigned int i=0;i<ips.size();i++){
		if(strcmp(s.c_str(),ips.at(i).c_str())==0){
			flag=1;
			}
		}

	if(flag!=1){
		ips.push_back(s);
	}
	flag=20;
	for(unsigned int i=0;i<ips_1.size();i++){
			if(strcmp(s.c_str(),ips_1.at(i).c_str())==0){
				flag_1=1;
				}
			}

		if(flag_1!=1){
			ips_1.push_back(s);
		}
		flag_1=20;


	file>>s;
	file>>s;
	file>>s;
	date_current=atoi(&s[1]);
	file>>s;
	file>>s;
	file>>s;

	date_chg=date_change(date_current,date_past);


	//pagevisit
	if((s.find("www.")!= std::string::npos)&&(s.find(".jpg")!= std::string::npos ||s.find(".png")!= std::string::npos ||s.find(".gif")!= std::string::npos ||s.find(".jpeg")!= std::string::npos)){
		counter++;
		//cout<<s<<endl;
	}

	//hits persites
	if(s.find("twibuzz.com")!= std::string::npos){
		twibuzz++;
	}
	if(s.find("kinneryandrajan.com")!= std::string::npos){
			kinneryandrajan++;
		}
	if(s.find("recipewithme.com")!= std::string::npos){
		recipewithme++;
		}


	file>>s;
	file>>s;

	count_hits++;
	if(s.find("200")!= std::string::npos){
		files++;
		hits_200++;
		count_files++;
	}
	if(s.find("206")!= std::string::npos){
		hits_206++;
	}
	if(s.find("404")!= std::string::npos){
		hits_404++;
	}
	if(s.find("302")!= std::string::npos){
		hits_302++;
	}
	if(s.find("304")!= std::string::npos){
		hits_304++;
	}
	if(s.find("500")!= std::string::npos){
		hits_500++;
	}

	file>>s;//databytes
	databytes+=atoi(s.c_str());
	count_kb_files+=atoi(s.c_str());
	if(date_chg){
		num_of_days++;
		max_files=(max_files>count_files ? max_files : count_files);
		count_files=0;
		max_hits=(max_hits>count_hits ? max_hits : count_hits);
		count_hits=0;
		max_kb_files=(max_kb_files>count_kb_files ? max_kb_files : count_kb_files);
		count_kb_files=0;

		count_sites=ips_1.size()-1;
		max_sites=(max_sites>count_sites ? max_sites : count_sites);
		ips_1.clear();
	}
getline(file,s);
date_past=date_current;
}
total_hits=twibuzz+kinneryandrajan+recipewithme;
cout<<setprecision(2)<<fixed;
cout<<"***********************************"<<endl;
cout<<"Analysis: "<<endl;
cout<<"***********************************"<<endl;
cout<<"Number of sites:\t\t "<<ips.size()<<endl;
cout<<"Total \'pages\' accessed :  \t " <<total_hits-counter<<endl;
cout<<"Number of files :  \t\t "<<files<<endl<<"Total number of bytes exchanged: "<<databytes/1024<<" Kbytes"<<endl;
cout<<"***********************************"<<endl;
cout<<"Hits per site:  \t"<<endl<<"www.recipewithme.com\t"<<recipewithme<<endl<<"www.kinneryandrajan.com\t"<<kinneryandrajan<<endl<<"www.twibuzz.com\t\t"<<twibuzz<<endl;
cout<<"Total hits (Verification by adding individual site hits): "<<total_hits<<endl;


cout<<"***********************************"<<endl;
cout<<"Hits by Response Code:"<<"\t\tCount"<<"\tPercentage"<<endl;
cout<<"Code 200-OK:\t\t\t"<<hits_200<<"\t"<<(hits_200/(float)total_hits)*100<<"%"<<endl;
cout<<"Code 206-Partial Content:\t"<<hits_206<<"\t"<<(hits_206/(float)total_hits)*100<<"%"<<endl;
cout<<"Code 302-Found:\t\t\t"<<hits_302<<"\t"<<(hits_302/(float)total_hits)*100<<"%"<<endl;
cout<<"Code 304-Not modified: \t\t"<<hits_304<<"\t"<<(hits_304/(float)total_hits)*100<<"%"<<endl;
cout<<"Code 404-Not found: \t\t"<<hits_404<<"\t"<<(hits_404/(float)total_hits)*100<<"%"<<endl;
cout<<"Code 500-Internal Server Error: "<<hits_500<<"\t"<<(hits_500/(float)total_hits)*100<<"%"<<endl;
cout<<"***********************************"<<endl;

cout<<"\t\tMaximum Observed"<<"\tAverage"<<endl;
cout<<"Files per day: \t\t"<<max_files<<"\t\t"<<files/num_of_days<<endl;
cout<<"Hits per day: \t\t"<<max_hits<<"\t\t"<<total_hits/num_of_days<<endl;
cout<<"Sites per day: \t\t"<<max_sites<<"\t\t"<<ips.size()/num_of_days<<endl;
cout<<"kb files per day: \t"<<max_kb_files/1024<<"\t\t"<<databytes/(1024*num_of_days)<<endl;
cout<<"***********************************"<<endl;
}


int main(int argc,char * argv[]){
	if(argc==2){
		HTTP h;
		h.Display_Info();
		h.Analyse(argv[1]);
	}else {
		cout<<"Please provide log file path as argument"<<endl;
		exit(1);
	}

}
