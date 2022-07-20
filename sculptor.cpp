#include "sculptor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <iomanip>

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
    r = r_;
    b = b_;
    g = g_;
}

void Sculptor::putVoxel(int x0, int y0, int z0)
{
    if(x0 >= 0 && x0 < nx && y0 >= 0 && y0 < ny && z0 >= 0 && z0 < nz){
        v[x0][y0][z0].isOn = true;
        v[x0][y0][z0].r = r;
        v[x0][y0][z0].g = g;
        v[x0][y0][z0].b = b;
        }
}

void Sculptor::cutVoxel(int x0, int y0, int z0)
{
    if(x0 >= 0 && x0 < nx && y0 >= 0 && y0 < ny && z0 >= 0 && z0 < nz){
        v[x0][y0][z0].isOn = false;
    }
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1)
{
    for(int i = x0; i <= x1; i++){
        for(int j = y0; j <= y1; j++){
            for(int k = z0; k <= z1; k++){
                putVoxel(i,j,k);
            }
        }
    }
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1)
{
    for(int i = x0; i <= x1; i++){
        for(int j = y0; j <= y1; j++){
            for(int k = z0; k <= z1; k++){
                cutVoxel(i,j,k);
            }
        }
    }
}

void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius)
{
    double dist;
    for(int i = 0; i < nx; i++){
        for(int j = 0; j < ny; j++){
            for(int k = 0; k < nz; k++){
                dist = pow(i - xcenter,2) + pow(j - ycenter,2) + pow(k - zcenter,2);
                if(dist <= pow(radius,2)){ // tentar tirando a raiz
                    putVoxel(i,j,k);
                }
            }
        }
    }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius)
{
    double dist;
    for(int i = 0; i < nx; i++){
        for(int j = 0; j < ny; j++){
            for(int k = 0; k < nz; k++){
                dist = pow(i - xcenter,2) + pow(j - ycenter,2) + pow(k - zcenter,2);
                if(dist <= pow(radius,2)){ // tentar tirando a raiz
                    cutVoxel(i,j,k);
                }
            }
        }
    }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{
    // FÓRMULA DA ELIPSOIDE: (x - x0)^2/a^2 + (y - y0)^2/b^2 + (z - z0)^2/c^2 <= 1
    // CASO O RAIO EM ALGUM DOS EIXOS SEJA IGUAL A -, TEREMOS UMA ELÍPSE
    double dist;
    if(rx == 0){
       for(int j = 0; j <= ny; j++){
           for(int k = 0; k <= nz; k++){
               dist = pow(j - ycenter, 2)/pow(ry, 2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(dist <= 1){
                   putVoxel(xcenter,j,k);
               }
           }
        }
    }else if(ry == 0){
       for(int i=0; i<= nx; i++){
           for(int k = 0; k <= nz; k++){
               dist = pow(i - xcenter, 2)/pow(rx, 2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(dist <= 1){
                   putVoxel(i,ycenter,k); //habilita os Voxels no intervalo
               }
           }
        }

    }else if(rz == 0){
       for(int i = 0; i <= nx; i++){
           for(int j=0; j <= ny; j++){
               dist = pow(i - xcenter, 2)/pow(rx, 2) + pow(j - ycenter, 2)/pow(ry, 2);
               if(dist <= 1){
                   putVoxel(i,j,zcenter); //habilita os Voxels no intervalo
               }
           }
        }
    }else{
       for(int i = 0; i < nx; i++){
           for(int j = 0; j < ny; j++){
               for(int k = 0; k < nz; k++){
                   dist = pow(i - xcenter,2)/pow(rx, 2) + pow(j - ycenter,2)/pow(ry, 2) + pow(k - zcenter,2)/pow(rz, 2);
                   if (dist <= 1){
                       putVoxel(i,j,k);
                   }
               }
           }
       }
    }
}

void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{
    // FÓRMULA DA ELIPSOIDE: (x - x0)^2/a^2 + (y - y0)^2/b^2 + (z - z0)^2/c^2 <= 1
    // CASO O RAIO EM ALGUM DOS EIXOS SEJA IGUAL A -, TEREMOS UMA ELÍPSE
    double dist;
    if(rx == 0){
       for(int j = 0; j <= ny; j++){
           for(int k = 0; k <= nz; k++){
               dist = pow(j - ycenter, 2)/pow(ry, 2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(dist <= 1){
                   cutVoxel(xcenter,j,k);
               }
           }
        }
    }else if(ry == 0){
       for(int i=0; i<= nx; i++){
           for(int k = 0; k <= nz; k++){
               dist = pow(i - xcenter, 2)/pow(rx, 2) + pow(k - zcenter, 2)/pow(rz, 2);
               if(dist <= 1){
                   cutVoxel(i,ycenter,k); //habilita os Voxels no intervalo
               }
           }
        }

    }else if(rz == 0){
       for(int i = 0; i <= nx; i++){
           for(int j=0; j <= ny; j++){
               dist = pow(i - xcenter, 2)/pow(rx, 2) + pow(j - ycenter, 2)/pow(ry, 2);
               if(dist <= 1){
                   cutVoxel(i,j,zcenter); //habilita os Voxels no intervalo
               }
           }
        }
    }else{
       for(int i = 0; i < nx; i++){
           for(int j = 0; j < ny; j++){
               for(int k = 0; k < nz; k++){
                   dist = pow(i - xcenter,2)/pow(rx, 2) + pow(j - ycenter,2)/pow(ry, 2) + pow(k - zcenter,2)/pow(rz, 2);
                   if (dist <= 1){
                       cutVoxel(i,j,k);
                   }
               }
           }
       }
    }
}

void Sculptor::writeOFF(string filename){
    /* FORMATO DO ARQUIVO OFF
        OFF
        NVertices  NFaces  NArestas
        x[0]  y[0]  z[0]
        ...
        x[NVertices-1]  y[NVertices-1]  z[NVertices-1]
        Nv v[0] v[1] ... v[Nv-1]  r g b a
        Nv v[0] v[1] ... v[Nv-1]  r g b a
        ...
        Nv v[0] v[1] ... v[Nv-1]  r g b a
    */

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
                    t++; //  QUANTIDADE DE VOXELS ATIVOS
                }
            }
        }
     }

    f << t*8 << " " << t*6 << " 0 \n"; //   VÉRTICES, FACES, ARESTAS = 0
    for(x = 0; x < nx; x++){
        for(y = 0; y < ny; y++){
            for(z = 0; z < nz; z++){
                if(v[x][y][z].isOn){
                    //  VÉRTICES DO VOXEL EM FORMATO DE CUBO
                    f << x - lado << " " << y + lado << " " << z - lado << "\n" << flush; //Vértice 0
                    f << x - lado << " " << y - lado << " " << z - lado << "\n" << flush; //Vértice 1
                    f << x + lado << " " << y - lado << " " << z - lado << "\n" << flush; //Vértice 2
                    f << x + lado << " " << y + lado << " " << z - lado << "\n" << flush; //Vértice 3
                    f << x - lado << " " << y + lado << " " << z + lado << "\n" << flush; //Vértice 4
                    f << x - lado << " " << y - lado << " " << z + lado << "\n" << flush; //Vértice 5
                    f << x + lado << " " << y - lado << " " << z + lado << "\n" << flush; //Vértice 6
                    f << x + lado << " " << y + lado << " " << z + lado << "\n" << flush; //Vértice 7
                }
            }
        }
    }

    t = 0;
    // ---- ESCRITA DOS ÍNDICES (VÉRTICES: 8 EM 8) ----
    /*
        Face 0: P0 P3 P2 P1
        Face 1: P4 P5 P6 P7
        Face 2: P0 P1 P5 P4
        Face 3: P0 P4 P7 P3
        Face 4: P7 P6 P2 P3
        Face 5: P1 P2 P6 P5
    */
    for(x = 0; x < nx; x++){
        for(y = 0; y < ny; y++){
            for(z = 0; z < nz; z++){
                if(v[x][y][z].isOn){
                    index = t*8;
                    // FACE 0
                    f << fixed;
                    f << 4 << " " << index + 0 << " " << index + 3 << " " << index + 2 << " " << index + 1 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    // FACE 1
                    f << 4 << " " << index + 4 << " " << index + 5 << " " << index + 6 << " " << index + 7 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    // FACE 2
                    f << 4 << " " << index + 0 << " " << index + 1 << " " << index + 5 << " " << index + 4 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    // FACE 3
                    f << 4 << " " << index + 0 << " " << index + 4 << " " << index + 7 << " " << index + 3 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    // FACE 4
                    f << 4 << " " << index + 7 << " " << index + 6 << " " << index + 2 << " " << index + 3 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    // FACE 5
                    f << 4 << " " << index + 1 << " " << index + 2 << " " << index + 6 << " " << index + 5 << " ";
                    f << setprecision(2) << v[x][y][z].r/255.0 << " " << setprecision(2) << v[x][y][z].g/255.0 << " " << setprecision(2) << v[x][y][z].b/255.0 << "\n";
                    t++;  // INCREMENTA O TOTAL DE VOXELS EM FORMATO DE CUBO
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
    /* future upgrades
      else if(plano == YZ){
        line.resize(nz);
        for(int j = 0; j < ny; j++){
            for(int k = 0; k < nz; k++){
                line[k].r = v[i][j][k].r;
                line[k].g = v[i][j][k].g;
                line[k].b = v[i][j][k].b;
                line[k].isOn = v[i][j][k].isOn;
            }
            re.push_back(line);
        }
    } else {
        line.resize(nx);
        for(int j = 0; j < nz; j++){
            for(int k = 0; k < nx; k++){
                line[k].r = v[j][i][k].r;
                line[k].g = v[j][i][k].g;
                line[k].b = v[j][i][k].b;
                line[k].isOn = v[j][i][k].isOn;
            }
            re.push_back(line);
        }
    }
    */
    return re;
}
