#include "paintwidget.h"
#include "ui_paintwidget.h"
#include <QMatrix4x4>


PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintWidget)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::StrongFocus);

    BaseLevel* root = new BaseLevel();
    root->pos = QPoint(200,200);
    root->UpdateNameUsingTime();
    m_baselevels.append(root);
}



// implement a sketch pad.
void PaintWidget::paintEvent(QPaintEvent *e)
{
    QPainter* painter = new QPainter(this);

    painter->setRenderHints(QPainter::Antialiasing);


    // render pure white background.
    painter->setBrush(QBrush(QColor(45,45,45)));
    QPen pen;
    pen.setColor(QColor(250,250,250));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawRect(QRect(0, 0, this->width(), this->height()));


    QPen penSelect;
    penSelect.setColor(QColor(250,10,10));
    penSelect.setWidth(5);


    if(isDrawLine)
    {
        painter->setPen(QPen(QBrush(QColor(255,255,255)), 3));
        painter->drawLine(this->m_curPtr->pos, this->m_curMousePos);
    }

    for(int i=0; i<m_baselevels.size(); ++i)
    {
        painter->setPen(QPen(QBrush(QColor(255,255,255)), 3));

        BaseLevel* a = m_baselevels[i];
        QList<BaseLevel*>& edges = m_edges[a];

        for(int k=0; k<edges.size(); ++k)
        {
            painter->drawLine(a->pos, edges[k]->pos);
        }
    }

    if(m_curEdgeA != nullptr && m_curEdgeB != nullptr)
    {
        painter->setPen(QPen(QBrush(QColor("#E3513E")), 3));
        painter->drawLine(this->m_curEdgeA->pos, this->m_curEdgeB->pos);
    }
    for(int i=0; i<m_baselevels.size(); ++i)
    {
        QColor color;

        if(m_baselevels[i]->type == "BaseLevel")
            color = QColor("#63a2bb");
        if(m_baselevels[i]->type == "TrunkLevel")
            color = QColor(255,165,165);
        if(m_baselevels[i]->type == "BranchLevel")
            color = QColor(105,105,205);
        if(m_baselevels[i]->type == "LeafLevel")
            color = QColor(105,205,105);

        if(m_curPtr == m_baselevels[i])
            painter->setPen(penSelect);
        else
            painter->setPen(pen);

        painter->setBrush(QBrush(color));
        painter->drawEllipse(m_baselevels[i]->pos, 25, 25);

        painter->setFont(QFont("Arial", 10, QFont::Bold));
        QString nodeType = GetPrintName(m_baselevels[i]);
        painter->drawText(m_baselevels[i]->pos+QPoint(-4*(nodeType.size()), 40), nodeType);
    }

    delete painter;
}


void PaintWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
    {
        if(this->m_curPtr != nullptr && m_curPtr->type != "BaseLevel" )   // 删除节点的情况
        {
            // 首先删除该节点对应的所有边。
            for(int i =0; i<m_baselevels.size(); ++i)
            {
                m_edges[m_baselevels[i]].removeAll(this->m_curPtr);
            }
            m_edges.remove(this->m_curPtr);

            for(int i=0; i<m_baselevels.count();++i)   // 遍历所有节点，找到m_curPtr，并删除
            {
                if(m_baselevels[i]->type == "BaseLevel" || i==0)
                    continue;
                if(this->m_curPtr == m_baselevels[i])
                {
                    delete m_baselevels[i];
                    m_baselevels.removeAt(i);
                    m_curPtr = nullptr;

                    break;
                }
            }
        }
        if(this->m_curEdgeA != nullptr && this->m_curEdgeB!= nullptr)   // 删除选中的边的情况。
        {
            m_edges[m_curEdgeA].removeAll(this->m_curEdgeB);
            m_edges[m_curEdgeB].removeAll(this->m_curEdgeA);

            m_curEdgeA = m_curEdgeB = nullptr;
        }
        this->update();
        return;
    }

        if(e->modifiers()&Qt::ControlModifier && e->key() == Qt::Key_V)
        {
            if(m_curPtr != nullptr)
            {
                
            }
            return;
        }

        if(e->key() == Qt::Key_Z && e->modifiers() & Qt::ControlModifier)
        {
            this->update();
        }

}

void PaintWidget::keyReleaseEvent(QKeyEvent *e)
{
}



void PaintWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton  && e->modifiers()&Qt::ControlModifier)
    {
        m_curEdgeA = m_curEdgeB = nullptr;
        m_curPtr = nullptr;
        BaseLevel* newNode = CreateNewNode(e->pos());
        this->m_baselevels.push_back(newNode);
        this->m_edges[newNode] =QList<BaseLevel*>();

        this->update();
        emit sig_selectedChanged();
        return;
    }

    if(e->buttons() & Qt::LeftButton && e->modifiers()&Qt::AltModifier)
    {
        float minDist = INT_MAX;
        m_curPtr = nullptr;
        for(int i=0; i<m_baselevels.count(); ++i)
        {
            float dist =  QVector2D(e->pos()).distanceToPoint(QVector2D(m_baselevels[i]->pos));

            if(dist < minDist && dist < 25)
            {
                m_curPtr = (m_baselevels[i]);
                minDist = dist;
            }
        }

        if(m_curPtr != nullptr)
        {
            m_curMousePos = e->pos();
            this->isDrawLine = true;
        }
        this->update();
        emit sig_selectedChanged();
        return;
    }

    if(e->buttons() & Qt::LeftButton)
    {
        // 首先判断选择顶点
        float minDist = INT_MAX;
        m_curPtr = nullptr;
        for(int i=0; i<m_baselevels.count(); ++i)
        {
            float dist =  QVector2D(e->pos()).distanceToPoint(QVector2D(m_baselevels[i]->pos));

            if(dist < minDist && dist < 25)
            {
                m_curPtr = (m_baselevels[i]);
                minDist = dist;
            }
        }

        emit sig_selectedChanged();

        // 如果没有选择到顶点，那么检查选择边。
        if(m_curPtr == nullptr)
        {
            float minDist = INT_MAX;
            m_curEdgeA= m_curEdgeB = nullptr;
            for(int i=0; i<m_baselevels.count(); ++i)
            {
                auto startNode = m_baselevels[i];
                auto edges = m_edges[startNode];

                for(int k=0; k<edges.size(); ++k)
                {
                    float dist =  UtilFuncs::pointToSegmentDistance(e->pos(), startNode->pos, edges[k]->pos);

                    if(dist < minDist && dist < 3)
                    {
                        m_curEdgeA = startNode;
                        m_curEdgeB = edges[k];
                        minDist = dist;
                    }
                }
            }
        }
        this->update();
        return;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_curPtr != nullptr && isDrawLine == false)
    {
        m_curPtr->pos = e->pos();
        this->update();
    }
    if(m_curPtr != nullptr && isDrawLine == true)
    {
        m_curMousePos = e->pos();
        this->update();
    }

}

void PaintWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(isDrawLine == true)
    {
        float minDist = INT_MAX;
        BaseLevel* m_nextPtr = nullptr;
        for(int i=0; i<m_baselevels.count(); ++i)
        {
            float dist =  QVector2D(e->pos()).distanceToPoint(QVector2D(m_baselevels[i]->pos));

            if(dist < minDist && dist < 25)
            {
                m_nextPtr = (m_baselevels[i]);
                minDist = dist;
            }
        }

        if(m_nextPtr != nullptr)
        {
            if(m_nextPtr != m_curPtr) // 不能是同一个点
            {
                if(m_edges[m_curPtr].contains(m_nextPtr) == false)
                    m_edges[m_curPtr].append(m_nextPtr);
                if(m_edges[m_nextPtr].contains(m_curPtr) == false)
                    m_edges[m_nextPtr].append(m_curPtr);
            }
        }

        m_curPtr = nullptr;
        isDrawLine = false;
        this->update();
        emit sig_selectedChanged();
        return;
    }

}



BaseLevel* PaintWidget::CreateNewNode(QPoint pos)
{
    if(this->currentAddNodeType == "TrunkLevel")
    {
        TrunkLevel* node = new TrunkLevel();
        node->type = currentAddNodeType;
        node->pos = pos;
        node->UpdateNameUsingTime();
        return node;
    }
    if(this->currentAddNodeType == "BranchLevel")
    {
        BranchLevel* node = new BranchLevel();
        node->type = currentAddNodeType;
        node->pos = pos;
        node->UpdateNameUsingTime();
        return node;
    }
    if(this->currentAddNodeType == "LeafLevel")
    {
        LeafLevel* node = new LeafLevel();
        node->type = currentAddNodeType;
        node->pos = pos;
        node->UpdateNameUsingTime();
        return node;
    }
}

QString PaintWidget::GetPrintName(BaseLevel*& node)
{
    if(node->type == "BaseLevel")
        return "Root";
    if(node->type == "TrunkLevel")
        return "Trunk";
    if(node->type == "BranchLevel")
        return "SubBranch";
    if(node->type == "LeafLevel")
        return "Leaf";
}


void PaintWidget::SaveConstructedTreeStructure(const QString& filenameNode, const QString& filenameEdge)
{
    // 清理Graph结构。
    {
        this->CleanConnectionGraph();

        QString contentEdges = "";
        QQueue<BaseLevel*> queue;
        queue.enqueue(m_baselevels[0]);

        while(queue.isEmpty() == false)
        {
            auto cur = queue.dequeue();
            QList<BaseLevel*> childs =  m_edges[cur];

            QString line = cur->name +" ";
            for(int i=0; i<childs.size(); ++i)
            {
                auto child = childs[i];
                line += (child->name +" ");
                queue.enqueue(child);
            }
            contentEdges += line +"\n";
        }

        QFile file(filenameEdge);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << contentEdges;
        file.close();
    }

    // 保存Json结点信息。
    {
        QString contentNodes = "";
        for(int i=0; i<this->m_baselevels.size(); ++i)
        {
            QString info;
            if(m_baselevels[i]->type == "BaseLevel")
                info = static_cast<BaseLevel*>(m_baselevels[i])->toString();
            if(m_baselevels[i]->type == "TrunkLevel")
                info = static_cast<TrunkLevel*>(m_baselevels[i])->toString();
            if(m_baselevels[i]->type == "BranchLevel")
                info = static_cast<BranchLevel*>(m_baselevels[i])->toString();
            if(m_baselevels[i]->type == "LeafLevel")
                info = static_cast<LeafLevel*>(m_baselevels[i])->toString();

            contentNodes +=info +"\n";
        }

        QFile file(filenameNode);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << contentNodes;
        file.close();
    }
    this->update();
}

