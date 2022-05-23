#include <iostream>
#include <cstdint>
#include "math.h"

#define DEFAULT_ROWS 40
#define DEFAULT_COLS 80
#define PI 3.14159265
#define U_FULL_BLOCK "\u2588"
//#define U_LOWH_BLOCK "\u2584"
#define U_LOWH_BLOCK "\u2584"
#define U_UPPH_BLOCK "\u2580"

#define RED	"\x1B[31m"
#define GRN	"\x1B[32m"
#define YLW	"\x1B[33m"
#define BLU	"\x1B[34m"
#define MAG	"\x1B[35m"
#define CYN	"\x1B[36m"
#define WHT	"\x1B[37m"
#define RST	"\x1B[0m"
//		"\x1b[48;2;155;100;69mTRUECOLOR\x1b[0m\n"
#define FC_S	"\x1b[38;2;%u;%u;%um\x1b[48;2;%u;%u;%um%s\x1b[0m"
#define FC_FS	"\x1b[38;2;%u;%u;%um%s\x1b[0m"
#define FC_C	"\x1b[38;2;%u;%u;%um\x1b[48;2;%u;%u;%um%c\x1b[0m"
#define FC_FC	"\x1b[38;2;%u;%u;%um%c\x1b[0m"

const char* g_clr[] = {RST, RED, WHT, YLW, GRN, CYN, BLU, MAG};

struct color
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct charcolor
{
	color fg;
	color bg;
	bool A = true;
};

class TermWindow
{
protected:
	char** m_canvas;
	charcolor** m_color;
	int m_width;
	int m_height;


public: 
	
	TermWindow() : TermWindow(DEFAULT_COLS, DEFAULT_ROWS)
	{
		printf("default constructor called\n");
	}

	explicit TermWindow(int width, int height)
	{
		m_width  = width;
		m_height = height;
		printf("constructor called with dims %d, %d \n", width, height);
		m_canvas = new char*[width];
		m_color = new charcolor*[width];
		for(int k = 0; k < width; ++k)
		{
			m_canvas[k] = new char[height];
			m_color[k] = new charcolor[height];
		}
		wipe();
	}

	void debug()
	{
		char* s;
		printf("\n\nTermWindow instance\nm_width: %d\nm_height:%d\n\n", m_width, m_height);
        //	system("stty raw");

	}

	~TermWindow()
	{
		for(int k = 0; k < m_width; ++k)
		{
			delete[] m_canvas[k];
			delete[] m_color[k];
		}
		delete[] m_canvas;
		delete[] m_color;
	}

	int getWidth()
	{
		return m_width;
	}
	int getHeight()
	{
		return m_height;
	}

	void setChar( int ssx, int ssy, char c)
	{
		m_canvas[ssx][ssy] = c;
	}

	void setFullColor( int ssx, int ssy, charcolor c)
	{
		m_color[ssx][ssy] = c;
	}

