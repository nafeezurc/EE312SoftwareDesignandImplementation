

#include <cstdint>
#include <chrono>
#include <vector>
#include <deque>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include "Bugs.h"

// CBugsSolutionApp construction

const Fl_Color BLACK{FL_BLACK};
const Fl_Color RED{FL_RED};
const Fl_Color GREEN{FL_GREEN};
const Fl_Color WHITE{FL_WHITE};

const int pixSize = 3;

const double tick_period = 0.020; // 20 milliseconds between updates -- 50Hz

/* Global Variables for the simulation */
int world[WORLD_SIZE][WORLD_SIZE];
std::vector<Bug> bug_list;

Fl_Color bugColor(Bug* b) {
	int straight = (b->genes[0] * 255) / GENE_TOTAL;
	int right = (b->genes[1] + b->genes[2] + b->genes[3]) * 255 / GENE_TOTAL;
	int left = (b->genes[5] + b->genes[6] + b->genes[7]) * 255 / GENE_TOTAL;
	return fl_rgb_color(straight, right, left);
}


class VerticalChart : public Fl_Box {
public:
    VerticalChart(int min, int max, int marker, int x, int y, int w, int h, const char *l=0) : Fl_Box(x,y,w,h,l){
        this->min = min;
        this->max = max;
        this->marker = marker;
    }

    void pushPoint(int pt){
        points.push_back(pt);
        if( points.size() > (h()-18)/lineSize ){
            points.pop_front();
        }
        draw();
    }

    void clear(){
        points.clear();
        draw();
    }

    void draw(void) {
        // Draw border
        fl_rectf(x(), y(), w(), h(), BLACK);

        // Draw chart data
        fl_push_clip(x()+pixSize, y()+pixSize, w()-2*pixSize, h()-2*pixSize-18);
        fl_rectf(x(), y(), w(), h(), WHITE);
        // Draw line for marker
        fl_color(RED);
        fl_line_style(FL_SOLID, lineSize);
        fl_line(getXPos(marker), y(), getXPos(marker), y() + h()-1);

        fl_color(GREEN);
        fl_line_style(FL_SOLID | FL_CAP_ROUND | FL_JOIN_MITER, lineSize);
        fl_begin_line();
        for( int i = 1; i < points.size(); i++ ){
            fl_vertex(getXPos(points[i]), i*lineSize + y());
        }
        fl_end_line();

        fl_line_style(0);
        fl_pop_clip();

        // Draw label
        fl_rectf(x(), y()+h() - 18, w(), 18, WHITE);
        int relX = (int)((w() - fl_width(label()))/2);
        fl_color(RED);
        fl_draw(label(), x() + relX, y()+h() - 6);
    }
private:
    int getXPos(int val){
        // if min <= val <= max
        // then 0 <= val - min <= max - min
        //      0 <= (val - min) / (max - min) <= 1
        //      0 <= (val - min) / (max - min) * (w - 2*ps) <= w - 2*ps
        //      x+ps <= (val - min) / (max - min) * (w - 2*ps) + x+ps <= x + w - ps
        return (int)((val - min)/(double)(max - min) * (w()-2*pixSize)) + x()+pixSize;
    }
    int min, max, marker;
    std::deque<int> points;
    static const int lineSize = 2;
};

class BugFrame : public Fl_Box {
public:
	BugFrame(int X,int Y,int W,int H,const char*L=0) :
            Fl_Box(X,Y,W,H,L),
            popChart(0, 200, 100, X+pixSize*(WORLD_SIZE+1), Y, 2*pixSize+200, H, "Population") {
        Fl::add_timeout(tick_period, Timer_Interrupt, (void*)this);
	}

    static void Timer_Interrupt(void* param) {
        BugFrame* me = (BugFrame*) param;
    	for (int k = 0; k < SPEED_UP; k += 1) {
    		timeStep(); // update the world
    		if (time_step > SLOW_TIME) { break; }
    	}
        me->redraw();
        Fl::repeat_timeout(tick_period, Timer_Interrupt, me);
    }

    void draw(void) {
    	/* erase the world (draw white over everything) */
        fl_rectf(x(), y(), pixSize*(WORLD_SIZE+2), pixSize*(WORLD_SIZE+2), BLACK);

    	fl_push_clip(x()+pixSize, y()+pixSize, pixSize*WORLD_SIZE, pixSize*WORLD_SIZE);
        fl_rectf(x(), y(), w(), h(), WHITE);

    	/* fill in the food and bugs */
    	for (int i = 0; i < WORLD_SIZE; i += 1) {
    		for (int j = 0; j < WORLD_SIZE; j += 1) {
    			switch(world[i][j]) {
    			case EMPTY: // draw nothing
    				break;
    			case FOOD: // draw food
    				fl_rectf(x() + (i+1) * pixSize, y() + (j+1) * pixSize, pixSize, pixSize, GREEN);
    				break;
    			default: // draw bug
    				if (world[i][j] == 0) {
    					fl_rectf(x() + i * pixSize, y() + j * pixSize, 3*pixSize, 3*pixSize,
    							bugColor(&bug_list[world[i][j]]));
    				} else {
    					fl_rectf(x() + (i+1) * pixSize, y() + (j+1) * pixSize, pixSize, pixSize,
    							bugColor(&bug_list[world[i][j]]));
    				}
    			}
    		}
    	}

        fl_pop_clip();

		if (time_step % 10 == 0) {
			/* update the population graph */
            popChart.pushPoint(bug_list.size());

			/* the text should br displayed in the window, but that's not working, I'll worry about that later */
			/* for now, I can at least printf it to the console! */
    		char buff[128];

            fl_rectf(x(), y() + pixSize*(WORLD_SIZE+2), pixSize*(WORLD_SIZE+1), 18, WHITE);
    		sprintf(buff, "time: %d, #bugs %d, avg age %d, avg gen %d,"
    			" straight %d%%, left %d%%, right %d%%, back %d%%",
    			time_step, bug_list.size(), average_age, average_generation,
    			percent_straight, percent_left, percent_right, percent_back);
            fl_color(RED);
    		fl_draw(buff, x()+1, y() + pixSize*(WORLD_SIZE+2) + 12);
    		//printf("%s\n", buff);
    	}
    }
    
private:
    VerticalChart popChart;
};

int main(void) {
	createWorld();

	Fl_Double_Window root_window(pixSize*(WORLD_SIZE+1) + 2*pixSize+200, pixSize*(WORLD_SIZE+2)+18,
        "EE312 Project 8 -- Bugs!");
    BugFrame canvas(0, 0, root_window.w(), root_window.h());

	root_window.end();
	root_window.show();


	return Fl::run();

}