void PaintWidget::LoadConstructedTreeStructure(const QString &filenameNode, const QString &filenameEdge)
{
    this->ClearAll(false);

    QMap<QString, BaseLevel*> idMap;
    {   // 首先加载nodes信息
        QFile file(filenameNode);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "无法打开文件:" << filenameNode;
            this->ClearAll(true);
            return;
        }
        QTextStream in(&file);
        QStringList lines = in.readAll().split("\n");

        for(int i=0; i<lines.size(); ++i)
        {
            QStringList items = lines[i].split(QRegExp("\\s+"));   // 分割
            items.removeAll("");
            if (items.size() == 0)  // 空行走开。
                continue;
            //            qDebug()<<items;

            if (items.size() >=2 && items[0] == "+++" && items[1] == "BaseLevel")  // 新增
            {
                BaseLevel* curNode = new BaseLevel();
                i = BaseLevel::paseBaseLevel(lines, i + 1, curNode);
                this->m_baselevels.append(curNode);
                idMap[curNode->name] = curNode;
            }
            if (items.size() >= 2 && items[0] == "+++" && items[1] == "TrunkLevel")  // 新增
            {
                TrunkLevel* curNode = new TrunkLevel();
                i = TrunkLevel::paseTrunkLevel(lines, i + 1, curNode);
                this->m_baselevels.append(curNode);
                idMap[curNode->name] = curNode;
            }

            if (items.size() >= 2 && items[0] == "+++" && items[1] == "BranchLevel")  // 新增
            {
                BranchLevel* curNode = new BranchLevel();
                i = BranchLevel::paseBranchLevel(lines, i + 1, curNode);
                this->m_baselevels.append(curNode);
                idMap[curNode->name] = curNode;
            }
            if (items.size() >= 2 && items[0] == "+++" && items[1] == "LeafLevel")  // 新增
            {
                LeafLevel* curNode = new LeafLevel();
                i = LeafLevel::paseLeafLevel(lines, i + 1, curNode);
                this->m_baselevels.append(curNode);
                idMap[curNode->name] = curNode;
            }
        }
    }
    { // 然后加载边连接的信息。
        QFile file(filenameEdge);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "无法打开文件:" << filenameEdge;
            this->ClearAll(true);
            return;
        }
        QTextStream in(&file);
        QStringList lines = in.readAll().split("\n");


        for (int i = 0; i < lines.size(); ++i)
        {
            QStringList items = lines[i].split(QRegExp("\\s+"));   // 分割
            items.removeAll("");
            if (items.size() == 0)  // 空行走开。
                continue;

            QString curNodeName = items[0];
            m_edges[idMap[curNodeName]] = QList<BaseLevel*>();

            for(int k=1; k< items.size(); ++k)
            {
                if(idMap.contains(items[k]))
                    m_edges[idMap[curNodeName]].append(idMap[items[k]]);
            }
        }
    }
    this->update();
}

void PaintWidget::CleanConnectionGraph()
{
    QMap<BaseLevel*, QList<BaseLevel* > > final_edges;

    QQueue<BaseLevel*> queue;
    queue.enqueue(m_baselevels[0]);

    QMap<BaseLevel*, bool> visited;
    for(int i=0; i<m_baselevels.size(); ++i)
        visited[m_baselevels[i]] = false;
    visited[m_baselevels[0]] = true;

    while(queue.isEmpty() == false)
    {
        auto cur = queue.dequeue();

        QList<BaseLevel*> childs =  m_edges[cur];

        for(int i=0; i<childs.size(); ++i)
        {
            auto child = childs[i];

            if(child != cur && visited[child] == false)   // 不能是同结点，且尚未访问过。
            {
                final_edges[cur].append(child);
                visited[child] = true;
                queue.enqueue(child);
            }
        }
    }

    this->m_edges.clear();

    this->m_edges = final_edges;
}


void PaintWidget::ClearAll(bool isCreatedRoot = false)
{
    for(int i=0; i<m_baselevels.size(); ++i)
        delete m_baselevels[i];
    m_baselevels.clear();
    m_edges.clear();
    m_curPtr = nullptr;
    m_curEdgeA = m_curEdgeB = nullptr;

    if(isCreatedRoot == true)
    {
        BaseLevel* root = new BaseLevel();
        root->pos = QPoint(200,200);
        root->UpdateNameUsingTime();
        m_baselevels.append(root);
    }
}

PaintWidget::~PaintWidget()
{
    delete ui;
}
