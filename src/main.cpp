#include <iostream>
#include <cstdint>
#include "math.h"

#define DEFAULT_ROWS 40
#define DEFAULT_COLS 80

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

	void wipe()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
				m_canvas[ssx][ssy] = ' ';
				m_color[ssx][ssy] = { 0, 0, 0, 0, 0, 0};
			}
	}

	char rawin()
	{
        	system("stty raw");
        	char c = getchar(); 
        	// terminate when "." is pressed
        	system("stty cooked");
        	system("clear");
        	//std::cout << c << " was pressed."<< std::endl;
        	if(c == 'q')
		{
        	    system("stty cooked");
        	    exit(0);
        	}  
		return c;
	}

	void render()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			for ( int ssx=0; ssx < m_width; ++ssx)
				printf("%c", m_canvas[ssx][ssy]);
			printf("\n");
		}
	}

	void printFullClr(char chr, charcolor clr)
	{
		if (clr.bg.R + clr.bg.G + clr.bg.B < 3*256)
			clr.A = false;
		switch(chr)
		{
			case ':':
			{
				if (clr.A)
					printf( FC_S, clr.fg.R, clr.fg.G, clr.fg.B, clr.bg.R, clr.bg.G, clr.bg.B, "\u2588" );
				else 
					printf( FC_FS, clr.fg.R, clr.fg.G, clr.fg.B,  "\u2588" );
				break;
			}
			case '.':
			{
				if (clr.A)
					printf( FC_S, clr.fg.R, clr.fg.G, clr.fg.B, clr.bg.R, clr.bg.G, clr.bg.B, "\u2584" );
				else 
					printf( FC_FS, clr.fg.R, clr.fg.G, clr.fg.B, "\u2584" );
				break;
			}
			case '\'':
			{
				if (clr.A)
					printf( FC_S, clr.bg.R, clr.bg.G, clr.bg.B, clr.fg.R, clr.fg.G, clr.fg.B, "\u2580" );
				else 
					printf( FC_FS, clr.bg.R, clr.bg.G, clr.bg.G, "\u2580" );
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

	
	charcolor crgb(color fg, color bg)
	{
		return (charcolor){fg, bg};
	}

	void renderFullColor()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			for ( int ssx=0; ssx < m_width; ++ssx)
				printFullClr(m_canvas[ssx][ssy], m_color[ssx][ssy]);
			printf("\n");
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
	
	void initValues()
	{
		m_offset.x = (double)m_width/2;
		m_offset.y = (double)m_height/2;
		m_stretch = 2.0;
		m_epsilon = 0.5;
		m_delta = 10.0;
		m_kappa = 1.1;
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
		return sqrt(pow(((10*sin(x/6)-y))*(x*x + y*y -512), 2)+64)-512;
	//	return -(double)cplxiter(x,y);
	}
/// END OF BULLSHIT ^^^ ///

	color calcColor(double x)
	{
		const int range = 256;
		double q = 1.1;
		return (color){(int)((-x)/q)%range, (int)((-x)/q)%range, (int)((-x)/q)%range};
	}

	void drawExpr()
	{
		//printf("%f", m_offset.y);
		double upper;
		double lower;
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
				upper = expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y));
			//	upper = expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y));
			//	sc(ss -o) => sc*ss - 
				lower = expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y+0.5));
				if( upper < m_epsilon && lower < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper)));	
					m_canvas[ssx][ssy] = ':';
				} 
				else if (upper < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper)));	
					m_canvas[ssx][ssy] = '\'';
				}
				else if (lower < m_epsilon)
				{
					setFullColor(ssx, ssy, crgb( calcColor(lower), calcColor(upper)));	
					m_canvas[ssx][ssy] = '.';
				}
				
			}
	}

	void drawAxis()
	{
		for( int ssx=0; ssx < m_width; ++ssx)
			m_canvas[ssx][(int)m_offset.y] = '-';
		for( int ssy=0; ssy < m_height; ++ssy)
			m_canvas[(int)m_offset.x][ssy] = '|';
	}

	void ctrl()
	{
		char c = rawin();

		switch(c)
		{
			case 'h':
			{
				translate(2*m_delta, 0);
				break;
			}
			case 'j':
			{
				translate(0, -m_delta);
				break;
			}
			case 'k':
			{
				translate(0, m_delta);
				break;
			}
			case 'l':
			{
				translate(-2*m_delta, 0);
				break;
			}
			case 'H':
			{
				translate(2.0, 0);
				break;
			}
			case 'J':
			{
				translate(0, -1.0);
				break;
			}
			case 'K':
			{
				translate(0, 1.0);
				break;
			}
			case 'L':
			{
				translate(-2.0, 0);
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
				g_x -= 0.005*m_delta;
				break;
			}
			case 's':
			{
				g_y += 0.005*m_delta;
				break;
			}
			case 'd':
			{
				g_y -= 0.005*m_delta;
				break;
			}
			case 'f':
			{
				g_x += 0.005*m_delta;
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
		window = new Graph(parseInt(argv[1]),parseInt(argv[2]));
	else 
		window = new Graph();

	window->debug();
	window->setChar( 1, 1, '=');
	window->setScale(0.05);
	window->setEpsilon(-8.0);
	while(1)
	{
		window->ctrl();
		window->drawExpr();
//		window->drawAxis();
		window->renderFullColor();
	}
//	printf("%d", window.getWidth());

	return 0;
}
