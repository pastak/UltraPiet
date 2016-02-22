#ifndef GLGAMEWIDGET_H
#define GLGAMEWIDGET_H
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>


class GLGameWidget : public QGLWidget{
    Q_OBJECT
private:
    GLGameWidget(QWidget *parent = 0) ;//QWidget *parent = 0);
    static GLGameWidget* UniqueGLWidget ;
public:
    static GLGameWidget* MakeUniqueGLWidget (QWidget *parent = 0);
    static GLGameWidget* getUniqueGLWidget(){return UniqueGLWidget;}
    QSize sizeHint() const{return QSize(w,h);}
    void setSize(int w,int h){if(w < 0 || h < 0) return;this->w = w;this->h = h; }
    void refresh ();
    void drawRect(int x ,int y,int w,int h,QColor color);
protected:
    void initializeGL();
    void resizeGL(int width,int height);
    void paintGL(){}
private:
    int w = 600, h = 400;
};


#endif // GLGAMEWIDGET_H