//! This class stores the data of an individual bird 
class boid{
public:
	//! gives the coordinates of bird 
	double x, y ,z ;
	//! gives the velocities of the bird 
	double v_x,v_y,v_z;

	double flap ;
	//! constructor
	boid(double a , double b , double c, double v_a,double v_b,double v_c);
};
//! this class stores data of a fixed points
class fixed{
public:
	//! coordinates of the fixed point
	double x, y, z ;
	//! constructor
	fixed(double a , double b , double c);
} ;



//! This class stores the data of the entire flock and their velocities
class flock{
public:
	//! Number of birds
	int n ;
	//! Number of fixed points 
	int f ;
	//! radius of cohesion, reflection, alignment
	double* r_sq;
	//! coefficient of cohesion, reflection, alignment
	double* coeff;
	//! all birds
	boid** stars;
	//! all fixed points
	fixed** fix;
	//! constructor
	flock(int n1 ,int f1,fixed** fix1 ,boid** s, double* r , double* coe);
  	//! calculates distance between two birds
	double  dist_sq(int a , int b,bool forb);
	  //! limits velocity boundary
  	void velocity_boundary(int a);
    //!limits position_boundary
    void position_boundary(int a,double* ans);
	//! individual cohesion calculation
	void coh(int a,double* ans);
	//! individual reflection calculation
	void ref(int a,bool forb,double* ans);
	//! individual alignment calculation
	double* ali(int a);
	//! summs up all the coefficients and updates it to next instant
	void nextInstant();
};


