#include <bits/stdc++.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include "calc_moves.cpp"
using namespace std;
#define RADDEG  57.29577951f
#define PI      3.14159265358979323
float XUP[3] = {1,0,0}, XUN[3] = {-1, 0, 0},
      YUP[3] = {0,1,0}, YUN[3] = { 0,-1, 0},
      ZUP[3] = {0,0,1}, ZUN[3] = { 0, 0,-1}, ORG[3] = {0,0,0};
 
GLfloat viewangle = 0, tippangle = 0, traj[120][3];
 
GLfloat d[3] = {0.1, 0.1, 0.1};
 
GLfloat  xAngle = 0.0, yAngle = 0.0, zAngle = 0.0;
//  Use arrow keys to rotate entire scene !!!

int n,m;
int flag;
int selected_disc;
int cntr = 0;

struct point{
    int px,py;
} pts[1005];
struct color{
    int r,g,b;
} boundary;

vector<float> list_discs[105]; 
vector<pair<pair<float,float>,float> >  disc_info;
vector<pair<pair<float,float>,float> > disc_color;
vector<pair<int,int> > moves;

float tox[105],toy[105]; //Shows the x and y coordinates of top 
void redraw (void);
int moving, khatam;
void Special_Keys (int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT :  viewangle -= 5;  break;
        case GLUT_KEY_RIGHT:  viewangle += 5;  break;
        case GLUT_KEY_UP   :  tippangle -= 5;  break;
        case GLUT_KEY_DOWN :  tippangle += 5;  break;
        default: printf ("   Special key %c == %d\n", key, key);
    }
    glutPostRedisplay();
}

GLdouble radius=1.5;
GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0}; //Red Color
GLfloat tmp_col[] = {0.0,0.0,0.0,1.0}; 
// Set lighting intensity and color
GLfloat qaAmbientLight[]    = {0.1, 0.1, 0.1, 1.0};
GLfloat qaDiffuseLight[]    = {1, 1, 1, 1.0};
// Light source position
GLfloat qaLightPosition[]    = {0, -5, 9.5, 1};// Positional Light
GLfloat qaLightDirection[]    = {1, 1, 1, 0};// Directional Light

