#include "sculptor.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <math.h>

using namespace std;

 Sculptor::Sculptor(int nx_, int ny_, int nz_)
{

    nx = nx_;
    ny = ny_;
    nz = nz_;





    v = new Voxel**[nx];

    for(int i = 0; i < nx; i++){
        v[i] = new Voxel*[ny];
        for(int j = 0; j < ny; j++){
            v[i][j] = new Voxel[nz];

        }
    }


    for(int i = 0; i < nx; i++){
        for(int j = 0; j < ny; j++){
            for(int k = 0; k < nz; k++){
                v[i][j][k].isOn = false;
            }
        }


    }
}

 Sculptor::~Sculptor()
{


     for(int i = 0; i < nx; i++){
        for(int j = 0; j < ny; j++){
            delete[] v[i][j];
        }
    }


    for(int i = 0; i < nx; i++){
        delete[] v[i];
    }

    delete[] v;
}

void Sculptor::setColor(float r_, float g_, float b_)
{
    r=r_;
    b=b_;
    g=g_;
}

void Sculptor::putVoxel(int x0, int y0, int z0)
{
    if(x0>=0&&x0<nx&&y0>=0&&y0<ny&&z0>=0&&z0<nz){


        v[x0][y0][z0].isOn = true;
        v[x0][y0][z0].r = r;
        v[x0][y0][z0].g = g;
        v[x0][y0][z0].b = b;

    }
}

void Sculptor::cutVoxel(int x0, int y0, int z0)
{
    if(x0>=0&&x0<nx&&y0>=0&&y0<ny&&z0>=0&&z0<nz){

        v[x0][y0][z0].isOn = false;

    }
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1)
{
    for(int i =x0;i<=x1;i++){
        for(int j =y0;j<=y1;j++){
            for(int k =z0;k<=z1;k++){
                putVoxel(i,j,k);

            }
        }
    }
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1)
{

    for(int i =x0;i<=x1;i++){
        for(int j =y0;j<=y1;j++){
            for(int k =z0;k<=z1;k++){
                cutVoxel(i,j,k);
            }
        }
    }
}

