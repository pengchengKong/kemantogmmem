#' Firstly, the Kmeans algorithm is iterated to calculate the center of gravity of the class, and then the Gaussian mixture model is iterated to classify the samples.
#'
#' @param samp the sample
#' @param g the Number of classes
#' @param pie the prior or posterior probabilities of all classifications
#' @param prob the probabilities of samples belong to all classifications
#' @return the mean and variance of each class and the Sample classification
#' @export
#' @importFrom ggplot2 ggplot
#' 
#' @examples
#' library(KemansToGmm)
#' samp = matrix(c(8.6338,8.8988,3.604,14.1556,19.2995,18.2959,31.7925,32.8074,32.0885,148.6553,141.5995,147.4205,7.0472,1.1754,7.5018,12.3305,17.0575,17.2132,33.5934,37.5266,36.9793,153.7012,151.2712,152.8625), nrow = 12, ncol = 2)
#' g = 4
#' kmtogmm(samp,g)
kmtogmm <- function(samp,g){
  m <- ncol(samp)
  n <- nrow(samp)
  out <- iteration_gmm(g,n,m,samp)
  samp1 <- as.data.frame(samp)
  color <- out$Classify
  color1 <- as.character(color)
  p <- ggplot(samp1, aes(x=samp1[,1], y=samp1[,2],colour=color1)) +
    geom_point() +
    ggtitle("样本分类图")
  return(list(classprob=out$pie,mean=out$miu,sigma=out$sig,loglikelihood=out$log,BIC=out$BIC,sampleprob=out$prob,Classify=out$Classify,sampleplot=p))
}

