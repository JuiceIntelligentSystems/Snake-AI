#ifndef NEAT_H
#define NEAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <SDL2/SDL.h>

class neuron;
class network;
class layer;

void calcOutput(network*, int, int);
network *confNet(int, int[]);
network *copyNet(network*);
void feedforward(network*);
neuron *getneuronP(network*, int, int);
void mutateNet(network*, int, int);
void printNet(network*, int);
float randomfloat(void);
void setNet(network*, float[]);
int sum(int*, int);
void exportNet(network*, char*);
network *importNet(char*);
float calcAvgWeight(network*);
void connectNeuron(neuron*, neuron*);
void disconnectNeuron(neuron*, neuron*);
void setWeight(neuron*, neuron*, float);
void drawNet(network*, SDL_Renderer*, int, int, int, int, int);
void SDL_RenderFillCircle(SDL_Renderer*, int, int, int);

typedef struct connection
{

    neuron *n;
    float weight;

} connection;

class neuron
{
public:
    connection *connect;
    int num_connections;
    float sum;
    float output;

    int l;
    int n;
};

class layer
{
public:
    neuron *n;
};

class network
{
public:
    layer *l;
    int *nmap;
    int layers;
};

int sum(int *array, int size)
{

    int s = 0;

    for (int i = 0; i < size; i++)
    {
        s += array[i];
    }

    return s;
}

neuron *getneuronP(network *Net, int L, int N)
{
    return &(Net->l[L].n[N]);
}

float randomfloat(void)
{

    int b = rand() % 2;
    int a = (rand() % 9999) + 999;

    //printf("a : %d - b : %d - 1/a = %f\n", a, b,(float) 999/a);

    return ((float) 999/a * b) + ((float) -999/a * (1-b));
}

float calcAvgWeight(network *Net)
{

    float s = 0;
    int num_connections = 0;

    for (int i = 0; i < Net->layers; i++)
    {
        for (int j = 0; j < Net->nmap[i]; j++)
        {
            for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {
                s += Net->l[i].n[j].connect[k].weight;
                num_connections++;
            }
        }
    }
    return (s / num_connections);
}

network *confNet(int _layers, int _nmap[])
{
    network *Net;

    Net = (network *)malloc(sizeof(network)); 

    Net->layers = _layers;
    Net->nmap = (int *)malloc( _layers * sizeof (int) );

    for(int i = 0; i < Net->layers; i++)
    {
        Net->nmap[i] = _nmap[i];
    }

    Net->l = (layer *)malloc(Net->layers * sizeof(layer));

    for(int i = 0; i < Net->layers; i++)
    {
        Net->l[i].n = (neuron *)malloc(sizeof(neuron) * Net->nmap[i]);

        for(int j = 0; j < Net->nmap[i]; j++)
        {
            Net->l[i].n[j].connect = (connection *)malloc(sizeof(connection) * sum(Net->nmap, i));

            Net->l[i].n[j].l = i;
            Net->l[i].n[j].n = j;

            for(int ii = 0, k = 0 ; ii < i && k < sum(Net->nmap, i); ii++)
            {
                for(int jj = 0; jj < Net->nmap[ii] && k < sum(Net->nmap, i); jj++, k++)
                {
                    Net->l[i].n[j].connect[k].n = getneuronP(Net, ii, jj);
                    Net->l[i].n[j].connect[k].weight = randomfloat();

                    if(Net->l[i].n[j].connect[k].weight > 1) {
                        Net->l[i].n[j].connect[k].weight = 1;
                    }
                    else if (Net->l[i].n[j].connect[k].weight < -1) {
                        Net->l[i].n[j].connect[k].weight = -1;
                    }
                }
            }

            Net->l[i].n[j].sum = 0; 
            Net->l[i].n[j].output = 0;
            Net->l[i].n[j].num_connections = (sum(Net->nmap, i));
        }
    }
    return Net;
}

void calcOutput(network *Net, int L, int N)
{

    float s = 0;

    if (L != 0)
    {

        for (int i = 0; i < Net->l[L].n[N].num_connections; i++)
        {

            s += Net->l[L].n[N].connect[i].n->output * Net->l[L].n[N].connect[i].weight;
        }

        Net->l[L].n[N].sum = s;
        Net->l[L].n[N].output = tanh(s);
    }
}

