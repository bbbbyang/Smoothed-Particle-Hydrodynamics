#ifndef __SPHSYSTEM_H__
#define __SPHSYSTEM_H__

#include "ObjLibrary/Vector2.h"
#include "DataStructure.h"

#define PI 3.141592f
#define INF 1E-12f

class SPH{
	private:
		float kernel;					// kernel or h in kernel function
		float mass;						// mass of particles
		int Max_Number_Paticles;		// initial array for particles
		int Number_Particles;			// paticle number

		Vector2 Grid_Size;				// grid size
		Vector2 World_Size;				// screen size
		float Cell_Size;				// cell size
		int Number_Cells;				// cell number

		Vector2 Gravity;
		float K;						// ideal pressure formulation k
		float Stand_Density;			// ideal pressure formulation p0
		float Time_Delta;
		float Wall_Hit;
		float Viscosity_Constant;

		float CONSTANT1;
		float CONSTANT2;

		Particle *Particles;
		Cell *Cells;
	public:
		SPH();
		~SPH();
		void Init_Fluid();									// initialize fluid
		void Init_Particle(Vector2 pos, Vector2 vel);		// initialize particle system
		Vector2 Calculate_Cell_Position(Vector2 pos);		// get cell position
		int Calculate_Cell_Hash(Vector2 pos);				// get cell hash number

		//kernel function
		float Poly6(float r2);		// for density
		float Spiky(float r);		// for pressure
		float Visco(float);			// for viscosity

		void Hash_Grid();
		void Comupte_Density_SingPressure();
		void Computer_Force();
		void Update_Pos_Vel();
		void Animation();

		int Get_Particle_Number();
		Vector2 Get_World_Size();
		Particle* Get_Paticles();
		Cell* Get_Cells();
};


#endif