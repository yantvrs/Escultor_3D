#ifndef SCULPTOR_H
#define SCULPTOR_H
#include <string>
#include <vector>

using namespace std;

struct Voxel {

    float r,g,b;
    bool isOn;

};

enum{XY,ZX,YZ};

class Sculptor
{

    protected:
        Voxel ***v;
        int nx,ny,nz;
        float r,g,b;


    public:
        Sculptor(int nx_, int ny_, int nz_);
        void setColor(float r_, float g_, float b_);
        ~Sculptor();


        void putVoxel(int x0, int y0, int z0);
        void cutVoxel(int x0, int y0, int z0);
        void putBox(int x0, int x1, int y0, int y1, int z0, int z1);
        void cutBox(int x0, int x1, int y0, int y1, int z0, int z1);
        void putSphere(int xcenter, int ycenter, int zcenter, int radius);
        void cutSphere(int xcenter, int ycenter, int zcenter, int radius);
        void putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
        void cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz);
        void writeOFF(string filename);
        vector <vector<Voxel>> getPlano(int i, int plano=XY);

};

#endif // SCULPTOR_H