void feedforward(network *Net)
{

    for (int i = 1; i < Net->layers; i++)
    {
        for (int j = 0; j < Net->nmap[i]; j++)
        {
            calcOutput(Net, i, j);
        }
    }
}

void printNet(network *Net, int detailed)
{

    if (detailed)
        fprintf(stdout, "----------------------------------------\n");

    if (detailed)
    {

        fprintf(stdout, "In Net 0x%x\n", Net);

        for (int j = 0; j < Net->nmap[0]; j++)
        {
            fprintf(stdout, "Net->l[0].n[%d].output = %f\n", j, Net->l[0].n[j].output);
            fprintf(stdout, "Net->l[0].n[%d].number of connections = 1\n", j);
            fprintf(stdout, "Net->l[0].n[%d].connect[0].weight = 1\n", j);
        }

        for (int i = 1; i < Net->layers - 1; i++)
        {
            fprintf(stdout, "----------------------------------------\n");

            for (int j = 0; j < Net->nmap[i]; j++)
            {
                fprintf(stdout, "Net->l[%d].n[%d].sum = %f\n", i, j, Net->l[i].n[j].sum);
                fprintf(stdout, "Net->l[%d].n[%d].output = %f\n", i, j, Net->l[i].n[j].output);
                fprintf(stdout, "Net->l[%d].n[%d].number of connections = %d\n", i, j, Net->l[i].n[j].num_connections);

                for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {
                    fprintf(stdout, "Net->l[%d].n[%d].connect[%d].weight = %f\n", i, j, k, Net->l[i].n[j].connect[k].weight);
                    /*    fprintf(stdout, "Net->l[%d].n[%d].connect[%d].n->sum = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->sum); */
                    fprintf(stdout, "Net->l[%d].n[%d].connect[%d].n->output = %f\n", i, j, k, Net->l[i].n[j].connect[k].n->output);
                }
            }
        }
    }

    fprintf(stdout, "----------------------------------------\n");

    if (!detailed)
        fprintf(stdout, "In Net 0x%x\n", Net);

    for (int i = 0; i < Net->nmap[Net->layers - 1]; i++)
    {
        fprintf(stdout, "Net->l[%d].n[%d].sum = %f\n", Net->layers - 1, i, Net->l[Net->layers - 1].n[i].sum);
        fprintf(stdout, "Net->l[%d].n[%d].output = %f\n", Net->layers - 1, i, Net->l[Net->layers - 1].n[i].output);

        fprintf(stdout, "Net->l[%d].n[%d].number of connections = %d\n", Net->layers - 1, i, Net->l[Net->layers - 1].n[i].num_connections);

        for (int k = 0; k < Net->l[Net->layers - 1].n[i].num_connections; k++)
        {
            fprintf(stdout, "Net->l[%d].n[%d].connect[%d].weight = %f\n", Net->layers - 1, i, k, Net->l[Net->layers - 1].n[i].connect[k].weight);
            /*    printf("Net->l[%d].n[%d].connect[%d].n->sum = %f\n", Net->layers-1, i, k, Net->l[Net->layers-1].n[i].connect[k].n->sum); */
            fprintf(stdout, "Net->l[%d].n[%d].connect[%d].n->output = %f\n", Net->layers - 1, i, k, Net->l[Net->layers - 1].n[i].connect[k].n->output);
        }
    }

    fprintf(stdout, "----------------------------------------\n");
}

void setNet(network *Net, float input[])
{

    for (int i = 0; i < Net->nmap[0]; i++)
    {
        Net->l[0].n[i].output = input[i]; // since there are no previous layers, we store the input as an output
    }
}

