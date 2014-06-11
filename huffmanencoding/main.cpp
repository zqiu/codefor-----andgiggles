#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <string.h>
#include <queue>
#include <stdlib.h>

class node;
class mycomparison;
void huffmanencode(std::istream &inFile,std::ostream &outFile);
void huffmandecode(std::istream &inFile,std::ostream &outFile);
node * maketree(std::vector<std::pair<char,int> > & data);
void freetree(node * root);
void gethash(std::map<char, std::string> & hashmap,node* root,std::string buffer);
void printtree(node *root,int index);
void printhash(std::map<char, std::string> & hashmap);

class node{
	public:
		node(char val_, int priority_):val(val_),priority(priority_){
			left = NULL;
			right = NULL;
		}
		char val;
		int priority;
		node * left , * right;
};

class mycomparison{
public:
  bool operator() (const node* lhs, const node* rhs) const{
    return (lhs->priority > rhs->priority);
  }
};

typedef std::priority_queue<node *,std::vector<node *>,mycomparison> node_queue;

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
		inFile.close();
		inFile.open(argv[1],std::ifstream::binary);
		strcpy(out,argv[1]);
		strcat(out,".dec");
		std::ofstream outFile(out);
		huffmandecode(inFile,outFile);
	}
	return 0;
}

void huffmanencode(std::istream &inFile,std::ostream &outFile){
	int max = 0,numwritten = 0,i,j;
	char temp,towrite = 0;
	std::map<char,int> charmap;
	std::map<char,int>::iterator it;
	std::map<char,std::string> hash;
	std::vector<std::pair<char,int> > find;
	std::string str,encoded;
	node * root, *tmp;
	
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
		outFile << find[i].first << find[i].second << (char)1;
	}
	outFile << (char)0;
	std::cout << "\nend\n";
	inFile.clear();
	inFile.seekg(0,inFile.beg);
	root = maketree(find);
	printtree(root,0);
	gethash(hash,root,"");
	printhash(hash);
	
	while(getline(inFile,str)){
		for(i = 0; i < str.size(); ++i){
			encoded = hash.find(str[i])->second;
			for(j = 0; j < encoded.size(); ++j){
				numwritten = (numwritten + 1)%7;
				towrite *= 2;
				towrite += (encoded[j]=='0')?0:1;
				if(!numwritten){
					outFile << towrite;
					towrite = 0;
				}
			}
		}
		encoded = hash.find('\n')->second;
		for(j = 0; j < encoded.size(); ++j){
			numwritten = (numwritten + 1)%7;
			towrite *= 2;
			towrite += (encoded[j]=='0')?0:1;
			if(!numwritten){
				outFile << towrite;
				towrite = 0;
			}
		}
	}
	while(numwritten){
		temp *= 2;
		numwritten = (numwritten + 1)%7;
	}
	outFile << temp;
	freetree(root);
}

void huffmandecode(std::istream &inFile,std::ostream &outFile){
	int i,j,temp;
	std::vector<std::pair<char,int> > data;
	std::string str;
	char *buffer, *copy;
	node * root, *current;
	
	getline(inFile,str,'\x00');
	buffer = new char[str.length()+1];
	strcpy(buffer,str.c_str());
	copy = strtok(buffer,"\x01");
	while(copy != NULL){
		temp = copy[0];
		copy++;
		data.push_back(std::pair<char,int>(temp,atoi(copy)));
		copy = strtok(NULL,"\x01");
	}
	delete[] buffer;	
	root = maketree(data);
	current = root;
	printtree(root,0);
	temp = inFile.get();
	while(temp != std::char_traits<char>::eof()){
		for(j = 6; j >= 0; --j){
			if((temp >> j)%2){
				current = current->right;
			}else{
				current = current->left;
			}
			if(current->val != (char)0){
//				std::cout << "wrote" << current->val << "\n";
				outFile << (current->val);
				current = root;
			}
		}
		temp = inFile.get();
	}
	freetree(root);
}

node * maketree(std::vector<std::pair<char,int> > & data){
	node *root, *left, *right;
	node_queue queue;
	int i;
	for(i = 0; i < data.size(); ++i){
		queue.push(new node(data[i].first,data[i].second));
	}
	while(queue.size() > 1){
		left = queue.top();
		queue.pop();
		right = queue.top();
		queue.pop();
		root = new node((char)0,left->priority + right->priority);
		root->left = left;
		root->right = right;
		queue.push(root);
	}
	return queue.top();
}

void freetree(node * root){
	if(!(root->left) && !(root->right)){
		delete root;
	}else{
		freetree(root->left);
		freetree(root->right);
		delete root;
	}
	root = NULL;
}

void gethash(std::map<char, std::string> & hashmap,node* root,std::string buffer){
	if(!(root->left) && !(root->right)){
		hashmap.insert(std::pair<char, std::string>(root->val,buffer));
	}else{
		gethash(hashmap,root->left,buffer+"0");
		gethash(hashmap,root->right,buffer+"1");
	}
}
//debugging information
void printtree(node *root,int index){
	int i;
	if(root != NULL){
		for(i = 0; i < index; ++i){
			std::cout << " ";
		}
		std::cout << "val: ";
		if(root-> val == '\n' || root->val == (char)0){
			std::cout << "NULL ";
		}else{
			std::cout << root->val;
		}
		std::cout << "priority: " << root->priority << "\n";
		printtree(root->left,index+1);
		printtree(root->right,index+1);
	}
}

void printhash(std::map<char, std::string> & hashmap){
	std::map<char, std::string>::iterator itr;
	for(itr = hashmap.begin(); itr != hashmap.end(); ++itr){
		std::cout << itr->first << "=>" << itr->second << "\n";
	}
}