void initLighting() {
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
pair<float,float> rot_mk;
pair<float,float> cencen;
float disc_num_rot;
int mkkk;
void anim_disc(int disc_num,int st,int en) {
    float stp = 0.5;
    float tot_steps = 0;
    float cx = tox[st], cy = toy[st];
    float ex = tox[en], ey = toy[en];
    for(;cy<7.25;cy+=stp) {
        mkkk++;
        disc_info[disc_num].first.first = tox[st];
        disc_info[disc_num].first.second = cy;
        redraw();
        tot_steps++;
    }
    float cenx = (tox[st] + tox[en])*0.5;
    float ceny = cy;
    float rad = fabs(tox[st]-tox[en]) * 0.5;
    if(tox[st] < tox[en]) {
        float theta = 0;
        for(;theta<=170;theta+=(72/tot_steps)) {
            mkkk++;
            cx = cenx - rad * cos((theta * PI) / 180);
            cy = ceny + rad * sin((theta * PI) / 180);
            disc_info[disc_num].first.first = cx;
            disc_info[disc_num].first.second = cy;
            disc_num_rot = disc_num;
            rot_mk.first = theta;
            rot_mk.second = -1; 
            cencen.first = cenx;
            cencen.second = ceny;
            redraw();
        }
    }
    else {
        float theta = 0;
        for(;theta<=170;theta+=(72/tot_steps)) {
            mkkk++;
            cx = cenx + rad * cos((theta * PI) / 180);
            cy = ceny + rad * sin((theta * PI) / 180);
            disc_info[disc_num].first.first = cx;
            disc_info[disc_num].first.second = cy;
            disc_num_rot = disc_num;
            rot_mk.first = -theta;
            rot_mk.second = 1;
            cencen.first = cenx;
            cencen.second = ceny;
            redraw();
        }
    }
    disc_num_rot = 0;
    for(;cy>=ey;cy-=stp) {
        mkkk = 3 - mkkk;
        disc_info[disc_num].first.first = ex;
        disc_info[disc_num].first.second = cy;
        redraw();
    }
    disc_info[disc_num].first.first = ex;
    disc_info[disc_num].first.second = ey;
    redraw();
}

void Keyboard (unsigned char key, int x, int y) {
    switch (key) {
        case 'j' : d[0] += 0.1;  break;
        case 'k' : d[1] += 0.1;  break;
        case 'l' : d[2] += 0.1;  break;

        case 'b' : d[0] -= 0.1;  break;
        case 'n' : d[1] -= 0.1;  break;
        case 'm' : d[2] -= 0.1;  break;

        case 'a' : {
            if(moving) break;
            moving = 1;
            int tmp = (int)moves.size();
            if(cntr != tmp) {
                int from = moves[cntr].first, to = moves[cntr].second;
                int num_of_disc = list_discs[from].back();
                list_discs[to].push_back(list_discs[from].back());
                list_discs[from].pop_back();

                anim_disc(num_of_disc,from,to);
                disc_info[num_of_disc].first.first = tox[to];
                disc_info[num_of_disc].first.second = toy[to];

                toy[from] -= 0.5;
                toy[to] += 0.5;
                cntr++;
            }
            else {
                khatam++;
            }
            moving = 0;
            break;
        }
        case 'x' : xAngle += 5;  break;
        case 'y' : yAngle += 5;  break;
        case 'z' : zAngle += 5;  break;
        default: printf ("   Keyboard %c == %d\n", key, key);
    }
    glutPostRedisplay();
}

void Draw_func (void) {
    tmp_col[0] = 0.5, tmp_col[1] = 0.35, tmp_col[2] = 0.05;
    glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
    glTranslatef(0,0,-50);
    glutSolidCube(100);
    glTranslatef(0,0,50);

    GLfloat mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    tmp_col[0] = 1, tmp_col[1] = 1, tmp_col[2] = 1;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_white);
    glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
    
    for(int i=1;i<=m;i++) {
        glTranslatef(tox[i],0,0);
        glutSolidCone(1,7,50,50);
        glTranslatef(-tox[i],0,0);
    }

    for(int i=1;i<=n;i++) {
        if(i==disc_num_rot) {
            glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);

            float vecx = disc_info[i].first.first - cencen.first;
            float vecy = disc_info[i].first.second - cencen.second;
            glRotatef(rot_mk.first, rot_mk.second,0,0);

            if(i==selected_disc) {
                tmp_col[0] = disc_color[i].first.first * 0.1;
                tmp_col[1] = disc_color[i].first.second * 0.1;
                tmp_col[2] = disc_color[i].second * 0.1;
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
                glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
            }
            else {
                tmp_col[0] = disc_color[i].first.first;
                tmp_col[1] = disc_color[i].first.second;
                tmp_col[2] = disc_color[i].second;
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
                glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
            }
            glutSolidTorus(0.5,disc_info[i].second,50,50);
            glRotatef(-rot_mk.first, rot_mk.second,0,0);
            glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
        }
        else {
            glTranslatef(disc_info[i].first.first, 0, disc_info[i].first.second);
            if(i==selected_disc) {
                tmp_col[0] = disc_color[i].first.first * 0.1;
                tmp_col[1] = disc_color[i].first.second * 0.1;
                tmp_col[2] = disc_color[i].second * 0.1;
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
                glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
            }
            else {
                tmp_col[0] = disc_color[i].first.first;
                tmp_col[1] = disc_color[i].first.second;
                tmp_col[2] = disc_color[i].second;
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tmp_col);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp_col);
                glColor3f(tmp_col[0],tmp_col[1],tmp_col[2]);
            }
            glutSolidTorus(0.5,disc_info[i].second,50,50);
            glTranslatef(-disc_info[i].first.first, 0, -disc_info[i].first.second);
        }
    }
    glEnd();
}

int flll = 0;