void mutateNet(network *Net, int degree, int percent)
{

    if (degree == 0)
    {

        for (int i = 1; i < Net->layers; i++)
        {

            for (int j = 0; j < Net->nmap[i]; j++)
            {

                for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {

                    if (rand() % 100 - percent < 0)
                    {
                        Net->l[i].n[j].connect[k].weight += randomfloat();
                    }

                    if (Net->l[i].n[j].connect[k].weight > 1)
                    {
                        Net->l[i].n[j].connect[k].weight = 1;
                    }
                    else if (Net->l[i].n[j].connect[k].weight < -1)
                    {
                        Net->l[i].n[j].connect[k].weight = -1;
                    }
                }
            }
        }
    }
    else if (degree == 1)
    {

        for (int i = 1; i < Net->layers; i++)
        {

            for (int j = 0; j < Net->nmap[i]; j++)
            {

                for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
                {

                    if (rand() % 100 - percent < 0)
                    {
                        Net->l[i].n[j].connect[k].weight = randomfloat();
                    }

                    if (Net->l[i].n[j].connect[k].weight > 1)
                    {
                        Net->l[i].n[j].connect[k].weight = 1;
                    }
                    else if (Net->l[i].n[j].connect[k].weight < -1)
                    {
                        Net->l[i].n[j].connect[k].weight = -1;
                    }
                }
            }
        }
    }
}

network *copyNet(network *Net)
{

    network *Net2;

    Net2 = confNet(Net->layers, Net->nmap);

    for (int i = 1; i < Net2->layers; i++)
    {

        for (int j = 0; j < Net2->nmap[i]; j++)
        {

            for (int k = Net2->l[i].n[j].num_connections - 1; k >= 0; k--)
            {

                disconnectNeuron(Net2->l[i].n[j].connect[k].n, getneuronP(Net2, i, j));
            }
        }
    }

    for (int i = 0; i < Net->layers; i++)
    {

        for (int j = 0; j < Net->nmap[i]; j++)
        {

            for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {

                connectNeuron(getneuronP(Net2, Net->l[i].n[j].connect[k].n->l, Net->l[i].n[j].connect[k].n->n), getneuronP(Net2, i, j));
                setWeight(getneuronP(Net2, Net->l[i].n[j].connect[k].n->l, Net->l[i].n[j].connect[k].n->n), getneuronP(Net2, i, j), Net->l[i].n[j].connect[k].weight);
            }
        }
    }

    return Net2;
}

void exportNet(network *Net, char *filename)
{

    FILE *file = fopen(filename, "wb");

    if (file == NULL)
    {

        printf("\nfopen faled to open file '%s'\n", filename);
        return;
    }

    float *data;
    unsigned int size = 1 + 1 + Net->layers;

    unsigned int index;

    for (int i = 0; i < Net->layers; i++)
    {

        for (int j = 0; j < Net->nmap[i]; j++)
        {

            size += (3 + 3 * Net->l[i].n[j].num_connections);
        }
    }

    data = (float *)malloc(size * sizeof(float));

    data[0] = size - 1;
    data[1] = Net->layers;

    for (index = 2; index < Net->layers + 2; index++)
    {

        data[index] = Net->nmap[index - 2];
    }

    for (int i = 0; i < Net->layers; i++)
    {

        for (int j = 0; j < Net->nmap[i]; j++)
        {

            data[index] = i;
            data[index + 1] = j;
            data[index + 2] = Net->l[i].n[j].num_connections;

            index += 3;

            for (int k = 0; k < Net->l[i].n[j].num_connections; k++)
            {

                data[index] = Net->l[i].n[j].connect[k].n->l;
                data[index + 1] = Net->l[i].n[j].connect[k].n->n;
                data[index + 2] = Net->l[i].n[j].connect[k].weight;

                index += 3;
            }
        }
    }

    if (index != size)
    {

        fprintf(stdout, "WARNING : index != size\n");
    }

    fseek(file, 0, 0);
    fwrite(data, size * sizeof(float), 1, file);
    fclose(file);
}

network *importNet(char *filename)
{

    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {

        printf("fopen failed to open file '%s'", filename);
        return (network *)0;
    }

    float *data;

    float size;
    unsigned int index = 1;

    fread(&size, sizeof(float), 1, file);
    data = (float *)malloc(size * (sizeof(float)));

    fread(data, size * sizeof(float), 1, file);

    int number_of_layers = data[0];
    int nmap[number_of_layers];

    for (int i = 0; i < number_of_layers; i++, index++)
    {

        nmap[i] = data[index];
    }

    network *Net = confNet(number_of_layers, nmap);

    for (int i = 0; i < Net->layers; i++)
    {

        for (int j = 0; j < Net->nmap[i]; j++)
        {

            for (int k = Net->l[i].n[j].num_connections - 1; k >= 0; k--)
            {

                disconnectNeuron(Net->l[i].n[j].connect[k].n, getneuronP(Net, i, j));
            }
        }
    }

    while (index < size)
    {

        int number_of_connections = data[index + 2];

        if (number_of_connections > 0)
        {

            for (int i = 0; i < number_of_connections; i++)
            {

                int src_1, src_n, dest_1, dest_n;
                float c_weight;

                src_1 = data[index];
                src_n = data[index + 1];
                dest_1 = data[index + 3 + i * 3];
                dest_n = data[index + 3 + i * 3 + 1];
                c_weight = data[index + 3 + i * 3 + 2];

                connectNeuron(getneuronP(Net, dest_1, dest_n), getneuronP(Net, src_1, src_n));
                setWeight(getneuronP(Net, dest_1, dest_n), getneuronP(Net, src_1, src_n), c_weight);
            }
        }

        index += number_of_connections * 3 + 3;
    }

    fclose(file);
    return Net;
}

