#include <RcppArmadillo.h>
using namespace Rcpp;

//' Calculate the Mahanobis Distance between the sample and the center of gravity of class
//'
//' @param x  the sample
//' @param center  the center of gravity of class
//' @param cov  the Covariance matrix
//' @return the Mahalanobis Distance
// [[Rcpp::depends("RcppArmadillo")]]
// [[Rcpp::export]]
double Distance(arma::vec x, arma::vec center, arma::mat cov){
  arma::vec h=(x-center).t()*(cov.i())*(x-center);
  double r=sum(h);
  return r;
}

//' Calculate the normal distribution probability density of the sample.
//'
//' @param x  the sample
//' @param mean  the center of gravity of class
//' @param sigma  the Covariance matrix
//' @return the normal distribution probability density of the sample
// [[Rcpp::depends("RcppArmadillo")]]
// [[Rcpp::export]]
double sampledensity ( arma::vec x,  arma::vec mean,  arma::mat sigma){

  int mm=x.n_rows;
  double distval = Distance(x,  mean, sigma);
  double logdet = sum(arma::log(arma::eig_sym(sigma)));
  double log2pi = 1.8378770664;
  double logretval = -( (mm * log2pi + logdet + distval)/2  ) ;
  return(exp(logretval));

}

//' Firstly, the Kmeans algorithm is iterated to calculate the center of gravity of the class, and then the Gaussian mixture model is iterated to classify the samples.
//'
//' @param g  the Number of classes
//' @param n  the Number of Samples
//' @param m  the Dimension of Sample
//' @return the mean and variance of each class and the Sample classification
//[[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export]]

Rcpp::List iteration_gmm(int g,int n,int m,arma::mat samp){

  arma::mat mean_k(g,m,arma::fill::zeros);
  List sig_k(g);
  arma::vec pie_k(g);
  int n_g = n / g;
  for (int i = 0; i < g; i++){
    for (int j = 0; j < n_g; j++){
      mean_k.row(i) = mean_k.row(i) + samp.row(i*n_g+j);
    }
    mean_k.row(i) = mean_k.row(i) / n_g;
  }

  for(int t=0;t<100;t++){
    //Each sample is classified according to its distance from the center of gravity of class
    int argmin = 0;
    arma::vec label(n);
    for (int i = 0; i < n; i++){
      double min = -1;
      for (int j = 0; j < g; j++){
        double distance = sum((samp.row(i).t() - mean_k.row(j).t()) % (samp.row(i).t() - mean_k.row(j).t()));
        if (min == -1 || min > distance) {
          argmin = j;
          min = distance;
        }
      }
      label(i) = argmin;
    }
    //Recalculate the center of gravity of class
    arma::mat mean_kk(g,m,arma::fill::zeros);
    for (int j = 0; j < g; j++){
      double number_sample = 0;
      for (int i = 0; i < n; i++){
        if (label(i) == j){
          mean_kk.row(j) = mean_kk.row(j) + samp.row(i);
          number_sample++;
        }
        pie_k(j) = number_sample / n;
      }
      if (number_sample == 0){
        number_sample = 1.0;
      }
      mean_kk.row(j) = mean_kk.row(j) / number_sample;
      //Recalculate the covariance matrix
      arma::mat sig_km(m,m,arma::fill::zeros);
      for (int i = 0; i < n; i++){
        if (label(i) == j){
          sig_km = sig_km + (samp.row(i)-mean_kk.row(j)).t()*(samp.row(i)-mean_kk.row(j));
        }
      }
      sig_k[j] = sig_km / number_sample;
    }
    mean_k = mean_kk;
  }

  arma::mat miu = mean_k.t();
  List sig = sig_k;
  arma::vec pie = pie_k;
  arma::mat prob(n,g,arma::fill::zeros);
  double oldlog;
  double newlog = 0;
  double BIC;

  for(int s=0;s<100;s++){
    oldlog=newlog;

    //E step
    arma::mat store(n,g);
    arma::mat weight(n,g);
    for(int j=0; j<g ;j++){
      arma::mat cova=sig[j];
      for(int i=0 ; i<n; i++){
        store(i,j)= sampledensity(samp.row(i).t(),miu.col(j),cova);
      }
      weight.col(j)=pie(j)*store.col(j);
    }

    //Calculate loglikelihood function and BIC
    arma::vec row_sum=sum(weight,1);
    for(int j=0;j<g;j++) prob.col(j)=weight.col(j)/row_sum;
    newlog=sum(log(row_sum)) + 0.04*sum(pie % log(pie));
    BIC= -2*oldlog+((m+m+1)*g-1)*log(n);

    // M step
    for(int j=0; j<g;j++){
      double sum1=sum(prob.col(j));
      pie(j)=sum1/n;
      arma::mat t(n,m);
      arma::mat temp(m,m,arma::fill::zeros);

      //Recalculate the mean values
      for(int i=0; i<n; i++){
        t.row(i)=prob(i,j)*samp.row(i);
      }
      miu.col(j)=(sum(t,0)/sum1).t();

      //Recalculate various covariance matrices
      for(int i=0; i<n; i++){
        arma::vec ve=samp.row(i).t()-miu.col(j);
        arma::mat vee=prob(i,j)*(ve*ve.t());
        temp=temp+vee;
      }
      sig[j]=temp/sum1;
    }
    //Determine whether convergence
    if(std::abs(newlog-oldlog)<1e-6) break;
  }
  //Calculate the classification of each sample
  arma::vec sample_Classify(n);
  for(int i=0 ; i<n; i++){
    int argmax = -1;
    double max = 0;
    for(int j=0; j<g; j++){
      if (max < prob(i,j)){
        argmax = j;
        max = prob(i,j);
      }
    }
    sample_Classify(i) = argmax;
  }

  Rcpp::List outcome=List::create(Named("pie")=pie,_["miu"]=miu,_["sig"]=sig,_["prob"]=prob,_["log"]=newlog,_["BIC"]=BIC,_["Classify"]=sample_Classify);
  return(outcome);
}
