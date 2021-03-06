#include "piettree.h"


//PietTree型全てOK
void PietTree::MakeStackByMod0(int n,std::vector<PietTree> & pts){
    //nは既にPopされているとして
    if(n <= 0) return ;
    int size = pts.size();
    int min = n < size ? n : size ;
    if(min < 0) min = 0;if(min == 0)return;
    std::vector<PietTree> NewNode;
    for(auto i:range(min)){
        NewNode.push_back(PietTree(pts[size  - min + i ]));
    }
    for(auto i:range(min)){ pts.pop_back();}
    pts.push_back(PietTree(NewNode));
}

//[[][HER[erere]]HH]
//aaaaa => a
//]     => ]
//[]
//[,
//e
//[] or e
//[[[]]]
//[[]]
PietTree PietTree::fromString(QString &treestr){
    if(treestr.isNull())return PietTree(QString(""));
    if(treestr.at(0).unicode() == '['){
        std::vector<PietTree> pts;
        treestr.remove(0,1);
        while (treestr.size() > 0){
            if(treestr.at(0).unicode() == '['){
                pts.push_back(fromString(treestr));
            }else  if(treestr.at(0).unicode() == ']'){
                treestr.remove(0,1);
                break;
            }else {
                auto c = treestr.at(0);
                treestr.remove(0,1);
                pts.push_back( PietTree(c));
            }
        }
        return PietTree(pts);
    }else{
        auto c = treestr.at(0);
        treestr.remove(0,1);
        return PietTree(c);
    }
}



PietTree::PietTree(const QString &str){
    isleaf = false;
    for(QChar c : str) nodes.push_back(PietTree(c));
}


//@show :OK
//isleaf:OK
//isEmptyTree:OK
QString PietTree::showStack(bool asNumber) const {
    if( isleaf ) { //ただし、[0 ~ 31(001F)], [127(007F) ~ 160(00A0)] はどちらも数字で表示
        if(asNumber) return QString::number(val);
        else if ((0 <= val  && val < 32) || (127 <= val  && val <= 160 ) || (val < 0))
            return QString("%(") + QString::number(val) + QString(")");
        else return QString(val);
    }
    QString res("[");
    for(auto i:range(nodes.size())){
        res += nodes[i].showStack(asNumber) ;
        if((i != nodes.size()-1) && asNumber)res +=  QString(",");
    }
    res += QString("]");
    return res;
}

//@Not  : OK
//isleaf: @NOT
//isEmptyTree : OK
int PietTree::Not() const{
   if(isleaf) return val == 0 ? 1 : 0;
   else return isEmptyTree();
}


//@Switch:×
//isleaf:OK
//isEmptyTree:OK
void PietTree::flatten(){
    if(isleaf) return;
    if(isEmptyTree())return;
    std::vector<PietTree> newNodes ;
    for(auto n : nodes){
        if(n.isLeaf()) newNodes.push_back(n);
        else {
            n.flatten();
            for(auto& n2 : n.Nodes()){
                newNodes.push_back(n2);
            }
        }
    }
    nodes = newNodes;
}

//@Point:×
//isleaf:×
//isEmptyTree:OK

PietTree PietTree::popHead(){
    //if(isleaf) //未定義
    if(isEmptyTree()) return nodes;
    PietTree top = nodes[nodes.size()-1];
    nodes.pop_back();
    return top;
}


//@Out(C): OK
//isLeaf : OK
//isEmptyTree:OK
//副作用 : flatten
QString PietTree::toString(){
    if(isleaf)return QString(val);
    flatten();
    QString res("");
    for(auto& n : nodes){
        res += QString(n.Val());
    }
    return res;
}


//@Add          : OK
//isLeaf*isleaf : @ADD
//isLeaf*______ : OK  A + [...]  => [A,...]
//______*isleaf : OK  [...] + A => [...,A]
//______*______ : OK
void PietTree::append(const PietTree& pt){ //@Add
    if(isleaf) {
        if(pt.isLeaf()) {
            val += pt.Val(); return;
        } else {
            isleaf = false;
            nodes.clear();
            nodes.push_back(PietTree(val));
        }
    } else if(pt.isLeaf()) {
        nodes.push_back(PietTree(pt.Val())); return;
    }
    auto an = pt.copyNodes();
    for(auto n : an )  nodes.push_back(n);
}

//@Sub          : OK
//isLeaf*isleaf : @Sub
//isLeaf*______ : OK
//______*isleaf : OK
//______*______ : OK
void PietTree::split(PietTree & pt){
    if(isleaf && pt.isLeaf()){
        val -= pt.Val(); return;
    }
    auto base = this->toString();
    auto rx = QRegExp(pt.toString());
    QStringList spilited = base.split(rx);
    isleaf = false;
    this->nodes.clear();
    for(auto str : spilited) nodes.push_back(PietTree(str));
}

//@Mul          : OK
//isLeaf*isleaf : @MUL
//isLeaf*______ : OK
//______*isleaf : OK
//______*______ : OK
void PietTree::product(const PietTree & pt){
    if(isleaf){
        if(pt.isLeaf()) {
            val *= pt.Val(); return;
        } else {
            isleaf = false;
            nodes.clear();
            nodes.push_back(PietTree(val));
        }
    }
    std::vector<PietTree> HerNodes =
        pt.isLeaf()?
        std::vector<PietTree>{PietTree(pt.Val())}:
        pt.copyNodes();
    std::vector<PietTree> newNodes ;
    for(auto her : HerNodes){
        for(auto my : nodes){
            std::vector<PietTree> New;
            New.push_back(my);
            New.push_back(her);
            newNodes.push_back( PietTree(New));
        }
    }
    nodes = newNodes;
}

//@Div          : OK
//isLeaf*isleaf : @Div
//isLeaf*______ : OK
//______*isleaf : OK
//______*______ : OK
void PietTree::match(PietTree &pt){
    if(isleaf && pt.isLeaf()){
        val /= pt.Val(); return;
    }
    QString base = this->toString();
    QRegExp rx = QRegExp(pt.toString());
    QStringList matched;
    for(int pos = 0; (pos = rx.indexIn(base, pos)) != -1;) {
        matched << rx.cap();
        pos += rx.matchedLength();
    }
    isleaf = false;
    this->nodes.clear();

    for(auto str : matched) nodes.push_back(PietTree(str));
    if(matched.size()>0) nodes.push_back(base);
}

//@Mod          : OK
//isLeaf*isleaf : @Mod
//isLeaf*______ : OK
//______*isleaf : OK
//______*______ : OK
void PietTree::zip(const PietTree &pt){
    if(isleaf){
        if(pt.isLeaf()) {
            val = val % pt.Val(); return;
        } else {
            isleaf = false;
            nodes.clear();
            nodes.push_back(PietTree(val));
        }
    }
    std::vector<PietTree> HerNodes =
        pt.isLeaf()?
        std::vector<PietTree>{PietTree(pt.Val())}:
        pt.copyNodes();
    int min = nodes.size() > HerNodes.size() ? HerNodes.size() : nodes.size();
    std::vector<PietTree> newNodes ;
    for(auto i:range(min)){
        std::vector<PietTree> New;
        New.push_back(nodes[i]);
        New.push_back(pt.Nodes()[i]);
        newNodes.push_back( PietTree(New));
    }
    nodes = newNodes;
}

//未実装 @Greater
void PietTree::loadFile(const PietTree &pt){
    if(isleaf){
        if(pt.isLeaf()){
            val = val < pt.Val() ? 0 : 1 ; return;
        }
    }
     return;
}
