struct tpRoll{
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

struct my_map{
	static const int mapsizeX = 46;
	static const int mapsizeY = 56;
	static const int pinkBinY = 13;
	static const int whiteBinY = 37;
	static const int blueBinY = 61;
};

class searchNsort{
public:
	int state0();
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
	int findLine();
	void updateOdometry(int moveX, int moveY, int moveTheta);
	void updateOdometry(int moveX, int moveY);
	void updateOdometry(int moveTheta);

	int currentX;
	int currentY;
	int currentTheta;
	int foundX;
	int foundY;
	int foundTheta;
	int usTheta;
	int usDist;
	
	tpRoll foundItem;
	my_map fullMap;
	colors theColors;
};