void connectNeuron(neuron *src_neuron, neuron *dest_neuron)
{

    for (int i = 0; i < dest_neuron->num_connections; i++)
    {
        if (dest_neuron->connect[i].n == src_neuron)
        {
            return;
        }
    }

    dest_neuron->num_connections++;
    connection *connect = (connection *)malloc(sizeof(connection) * dest_neuron->num_connections);

    memcpy(connect, dest_neuron->connect, sizeof(connection) * (dest_neuron->num_connections - 1));
    connect[dest_neuron->num_connections - 1].n = src_neuron;
    connect[dest_neuron->num_connections - 1].weight = randomfloat();

    free(dest_neuron->connect);
    dest_neuron->connect = (connection *)malloc(sizeof(connection) * dest_neuron->num_connections);
    memcpy(dest_neuron->connect, connect, sizeof(connection) * dest_neuron->num_connections);

    free(connect);

    return;
}

void disconnectNeuron(neuron *src_neuron, neuron *dest_neuron)
{

    int index = -1;

    for (int i = 0; i < dest_neuron->num_connections; i++)
    {

        if (dest_neuron->connect[i].n == src_neuron)
        {

            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return;
    }

    dest_neuron->num_connections--;
    connection *connect = (connection *)malloc(sizeof(connection) * dest_neuron->num_connections);

    memcpy(connect, dest_neuron->connect, index * sizeof(connection));
    memcpy(connect + index, dest_neuron->connect + index + 1, (dest_neuron->num_connections - index) * sizeof(connection));

    free(dest_neuron->connect);
    dest_neuron->connect = (connection *)malloc(sizeof(connection) * dest_neuron->num_connections);
    memcpy(dest_neuron->connect, connect, sizeof(connection) * dest_neuron->num_connections);

    free(connect);

    return;
}

void setWeight(neuron *src_neuron, neuron *dest_neuron, float weight)
{

    int index = -1;

    for (int i = 0; i < dest_neuron->num_connections; i++)
    {

        if (dest_neuron->connect[i].n == src_neuron)
        {

            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return;
    }

    dest_neuron->connect[index].weight = weight;

    return;
}

void drawNet(network *Net, SDL_Renderer *renderer, int x, int y, int width, int height, int nodeSize) {

    nodeSize = 15;

    for (int i = 1; i < Net->layers; i++) {
        for (int j = 0; j < Net->nmap[i]; j++) {
            for (int k = 0; k < Net->l[i].n[j].num_connections; k++) {

                if (Net->l[i].n[j].connect[k].weight > 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                }

                SDL_RenderDrawLine(renderer, (Net->l[i].n[j].connect[k].n->l+1) * (width / (Net->layers + 1)) + nodeSize + 1 + x, (Net->l[i].n[j].connect[k].n->n+1) * (height / (Net->nmap[Net->l[i].n[j].connect[k].n->l] + 1)) + y, (i+1) * (width / (Net->layers+1)) - nodeSize - 1 + x, (j+1) * (height / (Net->nmap[i] + 1)) + y);
            }
        }
    }

    for (int i = 0; i < Net->layers; i++) {
        for (int j = 0; j < Net->nmap[i]; j++) {
            
            if (Net->l[i].n[j].output > 0) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderFillCircle(renderer, (i+1) * (width / (Net->layers+1)) + x, (j+1) * (height / (Net->nmap[i] + 1)) + y, nodeSize);
        }
    }
}

void SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

#endif