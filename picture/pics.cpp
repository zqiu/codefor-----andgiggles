#include "random.h"
#include <cstdlib>
#include <cstring>

class Color{
	public:
		Color(unsigned char red=255, unsigned char green=255, unsigned char blue=255) : r(red),g(green),b(blue) {}
		bool isWhite() const { return r==255 && g==255 && b==255; }
		unsigned char r,g,b;
  
		bool operator==(const Color &comp)const{return r==comp.r && g==comp.g && b==comp.b;}
		bool operator!=(const Color &comp)const{return !(*this==comp);}
};

class Image{
	public:
		Image() : width(0), height(0), data(NULL) {}
		Image(const Image &image) : data(NULL) { 
			copy_helper(image); 
		}	
		const Image& operator=(const Image &image) { 
			if (this != &image)
				copy_helper(image);
			return *this; 
		}
		~Image() {
			delete [] data; 
		}
		void Allocate(int w, int h) {
			width = w;
			height = h;
			delete [] data;
			if (width == 0 && height == 0) {
				data = NULL;
			} else {
				//assert (width > 0 && height > 0);
				data = new Color[width*height]; 
			}
		}
		int Width() const { return width; }
		int Height() const { return height; }
		Color& GetPixel(int x, int y) const {
			//assert(x >= 0 && x < width);
			//assert(y >= 0 && y < height);
			return data[y*width + x]; 
		}  
		void SetPixel(int x, int y, const Color &value) {
			//assert(x >= 0 && x < width);
			//assert(y >= 0 && y < height);
			data[y*width + x] = value; 
		}
	private:
		void copy_helper(const Image &image) {
			Allocate (image.Width(), image.Height());
			for (int i = 0; i < image.Width(); i++) {
				for (int j = 0; j < image.Height(); j++) {
					this->SetPixel(i,j,image.GetPixel(i,j));
				}
			}
		}
		int width;
		int height;
		Color *data;
};

bool save(char *filename, Image & hashmap, int size){
	int i,j;
	Color temp;
	FILE *file = fopen(filename, "wb");
	if (file == NULL) {
		std::cerr << "Unable to open " << filename << " for writing\n";
		return false;
	}
	 // misc header information
	fprintf (file, "P6\n");
	fprintf (file, "%d %d\n", hashmap.Width()*size,hashmap.Height()*size);
	fprintf (file, "255\n");
	for(i = 0; i < hashmap.Width()*size; ++i){
		if(i%1000 == 0){
			std::cout << i << "\n";
		}
		for(j = 0; j < hashmap.Height()*size; ++j){
			temp = hashmap.GetPixel(i/size,j/size);
			fputc ((unsigned char)(temp.r),file);
			fputc ((unsigned char)(temp.g),file);
			fputc ((unsigned char)(temp.b),file);
		}
	}
	fclose(file);
	return true;
}

int main(int argc, char **argv){
	if(argc != 5){
		std::cerr << "need 4 arguments: start file, number of files, number of squares, size of each square \n";
		exit(EXIT_FAILURE);
	}
	int i,j,k,arg2 = atoi(argv[2]),arg3 = atoi(argv[3]),arg4 = atoi(argv[4]);
	char filename[15],ppm[15],png[15],temp[15],command[60];
	MTRand rand;
	Image hashmap;
	hashmap.Allocate(arg3,arg3);
	for(k = 0; k < arg2; ++k){
		strcpy(filename,"pic");
		sprintf(temp,"%d",atoi(argv[1])+ k);
		strcat(filename,temp);
		strcpy(ppm,filename);
		strcpy(png,filename);
		strcat(ppm,".ppm");		
		strcat(png,".jpg");
		for(i = 0; i < arg3; ++i){
			for(j = 0; j < arg3; ++j){
				hashmap.SetPixel(i,j,Color(rand.randInt(254),rand.randInt(254),rand.randInt(254)));
			}
		}
		save(ppm,hashmap,arg4);
		strcpy(command, "convert ");
		strcat(command, ppm);
		strcat(command, " ");
		strcat(command, png);
		strcat(command, "\r");
		std::cout << command << "\n";
		system(command);
		if(k%20 == 19){
			system("rm *.ppm");
		}
	}
	system("rm *.ppm");
	return EXIT_SUCCESS;
}