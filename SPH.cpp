#ifndef __SPH_H__
#define __SPH_H__

#include "SPH.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

using namespace std;

SPH::SPH(){
	kernel = 0.04f;
	mass = 0.02f;

	Max_Number_Paticles = 10000;
	Number_Particles = 0;

	World_Size.x = 2.56f;
	World_Size.y = 2.56f;
	Cell_Size = kernel;			// cell size = kernel or h
	Grid_Size = World_Size / Cell_Size;
	Grid_Size.x = (int)Grid_Size.x;
	Grid_Size.y = (int)Grid_Size.y;
	Number_Cells = (int)Grid_Size.x * (int)Grid_Size.y;

	Gravity = Vector2(0.0f, -3.0f);
	K = 1000.0f;
	Stand_Density = 1000.0f;
	Time_Delta = 0.002f;
	Wall_Hit = 0.0f;
	Viscosity_Constant = 8.0f;

	Particles = (Particle *)malloc(sizeof(Particle) * Max_Number_Paticles);
	Cells = (Cell *)malloc(sizeof(Cell) * Number_Cells);

	CONSTANT1 = 315.0f/(64.0f * PI * pow(kernel, 9));
	CONSTANT2 = 45.0f/(PI * pow(kernel, 6));

	cout<<"SPHSystem"<<endl;
	cout<<"Grid_Size_X : "<<Grid_Size.x<<endl;
	cout<<"Grid_Size_Y : "<<Grid_Size.y<<endl;
	cout<<"Cell Number : "<<Number_Cells<<endl;
}

SPH::~SPH(){
	free(Particles);
	free(Cells);
}

void SPH::Init_Fluid(){
	Vector2 pos;
	Vector2 vel(0.0f, 0.0f);

	for(float i = World_Size.x * 0.3f; i < World_Size.x * 0.7f; i += kernel * 0.6f)
		for(float j = World_Size.y * 0.3f; j < World_Size.y * 0.9f; j += kernel * 0.6f){
			pos = Vector2(i, j);
			Init_Particle(pos, vel);
		}
	cout<<"Number of Paticles : "<<Number_Particles<<endl;
}

void SPH::Init_Particle(Vector2 pos, Vector2 vel){
	Particle *p = &(Particles[Number_Particles]);
	p->pos = pos;
	p->vel = vel;
	p->acc = Vector2(0.0f, 0.0f);
	p->dens = Stand_Density;
	p->next = NULL;
	Number_Particles++;
}

Vector2 SPH::Calculate_Cell_Position(Vector2 pos){
	Vector2 cellpos = pos / Cell_Size;
	cellpos.x = (int)cellpos.x;
	cellpos.y = (int)cellpos.y;
	return cellpos;
}

int SPH::Calculate_Cell_Hash(Vector2 pos){
	if((pos.x < 0)||(pos.x >= Grid_Size.x)||(pos.y < 0)||(pos.y >= Grid_Size.y)){
		return -1;
	}
	int hash = pos.y * Grid_Size.x + pos.x;
	if(hash > Number_Cells){
		cout<<"Error";
	}
	return hash;
}

float SPH::Poly6(float r2){
	return CONSTANT1 * pow(kernel * kernel - r2, 3);
}

float SPH::Spiky(float r){
	return -CONSTANT2 * (kernel - r)* (kernel - r);
}

float SPH::Visco(float r){
	return CONSTANT2 * (kernel - r);
}

void SPH::Hash_Grid(){
	for(int i = 0; i < Number_Cells; i++)
		Cells[i].head = NULL;
	int hash;
	Particle *p;
	for(int i = 0; i < Number_Particles; i ++){
		p = &Particles[i];
		hash = Calculate_Cell_Hash(Calculate_Cell_Position(p->pos));
		if(Cells[hash].head == NULL){
			p->next = NULL;
			Cells[hash].head = p;
		}
		else{
			p->next = Cells[hash].head;
			Cells[hash].head = p;
		}
	}
}