void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius)
{
    double d;

    for(int i =0;i<nx;i++){
        for(int j =0;j<ny;j++){
            for(int k =0;k<nz;k++){
                d = pow(i-xcenter,2)+pow(j-ycenter,2)+pow(k-zcenter,2);
                if(d<=pow(radius,2)){
                    putVoxel(i,j,k);
                }
            }
        }
    }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius)
{
    double d;
    for(int i =0;i<nx;i++){
        for(int j =0;j<ny;j++){
            for(int k =0;k<nz;k++){
                d = pow(i-xcenter,2)+pow(j-ycenter,2)+pow(k-zcenter,2);
                if(d <= pow(radius,2)){
                    cutVoxel(i,j,k);
                }
            }
        }
    }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{
    double d;
    if(rx == 0){
       for(int j = 0; j <= ny; j++){
           for(int k = 0; k <= nz; k++){
               d=pow(j-ycenter,2)/pow(ry,2)+pow(k-zcenter,2)/pow(rz,2);
               if(d<= 1){
                   putVoxel(xcenter,j,k);
               }
           }
        }
    }else if(ry == 0){
       for(int i=0; i<= nx; i++){
           for(int k =0;k <= nz; k++){
               d=pow(i-xcenter,2)/pow(rx,2)+pow(k-zcenter,2)/pow(rz,2);
               if(d<= 1){
                   putVoxel(i,ycenter,k);
               }
           }
        }

    }else if(rz == 0){
       for(int i = 0; i <= nx; i++){
           for(int j=0; j <= ny; j++){
               d= pow(i - xcenter, 2)/pow(rx, 2) + pow(j - ycenter, 2)/pow(ry, 2);
               if(d<= 1){
                   putVoxel(i,j,zcenter);
               }
           }
        }
    }else{
       for(int i = 0; i < nx; i++){
           for(int j = 0; j < ny; j++){
               for(int k = 0; k < nz; k++){
                   d = pow(i - xcenter,2)/pow(rx, 2)+pow(j - ycenter,2)/pow(ry, 2) + pow(k - zcenter,2)/pow(rz, 2);
                   if (d <= 1){
                       putVoxel(i,j,k);
                   }
               }
           }
       }
    }
}

void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{

    double d;
    if(rx == 0){
       for(int j = 0; j <= ny; j++){
           for(int k = 0; k <= nz; k++){
               d=pow(j-ycenter,2)/pow(ry,2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(d <= 1){
                   cutVoxel(xcenter,j,k);
               }
           }
        }
    }else if(ry == 0){
       for(int i=0; i<= nx; i++){
           for(int k = 0; k <= nz; k++){
               d = pow(i - xcenter, 2)/pow(rx, 2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(d <= 1){
                   cutVoxel(i,ycenter,k);
               }
           }
        }

    }else if(rz == 0){
       for(int i = 0; i <= nx; i++){
           for(int j=0; j <= ny; j++){
               d = pow(i - xcenter, 2)/pow(rx, 2) + pow(j - ycenter, 2)/pow(ry, 2);
               if(d <= 1){
                   cutVoxel(i,j,zcenter);
               }
           }
        }
    }else{
       for(int i = 0; i < nx; i++){
           for(int j = 0; j < ny; j++){
               for(int k = 0; k < nz; k++){
                   d = pow(i - xcenter,2)/pow(rx, 2) + pow(j - ycenter,2)/pow(ry, 2) + pow(k - zcenter,2)/pow(rz, 2);
                   if (d <= 1){
                       cutVoxel(i,j,k);
                   }
               }
           }
       }
    }
}

void Sculptor::writeOFF(string filename){

    int x, y, z, t, index;
    float lado = 0.5;
    ofstream f;
    t = 0;
    f.open(filename);
    f << "OFF\n";

    for(x = 0; x < nx; x++){
        for(y = 0; y < ny; y++){
            for(z = 0; z < nz; z++){
                if(v[x][y][z].isOn){
                    t++;
                }
            }
        }
     }

    f << t*8 << " " << t*6 << " 0 \n";
    for(x = 0; x < nx; x++){
        for(y = 0; y < ny; y++){
            for(z = 0; z < nz; z++){
                if(v[x][y][z].isOn){
                    f << x - lado << " " << y + lado << " " << z - lado << "\n" << flush;
                    f << x - lado << " " << y - lado << " " << z - lado << "\n" << flush;
                    f << x + lado << " " << y - lado << " " << z - lado << "\n" << flush;
                    f << x + lado << " " << y + lado << " " << z - lado << "\n" << flush;
                    f << x - lado << " " << y + lado << " " << z + lado << "\n" << flush;
                    f << x - lado << " " << y - lado << " " << z + lado << "\n" << flush;
                    f << x + lado << " " << y - lado << " " << z + lado << "\n" << flush;
                    f << x + lado << " " << y + lado << " " << z + lado << "\n" << flush;
                }
            }
        }
    }

    t = 0;

    for(x = 0; x < nx; x++){
        for(y = 0; y < ny; y++){
            for(z = 0; z < nz; z++){
                if(v[x][y][z].isOn){
                    index = t*8;

                    f << fixed;
                    f << 4 << " " << index + 0 << " " << index + 3 << " " << index + 2 << " " << index + 1 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    f << 4 << " " << index + 4 << " " << index + 5 << " " << index + 6 << " " << index + 7 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    f << 4 << " " << index + 0 << " " << index + 1 << " " << index + 5 << " " << index + 4 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    f << 4 << " " << index + 0 << " " << index + 4 << " " << index + 7 << " " << index + 3 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    f << 4 << " " << index + 7 << " " << index + 6 << " " << index + 2 << " " << index + 3 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    f << 4 << " " << index + 1 << " " << index + 2 << " " << index + 6 << " " << index + 5 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";

                    t++;
                }
            }
        }
    }
    f.close();
}

vector<vector<Voxel>> Sculptor::getPlano(int i, int plano)
{
    vector<vector<Voxel>> re;
    vector<Voxel> line;
    if(plano == XY){
        line.resize(ny);
        for(int j = 0; j < nx; j++){
            for(int k = 0; k < ny; k++){
                line[k].r = v[j][k][i].r;
                line[k].g = v[j][k][i].g;
                line[k].b = v[j][k][i].b;
                line[k].isOn = v[j][k][i].isOn;
            }
            re.push_back(line);
        }
     }
    return re;
}
