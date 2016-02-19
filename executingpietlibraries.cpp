#include "executingpietlibraries.h"
#include "piettree.h"
ExecutingPietLibraries::ExecutingPietLibraries(){}

#include <QLibrary>
#include <stdarg.h>

using namespace std;
//template< typename F >auto curry( F f ){return [=]( auto a ){ return [=]( auto... args ){ return f( a, args... ); }; };}
#define LIBCALL(RES) QLibrary lib(libname);lib.load(); string stdpath =  Path.toStdString(); const char* stdcpath = stdpath.c_str(); auto func = (RES(*)(Args...)) lib.resolve(stdcpath) ;
template<typename... Args>
inline void retLibv(QString libname,QString Path,const Args&... args ){ LIBCALL(void)
    if(func != 0) { func(args...); } lib.unload();}
template<typename Res,typename... Args>
inline Res retLib(QString libname,QString Path,const  Args&... args ){ LIBCALL(Res)
      Res res; if(func != 0) { res = func(args...); } lib.unload();return res;}
template<typename Res,typename... Args>
inline Res retLibc(QString libname,QString Path,bool & Correct,const  Args&... args ){ LIBCALL(Res)
      Res res; Correct =false; if(func != 0) { res = func(args...); Correct = true; } lib.unload();return res;}

template <typename T> T FromPietTree(PietTree &pt){return (T)pt.Val();}
#define FROMPIETTREE(res,pt) string tmp ## pt = pt.toString().toStdString();const char* res = tmp ## pt .c_str();
template <void*> void* FromPietTree(PietTree &pt){return nullptr;}

union Various{
public:
    inline static Various FromFLOAT  (const PietTree &pt){Various v;v.FLT = (float)pt.Val(); return v;}
    inline static Various FromINT    (const PietTree &pt){Various v;v.INT = (int)pt.Val(); return v;}
    inline static Various FromUINT   (const PietTree &pt){Various v;v.UIN = (unsigned int)pt.Val(); return v;}
    inline static Various FromCHAR   (const PietTree &pt){Various v;v.CHA = (char)pt.Val(); return v;}
    inline static Various FromSHORT  (const PietTree &pt){Various v;v.SHT = (short)pt.Val(); return v;}
    inline static Various FromBYTE   (const PietTree &pt){Various v;v.BYT = (unsigned char)pt.Val(); return v;}
    inline static Various FromWORD   (const PietTree &pt){Various v;v.WRD = (unsigned short)pt.Val(); return v;}
    inline static Various FromHANDLE (const PietTree &pt){Various v;v.VP =(void*) pt.Val(); return v;}
    inline static Various FromLPSTR  (PietTree &pt){
        Various v;
        auto tmp = pt.toString().toStdString();
        auto size = tmp.size();
        v.CCP = (char *)calloc(size + 1,sizeof(char));
        REP(i,size){ v.CCP[i] = tmp[i];}
        v.CCP[size] = '\0';
        return v;}
    inline static Various FromQChar (PietTree pt,QChar c){
        switch(c.unicode()){
        case 'v':case'V':  return FromHANDLE(pt);
        case 'C':          return FromLPSTR(pt);
        case 'f':case 'F': return FromFLOAT(pt);
        case 'w':case 'W': return FromWORD(pt);
        case 'b':case 'B': return FromBYTE(pt);
        case 'u':case 'U': return FromUINT(pt);
        case 's':case 'S': return FromSHORT(pt);
        case 'c':          return FromCHAR(pt);
        default :
        case 'i':case 'I': return FromINT(pt);
        }
    }
    void* VP; char* CCP;
    unsigned int UIN;int INT; float FLT;char CHA;short SHT; unsigned char BYT;unsigned short WRD;
};
#define TMPSTR(NEWNAME,PTS) auto TMP ## PTS = PTS.toString().toStdString(); auto NEWNAME = TMP ## PTS.c_str();

#define PIETTREELOADDLLMISS {Miss = true; return PietTree(0);}

#define SWITCHZEROTYPE     \
if(size == 0){\
switch(TypeNames[size].unicode()){ \
    case 'v':case 'V':retLibv(dllname,funcname);Miss = true; break; \
    case 's':case 'S':res = PietTree((int)retLib<short>(dllname,funcname));break; \
    case 'c':case 'C':res = PietTree((int)retLib<char>(dllname,funcname ));break; \
    case 'u':case 'U':res = PietTree((int)retLib<int>(dllname,funcname  ));break; \
    case 'b':case 'B':res = PietTree((int)retLib<unsigned char>(dllname ,funcname));break; \
    case 'w':case 'W':res = PietTree((int)retLib<unsigned short>(dllname,funcname));break; \
    case 'f':case 'F':res = PietTree((int)retLib<float>(dllname,funcname ));break; \
    default:case 'i':case 'I':res = PietTree(retLib<int>(dllname,funcname));break; \
} \
}

#define SWITCHTYPE(N,...)     \
if(size == N){\
switch(TypeNames[size].unicode()){ \
    case 'v':case 'V':retLibv(dllname,funcname,__VA_ARGS__);Miss = true; break; \
    case 's':case 'S':res = PietTree((int)retLib<short>(dllname,funcname,__VA_ARGS__));break; \
    case 'c':case 'C':res = PietTree((int)retLib<char>(dllname,funcname,__VA_ARGS__));break; \
    case 'u':case 'U':res = PietTree((int)retLib<int>(dllname,funcname,__VA_ARGS__));break; \
    case 'b':case 'B':res = PietTree((int)retLib<unsigned char>(dllname,funcname,__VA_ARGS__));break; \
    case 'w':case 'W':res = PietTree((int)retLib<unsigned short>(dllname,funcname,__VA_ARGS__));break; \
    case 'f':case 'F':res = PietTree((int)retLib<float>(dllname,funcname,__VA_ARGS__));break; \
    default:case 'i':case 'I':res = PietTree(retLib<int>(dllname,funcname,__VA_ARGS__));break; \
} \
}

PietTree ExecutingPietLibraries::LoadDLL(bool& Miss ,QString dllname,QString funcname,QString TypeNames,std::vector<PietTree>& pts){
    int size = TypeNames.size() - 1;
    if (size > 12) PIETTREELOADDLLMISS // Too Many
    if (size < 0) PIETTREELOADDLLMISS
    if (pts.size() < size) PIETTREELOADDLLMISS
    vector<Various> v;
    PietTree res(0);
    REP(i,size)v.push_back(Various::FromQChar(pts[i],TypeNames[i]));
    SWITCHZEROTYPE(0);
    SWITCHTYPE(1,v[0]);
    SWITCHTYPE(2,v[0],v[1]);
    SWITCHTYPE(3,v[0],v[1],v[2]);
    SWITCHTYPE(4,v[0],v[1],v[2],v[3]);
    SWITCHTYPE(5,v[0],v[1],v[2],v[3],v[4]);
    SWITCHTYPE(6,v[0],v[1],v[2],v[3],v[4],v[5]);
    SWITCHTYPE(7,v[0],v[1],v[2],v[3],v[4],v[5],v[6]);
    SWITCHTYPE(8,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7]);
    SWITCHTYPE(9,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8]);
    SWITCHTYPE(10,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]);
    SWITCHTYPE(11,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10]);
    SWITCHTYPE(12,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11]);
    REP(i,size) {if (TypeNames[i].unicode() == 'V'){ pts[i] = PietTree((int)v[i].VP);}}
    REP(i,size) {if (TypeNames[i].unicode() == 'C'){ free (v[i].CCP);}}
    return res;
}