void SPH::Comupte_Density_SingPressure(){
	Particle *p;
	Particle *np;
	Vector2 CellPos;
	Vector2 NeighborPos;
	int hash;
	for(int k = 0; k < Number_Particles; k++){
		p = &Particles[k];
		p->dens = 0;
		p->pres = 0;
		CellPos = Calculate_Cell_Position(p->pos);
		for(int i = -1; i <= 1; i++)
			for(int j = -1; j <= 1; j++){
				NeighborPos = CellPos + Vector2(i, j);
				hash = Calculate_Cell_Hash(NeighborPos);
				if(hash == -1)
					continue;
				np = Cells[hash].head;
				while(np != NULL){
					Vector2 Distance;
					Distance = p->pos - np->pos;
					float dis2 = (float)Distance.getNormSquared();

					if((dis2 < INF)||(dis2 > kernel * kernel)){
						np = np->next;
						continue;
					}
					p->dens += mass * Poly6(dis2);
					np = np->next;
				}
			}
		p->dens += mass * Poly6(0.0f);
		p->pres = (pow(p->dens / Stand_Density, 7) - 1) * K;
	}
}

void SPH::Computer_Force(){
	Particle *p;
	Particle *np;
	Vector2 CellPos;
	Vector2 NeighborPos;
	int hash;
	for(int k = 0; k < Number_Particles; k++){
		p = &Particles[k];
		p->acc = Vector2(0.0f, 0.0f);
		CellPos = Calculate_Cell_Position(p->pos);
		for(int i = -1; i <= 1; i++)
			for(int j = -1; j <= 1; j++){
				NeighborPos = CellPos + Vector2(i, j);
				hash = Calculate_Cell_Hash(NeighborPos);
				if(hash == -1)
					continue;
				np = Cells[hash].head;
				while(np != NULL){
					Vector2 Distance;
					Distance = p->pos - np->pos;
					float dis2 = (float)Distance.getNormSquared();

					if((dis2 < kernel *kernel)&&(dis2 > INF)){
						float dis = sqrt(dis2);
						float Volume = mass / np->dens;
						float Force = Volume * (p->pres+np->pres)/2 * Spiky(dis);
						p->acc -= Distance*Force/dis;

						Vector2 RelativeVel = np->vel - p->vel;
						Force = Volume * Viscosity_Constant * Visco(dis);
						p->acc += RelativeVel*Force;
					}
					np = np->next;
				}
			}
		p->acc = p->acc/p->dens + Gravity;
	}
}

void SPH::Update_Pos_Vel(){
	Particle *p;
	for(int i=0; i < Number_Particles; i++){
		p = &Particles[i];
		p->vel = p->vel + p->acc*Time_Delta;
		p->pos = p->pos + p->vel*Time_Delta;

		if(p->pos.x < 0.0f){
			p->vel.x = p->vel.x * Wall_Hit;
			p->pos.x = 0.0f;
		}
		if(p->pos.x >= World_Size.x){
			p->vel.x = p->vel.x * Wall_Hit;
			p->pos.x = World_Size.x - 0.0001f;
		}
		if(p->pos.y < 0.0f){
			p->vel.y = p->vel.y * Wall_Hit;
			p->pos.y = 0.0f;
		}
		if(p->pos.y >= World_Size.y){
			p->vel.y = p->vel.y * Wall_Hit;
			p->pos.y = World_Size.y - 0.0001f;
		}
	}
}

void SPH::Animation(){
	Hash_Grid();
	Comupte_Density_SingPressure();
	Computer_Force();
	Update_Pos_Vel();
}

int SPH::Get_Particle_Number(){
	return Number_Particles;
}

Vector2 SPH::Get_World_Size(){
	return World_Size;
}

Particle* SPH::Get_Paticles(){
	return Particles;
}

Cell* SPH::Get_Cells(){
	return Cells;
}


#endif