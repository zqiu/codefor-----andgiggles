#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <string.h>

void huffmanencode(std::istream &inFile,std::ostream &outFile);
void huffmandecode(std::istream &inFile,std::ostream &outFile);

int main(int argc, char *argv[]){
	char out[50],test;
	if( argc != 2 && argc != 3){
        std::cerr << "Usage: " << argv[ 0 ] << " text file" << std::endl;
        return 1;
    }
	std::ifstream inFile(argv[1]);
	if( !inFile ){
        std::cerr << "Cannot open " << argv[ 1 ] << std::endl;
        return 1;
    }
	if(argc == 2){
		strcpy(out,argv[1]);
		strcat(out,".huf");
		std::ofstream outFile(out);
		huffmanencode(inFile,outFile);
	}else if(!strcmp(argv[2],"--decode")){
		strcpy(out,argv[1]);
		strcat(out,".dec");
		std::ofstream outFile(out);
//		huffmandecode(inFile,outFile);
	}
	return 0;
}

void huffmanencode(std::istream &inFile,std::ostream &outFile){
	int max=0,i,j,numwritten = 0,cha = 0;
	char temp;
	std::map<char,int> charmap;
	std::map<char,int>::iterator it;
	std::vector<std::pair<char,int> > find;
	std::string str;
	
	while(getline(inFile,str)){
		for(i = 0; i < str.size() ; ++i){
			charmap[str[i]]++;
		}
		charmap['\n']++;
	}
	while(charmap.size()){
		for(it = charmap.begin(); it != charmap.end(); ++it){
			if(it->second > max){
				max = it->second;
				temp = it->first;
			}
		}
		find.push_back(std::pair<char,int>(temp,max));
		max = 0;
		charmap.erase(temp);
	}
	std::cout << "characters:\n";
	for(i = 0; i < find.size(); ++i){
		std::cout << find[i].first;
	}
	
}