#include<iostream>
#include<graphics.h>
using namespace std;

struct vertex
{
    int x;
    int y;
};

vertex poly[40];
int n;

void draw_poly(vertex vlist[], int n)
{
    for(int i=0; i<n; i++)
        line(vlist[i].x, vlist[i].y, vlist[(i+1)%n].x, vlist[(i+1)%n].y);
}

void boundaryfill4 (int x, int y, int fil, int boundary)
{
    int color = getpixel(x,y);
    if (color!=boundary && color!=fil)
    {
        putpixel(x, y, fil);
        boundaryfill4(x+1, y, fil, boundary);
        boundaryfill4(x-1, y, fil, boundary);
        boundaryfill4(x, y+1, fil, boundary);
        boundaryfill4(x, y-1, fil, boundary);
    }
}

int main()
{
    int gd=DETECT, gm=0;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    cout<<"Enter no. of vertices of shape"<<endl;
    cin>>n;
    cout<<"Enter vertices (x,y) clockwise"<<endl;
    for (int i=0; i<n; i++)
        cin>>poly[i].x>>poly[i].y;
    draw_poly(poly, n);

    int x, y;
    cout<<"Enter vertex (x,y) to start filling from"<<endl;
    cin>>x>>y;

    boundaryfill4(x, y, GREEN, WHITE);

    getch();
    closegraph();
    return 0;
}
