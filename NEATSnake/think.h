#ifndef THINK_H
#define THINK_H

#include <iostream>
#include <list>

extern int POPULATION;

class Graph
{
    int numVertices;
    std::list<int> *adjlists;
    bool *visited;

public:
    Graph(int vertices);
    void addEdge(int src, int dest);
    int bfs(int startvertex);
};

Graph::Graph(int vertices)
{
    numVertices = vertices;
    adjlists = new std::list<int>[vertices];
}

void Graph::addEdge(int src, int dest)
{
    adjlists[src].push_back(dest);
    adjlists[dest].push_back(src);
}

int Graph::bfs(int startVertex)
{
    visited = new bool[numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        visited[i] = false;
    }

    std::list<int> queue;

    visited[startVertex] = true;
    queue.push_back(startVertex);

    std::list<int>::iterator i;

    while (!queue.empty())
    {
        int currVertex = queue.front();
        return currVertex;
        queue.pop_front();

        for (i = adjlists[currVertex].begin(); i != adjlists[currVertex].end(); i++)
        {
            int adjVertex = *i;
            if (!visited[adjVertex])
            {
                visited[adjVertex] = true;
                queue.push_back(adjVertex);
            }
        }
    }
    return 0;
}

void think(network **brain, Snake *snake, int foodposx, int foodposy)
{
    float input[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < POPULATION; i++)
    {
        if (snake[i].dead == 0)
        {
            //Graph graph(GridSize * GridSize);

            //int headx = snake[i].arr[0].x + (BlockSize / 2);
            //int heady = snake[i].arr[0].y + (BlockSize / 2);

            /*
            int foodx = foodposx + (BlockSize / 2);
            int foody = foodposy + (BlockSize / 2);

            bool foodleft = false;
            bool foodright = false;
            bool foodup = false;
            bool fooddown = false;

            bool collideright = false;
            bool collideleft = false;
            bool collideup = false;
            bool collidedown = false;

            if (headx > foodx)
            {
                foodright = true;
            }
            else
            {
                foodright = false;
            }
            if (headx < foodx)
            {
                foodleft = true;
            }
            else
            {
                foodleft = false;
            }
            if (heady > foody)
            {
                foodup = true;
            }
            else
            {
                foodup = false;
            }
            if (heady < foody)
            {
                fooddown = true;
            }
            else
            {
                fooddown = false;
            }

            for (int j = 1; j < snake[i].length; j++)
            {

                if (headx + BlockSize >= SnakeCanvasW || (headx + BlockSize) / GridSize == snake[i].arr[j].x / GridSize)
                {
                    collideright = true;
                }
                else
                {
                    collideright = false;
                }
                if (headx - (BlockSize * 2) < SnakeCanvasX || (headx - (BlockSize * 2)) / GridSize == snake[i].arr[j].x / GridSize)
                {
                    collideleft = true;
                }
                else
                {
                    collideleft = false;
                }
                if (heady + BlockSize >= SnakeCanvasH || (heady + BlockSize) / GridSize == snake[i].arr[j].y / GridSize)
                {
                    collidedown = true;
                }
                else
                {
                    collidedown = false;
                }
                if (heady - (BlockSize * 2) < SnakeCanvasY || (heady - (BlockSize * 2)) / GridSize == snake[i].arr[j].y / GridSize)
                {
                    collideup = true;
                }
                else
                {
                    collideup = false;
                }
            }

            //graph.addEdge(foodx, headx);
            //graph.addEdge(foody, heady); 
            

            input[0] = fooddown;
            input[1] = foodup;
            input[2] = foodleft;
            input[3] = foodright;
            input[4] = collideright;
            input[5] = collideleft;
            input[6] = collideup;
            input[7] = collidedown;
            input[8] = headx;
            input[8] /= 1000;
            input[9] = heady;
            input[9] /= 1000;
            input[10] = foodx;
            input[10] /= 1000;
            input[11] = foody;
            input[11] /= 1000;
            //input[12] = (float)graph.bfs(headx);
            //input[12] /= 1000;
            //input[13] = (float)graph.bfs(heady);
            //input[13] /= 1000;
            input[12] = snake[i].getDis(headx, heady, foodx, heady);
            input[12] /= 1000;
            input[13] = snake[i].getDis(headx, heady, headx, foody);
            input[13] /= 1000;
            input[14] = snake[i].length;
            input[14] /= 100;
            */

            snake[i].appendVision();

            input[0] = snake[i].rightDisFood;
            input[0] /= 1000;
            input[1] = snake[i].rightDisWall;
            input[1] /= 1000;
            input[2] = snake[i].rightDisTail;
            input[2] /= 1000;
            input[3] = snake[i].leftDisFood;
            input[3] /= 1000;
            input[4] = snake[i].leftDisWall;
            input[4] /= 1000;
            input[5] = snake[i].leftDisTail;
            input[5] /= 1000;
            input[6] = snake[i].upDisFood;
            input[6] /= 1000;
            input[7] = snake[i].upDisWall;
            input[7] /= 1000;
            input[8] = snake[i].upDisTail;
            input[8] /= 1000;
            input[9] = snake[i].downDisFood;
            input[9] /= 1000;
            input[10] = snake[i].downDisWall;
            input[10] /= 1000;
            input[11] = snake[i].downDisTail;
            input[11] /= 1000;
            //input[12] = (float)graph.bfs(headx);
            //input[12] /= 1000;
            //input[13] = (float)graph.bfs(heady);
            //input[13] /= 1000;
            

            setNet(brain[i], input);
            feedforward(brain[i]);

            //up
            if (brain[i]->l[brain[i]->layers - 1].n[0].output > brain[i]->l[brain[i]->layers - 1].n[1].output &&
                brain[i]->l[brain[i]->layers - 1].n[0].output > brain[i]->l[brain[i]->layers - 1].n[2].output &&
                brain[i]->l[brain[i]->layers - 1].n[0].output > brain[i]->l[brain[i]->layers - 1].n[3].output &&
                brain[i]->l[brain[i]->layers - 1].n[0].output > brain[i]->l[brain[i]->layers - 1].n[4].output)
            {
                snake[i].changeDir(up);
            }

            //down
            if (brain[i]->l[brain[i]->layers - 1].n[1].output > brain[i]->l[brain[i]->layers - 1].n[0].output &&
                brain[i]->l[brain[i]->layers - 1].n[1].output > brain[i]->l[brain[i]->layers - 1].n[2].output &&
                brain[i]->l[brain[i]->layers - 1].n[1].output > brain[i]->l[brain[i]->layers - 1].n[3].output &&
                brain[i]->l[brain[i]->layers - 1].n[1].output > brain[i]->l[brain[i]->layers - 1].n[4].output)
            {
                snake[i].changeDir(down);
            }

            //left
            if (brain[i]->l[brain[i]->layers - 1].n[2].output > brain[i]->l[brain[i]->layers - 1].n[1].output &&
                brain[i]->l[brain[i]->layers - 1].n[2].output > brain[i]->l[brain[i]->layers - 1].n[0].output &&
                brain[i]->l[brain[i]->layers - 1].n[2].output > brain[i]->l[brain[i]->layers - 1].n[3].output &&
                brain[i]->l[brain[i]->layers - 1].n[2].output > brain[i]->l[brain[i]->layers - 1].n[4].output)
            {
                snake[i].changeDir(left);
            }

            //right
            if (brain[i]->l[brain[i]->layers - 1].n[3].output > brain[i]->l[brain[i]->layers - 1].n[1].output &&
                brain[i]->l[brain[i]->layers - 1].n[3].output > brain[i]->l[brain[i]->layers - 1].n[2].output &&
                brain[i]->l[brain[i]->layers - 1].n[3].output > brain[i]->l[brain[i]->layers - 1].n[0].output &&
                brain[i]->l[brain[i]->layers - 1].n[3].output > brain[i]->l[brain[i]->layers - 1].n[4].output)
            {
                snake[i].changeDir(right);
            }

            //current
            /*if (brain[i]->l[brain[i]->layers - 1].n[4].output > brain[i]->l[brain[i]->layers - 1].n[1].output &&
                brain[i]->l[brain[i]->layers - 1].n[4].output > brain[i]->l[brain[i]->layers - 1].n[2].output &&
                brain[i]->l[brain[i]->layers - 1].n[4].output > brain[i]->l[brain[i]->layers - 1].n[3].output &&
                brain[i]->l[brain[i]->layers - 1].n[4].output > brain[i]->l[brain[i]->layers - 1].n[0].output)
            {
                snake[i].changeDir(snake[i].direction);
            }*/

            snake[i].update();
        }
    }
}

#endif