#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <thread>
#include "front.h"




boid::boid(double a , double b , double c, double v_a,double v_b,double v_c){
	x = a ;
	y = b ;
	z = c ;
	v_x = v_a;
	v_y = v_b;
	v_z = v_c;
	flap =0 ;
}

fixed::fixed(double a , double b , double c){
	x = a ;
	y = b ;
	z = c ;
}	
flock::flock(int n1 ,int f1,fixed** fix1 ,boid** s, double* r , double* coe){
	n = n1;
	f = f1 ;
	r_sq = r;
	coeff = coe;
	stars = s ;
	fix = fix1 ; 
}
double  flock::dist_sq(int a , int b,bool forb){
	if(forb) return (stars[a] ->x - stars[b] ->x)*(stars[a] ->x - stars[b] ->x) + (stars[a] ->y - stars[b] ->y)*(stars[a] ->y - stars[b] ->y) + (stars[a] ->z - stars[b] ->z)*(stars[a] ->z - stars[b] ->z) ; 
	else return (stars[a] ->x - fix[b] ->x)*(stars[a] ->x - fix[b] ->x) + (stars[a] ->y - fix[b] ->y)*(stars[a] ->y - fix[b] ->y) + (stars[a] ->z - fix[b] ->z)*(stars[a] ->z - fix[b] ->z) ; 	
}
void flock::velocity_boundary(int a){
    double speed = ((stars[a]->v_x)*(stars[a]->v_x))+((stars[a]->v_y)*(stars[a]->v_y))+((stars[a]->v_z)*(stars[a]->v_z));
    speed = pow(speed,0.5);
    if (speed<0.005)
    {
      stars[a]->v_x = (stars[a]->v_x) * 2;
      stars[a]->v_y = (stars[a]->v_y) * 2;
      stars[a]->v_z = (stars[a]->v_z) * 2;
    }
    if (speed>0.05)
    {
      stars[a]->v_x = (stars[a]->v_x) * 0.5;
      stars[a]->v_y = (stars[a]->v_y) * 0.5;
      stars[a]->v_z = (stars[a]->v_z) * 0.5;
    }
    return;
}
void flock::position_boundary(int a, double* ans ){
    
    ans[0] =0 ;
    ans[1] = 0;
    ans[2] =0 ;
      if(stars[a]->x > 0.66)
      {
        ans[0] = 0.66 - stars[a]->x;

      }
      else if(stars[a]->x < -0.66)
      {
        ans[0] = -0.66 - stars[a]->x;
      }

      if(stars[a]->y > 0.66)
      {
           ans[1] = 0.66 - stars[a]->y;
      }
      else if(stars[a]->y < -0.66)
      {
        ans[1] = -0.66 - stars[a]->y;
      }     
      if(stars[a]->z > 0.66)
      {
           ans[2] = 0.66 - stars[a]->z;
      }
      else if(stars[a]->z < -0.66)
      {
        ans[2] = -0.66 - stars[a]->z;
      } 
      ans[0] = ans[0]*0.1;
      ans[1] = ans[1]*0.1;
      ans[2] = ans[2]*0.0001;
      //return ans;

}
void flock::coh(int a, double* ans){
	//ans = (double*) malloc(3*sizeof(double));
	ans[0] =0;
	ans[1] =0;
	ans[2] =0;
	int count = 0;
	for(int i =0 ;i<n ; i++){
		if(i !=a && dist_sq(a , i,true) <= r_sq[0]){
			ans[0] = ans[0] + stars[i]->x ; 
			ans[1] = ans[1] + stars[i]->y ; 
			ans[2] = ans[2] + stars[i]->z ; 
			count ++ ;
		}
	}
	if(count != 0){
	ans[0] = (ans[0] )/count;
	ans[0] = ans[0] - stars[a] ->x ;
	ans[0] = ans[0] *  coeff[0]  ;
	ans[1] = (ans[1]  )/count;
	ans[1] = ans[1] - stars[a] ->y ;
	ans[1] = ans[1] * coeff[0];
	ans[2] = (ans[2]  )/count;
	ans[2] = ans[2] - stars[a] ->z ; 
	ans[2] = ans[2] * coeff[0];
	}
	//return ans ;
}
  	
