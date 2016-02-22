#include "glgamewidget.h"
#include "defines.h"

GLGameWidget* GLGameWidget::UniqueGLWidget = nullptr;
GLGameWidget::GLGameWidget(QWidget *parent):QGLWidget(parent){
    setFormat(QGLFormat(QGL::DoubleBuffer ));
    connect(this,GLGameWidget::destroyed ,[=](){UniqueGLWidget = nullptr;});
}

GLGameWidget* GLGameWidget::MakeUniqueGLWidget (QWidget *parent ){
    if(UniqueGLWidget == nullptr){
        return UniqueGLWidget = new GLGameWidget(parent);
    }else return nullptr;
}


void GLGameWidget::initializeGL(){
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    qglClearColor(Qt::black);
    glOrtho(0.0, w, 0.0, h , -1.0, 1.0);
}

void GLGameWidget::resizeGL(int width,int height){
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, -1.0, 1.0);
}
void GLGameWidget::refresh (){
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT);
}


void GLGameWidget::drawRect(int x ,int y,int w,int h,QColor color){
    qglColor(color);
    glBegin(GL_QUADS);
    glVertex2d(x,y);
    glVertex2d(x+w,y);
    glVertex2d(x+w,y+h);
    glVertex2d(x,y+h);
    glEnd();
}


// 当面2Dで考える 3DにしたかったらGLGameWidget3Dとかつくればいい
// 本当の最低限だけでいいや (本当のゲームのライブラリならもっと関数が必要なところだ)
// 音楽も同期をとらなければ
// texture = bindTexture(QImage(":/image.jpg", GL_TEXTURE_2D));
// QMediaPlayer
//0. L/system         ["ls"]            => output ni are
//1. L/GLShowWindow   [600,400,"title"] => [0|1] (SingleTon) / MainWindow | NULL
//2. L/GLLoadImage    ["res/chihaya.png"]  => [0|handle]     / GLView
//   L/MPLoadAudio    ["res/arcadia.mp3"]  => [0|mhandle] (SingleTon)
//3. L/GLDrawImage    [handle,100,200]  => None              / GLView
//   L/MPPlayAudio    [mhandle]         => None
//4. L/GLProcessAll   []                => None              / GLView
//   L/MPGetPosition  [mhandle]         => 100
//5. L/sleep          [16]              => None
//   L/processmessage []                => 0 | 1
//N. L/GLProcessAll   []                => (processMessage & sleep(1) & showbuffer)
//test := L/GLShowWindow
//                => L/GLDrawRect  [x,y,20,20,FFFFFF] => L/GLProcessAll => ...

//3D memo
//construct : | QGL::DepthBuffer));
//init      : glEnable(GL_DEPTH_TEST);
//resize    : glMatrixMode(GL_PROJECTION);  auto x = GLfloat(width) / height;  glFrustum(-x,x,-1.0,1.0,4.0,15.0);  glMatrixMode(GL_MODELVIEW); //2Dなのでプロジェクションする意味はない
//draw      : glCLear(~ | GL_DEPTH_BUFFER_BIT); glMatrixMode(GL_MODELVIEW);   glLoadIdentity();  glTranslatef(0.0,0.0,-10.0);