#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <string.h>

void huffmanencode(std::istream &inFile,std::ostream &outFile);
void huffmandecode(std::istream &inFile,std::ostream &outFile);
void huffmanencodew(std::istream &inFile,std::ostream &outFile);
void huffmandecodew(std::istream &inFile,std::ostream &outFile);

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
		inFile >> test;
		if(!test){
			huffmandecode(inFile,outFile);
		}else if(test == (char)1){
			huffmandecodew(inFile,outFile);			
		}else{
			std::cerr << "not a huffman file";
			return 1;
		}
	}else if(!strcmp(argv[2],"--word")){
		strcpy(out,argv[1]);
		strcat(out,".huf");
		std::ofstream outFile(out);
		huffmanencodew(inFile,outFile);
	}
	return 0;
}

void huffmanencode(std::istream &inFile,std::ostream &outFile){
	int max=0,i,j,numwritten = 0,cha = 0;
	char temp;
	std::map<char,int> charmap;
	std::map<char,int>::iterator it;
	std::vector<char> find;
	std::string str;
	outFile << (char)0;
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
		max = 0;
		find.push_back(temp);
		charmap.erase(temp);
	}
	std::cout << "characters:\n";
	for(i = 0; i < find.size(); ++i){
		std::cout << find[i];
		outFile << find[i];
	}
	outFile << (char)0;
	std::cout << "\nend\n";
	inFile.clear();
	inFile.seekg(0,inFile.beg);
	temp = 0;
	while(getline(inFile,str)){
		for(i = 0; i < str.size(); ++i){
			for(j = 0; j < find.size(); ++j){
				if(str[i] == find[j]){
					for(;j > 0; j--){
						numwritten = (numwritten + 1)%7;
						temp *= 2;
						temp += 1;
						if(!numwritten){
							outFile << temp;
							temp = 0;
						}
					}
					temp *= 2;
					numwritten = (numwritten + 1)%7;
					if(!numwritten){
						outFile << temp;
						temp = 0;
					}
					break;
				}
			}
		}
		for(j = 0; j < find.size(); ++j){
			if(find[j] == '\n'){
				for(;j > 0; j--){
					numwritten = (numwritten + 1)%7;
					temp *= 2;
					temp += 1;
					if(!numwritten){
						outFile << temp;
						temp = 0;
					}
				}
				temp *= 2;
				numwritten = (numwritten + 1)%7;
				if(!numwritten){
					outFile << temp;
					temp = 0;
				}
				break;
			}
		}
	}
	while(numwritten){
		temp *= 2;
		temp += 1;
		numwritten = (numwritten + 1)%7;
	}
	outFile << temp;
}

void huffmandecode(std::istream &inFile,std::ostream &outFile){
	std::vector<char> find;
	std::string str;
	bool exit = false;
	int i,j,temp = 0;
	while(getline(inFile,str)){
		for(i = 0; i < str.size(); ++i){
			if(str[i] == (char)0){
				exit = true;
				break;
			}
			find.push_back(str[i]);
		}
		if(exit){
			break;
		}
		find.push_back('\n');
	}
	for(++i; i < str.size(); ++i){
		for(j = 6; j >= 0; --j){
			if((str[i] >> j)%2){
				++temp;
			}else{
				outFile << find[temp];
				temp = 0;
			}
		}
	}
}

void huffmandecodew(std::istream &inFile,std::ostream &outFile){
	std::vector<std::string> find;
	std::string str,tmp;
	bool exit = false;
	int i,j,temp = 0;
	while(getline(inFile,str)){
		for(i = 0; i < str.size(); ++i){
			if(str[i] == (char)0){
				exit = true;
				break;
			}
			if(str[i] == ','){
				find.push_back(tmp);
				tmp = "";
			}else{
				tmp += str[i];
			}
		}
		if(exit){
			break;
		}
		find.push_back("\n");
	}
	for(++i; i < str.size(); ++i){
		for(j = 6; j >= 0; --j){
			if((str[i] >> j)%2){
				++temp;
			}else{
				outFile << find[temp];
				if(find[temp] != "\n"){
					outFile << " ";
				}
				temp = 0;
			}
		}
	}
}


void huffmanencodew(std::istream &inFile,std::ostream &outFile){
	int max=0,i,j,numwritten = 0,cha = 0;
	std::map<std::string,int> charmap;
	std::map<std::string,int>::iterator it;
	std::vector<std::string> find;
	std::string str,temp = "";
	char towrite;
	outFile << (char)1;
	while(getline(inFile,str)){
		for(i = 0; i < str.size() ; ++i){
			if(str[i] == ' '){
				charmap[temp]++;
				temp = "";
			}else{
				temp += str[i];
			}
		}
		charmap[temp]++;
		temp = "";
		charmap["\n"]++;
	}
	while(charmap.size()){
		for(it = charmap.begin(); it != charmap.end(); ++it){
			if(it->second > max){
				max = it->second;
				temp = it->first;
			}
		}
		max = 0;
		find.push_back(temp);
		charmap.erase(temp);
	}
	std::cout << "words:\n";
	for(i = 0; i < find.size(); ++i){
		std::cout << find[i] << '\n';
		outFile << find[i];
		if(find[i] != "\n"){
			outFile << ',';
		}
	}
	outFile << (char)0;
	std::cout << "\nend\n";
	inFile.clear();
	inFile.seekg(0,inFile.beg);
	towrite = 0;
	temp = "";
	while(getline(inFile,str)){
		for(i = 0; i < str.size(); ++i){
			if(str[i] != ' '){
				temp += str[i];
			}else{
				for(j = 0; j < find.size(); ++j){
					if(temp == find[j]){
						for(;j > 0; j--){
							numwritten = (numwritten + 1)%7;
							towrite *= 2;
							towrite += 1;
							if(!numwritten){
								outFile << towrite;
								towrite = 0;
							}
						}
						towrite *= 2;
						numwritten = (numwritten + 1)%7;
						if(!numwritten){
							outFile << towrite;
							towrite = 0;
						}
						break;
					}
				}
				temp = "";
			}
		}
		for(j = 0; j < find.size(); ++j){
			if(temp == find[j]){
				for(;j > 0; j--){
					numwritten = (numwritten + 1)%7;
					towrite *= 2;
					towrite += 1;
					if(!numwritten){
						outFile << towrite;
						towrite = 0;
					}
				}
				towrite *= 2;
				numwritten = (numwritten + 1)%7;
				if(!numwritten){
					outFile << towrite;
					towrite = 0;
				}
				break;
			}
		}
		temp = "";
		for(j = 0; j < find.size(); ++j){
			if(find[j] == "\n"){
				for(;j > 0; j--){
					towrite = (numwritten + 1)%7;
					towrite *= 2;
					towrite += 1;
					if(!numwritten){
						outFile << towrite;
						towrite = 0;
					}
				}
				towrite *= 2;
				numwritten = (numwritten + 1)%7;
				if(!numwritten){
					outFile << towrite;
					towrite = 0;
				}
				break;
			}
		}
	}
	while(numwritten){
		towrite *= 2;
		towrite += 1;
		numwritten = (numwritten + 1)%7;
	}
	outFile << towrite;
}