	void moveCursor(int row, int column)
	{
		printf("\033[%d;%dH", row, column);
	}
	void gotoxy(int x,int y)
	{
	    printf("%c[%d;%df",0x1B,y,x);
	}
	void wipe()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
				m_canvas[ssx][ssy] = ' ';
				m_color[ssx][ssy] = { 0, 0, 0, 0, 0, 0};
			}
	}

	color hsv( double h, double s, double v)
	{
	//	s -= floor(s);
	//	v -= floor(v);
		double C = s*v;
		double H = fmod(h/PI*180,360);
		double X = C*(1-fabs(fmod(H/60.0, 2)-1));
		double m = v-C;
		double r,g,b;
		if(H >= 0 && H < 60){
		    r = C,g = X,b = 0;
		}
		else if(H >= 60 && H < 120){
		    r = X,g = C,b = 0;
		}
		else if(H >= 120 && H < 180){
		    r = 0,g = C,b = X;
		}
		else if(H >= 180 && H < 240){
		    r = 0,g = X,b = C;
		}
		else if(H >= 240 && H < 300){
		    r = X,g = 0,b = C;
		}
		else{
		    r = C,g = 0,b = X;
		}
		uint8_t R = (r+m)*255;
		uint8_t G = (g+m)*255;
		uint8_t B = (b+m)*255;
	
		return (color){R, G, B};
	}

	char rawin()
	{
        	system("stty raw");
        	char c = getchar(); 
        	system("stty cooked");
        //	system("clear");
        //	std::cout << c << " was pressed."<< std::endl;
        	if(c == 'q')
		{
			printf("\n");
			exit(0);
        	}  
		return c;
	}

	void render()
	{
		gotoxy(0,0);
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			for ( int ssx=0; ssx < m_width; ++ssx)
				printf("%c", m_canvas[ssx][ssy]);
			printf("\n");
		}
	}

	void printFullClr(char chr, charcolor clr)
	{
	//	if (clr.bg.R + clr.bg.G + clr.bg.B < 2)
	//		clr.A = false;
	//		clr.A = false;
		switch(chr)
		{
			case ':':
			{
				if (clr.A)
					printf( FC_S, clr.fg.R, clr.fg.G, clr.fg.B, clr.bg.R, clr.bg.G, clr.bg.B, U_FULL_BLOCK );
				else 
					printf( FC_FS, clr.fg.R, clr.fg.G, clr.fg.B,  U_FULL_BLOCK );
				break;
			}
			case '.':
			{
				if (clr.A)
					printf( FC_S, clr.fg.R, clr.fg.G, clr.fg.B, clr.bg.R, clr.bg.G, clr.bg.B, U_LOWH_BLOCK );
				else 
					printf( FC_FS, clr.fg.R, clr.fg.G, clr.fg.B, U_LOWH_BLOCK );
				break;
			}
			case '\'':
			{
				if (clr.A)
					printf( FC_S, clr.bg.R, clr.bg.G, clr.bg.B, clr.fg.R, clr.fg.G, clr.fg.B, U_UPPH_BLOCK );
				else 
					printf( FC_FS, clr.bg.R, clr.bg.G, clr.bg.G, U_UPPH_BLOCK );
				break;
			}
			default:
			{
				if (clr.A)
					printf( FC_C, clr.fg.R, clr.fg.G, clr.fg.B, clr.bg.R, clr.bg.G, clr.bg.B, chr );
				else 
					printf( FC_FC, clr.fg.R, clr.fg.G, clr.fg.B, chr );
				break;
			}
		}
	}

	void printClr(char chr, int clr)
	{
		switch(chr)
		{
			case ':':
			{
				printf( "%s%s%s", g_clr[clr], "\u2588", g_clr[0]);
				break;
			}
			case '.':
			{
				printf( "%s%s%s", g_clr[clr], "\u2584", g_clr[0]);
				break;
			}
			case '\'':
			{
				printf( "%s%s%s", g_clr[clr], "\u2580", g_clr[0]);
				break;
			}
			default: {printf( "%s%c%s", g_clr[clr], chr, g_clr[0]);break;}
		}
	}

	/*
	void renderColor()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			for ( int ssx=0; ssx < m_width; ++ssx)
				printClr(m_canvas[ssx][ssy], m_color[ssx][ssy]);
			printf("\n");
		}
	}
	*/

	color rgb(uint8_t r, uint8_t g, uint8_t b)
	{
		return color{ r, g, b};
	}

	
	charcolor crgb(color fg, color bg, bool a)
	{
		return (charcolor){fg, bg, a};
	}

	void renderFullColor()
	{
		moveCursor(10,10);
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			for ( int ssx=0; ssx < m_width; ++ssx)
				printFullClr(m_canvas[ssx][ssy], m_color[ssx][ssy]);
			printf("\n");
		}
	}

	void smartrenderFullColor()
	{
	moveCursor(0,0);
		for( int ssy=0; ssy < m_height; ++ssy)
		{
		moveCursor(ssy, 0);
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
			//	moveCursor(ssy, ssx);
				printFullClr(m_canvas[ssx][ssy], m_color[ssx][ssy]);
			}
		}
	}
};

class Graph : public TermWindow 
{
private:
	Point m_offset;
	double m_scale;
	double m_stretch;
	double m_epsilon;
	double m_delta;
	double m_kappa;
	double m_pdelta;

	void initValues()
	{
		m_offset.x = 0.0;
		m_offset.y = 0.0;
		m_stretch = 2.0;
		m_epsilon = 0.5;
		m_delta = 10.0;
		m_kappa = 1.1;
		m_pdelta = 0.005;
	}
public:
	
	explicit Graph( int width, int height) : TermWindow(width, height)
	{
		initValues();
		printf("Press any key to start, press 'q' to quit ... ");
	}
	explicit Graph()
	{
		TermWindow();
	}
	void setEpsilon(double epsilon)
	{
		m_epsilon = epsilon;
	}
	void setScale(double scale)
	{
		m_scale = scale;
	}
	void setOffset(double offset_x, double offset_y)
	{
		m_offset.x = offset_x;
		m_offset.y = offset_y;
	}
	void translate(double delta_x, double delta_y)
	{
		m_offset.x += delta_x;
		m_offset.y += delta_y;
	}
	void scale(double kappa)
	{
		m_scale *= kappa;
	}

