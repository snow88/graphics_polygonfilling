#include<iostream>
#include<graphics.h>
#include<algorithm>
using namespace std;

int pos_mod(int i, int n)
{
    if (i>=0)
    return i%n;

    return (i%n)+n;
}

class vertex {
public:
int x;
int y;
};

class poly {
public:
int n;
vertex vlist[20];
int ymax;
int ymin;
}mypoly;

struct edge {
    int ymax;
    float x_ymin;
    float onebym;
    struct edge *link;
}*e, *ptr, *ptrprev, *ptract;

void poly_fill(int color)
{
    mypoly.ymin = mypoly.vlist[0].y;
    for(int i=0; i<mypoly.n; i++) {
        if (mypoly.vlist[i].y<mypoly.ymin)
            mypoly.ymin = mypoly.vlist[i].y;
    }
    mypoly.ymax = mypoly.vlist[0].y;
    for(int i=0; i<mypoly.n; i++) {
        if (mypoly.vlist[i].y>mypoly.ymax)
            mypoly.ymax = mypoly.vlist[i].y;
    }
    edge *globaledgetable[200];

    //building global edge table
    for (int y=mypoly.ymin; y<=mypoly.ymax; y++) {
        globaledgetable[y] = NULL;
    }

    for (int y=mypoly.ymin; y<=mypoly.ymax; y++) {
        for (int i=0; i<mypoly.n; i++) {
            if (mypoly.vlist[i].y == y) {
                if (mypoly.vlist[(i+1)%mypoly.n].y>y) {
                    e = new edge;
                    e->ymax = mypoly.vlist[(i+1)%mypoly.n].y;
                    e->x_ymin = mypoly.vlist[i].x;
                    e->onebym = (mypoly.vlist[(i+1)%mypoly.n].x - mypoly.vlist[i].x) / (float)(mypoly.vlist[(i+1)%mypoly.n].y - mypoly.vlist[i].y);
                    e->link = NULL;

                    if (globaledgetable[y] == NULL)
                        globaledgetable[y] = e;
                    else {
                        ptr = globaledgetable[y];
                        if (e->x_ymin<=ptr->x_ymin) {
                            e->link = ptr;
                            globaledgetable[y] = e;
                        }
                        else {
                            ptrprev = ptr;
                            ptr = ptr->link;
                            while (ptr != NULL && e->x_ymin>ptr->x_ymin) {
                                    ptr = ptr->link;
                                    ptrprev = ptrprev->link;
                            }
                            e->link = ptr;
                            ptrprev->link = e;
                        }
                    }
                }
                if (mypoly.vlist[pos_mod(i-1,mypoly.n)].y>y) {
                    e = new edge;
                    e->ymax = mypoly.vlist[pos_mod(i-1,mypoly.n)].y;
                    e->x_ymin = mypoly.vlist[i].x;
                    e->onebym = (mypoly.vlist[pos_mod(i-1,mypoly.n)].x - mypoly.vlist[i].x) / (float)(mypoly.vlist[pos_mod(i-1,mypoly.n)].y - mypoly.vlist[i].y);
                    e->link = NULL;

                    if (globaledgetable[y] == NULL)
                        globaledgetable[y] = e;
                    else {
                        ptr = globaledgetable[y];
                        if (e->x_ymin<=ptr->x_ymin) {
                            e->link = ptr;
                            globaledgetable[y] = e;
                        }
                        else {
                            ptrprev = ptr;
                            ptr = ptr->link;
                            while (ptr != NULL && e->x_ymin>ptr->x_ymin) {
                                    ptr = ptr->link;
                                    ptrprev = ptrprev->link;
                            }
                            e->link = ptr;
                            ptrprev->link = e;
                        }
                    }
                }
            }

        }
    }

    //filling color using active edge table
    edge *activeedgetable = NULL;
    int s = mypoly.ymin;             //scanline
    //initially
    ptr = globaledgetable[s];
        e = new edge;
        e->ymax = ptr->ymax;
        e->x_ymin = ptr->x_ymin;
        e->onebym = ptr->onebym;
        e->link = NULL;
    activeedgetable = e;
    ptr = ptr->link;
    ptract = activeedgetable;
    while (ptr!=NULL) {
        e = new edge;
        e->ymax = ptr->ymax;
        e->x_ymin = ptr->x_ymin;
        e->onebym = ptr->onebym;
        e->link = NULL;

        ptract->link = e;
        ptr = ptr->link;
        ptract = ptract->link;
    }

    while (s<=mypoly.ymax) {
        ptract = activeedgetable;
        //draw line
        delay(100);
        line(activeedgetable->x_ymin,s,activeedgetable->link->x_ymin,s);
        s++;
        //update values
        activeedgetable->x_ymin+= activeedgetable->onebym;
        activeedgetable->link->x_ymin+= activeedgetable->link->onebym;
        //delete expired edge
        while(1) {
            if (activeedgetable == NULL)
                break;
            else if (activeedgetable->ymax == s)
                activeedgetable = activeedgetable->link;
            else
                break;
        }
        if (activeedgetable!=NULL) {
        ptrprev = activeedgetable;
        ptract = activeedgetable->link;
        while (ptract != NULL) {
            if (ptract->ymax == s) {
                ptrprev->link = ptract->link;
                ptract = ptract->link;
            }
            else {
                ptract = ptract->link;
                ptrprev = ptrprev->link;
            }
        }
        }
        //merge new edge
        if (globaledgetable[s]!=NULL)
        {
            ptr = globaledgetable[s];
            while(ptr!=NULL) {
                e = new edge;
                e->ymax = ptr->ymax;
                e->x_ymin = ptr->x_ymin;
                e->onebym = ptr->onebym;
                e->link = ptr->link;

                if (activeedgetable == NULL) {
                    activeedgetable = e;
                    ptr = ptr->link;
                }
                else if (ptr->x_ymin < activeedgetable->x_ymin) {
                    e->link = activeedgetable;
                    activeedgetable = e;
                    ptr = ptr->link;
                }
                else {
                    ptrprev = activeedgetable;
                    ptract = activeedgetable->link;
                    if (ptract == NULL)
                        ptrprev->link = e;
                    else {
                        while (ptract!=NULL && ptr->x_ymin>ptract->x_ymin) {
                            ptrprev = ptrprev->link;
                            ptract = ptract->link;
                        }
                        e->link = ptract;
                        ptrprev->link = e;
                    }
                }
            }
        }
    }

}

void poly_draw()
{
    for (int i=0; i<mypoly.n; i++)
        line(mypoly.vlist[i].x,mypoly.vlist[i].y,mypoly.vlist[(i+1)%mypoly.n].x,mypoly.vlist[(i+1)%mypoly.n].y);
}

int main()
{
    int gd=DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    cout<<"Enter number of vertices"<<endl;
    cin>>mypoly.n;
    cout<<"Enter x,y for each vertex in clockwise order"<<endl;
    for (int i=0; i<mypoly.n; i++)
    {
        cin>>mypoly.vlist[i].x>>mypoly.vlist[i].y;
    }
    poly_draw();
    char ch;
    cout<<"Press any key to fill"<<endl;
    cin>>ch;
    poly_fill(WHITE);
    getch();
    closegraph();
    return 0;
}
