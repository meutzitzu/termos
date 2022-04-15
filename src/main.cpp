#include <iostream>
#include "math.h"

#define DEFAULT_ROWS 40
#define DEFAULT_COLS 80

class TermWindow
{
protected:
	char** m_canvas;
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
		for(int k = 0; k < width; ++k)
		{
			m_canvas[k] = new char[height];
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
		}
		delete[] m_canvas;
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
		printf("updated char\n");
		m_canvas[ssx][ssy] = c;
	}

	void wipe()
	{
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
				m_canvas[ssx][ssy] = ' ';
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
		printf("rendering canvas ...\n");
		for( int ssy=0; ssy < m_height; ++ssy)
		{
			//printf("%s", m_canvas[ssy]);
			for ( int ssx=0; ssx < m_width; ++ssx)
				printf("%c", m_canvas[ssx][ssy]);
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
		m_delta = 1.0;
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
		//return (abs(100*sin(x)-y)-10)*(abs(x*x + y*y -512)-16);
		return sqrt(pow(((10*sin(x/6)-y))*(x*x + y*y -512), 2)+64)-512;
	}
/// END OF BULLSHIT ^^^ ///

	void drawExpr()
	{
		//printf("%f", m_offset.y);
		for( int ssy=0; ssy < m_height; ++ssy)
			for ( int ssx=0; ssx < m_width; ++ssx)
			{
				if(expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y))<m_epsilon && expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y+0.5))<m_epsilon)
				{
					m_canvas[ssx][ssy] = ':';
				} 
				else if (expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y))<m_epsilon)
				{
					m_canvas[ssx][ssy] = '\'';
				}
				else if (expr(m_scale*(ssx-m_offset.x)/m_stretch,m_scale*(ssy-m_offset.y+0.5))<m_epsilon)
				{
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
	window->setScale(1.0);
	while(1)
	{
		window->ctrl();
		window->drawExpr();
		window->drawAxis();
		window->render();
	}
//	printf("%d", window.getWidth());

	return 0;
}