	void drawGrid()
	{

	}
		
/// THIS IS SUM BULLSHIT (because I'm too lazy to write an expression parser) /// 
	double expr(double x, double y)
	{
	//	return (abs(100*sin(x)-y)-10)*(abs(x*x + y*y -512)-16);
	//	return sqrt(pow(((10*sin(x/6)-y))*(x*x + y*y -512), 2)+64)-512;
		return -(double)cplxiter(x,y);
	//	return -2*PI+atan2(x,y);
	//	return -sqrt(x*x+y*y);
	//	return abs((3*x + 2)-y)-0.1;
	}
/// END OF BULLSHIT ^^^ ///

	color calcColor(double x)
	{
		const int range = 256;
		double q = 100.0;
	//	return (color){(int)(0.9*(-x)/q)%range, (int)(0.9*(-x)/q)%range, (int)(0.9*(-x)/q)%range};
		return hsv(-x , 0.1, -x);
	}

	void drawExpr()
	{
		//printf("%f", m_offset.y);
		double upper;
		double lower;
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
			//	upper = expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y));
			//	sc(ss -o) => sc*ss - 
				upper = expr((m_scale*(ssx-m_width/2)-m_offset.x)/m_stretch,m_scale*(ssy-m_height/2)-m_offset.y);
				lower = expr((m_scale*(ssx-m_width/2)-m_offset.x)/m_stretch,m_scale*(ssy-m_height/2+0.5)-m_offset.y);
				if(upper < m_epsilon && lower < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper), true));	
					m_canvas[ssx][ssy] = '.';
				} 
				else if (upper < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper), false));	
					m_canvas[ssx][ssy] = '\'';

				}
				else if (lower < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper), false));	
					m_canvas[ssx][ssy] = '.';
				}
				else 
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper), false));
			}
	}

	void drawAxis()
	{
		for( int ssx=0; ssx < m_width; ++ssx)
		{
			m_canvas[ssx][(int)m_offset.y] = '-';
		}
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			m_canvas[(int)m_offset.x][ssy] = '|';
		}
	}
	
	void drawCenter()
	{
		m_canvas[m_width/2][m_height/2] = '^';
		uint8_t brightness = (m_color[m_width/2][m_height/2].fg.R + m_color[m_width/2][m_height/2].fg.R + m_color[m_width/2][m_height/2].fg.R < 500 ? 255 : 0);
		m_color[m_width/2][m_height/2].fg.R = brightness;
		m_color[m_width/2][m_height/2].fg.G = brightness;
		m_color[m_width/2][m_height/2].fg.B = brightness;
//		setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper), true));	
	}
	void sampleCenter()
	{
		moveCursor(m_height, 0);
		printf("+ = %.2f", expr((-m_offset.x)/m_stretch,-m_offset.y));
	}
	void ctrl()
	{
		char c = rawin();

		switch(c)
		{
			case 'h':
			{
				translate(m_stretch*m_scale*m_delta, 0);
				break;
			}
			case 'j':
			{
				translate(0, -m_scale*m_delta);
				break;
			}
			case 'k':
			{
				translate(0, m_scale*m_delta);
				break;
			}
			case 'l':
			{
				translate(-m_stretch*m_scale*m_delta, 0);
				break;
			}
			case 'H':
			{
				translate(m_scale, 0);
				break;
			}
			case 'J':
			{
				translate(0, -m_scale*1.0/m_stretch);
				break;
			}
			case 'K':
			{
				translate(0, m_scale*1.0/m_stretch);
				break;
			}
			case 'L':
			{
				translate(-m_scale, 0);
				break;
			}
			case '-':
			{
				scale(m_kappa);
				break;
			}
			case '+':
			{
				scale(1./m_kappa);
				break;
			}
			case '=':
			{
				setScale(1.0);
				break;
			}
			case 'a':
			{
				g_x -= m_scale*m_pdelta*m_delta;
				break;
			}
			case 's':
			{
				g_y += m_scale*m_pdelta*m_delta;
				break;
			}
			case 'd':
			{
				g_y -= m_scale*m_pdelta*m_delta;
				break;
			}
			case 'f':
			{
				g_x += m_scale*m_pdelta*m_delta;
				break;
			}
			case 'n':
			{
				//g_maxiters /= 2;
				m_epsilon /= 2;
				break;
			}
			case 'm':
			{
				//g_maxiters *= 2;
				m_epsilon *= 2;
				break;
			}
		}
		wipe();
	}
};

int parseInt(char* s)
{
	int n;
	std::sscanf(s, "%d", &n);
	return n;
}

int main (int argc, char* argv[])
{
	Graph* window = NULL;

	if(argc==3)
		window = new Graph(parseInt(argv[1]),parseInt(argv[2])-0);
	else 
		window = new Graph();

	window->debug();
	window->setScale(0.05);
	window->setEpsilon(-8.0);
	while(1)
	{
		window->ctrl();
		window->drawExpr();
	//	window->drawAxis();
		window->drawCenter();
		window->smartrenderFullColor();
	//	window->sampleCenter();
	}

	return 0;
}
