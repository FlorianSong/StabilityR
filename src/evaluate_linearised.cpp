#include <RcppArmadillo.h>
using namespace Rcpp;

// evaluate stability function (linearised) given the observed partition matrix
// input: [p] partition matrix with n x m, where n is a number of nodes in the graph, and m is a number of partition vectors; [s] graph matrix, linearised; [d] vector with stationary distribution per node; [time] markov time.
// output: vector with stability values per each partition vector m.

// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::export]]
NumericVector evaluate_linearised(arma::mat p, arma::mat s, arma::vec d, double time) {
	int n = p.n_rows;
	int m = p.n_cols;
	arma::vec v0 = arma::ones(n);  
	arma::vec v1 = arma::linspace(1,n,n);
	
	NumericVector res(m);
	for (int i=0; i<m; ++i) {
		arma::vec v2 = p.col(i);
		arma::mat ind = arma::join_cols(v1.t(), v2.t());
		arma::umat indx = arma::conv_to<arma::umat>::from(ind);
		arma::sp_mat H(indx-1, v0);
		
		arma::mat f1 = (trans(d) * H) * (trans(H) * d);
		res(i) = (1-time) + trace(trans(H) * (s*time) * H) - f1(0,0);
	}
	
	return res;
}
