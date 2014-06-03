#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <string.h>
#include <queue>

class node;
class mycomparison;
void huffmanencode(std::istream &inFile,std::ostream &outFile);
void huffmandecode(std::istream &inFile,std::ostream &outFile);
node * maketree(std::vector<std::pair<char,float> > & data);
void freetree(node * root);
void gethash(std::map<char, std::string> & hashmap,node* root,std::string buffer);

class node{
	public:
		node(char val_, float priority_):val(val_),priority(priority_){
			left = NULL;
			right = NULL;
		}
		char val;
		float priority;
		node * left , * right;
};

class mycomparison{
public:
  bool operator() (const node* lhs, const node* rhs) const{
    return (lhs->priority < rhs->priority);
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
		strcpy(out,argv[1]);
		strcat(out,".dec");
		std::ofstream outFile(out);
//		huffmandecode(inFile,outFile);
	}
	return 0;
}

void huffmanencode(std::istream &inFile,std::ostream &outFile){
	int absmax = 0,max=0,i;
	char temp;
	std::map<char,int> charmap;
	std::map<char,int>::iterator it;
	std::map<char,std::string> hash;
	std::vector<std::pair<char,float> > find;
	std::string str;
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
				absmax = (max > absmax)?max:absmax;
			}
		}
		find.push_back(std::pair<char,float>(temp,((float)max)/absmax));
		max = 0;
		charmap.erase(temp);
	}
	std::cout << "characters:\n";
	for(i = 0; i < find.size(); ++i){
		std::cout << find[i].first;
		outFile << find[i].first << (char)1 << std::setprecision(4) << find[i].second << (char)1;
	}
	outFile << (char)0;
	std::cout << "\nend\n";
	inFile.clear();
	inFile.seekg(0,inFile.beg);
	root = maketree(find);
	
}

node * maketree(std::vector<std::pair<char,float> > & data){
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
	if(!root->left && !root->right){
		delete root;
	}else{
		freetree(root->left);
		freetree(root->right);
		delete root;
	}
	root = NULL;
}

void gethash(std::map<char, std::string> & hashmap,node* root,std::string buffer){
	
}