void flock::ref(int a,bool forb, double* ans){
	//double* ans = (double*) malloc(3*sizeof(double));
	ans[0] =0;
	ans[1] =0;
	ans[2] =0;
	int count = 0;
	if(forb){
		for(int i =0 ;i<n ; i++){
			if(i !=a && dist_sq(a , i , forb) <= r_sq[1]){
				ans[0] = ans[0] + stars[i]->x ; 
				ans[1] = ans[1] + stars[i]->y ; 
				ans[2] = ans[2] + stars[i]->z ; 
				count ++ ;
			}
		}
	}
	else{
		for(int i =0 ;i<f ; i++){
			if( dist_sq(a , i , forb) <= (r_sq[1])*10  ){
				ans[0] = ans[0] + fix[i]->x ; 
				ans[1] = ans[1] + fix[i]->y ; 
				ans[2] = ans[2] + fix[i]->z ; 
				count ++ ;
			}
		}
	}
	if(count != 0){
	ans[0] = ans[0] - count*stars[a] ->x ;
	ans[1] = ans[1] - count*stars[a] ->y ;
	ans[2] = ans[2] - count*stars[a] ->z ; 
	if(forb){
	ans[0] = -ans[0] *  coeff[1]  ;
	ans[1] = -ans[1] * coeff[1];
	ans[2] = -ans[2] * coeff[1];}
	else{
		ans[0] = -ans[0] *  coeff[1]  ;
		ans[1] = -ans[1] * coeff[1]   ;
		ans[2] = -ans[2] * coeff[1]   ;}
	
	}
}

double* flock::ali(int a){
	double* ans = (double*) malloc(3*sizeof(double));
	ans[0] =0;
	ans[1] =0;
	ans[2] =0;
	int count = 0;
	for(int i =0 ;i<n ; i++){
		if(i !=a && dist_sq(a , i,true) <= r_sq[2]){
			ans[0] = ans[0] + stars[i]->v_x ; 
			ans[1] = ans[1] + stars[i]->v_y ; 
			ans[2] = ans[2] + stars[i]->v_z ; 
			count ++ ;
		}
	}
	if(count != 0){
	ans[0] = (ans[0]  )/count;
	ans[0] = ans[0] - stars[a] ->v_x ;
	ans[0] = ans[0] * coeff[2];
	ans[1] = (ans[1]  )/count;
	ans[1] = ans[1] - stars[a] ->v_y ;
	ans[1] =  ans[1] * coeff[2] ;
	ans[2] = (ans[2]  )/count;
	ans[2] = ans[2] - stars[a] ->v_z ;
	ans[2] = ans[2] * coeff[2] ;
	} 				
	return ans ;
}

void flock::nextInstant(){
	for(int i =0 ; i< n ;i++){
		velocity_boundary(i);
	}

	for(int i =0 ;i<n;i++){
		stars[i]->x = stars[i]->x + stars[i]->v_x ;
		stars[i]->y = stars[i]->y + stars[i]->v_y ;
		stars[i]->z = stars[i]->z + stars[i]->v_z ;
	}
	double** align = (double**) malloc(n*sizeof(double*));
	for(int i =0 ;i<n;i++){
		align[i] = this -> ali(i);
	}
	for(int i =0 ; i<n ; i++){
		double* coh = (double*) malloc(3*sizeof(double));
		std::thread th_coh(&flock::coh,this,i,coh);// this->coh(i,coh);
		double* pos = (double*) malloc(3*sizeof(double));
		std::thread th_pos(&flock::position_boundary,this,i,pos);// this->coh(i,coh);

		double* ref  = (double*) malloc(3*sizeof(double));
			std::thread th_ref1(&flock::ref,this,i,true,ref);// this->coh(i,coh);
		double* ref_2  = (double*) malloc(3*sizeof(double));
			std::thread th_ref2(&flock::ref,this,i,false,ref_2);// this->coh(i,coh);
		th_coh.join();
		th_pos.join();
		th_ref1.join();
		th_ref2.join();
		stars[i]->v_x = stars[i]->v_x +coh[0] + ref[0] + align[i][0] + ref_2[0]+pos[0];
		stars[i]->v_y = stars[i]->v_y +coh[1] + ref[1] + align[i][1] + ref_2[1]+pos[1];
		stars[i]->v_z = stars[i]->v_z +coh[2] + ref[2] + align[i][2] + ref_2[2]+pos[2]; 
		
	}
}


