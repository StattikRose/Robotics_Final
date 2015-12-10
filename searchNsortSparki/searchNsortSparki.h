struct item{
	int locationX;
	int locationY;
	String color;
};

struct colors{
	int pinkIR;
	int whiteIR;
	int blueIR;
	int pinkColor;
	int whiteColor;
	int blueColor;
};

const struct map{
	const int mapsizeX = 46;
	const int mapsizeY = 56;
	const int pinkBinY = 13;
	const int whiteBinY = 37;
	const int blueBinY = 61;
};

class searchNsort{
public:
	void state1();
	void state2();
	void state3();
	void state4();
	void state5();
	void party();

	void printLocation();
	int getCurrentTheta();
	void setFound();
	int state;

private:
	int lookAround();
	int readColor();
	void classifyColor();
	int state0();
	int findLine();
	void updateOdometry(int moveX, int moveY, int moveTheta);
	void updateOdometry(int moveX, int moveY);
	void updateOdometry(int moveTheta);

	int currentX = 0;
	int currentY = 0;
	int currentTheta = 0;
	int foundX;
	int foundY;
	int foundTheta;
	int usTheta;
	int usDist;
	
	item foundItem;
	map fullMap;
	colors theColors;
};

