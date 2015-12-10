struct item{
	int locationX;
	int locationY;
	String color;
};

const struct map{
	const int mapsizeX = 46;
	const int mapsizeY = 56;
	const int pinkBinY;
	const int whiteBinY;
	const int blueBinY;
};

class searchNsort{
public:
	void state1();
	void state2();
	void state3();
	void state4();
	void state5();
	void party();

private:
	int currentX = 0;
	int currentY = 0;
	int theta = 0;
	int foundX;
	int foundY;
	int foundTheta;
	
	map fullMap;
};