void redraw (void) {
    int v;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);
    glEnable (GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLoadIdentity();

    if((int)list_discs[m].size() == n and khatam == 1 and flll == 0) {
        glClearColor(0.1,0.1,0.1,1);
        glClear  (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        flll = 1;
        glutPostRedisplay();
    }
    glTranslatef (0, 0, -3) ;
    glRotatef (tippangle, 1,0,0);  // Up and down arrow keys 'tip' view.
    glRotatef (viewangle, 0,1,0);  // Right/left arrow keys 'turn' view.
    glScalef(0.5,0.5,0.5);

    glPushMatrix ();
    glTranslatef (d[0], d[1], d[2]);    // Move box down X axis.
    glScalef (0.2, 0.2, 0.2);
    glRotatef (zAngle, 0,0,1);
    glRotatef (yAngle, 0,1,0);
    glRotatef (xAngle, 1,0,0);
    if(flll == 0) Draw_func ();
    glPopMatrix ();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    if(flll == 0) {
        glColor3f(1,1,1);
        char txt[] = "Moves played : ";
        vector<int> to_num;
        int to_num_sz=0,st = strlen(txt);
        if(cntr != 0) {
            int tmp_cntr = cntr;
            while(tmp_cntr) {
                to_num_sz++;
                to_num.push_back(tmp_cntr%10);
                tmp_cntr/=10;
            }
            reverse(to_num.begin(),to_num.end());
        }
        else {
            to_num_sz++;
            to_num.push_back(0);
        }
        for(int i=0;i<to_num_sz;i++) {
            txt[st+i] = (char)(to_num[i]+'0');
        }
        glRasterPos3f(1,1,-2);
        for(int i=0;i<st+to_num_sz;i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,txt[i]);
    }
    else {
        glColor3f(1,1,1);
        char pp[] = "Game over.";
        glRasterPos3f(-0.2,0.2,-2);
        for(int i=0;pp[i]!='\0';i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,pp[i]);
        char txt[] = "Number of moves: ";
        vector<int> to_num;
        int to_num_sz=0,st = strlen(txt);
        if(cntr != 0) {
            int tmp_cntr = cntr;
            while(tmp_cntr) {
                to_num_sz++;
                to_num.push_back(tmp_cntr%10);
                tmp_cntr/=10;
            }
            reverse(to_num.begin(),to_num.end());
        }
        else {
            to_num_sz++;
            to_num.push_back(0);
        }
        for(int i=0;i<to_num_sz;i++) {
            txt[st+i] = (char)(to_num[i]+'0');
        }
        txt[st+to_num_sz] = '\0';
        glRasterPos3f(-0.25,0,-2);
        for(int i=0;i<st+to_num_sz;i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,txt[i]);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glFlush(); 
    glutSwapBuffers();
}
int from,to;
float rr[] = {1,0,0,1,1,0,0.5,0.2};
float bb[] = {0,1,0,1,0,1,0.5,0.8};
float gg[] = {0,0,1,0,1,1,0.5,0.4};
int main (int argc, char **argv) {
    from = to = -1;
    while(1) {
        cout<<"Enter number of towers:";
        cin>>m;
        cout<<"Enter number of discs:";
        cin>>n;
        if(m<=2) {
            cout<<"Number of towers should be greater than 2.\n";
            continue;
        }
        if(m > 6) {
            cout<<"Please enter a less number of towers.\n";
            continue;
        }
        if(n > 8) {
            cout<<"Please enter a less number of discs.\n";
            continue;
        }
        if(n < 1) {
            cout<<"Please enter a positive number of discs.\n";
            continue;    
        }
        break;
    }
    disc_info.resize(n+5);
    disc_color.resize(n+5);
    bool re = 0;
    float rad = 1 + n * 0.5;
    //Base positions of all the towers
    toy[1] = 0.5 + 0.5*n;
    for(int i=2;i<=m;i++) toy[i] = 0.5;
    int startx;
    if(m==2) {
        tox[1] = startx = -15;
        tox[2] = 15;
    }
    if(m==3) {
        tox[1] = startx = -15;
        tox[2] = 0;
        tox[3] = 15;
    }
    if(m==4) {
        tox[1] = startx = -15;
        tox[2] = -5;
        tox[3] = 5;
        tox[4] = 15;
    }
    if(m==5) {
        tox[1] = startx = -20;
        tox[2] = -10;
        tox[3] = 0;
        tox[4] = 10;
        tox[5] = 20;
    }
    if(m==6) {
        tox[1] = startx = -25;
        tox[2] = -15;
        tox[3] = -5;
        tox[4] = 5;
        tox[5] = 15;
        tox[6] = 25;
    }
    ///////
    for(int i=1;i<=n;i++) {
        list_discs[1].push_back(i),re^=1;
        disc_info[i] = make_pair(make_pair(startx,0.5 + 0.5*(i-1)),rad);
        disc_color[i].first.first = rr[i-1];
        disc_color[i].first.second = bb[i-1];
        disc_color[i].second = gg[i-1];
        rad -= 0.5;
    }
    moves = calc(n,m);
    glutInit               (&argc, argv);
    glutInitWindowSize     (1500, 1500);
    glutInitWindowPosition (300, 300);
    glutInitDisplayMode    (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB | GLUT_DOUBLE);
    initLighting(); 
    glutCreateWindow ("Computer Play");
    glutDisplayFunc  (   redraw   );
    glutKeyboardFunc (  Keyboard  );
    glutSpecialFunc  (Special_Keys);
    glClearColor (0, 0.423, 0.6, 0.0);

    glMatrixMode   (GL_PROJECTION);
    gluPerspective (60, 1.5, 1, 10);
    glMatrixMode   (GL_MODELVIEW);
    glutMainLoop   ();
    glDisable(GL_LIGHTING);
    return 1;
}
