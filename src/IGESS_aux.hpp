//
//  IGESS_aux.hpp
//  IGESSArma
//
//  Created by DaiMingwei on 16/11/8.
//  Copyright © 2016年 daviddai. All rights reserved.
//

#ifndef IGESS_aux_hpp
#define IGESS_aux_hpp

#include <stdio.h>
#include <armadillo>
using namespace arma;
//#include <omp.h>

const int kPValueVer = 0;
const int kVbVer = 1;

class PerformanceObject{//object to record the performance
public:
    double FDR;
    double power;
    double auc;

};

class Vardist{
public:
    vec gamma;
    vec mu;
    vec sigma2beta;
    Vardist(uword P, double mu0, double gamma0){
        gamma = vec(P);
        gamma.fill(gamma0);
        mu = vec(P);
        mu.fill(0);
    }
};

class DataModel{ //model to store the original data
public:
    DataModel(mat* X, vec* y, Col<int>* labels, vec* beta, double h){
        this -> X = new Mat<double>(X -> memptr(), X->n_rows, X -> n_cols, false);
        this -> y = new Col<double>(y -> memptr(), y->size(), false);
        this -> labels = new Col<int>(labels -> memptr(), labels->size(), false);
        this -> beta = new Col<double> (beta -> memptr(), beta -> size(), false);
        this -> h = h;

    };
    ~DataModel(){
        delete this -> X;
        delete this -> y;
        delete this -> labels;
        delete this -> beta;
    }
    mat* X;
    vec* y;
    Col<int>* labels;
    vec* beta;
    double h; // heritability

};


class IGESSfit{
    //class for the model generated

public:
    IGESSfit( uword N, uword P,  uword K, uword iter, double L,  double sigam2e, double sigma2beta, double Pi, vec gammas, vec mu, vec S, vec* pParam, vec Xr, mat cov){
        this -> N = N;
        this -> P = P;
        this -> K = K;
        this -> L = L;
        this -> iter = iter;

        this -> sigma2e = sigam2e;
        this -> sigma2beta = sigma2beta;
        this -> Pi = Pi;

        this -> gammas = gammas;
        this -> mu = mu;
        this -> S = S;
        this -> pParam = pParam;
        this -> Xr = Xr;
        this -> cov = cov;

    }

    IGESSfit(vec gammas, vec mu, mat cov){
       this -> gammas = gammas;
       this -> mu = mu;
       this -> cov = cov;
    }
    ~IGESSfit( ){

    }

    vec predict(mat* X, mat* Z = NULL);
    void cal_powerfdr(DataModel* model, double threshold,PerformanceObject* po);
    double cal_auc(DataModel* model);

    double sigma2e;
    double sigma2beta;
    double Pi; //number of variables
    double L; //lower bound
    uword N;
    uword P;
    uword K;
    uword iter;
    vec gammas;
    vec mu;
    vec S;
    vec* pParam;
    vec Xr;
    mat cov;


};

class Options{
public:
    Options(){
        this -> max_iter = 600;
        this -> display_gap = 60;
        this -> n_fold = 5;
    }

    Options(int max_iter, int display_gap){
        this -> max_iter = max_iter;
        this -> display_gap = display_gap;
    }

    Options(int max_iter, int display_gap, int n_fold){
        this -> max_iter = max_iter;
        this -> display_gap = display_gap;
        this -> n_fold = n_fold;
    }
    int max_iter;
    int display_gap;
    int n_fold;

};


vec fdr2FDR(vec fdr);

vec getDiag(float* X, uword P, uword N);

double lb_linear(vec ytilde, vec diagXTX, vec y, double sigma2e, Vardist vardist);
double lb_gamma(vec gamma, double log_pi);
double lb_klbeta(Vardist vardist, double sigma2beta);

template <typename T>
double dotX (T* x, double* y, int n);
//double dotX (double* x, double* y, int n);
void addX (double* y, double a, float* x, int n);

void igess_update(float* x_j, double* gamma, double* mu, double d, double s, double* xty_pt, double logPi, double sigma2beta, double sigma2e, int N, double xy, double* ytilde_pt, double* lpsummay = NULL, vec* lpparam = NULL);

/*update the parameters of the gamma distributions*/
void update_betaparam(uword P, uword K, double gamma_sum, double * lpsummary, double* lpgamma, vec* lpparams);

double lb_pvalue(uword P, uword K, double * lpsummary, double* lpgamma, vec* lpparams);

void update_param(uword N, uword P, Vardist vardist, double sumyytilde, vec diagXTX, double& sigma2e, double& sigma2beta, double& pi_p);

//remove the effects of covariates Z
void remove_cov(float* lpfX, int P, vec& y, mat* Z,mat* SZX,mat* SZy);
double calauc(vec label, vec pred);
mat MatXfloat(mat& Zt, float* lpfX, int P);
vec vecXfloat(vec& Zt, float* lpfX, int P);

#endif /* IGESS_aux_hpp */
